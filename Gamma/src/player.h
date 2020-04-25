/** @file
 * Interfejs klasy przechowującej gracza i
 * jego statystyki w grze.
 * Definicja:
 * Pole przyległe dla danego gracza - pole sąsiadujące
 * z co najmniej jednym polem należącym do danego gracza.
 * Pola przyległe to jedyne pola, na których gracz może postawić
 * swój pionek bez zwiększenia liczby swoich obszarów, dlatego
 * ich liczenie na bieżąco jest niezbędne do szybkiego określenia
 * ile pól może zająć dany gracz (@ref gamma_free_fields).
 */
#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include <stdint.h>

/**
 * Struktrura reprezentująca gracza.
 */
struct player{
    /*@{*/
    uint32_t areas; /**< liczba obszarów gracza */
    uint64_t occupied; /**< liczba pól zajętych przez gracza */
    uint64_t adjacent_free; /**< liczba wolnych pól przyległych dla gracza */
    bool golden; /**< możliwość wykonania złotego ruchu */
    /*@{*/

};
/**
 * Typ Player - alias na struct player.
 */
typedef struct player Player;

/** @brief Tworzy nowego gracza.
 * @return Struktura reprezentująca gracza rozpoczynającego grę.
 */
Player new_Player();

#endif //PLAYER_H
