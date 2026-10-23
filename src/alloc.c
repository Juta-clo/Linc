#include <stddef.h>
#include <stdlib.h>

#include "internal.h"

void *linc_alloc(size_t size, linc_site site) {
    if (size == 0)
        return NULL;

    void *p = malloc(size);
    if (!p)
        LINC_FATAL("malloc", site, LINC_FATAL_OOM);

    return p;
}

void *linc_calloc(size_t n, size_t size, linc_site site) {
    if (n == 0 || size == 0)
        return NULL;

    void *p = calloc(n, size);
    if (!p)
        LINC_FATAL("calloc", site, LINC_FATAL_OOM);

    return p;
}

void *linc_realloc(void *ptr, size_t size, linc_site site) {
    if (size == 0)
        return NULL;

    void *p = realloc(ptr, size);
    if (!p)
        LINC_FATAL("realloc", site, LINC_FATAL_OOM);

    return p;
}

void linc_free(void *ptr) {
    free(ptr);
}
