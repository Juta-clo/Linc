#ifndef LINC_TOOLS_HDS_S
#define LINC_TOOLS_HDS_S

#include <string.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define LINC_HDS_INIT nullptr

#define linc_hds_append(hds, s) \
    ({ \
        __auto_type linc_hds_append_ptr = &(hds); \
        (*linc_hds_append_ptr = \
            linc_hds_append_imp(*linc_hds_append_ptr, s, strlen(s))); \
    })

#define linc_hds_append_n(hds, s, n) \
    ({ \
        __auto_type linc_hds_append_n_ptr = &(hds); \
        (*linc_hds_append_n_ptr = \
            linc_hds_append_imp(*linc_hds_append_n_ptr, s, n)); \
    })

#define linc_hds_printf(hds, fmt, ...) \
    ({ \
        __auto_type linc_hds_printf_ptr = &(hds); \
        (*linc_hds_printf_ptr = linc_hds_append_fmt_imp(*linc_hds_printf_ptr, \
            fmt __VA_OPT__(,) __VA_ARGS__)); \
    })

#define linc_hds_copy(hds, s) \
    ({ \
        __auto_type linc_hds_copy_ptr = &(hds); \
        (*linc_hds_copy_ptr = \
            linc_hds_copy_imp(*linc_hds_copy_ptr, s, strlen(s))); \
    })

#define linc_hds_copy_n(hds, s, n) \
    ({ \
        __auto_type linc_hds_copy_n_ptr = &(hds); \
        (*linc_hds_copy_n_ptr = linc_hds_copy_imp(*linc_hds_copy_n_ptr, s, n)); \
    })

size_t linc_hds_len(const void *hds);
void *linc_hds_append_imp(void *hds, const void *b, size_t n);
void *linc_hds_copy_imp(char *hds, const char *s, size_t n);
void linc_hds_free(void *hds);

__attribute__ ((__format__(printf, 2, 3)))
void *linc_hds_append_fmt_imp(void *hds, const char *fmt, ...);

#endif
