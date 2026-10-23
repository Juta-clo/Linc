#include <linc/args.h>

#include <stddef.h>
#include <assert.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <stdbool.h>

#include "internal.h"

__attribute__ ((format(printf, 1, 2)))
static void linc_args_report_err(const char *fmt, ...) {
    if (opterr == 0)
        return;

    va_list va;

    va_start(va, fmt);
    vfprintf(stderr, fmt, va);
    va_end(va);
}

static int linc_args_get_int(char *s, int *out) {
    assert(s && out);

    if (*s == '\0')
        return -1;

    char *endptr;
    long num = strtol(s, &endptr, 10);

    if (num < INT_MIN || num > INT_MAX)
        return -1;

    if (*endptr != '\0')
        return -1;

    *out = num;
    return 0;
}

static void linc_args_parse_opt_arg_str(char *opt_arg, linc_args *data) {
    if (!opt_arg)
        return;

    data->val_str = opt_arg;
}

static int linc_args_parse_opt_arg_int(char *opt_arg, linc_args *data) {
    if (!opt_arg) {
        data->opt_arg_found = false;
        return 0;
    }

    int num;
    if (linc_args_get_int(opt_arg, &num) == -1)
        return -1;

    data->val_int = num;
    data->opt_arg_found = true;

    return 0;
}

static linc_args *linc_args_lookup_opt_by_short_opt(linc_args *opts,
        char short_opt)
{
    for (linc_args *opt = opts;; ++opt) {
        if (memcmp(opt, &(linc_args){0}, sizeof(linc_args)) == 0)
            break;

        if (short_opt == opt->short_name)
            return opt;
    }

    return NULL;
}

inline int linc_args_parse(int argc, char * const *argv, linc_args *opts,
        int flags)
{
    assert(argv && opts);

    optind = 0;

    int saved_opterr = opterr;

    if (flags & LINC_ARGS_SILENT)
        opterr = 0;

    size_t opt_n = 0;
    for (linc_args *opt = opts;; ++opt) {
        if (memcmp(opt, &(linc_args){0}, sizeof(linc_args)) == 0)
            break;
        ++opt_n;
    }

    int long_optind;

    struct option *long_opts = LINC_CALLOC(opt_n + 1, sizeof(*long_opts));
    char *short_opts = LINC_CALLOC(sizeof(char), opt_n * 3 + 1);

    for (size_t i = 0, y = 0; i < opt_n; ++i, ++y) {
        long_opts[i].name    = opts[i].long_name;
        long_opts[i].val     = opts[i].short_name;
        long_opts[i].has_arg = opts[i].has_arg;
        long_opts[i].flag    = NULL;

        short_opts[y] = opts[i].short_name;

        if (opts[i].has_arg == LINC_ARGS_ARG_REQ)
            short_opts[++y] = ':';
        if (opts[i].has_arg == LINC_ARGS_ARG_OPT) {
            short_opts[++y] = ':';
            short_opts[++y] = ':';
        }
    }

    while (true) {
        char c = getopt_long(argc, argv, short_opts, long_opts, &long_optind);

        if (c == -1) /* done parsing */
            break;

        if (c == '?') /* unrecognized option or no arg when required */
            goto fail;

        linc_args *opt = linc_args_lookup_opt_by_short_opt(opts, c);
        if (!opt) {
            linc_args_report_err("%s: unexpected opt value: %c\n", argv[0], c);
            goto fail;
        }

        opt->found = true;

        if (opt->has_arg == LINC_ARGS_ARG_NONE)
            continue;

        switch (opt->arg_type) {
        case LINC_ARGS_ARG_INT:
            if (linc_args_parse_opt_arg_int(optarg, opt) == -1) {
                linc_args_report_err("%s: bad arg to '--%s' -- '%s'\n",
                        argv[0], opt->long_name, optarg);
                goto fail;
            }
            break;
        case LINC_ARGS_ARG_STR:
            linc_args_parse_opt_arg_str(optarg, opt);
            break;
        }
    }

    if (flags & LINC_ARGS_NO_NON_OPTS) {
        if (optind < argc) {
            linc_args_report_err("%s: unrecognized argv elements: ", argv[0]);
            while (optind < argc)
                linc_args_report_err("%s ", argv[optind++]);
            linc_args_report_err("\n");

            goto fail;
        }
    }

    LINC_FREE(long_opts);
    LINC_FREE(short_opts);
    opterr = saved_opterr;

    return 0;

fail:
    LINC_FREE(long_opts);
    LINC_FREE(short_opts);
    opterr = saved_opterr;

    return -1;
}
