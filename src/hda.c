#include <linc/hda.h>
#include <linc/xsys.h>

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "internal.h"

#define LINC_HEAD_N (2 * sizeof(size_t))

#define LINC_HDA_START(arr) \
    ((size_t *)((char *)(arr) - LINC_HEAD_N))

#define LINC_HDA_SIZE(arr) \
    (LINC_HDA_START(arr)[0])

#define LINC_HDA_CAP(arr) \
    (LINC_HDA_START(arr)[1])

static size_t *linc_hda_start(const void *hda) {
    return (hda) ? LINC_HDA_START(hda) : NULL;
}

size_t linc_hda_size(const void *hda) {
    return (hda) ? LINC_HDA_SIZE(hda) : 0;
}

size_t linc_hda_cap(const void *hda) {
    return (hda) ? LINC_HDA_CAP(hda) : 0;
}

void linc_hda_free_imp(void *hda) {
    if (!hda)
        return;

    free(LINC_HDA_START(hda));
}

void *linc_hda_reserve(void *hda, size_t el_sz, size_t min) {
    size_t cap = linc_hda_cap(hda);

    if (cap >= min)
        return hda;

    size_t new_cap = LINC_MAX(min, LINC_MAX((size_t)2, cap * 2));
    void *p = xrealloc(linc_hda_start(hda), LINC_HEAD_N + (new_cap * el_sz));

    hda = (char *)p + LINC_HEAD_N;
    LINC_HDA_CAP(hda) = new_cap;

    return hda;
}

void *linc_hda_last_imp(void *hda, size_t el_sz) {
    if (linc_hda_size(hda) == 0)
        return NULL;

    char *p = hda;
    return &p[(linc_hda_size(hda) - 1) * el_sz];
}

void *linc_hda_grow(void *hda, size_t el_sz, size_t n) {
    size_t size = linc_hda_size(hda);

    hda = linc_hda_reserve(hda, el_sz, size + n);
    LINC_HDA_SIZE(hda) = size + n;

    return hda;
}

void *linc_hda_dup(const void *hda) {
    void *out = xmalloc(LINC_HEAD_N + LINC_HDA_CAP(hda));
    memcpy(out, LINC_HDA_START(hda), LINC_HEAD_N + LINC_HDA_CAP(hda));

    return (char *)out + LINC_HEAD_N;
}

void *linc_hda_insert_imp(void *hda, const void *els,
        size_t el_sz, size_t idx, size_t n)
{
    assert(els);
    assert(el_sz > 0);
    assert(idx <= linc_hda_size(hda));

    if (n == 0)
        return hda;

    if (n > SIZE_MAX / el_sz)
        linc_fatal("allocation overflow", LINC_SITE, LINC_FATAL_LIB);

    /* to guard against overlapping hda and el on realloc.
     * Can avoid the malloc on every insert by checking for
     * address overlap (uintptr_t) */
    void *el_copy = xmalloc(el_sz * n);

    memcpy(el_copy, els, el_sz * n);

    size_t size = linc_hda_size(hda);
    hda = linc_hda_grow(hda, el_sz, n);

    char *p = hda;
    memmove(&p[(idx + n) * el_sz], &p[idx * el_sz], (size - idx) * el_sz);
    memcpy(&p[idx * el_sz], el_copy, el_sz * n);

    free(el_copy);
    return hda;
}

void linc_hda_delete_imp(void *hda, size_t el_sz, size_t index,
        size_t n)
{
    assert(el_sz > 0);
    assert(index + n <= linc_hda_size(hda));

    if (n == 0)
        return;

    char *p = hda;
    size_t shift_n = LINC_HDA_SIZE(hda) - (index + n);

    memmove(&p[index * el_sz], &p[(index + n) * el_sz], shift_n * el_sz);

    LINC_HDA_SIZE(hda) -= n;
}
