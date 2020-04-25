#include "gamma.h"

gamma_t *gamma_new(uint32_t width, uint32_t height,
                   uint32_t players, uint32_t areas) {
    if (width == 0 || height == 0 || players == 0 || areas == 0) {
        return NULL;
    }

    gamma_t *g = NULL;
    g = safe_malloc(g, sizeof(gamma_t));

    g->board = safe_malloc(g->board, height * sizeof(uint32_t *));
    for (uint32_t i = 0; i < height; i++) {
        g->board[i] = safe_malloc(g->board[i], width * sizeof(uint32_t));
    }
    for (uint32_t i = 0; i < height; i++) {
        for (uint32_t j = 0; j < width; j++) {
            g->board[i][j] = 0;
        }
    }

    g->players = safe_malloc(g->players, (players + 1) * sizeof(Player));
    for (uint32_t i = 1; i <= players; i++) {
        g->players[i] = new_Player();
    }

    g->width = width;
    g->height = height;
    g->number_of_players = players;
    g->areas = areas;
    g->free = g->width * g->height;
    return g;
}

void gamma_delete(gamma_t *g) {
    if (g != NULL) {
        free(g->players);
        for (uint32_t i = 0; i < g->height; i++) {
            free(g->board[i]);
        }
        free(g->board);
        free(g);
    }
}

bool gamma_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
    if (g == NULL) { return false; }
    if (player == 0 || player > g->number_of_players) { return false; }
    if (x >= g->width || y >= g->height) { return false; }
    if (g->board[y][x] != 0) { return false; }

    /* Sprawdzamy, jak zmieni się liczba obszarów nowego własciciela pola.
    * Jeśli po ruchu liczba obszarów przekroczy dopuszczalną, zwracamy false.
    */
    uint64_t areas_before = count_adjacent_areas(g->board, y, x, player,
                                                 g->width, g->height);
    uint64_t potential_areas = g->players[player].areas - areas_before + 1;
    if (potential_areas > g->areas) {
        return false;
    }
    g->players[player].areas = potential_areas;
    // W tym momencie wiemy już, że ruch jest poprawny.

    // Po zajęciu pola zmienia się takżę liczba wolnych pól przyległych.
    uint64_t new_adjacent_free = count_adjacent_free(g->board, y, x, player,
                                                     g->width, g->height);
    g->players[player].adjacent_free += new_adjacent_free;
    if (has_adjacent(g->board, y, x, player, g->width, g->height)) {
        g->players[player].adjacent_free--;
    }

    /* Zajęcie pola mogło zabrać przyległe pole innym graczom,
     * odejmujemy je, jeśli to konieczne. Pilnujemy, żeby żadnemu
     * graczowi nie odjąć więcej niż 1 pola przyległego.
     */
    uint32_t n1 = 0, n2 = 0, n3 = 0, n4 = 0;
    if (x > 0) { n1 = g->board[y][x - 1]; }
    if (x < g->width - 1) { n2 = g->board[y][x + 1]; }
    if (y > 0) { n3 = g->board[y - 1][x]; }
    if (y < g->height - 1) { n4 = g->board[y + 1][x]; }
    if (n1 != 0 && n1 != player) {
        g->players[n1].adjacent_free--;
    }
    if (n2 != 0 && n2 != player && n2 != n1) {
        g->players[n2].adjacent_free--;
    }
    if (n3 != 0 && n3 != player && n3 != n2 && n3 != n1) {
        g->players[n3].adjacent_free--;
    }
    if (n4 != 0 && n4 != player && n4 != n3 && n4 != n2 && n4 != n1) {
        g->players[n4].adjacent_free--;
    }
    /* Dopiero teraz przypisujemy nr gracza do pola, w przeciwnym
     * razie funkcje has_adjacent wywołane powyżej zawsze zwracałyby true.
     */
    g->players[player].occupied++;
    g->free--;
    g->board[y][x] = player;
    return true;
}

bool gamma_golden_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
    if (g == NULL) { return false; }
    if (player == 0 || player > g->number_of_players) { return false; }
    if (!g->players[player].golden) { return false; }
    if (x >= g->width || y >= g->height) { return false; }
    if (g->board[y][x] == 0 || g->board[y][x] == player) { return false; }
    uint32_t former = g->board[y][x];

    /* Sprawdzamy, jak zmieni się liczba obszarów byłego i nowego
     * właściciela pola. Pole chwilowo staje się puste, aby poprawnie
     * przeprowadzić wszystkie obliczenia. Jeśli po ruchu liczba obszarów
     * któregoś z graczy przekroczy dopuszczalną, oddajemy pole poprzedniemu
     * właścicielowi i zwracamy false.
     */
    g->board[y][x] = 0;
    uint64_t areas_before = count_adjacent_areas(g->board, y, x, player,
                                                 g->width, g->height);
    uint64_t former_areas_before = count_adjacent_areas(g->board, y, x,
                                                        former, g->width,
                                                        g->height);
    uint64_t potential_areas = g->players[player].areas - areas_before + 1;
    uint64_t former_potential_areas = g->players[former].areas +
                                      former_areas_before - 1;
    if (potential_areas > g->areas || former_potential_areas > g->areas) {
        g->board[y][x] = former;
        return false;
    }
    // W tym momencie wiemy już, że ruch jest poprawny.
    g->players[player].areas = potential_areas;
    g->players[former].areas = former_potential_areas;

    // Nowy właściciel może zyskać pola przyległe, były może je stracić.
    uint64_t adj_to_add = count_adjacent_free(g->board, y, x, player,
                                              g->width, g->height);
    uint64_t adj_to_delete = count_adjacent_free(g->board, y, x, former,
                                                 g->width, g->height);
    g->players[player].adjacent_free += adj_to_add;
    g->players[former].adjacent_free -= adj_to_delete;

    /* Dopiero teraz przypisujemy nr gracza do pola, w przeciwnym
     * razie funkcje has_adjacent wywołane powyżej zawsze zwracałyby true.
     */
    g->players[player].occupied++;
    g->players[former].occupied--;
    g->players[player].golden = false;
    g->board[y][x] = player;
    return true;
}

uint64_t gamma_busy_fields(gamma_t *g, uint32_t player) {
    if (g == NULL) { return 0; }
    if (player == 0 || player > g->number_of_players) { return 0; }
    return g->players[player].occupied;
}

uint64_t gamma_free_fields(gamma_t *g, uint32_t player) {
    if (g == NULL) { return 0; }
    if (player == 0 || player > g->number_of_players) { return 0; }
    if (g->players[player].areas == g->areas) {
        return g->players[player].adjacent_free;
    }
    return g->free;
}

bool gamma_golden_possible(gamma_t *g, uint32_t player) {
    if (g == NULL) { return false; }
    if (player == 0 || player > g->number_of_players) { return false; }
    if (!g->players[player].golden) { return false; }
    return g->width * g->height - (g->free + g->players[player].occupied) > 0;
}

char *gamma_board(gamma_t *g) {
    if (g == NULL) { return NULL; }
    void *test = malloc(sizeof(struct stringBuilder));
    if (test == NULL) { return NULL; }
    free(test);
    String s = newString();
    bool trouble = false;
    for (uint32_t i = 0; i < g->height && !trouble; i++) {
        for (uint32_t j = 0; j < g->width && !trouble; j++) {
            if (!append_number(s, g->board[g->height - i - 1][j])) {
                trouble = true;
            }
        }
        if (!append(s, '\n')) {
            trouble = true;
        }
    }
    if (trouble) {
        free(s->word);
        free(s);
        return NULL;
    }
    char *result = s->word;
    free(s);
    return result;
}