#ifndef LINC_H
#define LINC_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

#define LINC_SITE \
    (linc_site){ \
        .file = __FILE__, \
        .func = __func__, \
        .line = __LINE__ \
    }

#define LINC_MAX(_a, _b) \
    ({ \
        __auto_type _linc_max_a = _a; \
        __auto_type _linc_max_b = _b; \
    \
        ((_linc_max_a) > (_linc_max_b)) ? (_linc_max_a) : (_linc_max_b); \
    })

#define LINC_MIN(_a, _b) \
    ({ \
        __auto_type _linc_min_a = _a; \
        __auto_type _linc_min_b = _b; \
    \
        ((_linc_min_a) < (_linc_min_b)) ? (_linc_min_a) : (_linc_min_b); \
    })

typedef struct {
    void *(*alloc)(size_t size, void *ctx);
    void (*free)(void *ptr, size_t size, void *ctx);
    void *ctx;
} linc_allo;

typedef enum {
    LINC_FATAL_OOM,
    LINC_FATAL_LIB,
    LINC_FATAL_SYS
} linc_fatal_type;

typedef struct {
    const char *file;
    const char *func;
    int line;
} linc_site;

typedef struct {
    linc_fatal_type type;
    const char *msg;
    int saved_errno;
    pid_t pid;
    pid_t ppid;
    pid_t pgid;
    linc_site site;
} linc_fatal_ctx;

typedef void (linc_fatal_handler)(const linc_fatal_ctx *, void *);

typedef struct {
    linc_fatal_handler *handler;
    void *ctx;
} linc_fatal_target;

void linc_fatal_set_target(linc_fatal_target target);

/* Allocation */
void *linc_alloc(size_t size, linc_allo *allo, linc_site site);
void linc_free(void *ptr, size_t size, linc_allo *allo);
void *linc_realloc(void *ptr, size_t new_size, size_t old_size,
        linc_allo *allo, linc_site site);

#endif
