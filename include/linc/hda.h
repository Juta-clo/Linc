#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINC_HDA_INIT nullptr

#define LINC_HDA_EL_SZ(hda) \
    sizeof(*(hda))

#define LINC_HDA_PUSH_IMP(_hda_arg, _hda, ...) \
    ({ \
        __auto_type _hda = &_hda_arg; \
        \
        *_hda = linc_hda_grow(*_hda, LINC_HDA_EL_SZ(*_hda), 1); \
        *linc_hda_last(*_hda) = (typeof(*(*_hda)))__VA_ARGS__; \
        linc_hda_last(*_hda); \
    })

#define linc_hda_push(_hda_arg, ...) \
    LINC_HDA_PUSH_IMP((_hda_arg), linc_hda_push_hda, __VA_ARGS__)

#define LINC_HDA_INSERT_IMP(_hda_arg, _index_arg, _n_arg, _hda, _index, _n, ...) \
    ({ \
        __auto_type _hda = &_hda_arg; \
        __auto_type _index = _index_arg; \
        __auto_type _n = _n_arg; \
        \
        (*_hda = linc_hda_insert_imp( \
                *_hda, \
                (typeof(**_hda)[])__VA_ARGS__, \
                LINC_HDA_EL_SZ(*_hda), \
                _index, \
                _n \
        )); \
    })

#define linc_hda_insert(_hda_arg, _index_arg, _n_arg, ...) \
    LINC_HDA_INSERT_IMP( \
        (_hda_arg), (_index_arg), (_n_arg), \
        linc_hda_insert_hda, linc_hda_insert_index, linc_hda_insert_n, \
        __VA_ARGS__ \
    )

#define LINC_HDA_INSERT_BUF_IMP(_hda_arg, _index_arg, _n_arg, \
        _hda, _index, _n, ...) \
    ({ \
        __auto_type _hda = &_hda_arg; \
        __auto_type _index = _index_arg; \
        __auto_type _n = _n_arg; \
        \
        *_hda = linc_hda_insert_imp( \
            *_hda, \
            (__VA_ARGS__), \
            LINC_HDA_EL_SZ(*_hda), \
            _index, \
            _n \
        ); \
        \
        (void)0; \
    })

#define linc_hda_insert_buf(_hda_arg, _index_arg, _n_arg, ...) \
    LINC_HDA_INSERT_BUF_IMP( \
        (_hda_arg), (_index_arg), (_n_arg), \
        linc_hda_insert_buf_hda, linc_hda_insert_buf_index, \
        linc_hda_insert_buf_n, \
        __VA_ARGS__ \
    )

#define LINC_HDA_DELETE_IMP(_hda_arg, _index_arg, _n_arg, _hda, _index, _n) \
    ({ \
        __auto_type _hda = &_hda_arg; \
        __auto_type _index = _index_arg; \
        __auto_type _n = _n_arg; \
        \
        linc_hda_delete_imp(*_hda, LINC_HDA_EL_SZ(*_hda), _index, _n); \
    })

#define linc_hda_delete(_hda_arg, _index_arg, _n_arg, ...) \
    LINC_HDA_DELETE_IMP( \
        (_hda_arg), (_index_arg), (_n_arg), \
        linc_hda_delete_hda, linc_hda_delete_index, \
        linc_hda_delete_n \
    )

#define LINC_HDA_LAST_IMP(_hda_arg, _hda) \
    ({ \
        __auto_type _hda = &_hda_arg; \
        \
        ((typeof(*_hda))linc_hda_last_imp(*_hda, LINC_HDA_EL_SZ(*_hda))); \
    })

#define linc_hda_last(_hda_arg) \
    LINC_HDA_LAST_IMP((_hda_arg), linc_hda_last_hda)

#define LINC_HDA_FREE_IMP(_hda_arg, _hda) \
    do { \
        __auto_type _hda = &_hda_arg; \
        \
        linc_hda_free_imp(*_hda); \
        *_hda = nullptr; \
    } while (false)

#define linc_hda_free(_hda_arg) \
    LINC_HDA_FREE_IMP((_hda_arg), linc_hda_free_hda)

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
