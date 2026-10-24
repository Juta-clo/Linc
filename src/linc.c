#include <errno.h>
#include <unistd.h>
#include <linc.h>
#include <stdio.h>
#include <string.h>

#include "internal.h"

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

static linc_fatal_target fatal_target = { .handler = fatal_default_handler, };

void linc_fatal_set_target(linc_fatal_target target) {
    if (target.handler) {
        fatal_target = target;
    } else {
        fatal_target = (linc_fatal_target){
            .handler = fatal_default_handler,
        };
    }
}

void linc_fatal(const char *msg, linc_site site, linc_fatal_type type) {
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

    fatal_target.handler(&ctx, fatal_target.ctx);
    abort();
}

void *linc_alloc(size_t size, linc_allo *allo, linc_site site) {
    void *p;

    if (!allo->alloc) {
        if (!(p = malloc(size)))
            linc_fatal("malloc", site, LINC_FATAL_OOM);
    } else if (!(p = allo->alloc(size, allo->ctx))) {
        linc_fatal("linc_alloc", site, LINC_FATAL_OOM);
    }

    return p;
}

void linc_free(void *ptr, size_t size, linc_allo *allo) {
    if (!allo->free) {
        free(ptr);
        return;
    }

    allo->free(ptr, size, allo->ctx);
}

void *linc_realloc(void *ptr, size_t new_size, size_t old_size,
                   linc_allo *allo, linc_site site) {
    if (!ptr)
        return linc_alloc(new_size, allo, site);

    if (new_size < old_size)
        old_size = new_size;

    void *p = linc_alloc(new_size, allo, site);
    memcpy(p, ptr, old_size);
    linc_free(ptr, 0, allo);

    return p;
}
