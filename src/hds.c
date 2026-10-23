#include <linc/hds.h>
#include <linc/hda.h>

#include <string.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "internal.h"

size_t linc_hds_len(const void *hds) {
    size_t size = linc_hda_size(hds);
    return (size == 0) ? 0 : size - 1;
}

void *linc_hds_append_imp(void *hds, const void *b, size_t n) {
    size_t size = linc_hda_size(hds);
    size_t idx = (size == 0) ? 0 : size - 1;

    hds = linc_hda_insert_imp(hds, b, sizeof(char), idx, n);

    if (size == 0) {
        hds = linc_hda_grow(hds, sizeof(char), 1);
        *(char *)linc_hda_last_imp(hds, 1) = '\0';
    }

    return hds;
}

void *linc_hds_append_fmt_imp(void *hds, const char *fmt, ...) {
    va_list va;
    char *s;

    va_start(va, fmt);
    int n = vasprintf(&s, fmt, va);
    va_end(va);

    if (n == -1)
        LINC_FATAL("vasprintf: failed", LINC_SITE, LINC_FATAL_LIB);

    linc_hds_append_n(hds, s, n);

    linc_free(s);

    return hds;
}

void *linc_hds_copy_imp(char *hds, const char *s, size_t n) {
    linc_hda_delete(hds, 0, linc_hda_size(hds));
    linc_hds_append_n(hds, s, n);

    return hds;
}

void linc_hds_free(void *hds) {
    linc_hda_free(hds);
}
