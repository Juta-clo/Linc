#ifndef LINC_LOG_H
#define LINC_LOG_H

#include <assert.h>
#include <execinfo.h>
#include <stdarg.h>
#include <stddef.h>
#include <unistd.h>

#include "linc.h"
#include "common.h"

#define LINC_LOG_INFO(...) \
    linc_log_write(linc_log_default, LINC_LOG_INFO, LINC_SITE, __VA_ARGS__)

#define LINC_LOG_WARN(...) \
    linc_log_write(linc_log_default, LINC_LOG_WARN, LINC_SITE, __VA_ARGS__)

#define LINC_LOG_ERROR(...) \
    linc_log_write(linc_log_default, LINC_LOG_ERROR, LINC_SITE, __VA_ARGS__)

typedef enum {
    LINC_LOG_INFO,
    LINC_LOG_WARN,
    LINC_LOG_ERROR,
} linc_log_level;

typedef struct {
    linc_log_level level;
    linc_site site;
    const char *msg;
    int saved_errno;
    pid_t pid;
    pid_t ppid;
    pid_t pgid;
} linc_log_ctx;

typedef void (linc_log_sink)(const linc_log_ctx *, void *);

typedef struct {
    linc_log_sink *sink;
    void *user;
} linc_log;

extern linc_log linc_log_default;

__attribute__ ((__format__(printf, 4, 5)))
void linc_log_write(
    linc_log log, linc_log_level level, linc_site site,
    const char *fmt, ...
);

void linc_log_set_default(linc_log log);
char *linc_log_level_str(linc_log_level level);

#endif
