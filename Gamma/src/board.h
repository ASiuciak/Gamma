/** @file
 * Interfejs funkcji odczytujących odpowiednie dane z planszy gry
 * za pomocą operacji na tablicy dwuwymiarowej o danych rozmiarach.
 * Przyjmuję definicje pol sąsiednich i obszarów z treści zadania.
 * Uwaga! W poniższych funkcjach argument x opisuje nr wiersza, a y
 * nr kolumny danego pola. Ponieważ funkcje z @ref gamma.h przyjmują
 * odwrotnie, będą one wywoływać poniższe z odwróconą kolejnością
 * tych parametrów. Przykład: wywołanie funkcji @ref has_adjacent
 * dla dla gry g, pola o współrzędnych (3,8) w tej grze i gracza nr 1
 * będzie wyglądało tak: has_adjacent(g->board, 8, 3, 1, g->width, g->height)
 */

#ifndef BOARD_H
#define BOARD_H

#include "stack.h"
#include <stdbool.h>
#include <stdint.h>

/** @brief Sprawdza, czy dane pole graniczy z polem należącym do danego gracza.
 * @param[in] board – plansza gry
 * @param[in] x - nr wiersza danego pola
 * @param[in] y - nr kolumny danego pola
 * @param[in] p - nr danego gracza
 * @param[in] width - wysokość planszy
 * @param[in] height - szerokość planszy
 * @return wartość @p true, jesli dane pole graniczy z co najmniej jednym
 * polem należącym do danego gracza, @p false w przeciwnym wypadku.
 */
bool has_adjacent(uint32_t **board, uint32_t x, uint32_t y, uint32_t p,
                  uint32_t width, uint32_t height);

/** @brief Liczy nowe puste pola przyległe do danego.
 * Ponieważ celem funkcji jest informowanie, ile nowych wolnych
 * pól przyległych zyska gracz po zajęciu danego pola, do nowych pól
 * przyległych nie zaliczamy tych, które mają już innego "naszego" sąsiada,
 * ponieważ zostały one wliczone już wcześniej.
 * Przykład: gracz nr 1 zajął pole (0,0), pozostałe są wolne.
 * Kiedy ten gracz zechce zająć pole (1,1) zyska 2 nowe pola przyległe:
 * (1,2) i (2,1), nie liczymy już pól (0,1) i (1,0), one zostały dodane przy
 * zajmowaniu pola (0,0).
 * @param[in] board – plansza gry
 * @param[in] x - nr wiersza danego pola
 * @param[in] y - nr kolumny danego pola
 * @param[in] p - nr danego gracza
 * @param[in] width - wysokość planszy
 * @param[in] height - szerokość planszy
 * @return liczba nowych (wg def. powyżej) pustych pól przyległych do danego.
 */
uint64_t count_adjacent_free(uint32_t **board, uint32_t x, uint32_t y,
                             uint32_t p, uint32_t width, uint32_t height);


/** @brief Sprawdza, czy dane 2 pola należą do jednego obszaru,
 * z wykorzystaniem algorytmu DFS.
 * @param[in] board   – plansza gry,
 * @param[in] start_x - nr wiersza pola 1 (startowego)
 * @param[in] start_y - nr kolumny pola 1 (startowego)
 * @param[in] dest_x - nr wiersza pola 2 (docelowego)
 * @param[in] dest_y - nr kolumny pola 2 (docelowego)
 * @param[in] width - wysokość planszy
 * @param[in] height - szerokość planszy
 * @return wartość @p true, jesli pola należą do jednego obszaru,
 * @p false w przeciwnym wypadku.
 */
bool are_connected(uint32_t **board, uint32_t start_x, uint32_t start_y,
         uint32_t dest_x, uint32_t dest_y, uint32_t width, uint32_t height);

/** @brief Sprawdza, do ilu obszarów należą dane pola (max 4)
 * Nietypowa postać przyjmowanych argumentów spowodowana jest tym, że
 * funkcję tą wykorzystuje @ref count_adjacent_areas. Przekazuje ona
 * tej funkcji pola sąsiadujące z danym i należące do danego gracza.
 * Takich pól może być od 0 do 4, a taki format pozwala na ich
 * przekazywanie w jednolity sposób, bez alokacji dodatkowej pamięci,
 * niezależnie od ich ilości.
 * @param[in] board – plansza gry
 * @param[in] active - informuje o tym, które elementy z tablicy
 * fields brać pod uwagę. (zakłada, że jesli active[i] == false, to wartość
 * pod fields[i] jest niezainicjalizowana i nie bierze jej pod uwagę)
 * @param[in] fields - tablica pól do sprawdzenia
 * @param[in] board   – plansza reprezentująca stan gry,
 * @param[in] width - wysokość planszy
 * @param[in] height - szerokość planszy
 * @return liczba obszarów, do których należą dane pola.
 */
uint64_t count_areas(uint32_t **board, Pair fields[4], bool active[4],
                     uint32_t width, uint32_t height);

/** @brief Liczy, do ilu obszarów danego gracza należą pola
 * sąsiadujące z danym, z wykorzystaniem @ref count_areas.
 * Wykorzystywana w celu określenia zmiany liczby obszarów
 * danego gracza po zajęciu danego pola (na skutek zwykłego lub
 * złotego ruchu) lub utraty danego pola (na skutek złotego ruchu innego
 * gracza). Przykład: gracz 1 zajął 4 pola - (0,0), (0,1), (1,0) i (1,2).
 * Funkcja wywołana dla gracza 1 i pola (1,1) zwróci 2 (bo gracz posiada 3
 * pola sąsiadujące z (1,1), ale 2 z nich należą do 1 obszaru). Ponieważ
 * wszystkie obszary, do których należą pola sąsiednie złaczą się w 1 po
 * zajęciu przez gracza danego pola, możemy ustalić jak zmieni się liczba
 * obszarów gracza. W przykładzie 2 obszary zredukują sie do 1, zatem liczba
 * obszarów należących do gracza 1 zmniejszy się o 1.
 * @param[in] board - plansza gry
 * @param[in] x - nr wiersza danego pola
 * @param[in] y - nr kolumny danego pola
 * @param[in] p - nr danego gracza
 * @param[in] width - wysokość planszy
 * @param[in] height - szerokość planszy
 * @return liczba obszarów danego gracza, do których należą
 * pola sąsiadujące z danym
 */
uint64_t count_adjacent_areas(uint32_t **board, uint32_t x, uint32_t y,
                              uint32_t p, uint32_t width, uint32_t height);


#endif //GAMMA_BOARD_H
