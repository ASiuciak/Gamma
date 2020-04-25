/** @file
 * Interfejs funkcji odpowiedzialnych za
 * bezpieczną alokację pamięci.
 */

#ifndef MALLOC_H
#define MALLOC_H

#include <stdlib.h>
#include <stdint.h>

/** @brief Alokuje daną ilość pamięci na dane miejsce.
 * @param[in, out] p - wskaźnik na miejsce, w którym będziemy trzymać
 * zaalokowaną pamięć
 * @param s - ilosć pamięci do zaalokowania
 * @return - wskaźnik na zaalokowaną pamięć. Kończy program z kodem
 * błędu 1 w przypadku nieudanej alokacji.
 */
void *safe_malloc(void *p, size_t s);

#endif //MALLOC_H
