#ifndef LINC_INTERNAL_H
#define LINC_INTERNAL_H

#include <linc.h>
#include <linc/common.h>

#include <stddef.h>
#include <stdlib.h>

#define MAX(a, b) ((a) > (b)) ? (a) : (b)

#define LINC_FATAL(_msg, _site, _type) \
    linc_fatal("" _msg, _site, _type)

extern linc_fatal_target fatal_target;
_Noreturn void linc_fatal(const char *msg, linc_site site, linc_fatal_type type);

#define LINC_ALLOC(_size) linc_alloc(_size, LINC_SITE)
#define LINC_REALLOC(_ptr, _size) linc_realloc(_ptr, _size, LINC_SITE)
#define LINC_CALLOC(_n, _size) linc_calloc(_n, _size, LINC_SITE)
#define LINC_FREE(_ptr) linc_free(_ptr)

void *linc_alloc(size_t size, linc_site site);
void *linc_calloc(size_t nmemb, size_t size, linc_site site);
void *linc_realloc(void *ptr, size_t size, linc_site site);
void linc_free(void *ptr);

#endif
