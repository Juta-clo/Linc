#include <linc.h>

#include <errno.h>
#include <execinfo.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "internal.h"

#define LINC_FATAL_HAS_SID   1

_Noreturn void linc_fatal(const char *msg, linc_site site, linc_fatal_type type) {
    linc_fatal_ctx ctx;

    ctx.saved_errno = errno;

    if (type == LINC_FATAL_SYS && ctx.saved_errno == ENOMEM) {
        ctx.type = LINC_FATAL_OOM;
    } else {
        ctx.type = type;
    }

    ctx.pid = getpid();
    ctx.ppid = getppid();
    ctx.pgid = getpgrp();
    ctx.site = site;
    ctx.msg = msg;

    fatal_target.handler(&ctx, fatal_target.user);
    abort();
}

void fatal_default_handler(const linc_fatal_ctx *ctx, void *_) {
    fputs("fatal: ", stderr);

    if (ctx->type == LINC_FATAL_OOM) {
        fputs(ctx->msg, stderr);
        fputs(": out of memory", stderr);
    } else {
        fprintf(stderr, "%s:%d %s(): %s",
                ctx->site.file, ctx->site.line, ctx->site.func, ctx->msg);

        if (ctx->type == LINC_FATAL_SYS)
            fprintf(stderr, ": %s", strerror(ctx->saved_errno));
    }

    fputs("\n", stderr);
}

linc_fatal_target fatal_target = { .handler = fatal_default_handler, };

void linc_fatal_set_target(linc_fatal_target target) {
    if (target.handler) {
        fatal_target = target;
    } else {
        fatal_target = (linc_fatal_target){
            .handler = fatal_default_handler,
        };
    }
}
