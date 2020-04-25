#include <stdlib.h>
#include <string.h>
#include "stringBuilder.h"

/* Przyjęta przeze mnie początkowa ilość pamięci do
 * zaalokowania na nowe słowo.
 */
const size_t init_cap = 2;

String newString() {
    String n = NULL;
    n = safe_malloc(n, sizeof(struct stringBuilder));
    n->word = safe_malloc(n->word, init_cap);
    *n->word = '\0';
    n->capacity = init_cap;
    n->length = 0;
    return n;
}

bool ensureSpace(String s, size_t addLen) {
    if (s == NULL) { return false; }
    if (addLen == 0) { return true; }

    if (s->capacity >= s->length + addLen + 1) { return true; }

    while (s->capacity < s->length + addLen + 1) {
        s->capacity *= 2;
        if (s->capacity == 0) {
            s->capacity--;
        }
    }
    s->word = realloc(s->word, s->capacity);
    if (s->word == NULL) { return false; }
    return true;
}

bool append(String s, char c) {
    if (s == NULL) { return false; }
    if (!ensureSpace(s, 1)) { return false; }
    s->word[s->length] = c;
    s->length++;
    s->word[s->length] = '\0';
    return true;
}

bool append_number(String s, uint32_t num) {
    // Kropka reprezentuje wolne pole.
    if (num == 0) {
        return append(s, '.');
    }

    // Więcej niż 1-cyfrowe numery graczy oddzielamy spacjami.
    if (num > 9) {
        if (!append(s, ' ')) {
            return false;
        }
    }

    // Dodajemy kolejne cyfry liczby.
    uint32_t actual = num;
    while (actual > 0) {
        uint32_t last = num % 10;
        char c = last + '0';
        if (!append(s, c)) {
            return false;
        }
        actual = actual / 10;
    }

    // Więcej niż 1-cyfrowe numery graczy oddzielamy spacjami.
    if (num > 9) {
        if (!append(s, ' ')) {
            return false;
        }
    }
    return true;
}