#include "malloc.h"

void *safe_malloc(void *p, size_t s) {
    p = malloc(s);
    if (p == NULL) {
        exit(1);
    }
    return p;
}

