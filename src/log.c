#include <linc/log.h>
#include <linc/string.h>

#include <assert.h>
#include <execinfo.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>

#include "internal.h"

#define LOG_TYPE_MAX 3

static char *level_strings[LOG_TYPE_MAX] = {
    "INFO", "WARN", "ERROR"
};

static linc_string convert_special_chars(char *output_msg) {
    linc_string out = LINC_STRING_INIT;

    for (char *c = output_msg; *c != '\0'; ++c) {
        switch (*c) {
        case '\n': linc_string_append(&out, "\\n"); break;
        case '\t': linc_string_append(&out, "\\t"); break;
        case '\r': linc_string_append(&out, "\\r"); break;
        case '\b': linc_string_append(&out, "\\b"); break;
        case '\a': linc_string_append(&out, "\\a"); break;
        case '\f': linc_string_append(&out, "\\f"); break;
        case '\v': linc_string_append(&out, "\\v"); break;
        default:
            if (*c < 0x20) { /* fallback to caret notation */
                linc_string_push(&out, '^');
                linc_string_push(&out, *c + '@');
            } else if (*c == 0x7F) { /*DEL*/
                linc_string_push(&out, *c);
            } else {
                linc_string_push(&out, *c);
            }
        }
    }

    return out;
}

static void stderr_sink(const linc_log_ctx *ctx, void *_) {
    fprintf(stderr, "%s %s:%d: %s: %s\n",
            linc_log_level_str(ctx->level),
            ctx->site.file, ctx->site.line, ctx->site.func, ctx->msg);
}

linc_log linc_log_default = { .sink = stderr_sink };

void linc_log_set_default(linc_log log) {
    if (!log.sink) {
        linc_log_default = (linc_log){ .sink = stderr_sink };
    } else {
        linc_log_default = log;
    }
}

char *linc_log_level_str(linc_log_level level) {
    assert(level >= 0 && level < LOG_TYPE_MAX);
    return level_strings[level];
}

void linc_log_write(linc_log log, linc_log_level level, linc_site site,
        const char *fmt, ...) {
    assert(fmt);

    if (!log.sink)
        return;

    int saved_errno = errno;

    linc_log_ctx ctx = {0};
    va_list va;

    ctx.level = level;
    ctx.site = site;
    ctx.saved_errno = saved_errno;
    ctx.pid = getpid();
    ctx.ppid = getppid();
    ctx.pgid = getpgrp();

    char *p;

    va_start(va, fmt);
     if (vasprintf(&p, fmt, va) < 0)
         linc_fatal("vasprintf", LINC_SITE, LINC_FATAL_SYS);
     va_end(va);

    linc_string msg = convert_special_chars(p);

    ctx.msg = msg.cstr;
    log.sink(&ctx, log.user);

    free(p);
    linc_string_free(&msg);

    errno = saved_errno;
    return;
}
