#ifndef LINC_ARGS_H
#define LINC_ARGS_H

#define LINC_ARGS_SILENT      1
#define LINC_ARGS_NO_NON_OPTS 2

#define LINC_ARGS_ARG_INT 1
#define LINC_ARGS_ARG_STR 2

/* These are passed directly to getopt, don't change the value! */
#define LINC_ARGS_ARG_NONE 0
#define LINC_ARGS_ARG_REQ  1
#define LINC_ARGS_ARG_OPT  2

typedef struct {
    char *long_name;
    char short_name;
    int arg_type;
    int has_arg;
    bool found;
    bool opt_arg_found;
    union {
        int val_int;
        char *val_str;
    };
} linc_args;

int linc_args_parse(int argc, char *const *argv, linc_args *opts, int flags);

#endif
