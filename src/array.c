#include <linc/array.h>

#include <stdlib.h>

#include "internal.h"

void linc_array_free_imp(void *data) {
    LINC_FREE(data);
}

void *linc_array_reserve_imp(void *data, size_t el_sz, size_t min, size_t *cap) {
    if (*cap >= min)
        return data;

    *cap = MAX(min, MAX(2, *cap * 2));
    void *tmp = LINC_REALLOC(data, *cap * el_sz);
    data = tmp;

    return data;
}
