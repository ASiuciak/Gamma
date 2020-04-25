/** @file
 * Interfejs klasy przechowującej stos pól.
 */
#ifndef STACK_H
#define STACK_H

#include "malloc.h"
#include <stdbool.h>

/**
 * Struktura przechowująca parę współrzędnych pola.
 * Podobnie jak w @ref board.h, współrzędna x opisuje
 * nr wiersza, y - nr kolumny.
 */
struct pair{
    /*@{*/
    uint32_t x; /**< nr wiersza pola */
    uint32_t y; /**< nr kolumny pola */
    /*@{*/
};
/**
 * Typ Pair - alias na typ struct pair
 */
typedef struct pair Pair;

/** @brief Zapisuje wspólrzędne pola w struktrze typu Pair.
 * @param[in] x  - nr wiersza pola
 * @param[in] y - nr kolumny pola
 * @return struktura Pair reprezentującą dane pole
 */
Pair make_Pair(uint32_t x, uint32_t y);

/**
 * Struktura wskaźnikowa reprezentująca stos par.
 * Elementy na stosie będą parami współrzędnych pól,
 * stos wykorzystwyany będzie przez algorytm DFS.
 * Stos realizowany jest w formie listy z atrapą.
 * z @ref are_connected.
 */
struct stack{
    /*@{*/
    Pair element; /**< para współrzędnych leżąca na stosie */
    struct stack *next; /**< wskaźnik na kolejny element stosu,
                         * leżący pod aktualnym */
    /*@{*/
};
/**
 * Typ Stack - wskaźnik na strukture typu stack.
 */
typedef struct stack* Stack;

/** @brief Tworzy nowy, pusty stos.
 * @return Wskaźnik na nowy stos, kończy program
 * z kodem błędu 1 w przypadku nieudanej alokacji pamięci.
 */
Stack new_Stack();

/** @brief Kładzie element (typu Pair) na szczycie stosu.
 * @param[in, out] s - wskaźnik na stos
 * @param[in] p - struktura typu Pair do wrzucenia na stos.
 * Kończy program z kodem błędu 1 w przypadku nieudanej alokacji
 * pamięci, z kodem 2, jeśli @param[in, out] s jest NULLem.
 */
void push(Stack s, Pair p);

/** @brief Sprawdza, czy dany stos jest pusty.
 * @param[in] s - wskaźnik na stos
 * @return wartość @p true, jeśli stos jest pusty (czyli jest NULLem
 * lub zawiera samą atrapę), @p false w przeciwnym wypadku.
 */
bool empty(Stack s);

/** @ brief Zwraca element ze szczytu stosu.
 * @param[in, out] s - wskaźnik na stos
 * @return para liczb znajdująca się na szczycie stosu.
 * Kończy program z kodem błedu 2, jeśli stos @param[in, out] s
 * jest pusty.
 */
Pair top(Stack s);

/** @brief Zdejmuje element ze szczytu stosu.
 * Nie robi nic, jęsli stos jest pusty.
 * @param[in, out] s - wskaźnik na stos
 */
void pop(Stack s);

/** Usuwa stos.
 * Zwalnia pamięć zaalokowaną na ten stos.
 * @param[in, out] s - wskaźnik  na stos.
 */
void del(Stack s);

#endif //STACK_H
