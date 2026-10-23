#ifndef LINC_H
#define LINC_H

#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include <sys/types.h>

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
    void *user;
} linc_fatal_target;

void linc_fatal_set_target(linc_fatal_target target);

#endif
