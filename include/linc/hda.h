#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINC_HDA_INIT nullptr

#define LINC_HDA_EL_SZ(hda) \
    sizeof(*(hda))

#define linc_hda_push(hda, ...) \
    ({ \
        __auto_type linc_hda_push_ptr = &(hda); \
        *linc_hda_push_ptr = linc_hda_grow(*linc_hda_push_ptr, \
                LINC_HDA_EL_SZ(*linc_hda_push_ptr), 1); \
        *linc_hda_last(*linc_hda_push_ptr) = \
            (typeof(*(*linc_hda_push_ptr)))__VA_ARGS__; \
        linc_hda_last(*linc_hda_push_ptr); \
    })

#define linc_hda_insert(hda, index, n, ...) \
    ({ \
        __auto_type linc_hda_insert_ptr = &(hda); \
        (*linc_hda_insert_ptr = linc_hda_insert_imp( \
                *linc_hda_insert_ptr, \
                (typeof(**linc_hda_insert_ptr)[])__VA_ARGS__, \
                LINC_HDA_EL_SZ(*linc_hda_insert_ptr), \
                index, \
                n \
        )); \
    })


#define linc_hda_insert_buf(hda, index, n, ...) \
    ({ \
        __auto_type linc_hda_insert_ptr = &(hda); \
        (*linc_hda_insert_ptr = linc_hda_insert_imp( \
                *linc_hda_insert_ptr, \
                (__VA_ARGS__), \
                LINC_HDA_EL_SZ(*linc_hda_insert_ptr), \
                index, \
                n \
        )); \
    })

#define linc_hda_delete(hda, index, n) \
    ({ \
        __auto_type linc_hda_delete_ptr = &(hda); \
        linc_hda_delete_imp(*linc_hda_delete_ptr, \
                LINC_HDA_EL_SZ(*linc_hda_delete_ptr), \
                index, n); \
    })

#define linc_hda_last(hda) \
    ({ \
        __auto_type linc_hda_last_ptr = &(hda); \
        ((typeof(*linc_hda_last_ptr))linc_hda_last_imp(*linc_hda_last_ptr, \
            LINC_HDA_EL_SZ(*linc_hda_last_ptr))); \
    })

#define linc_hda_free(hda) \
    do { \
        __auto_type linc_hda_ptr = &(hda); \
        linc_hda_free_imp(*linc_hda_ptr); \
        *linc_hda_ptr = nullptr; \
    } while (false)

size_t linc_hda_size(const void *hda);
size_t linc_hda_cap(const void *hda);
void linc_hda_free_imp(void *hda);
void *linc_hda_reserve(void *hda, size_t el_sz, size_t min);
void *linc_hda_last_imp(void *hda, size_t el_sz);
void *linc_hda_grow(void *hda, size_t el_sz, size_t n);
void *linc_hda_dup(const void *hda);
void *linc_hda_insert_imp(void *hda, const void *els,
        size_t el_sz, size_t idx, size_t n);
void linc_hda_delete_imp(void *hda, size_t el_sz, size_t index, size_t n);
