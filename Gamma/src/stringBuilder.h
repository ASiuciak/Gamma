/** @file
 * Interfejs przechowujący napisy.
 */

#ifndef STRINGBUILDER_H
#define STRINGBUILDER_H

#include "malloc.h"
#include <stddef.h>
#include <stdbool.h>

/**
 * Struktura przechowująca słowa.
 */
struct stringBuilder {
    /*@{*/
    char *word; /**< właściwe słowo */
    size_t length; /**< długość słowa */
    size_t capacity; /**< ilosć pamięci atualnie zaalokowanej na słowo */
    /*@{*/
};

/**
 * Typ String - wskaźnik na strukturę typu stringBuilder.
 */
typedef struct stringBuilder *String;

/** @brief Tworzy nowy obiekt typu String.
 * @return Nowy obiekt typu String z pustym słowem w środku, kończy
 * program z kodem błędu 1, jeśli nie udało się zaalokować pamięci.
 */
String newString();

/** @brief Tworzy miejsce na dodanie okreslonej ilości znaków.
 * @param[in] s - obiekt typu String przechowujący dane słowo
 * @param[in] addLen - liczba znaków, o które chcemy wydłużyć słowo
 * @return wartość @p true, jeśli udało się zaalokować pamięć na dodatkowe
 * znaki, false w przeciwnym wypadku
 */
bool ensureSpace(String s, size_t addLen);

/** @brief Dopisuje znak na końcu słowa.
 * @param[in] s - obiekt typu String przechowujący dane słowo
 * @param[in] c - znak, który chcemy dodać na końcu słowa
 * @return wartość @p true, jeśli z powodzeniem dodano znak,
 * @p false, jeśli nie udało się zaalokować pamięci.
 */
bool append(String s, char c);

/** @brief Dopisuje znak lub ciąg znaków reprezentujących liczbę
 * na końcu słowa. Liczby składające się z więcej niż 1 cyfry
 * oddziela spacją.
 * @param[in] s - obiekt typu String przechowujący dane słowo
 * @param[in] num - liczba, której reprezentację checmy dodać na końcu słowa
 * @return wartość @p true, jesli z powodzeniem dodano znak(i),
 * @p false, jeśli nie udało się zaalokować pamięci.
 */
bool append_number(String s, uint32_t num);

#endif // STRINGBUILDER_H
