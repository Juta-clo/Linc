#ifndef LINC_STRING_H
#define LINC_STRING_H

#include <linc.h>

#include <stddef.h>

#define LINC_STRING_INIT (linc_string){ .cstr = "" }

typedef struct {
    char *cstr;
    size_t len;
    size_t cap;
    linc_allo allo;
} linc_string;

void linc_string_free(linc_string *str);
void linc_string_reserve(linc_string *str, size_t min);
void linc_string_push(linc_string *str, char c);
void linc_string_clear(linc_string *str);
void linc_string_set(linc_string *dst, const char *src);
void linc_string_append(linc_string *dst, const char *src);
void linc_string_set_n(linc_string *dst, const char *src, size_t n);
void linc_string_append_n(linc_string *dst, const char *src, size_t n);
void linc_string_setf(linc_string *dst, const char *fmt, ...);

__attribute__ ((__format__(printf, 2, 3)))
void linc_string_appendf(linc_string *dst, const char *fmt, ...);

#endif
