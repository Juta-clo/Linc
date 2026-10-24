#include <linc/string.h>

#include <stddef.h>
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <linc.h>
#include "internal.h"

void linc_string_free(linc_string *str) {
    assert(str);

    if (str->cap > 0)
        linc_free(str->cstr, 0, &str->allo);

    *str = LINC_STRING_INIT;
}

void linc_string_reserve(linc_string *str, size_t min) {
    assert(str);

    if (str->cap >= ++min)
        return;

    if (str->cap == 0)
        str->cstr = nullptr;

    size_t new_cap = LINC_MAX(min, LINC_MAX((size_t)2, str->cap * 2));

    str->cstr = linc_realloc(
        str->cstr, new_cap, str->cap, &str->allo, LINC_SITE
    );

    if (str->cap == 0)
        str->cstr[0] = '\0';

    str->cap = new_cap;
}

void linc_string_push(linc_string *str, char c) {
    assert(str);

    linc_string_reserve(str, str->len++ + 1);
    str->cstr[str->len - 1] = c;
    str->cstr[str->len] = '\0';
}

void linc_string_clear(linc_string *str) {
    assert(str);

    str->cstr[0] = '\0';
    str->len = 0;
}

void linc_string_append(linc_string *str, const char *src) {
    assert(str);
    assert(src);

    size_t src_len = strlen(src);

    linc_string_reserve(str, str->len + src_len);
    strncat(str->cstr, src, src_len);

    str->len += src_len;
}

void linc_string_appendf(linc_string *str, const char *fmt, ...) {
    assert(str && fmt);

    va_list va;
    char *p;

    va_start(va, fmt);
    if (vasprintf(&p, fmt, va) < 0)
        linc_fatal("vasprintf: failed", LINC_SITE, LINC_FATAL_LIB);
    va_end(va);

    linc_string_append(str, p);
    free(p);
}

void linc_string_set(linc_string *str, const char *src) {
    assert(str && src);

    size_t src_len = strlen(src);

    linc_string_reserve(str, str->len + src_len);
    strncpy(str->cstr, src, src_len);

    str->len = src_len;
    str->cstr[str->len] = '\0';
}

void linc_string_set_n(linc_string *str, const char *src, size_t n) {
    assert(str && src);

    linc_string_reserve(str, n);
    strncpy(str->cstr, src, n);

    str->len = n;
    str->cstr[str->len] = '\0';
}

void linc_string_append_n(linc_string *str, const char *src, size_t n) {
    assert(str && src);

    linc_string_reserve(str, str->len + n);
    strncat(&str->cstr[str->len], src, n);

    str->len += n;
}
