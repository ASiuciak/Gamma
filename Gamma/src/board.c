#include "board.h"
#include "malloc.h"

bool has_adjacent(uint32_t **board, uint32_t x, uint32_t y, uint32_t p,
                  uint32_t width, uint32_t height) {
    if (x >= height || y >= width) { return false; }
    bool result = false;
    if (x > 0 && board[x - 1][y] == p) {
        result = true;
    }
    if (x < height - 1 && board[x + 1][y] == p) {
        result = true;
    }
    if (y > 0 && board[x][y - 1] == p) {
        result = true;
    }
    if (y < width - 1 && board[x][y + 1] == p) {
        result = true;
    }
    return result;
}

uint64_t count_adjacent_free(uint32_t **board, uint32_t x, uint32_t y,
                             uint32_t p, uint32_t width, uint32_t height) {
    if (x >= height || y >= width) { return 0; }
    uint64_t result = 0;
    if (x > 0 && board[x - 1][y] == 0) {
        if (!has_adjacent(board, x - 1, y, p, width, height)) {
            result++;
        }
    }
    if (x < height - 1 && board[x + 1][y] == 0) {
        if (!has_adjacent(board, x + 1, y, p, width, height)) {
            result++;
        }
    }
    if (y > 0 && board[x][y - 1] == 0) {
        if (!has_adjacent(board, x, y - 1, p, width, height)) {
            result++;
        }
    }
    if (y < width - 1 && board[x][y + 1] == 0) {
        if (!has_adjacent(board, x, y + 1, p, width, height)) {
            result++;
        }
    }
    return result;
}

bool are_connected(uint32_t **board, uint32_t start_x, uint32_t start_y, uint32_t dest_x,
                   uint32_t dest_y, uint32_t width, uint32_t height) {
    if (start_x >= height || start_y >= width ||
        dest_x >= height || dest_y >= width) {
        return false;
    }
    Stack stack = new_Stack();
    bool **visited = NULL;
    visited = safe_malloc(visited, height * sizeof(bool *));
    for (uint32_t i = 0; i < height; i++) {
        visited[i] = safe_malloc(visited[i], width * sizeof(bool));
    }
    push(stack, make_Pair(start_x, start_y));
    uint32_t p = board[start_x][start_y];
    for (uint32_t i = 0; i < height; i++) {
        for (uint32_t j = 0; j < width; j++) {
            visited[i][j] = false;
        }
    }
    visited[start_x][start_y] = true;
    bool found = false;
    while (!empty(stack) && !found) {
        Pair tp = top(stack);
        pop(stack);
        uint32_t x = tp.x;
        uint32_t y = tp.y;
        if (x == dest_x && y == dest_y) { found = true; }
        else {
            if (x > 0 && board[x - 1][y] == p && !visited[x - 1][y]) {
                push(stack, make_Pair(x - 1, y));
                visited[x - 1][y] = true;
            }
            if (x < height - 1 && board[x + 1][y] == p && !visited[x + 1][y]) {
                push(stack, make_Pair(x + 1, y));
                visited[x + 1][y] = true;
            }
            if (y > 0 && board[x][y - 1] == p && !visited[x][y - 1]) {
                push(stack, make_Pair(x, y - 1));
                visited[x][y - 1] = true;
            }
            if (y < width - 1 && board[x][y + 1] == p && !visited[x][y + 1]) {
                push(stack, make_Pair(x, y + 1));
                visited[x][y + 1] = true;
            }
        }
    }
    del(stack);
    for (uint32_t i = 0; i < height; i++) { free(visited[i]); }
    free(visited);
    return found;
}

uint64_t count_areas(uint32_t **board, Pair fields[4], bool active[4],
                     uint32_t width, uint32_t height) {
    int how_many = 0;
    int representants[4];
    int first_active = 0;
    while (first_active < 4 && !active[first_active]) {
        first_active++;
    }
    if (first_active < 4) {
        how_many = 1;
        representants[0] = first_active;
    } else {
        return 0;
    }
    for (int i = first_active + 1; i < 4; i++) {
        if (active[i]) {
            bool connected = false;
            for (int j = 0; j < how_many && !connected; j++) {
                uint32_t or_x = fields[i].x;
                uint32_t or_y = fields[i].y;
                uint32_t dest_x = fields[representants[j]].x;
                uint32_t dest_y = fields[representants[j]].y;
                if (are_connected(board, or_x, or_y,
                                  dest_x, dest_y, width, height)) {
                    connected = true;
                }
            }
            if (!connected) {
                representants[how_many] = i;
                how_many++;
            }
        }
    }
    return how_many;
}

uint64_t count_adjacent_areas(uint32_t **board, uint32_t x, uint32_t y,
                              uint32_t p, uint32_t width, uint32_t height) {
    if (x >= height || y >= width) { return 0; }
    /* Przekazujemy funkcji count_areas (w sposób opisany w board.h)
     * tylko pola sąsiednie należące da danego gracza.
     */
    Pair fields[4];
    bool active[4] = {false, false, false, false};
    if (x > 0 && board[x - 1][y] == p) {
        fields[0] = make_Pair(x - 1, y);
        active[0] = true;
    }
    if (x < height - 1 && board[x + 1][y] == p) {
        fields[1] = make_Pair(x + 1, y);
        active[1] = true;
    }
    if (y > 0 && board[x][y - 1] == p) {
        fields[2] = make_Pair(x, y - 1);
        active[2] = true;
    }
    if (y < width - 1 && board[x][y + 1] == p) {
        fields[3] = make_Pair(x, y + 1);
        active[3] = true;
    }
    return count_areas(board, fields, active, width, height);
}