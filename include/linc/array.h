#ifndef LINC_ARRAY_H
#define LINC_ARRAY_H

#include <linc.h>

#define LINC_ARRAY_INIT {0}

#define LINC_ARRAY_TYPE(name, type) \
    typedef struct { \
        type *data; \
        size_t size; \
        size_t cap; \
        linc_allo allo; \
    } name;

#define LINC_ARRAY_FREE_IMP(_arr_arg, _arr) \
    ({ \
        __auto_type _arr = (_arr_arg); \
        \
        linc_free(_arr->data, 0, &_arr->allo); \
        *_arr = (typeof(*_arr)){0}; \
        \
        (void)0; \
    })

#define linc_array_free(_arr_arg) \
    LINC_ARRAY_FREE_IMP(_arr_arg, linc_array_free_arr)

#define LINC_ARRAY_RESERVE_IMP(_arr_arg, _min_arg, _arr, _min, _el_sz, _new_cap) \
    ({ \
        __auto_type _arr = _arr_arg; \
        size_t _min = _min_arg; \
        size_t _el_sz = sizeof(*_arr->data); \
        size_t _new_cap = LINC_MAX(_min, LINC_MAX((size_t)2, _arr->cap * 2)); \
        \
        if (_arr->cap < _min) { \
            _arr->data = linc_realloc(\
                _arr->data, \
                _new_cap * _el_sz, \
                _arr->cap * _el_sz, \
                &_arr->allo, \
                LINC_SITE \
            ); \
            _arr->cap = _new_cap; \
        } \
        \
        (void)0; \
    })

#define linc_array_reserve(_arr_arg, _min_arg) \
    LINC_ARRAY_RESERVE_IMP( \
        (_arr_arg), (_min_arg), \
        linc_array_reserve_arr, \
        linc_array_reserve_min, \
        linc_array_reserve_el_sz, \
        linc_array_reserve_new_cap \
    )

#define LINC_ARRAY_PUSH_IMP(_arr_arg, _arr) \
    ({ \
        __auto_type _arr = (_arr_arg); \
        \
        linc_array_reserve(_arr, _arr->size + 1); \
        &(_arr->data[_arr->size++]); \
    })

#define linc_array_push(_arr_arg) \
    LINC_ARRAY_PUSH_IMP((_arr_arg), linc_array_push_arr)

#define LINC_ARRAY_DELETE_IMP(_arr_arg, _index_arg, _arr, _index) \
    ({ \
        __auto_type _arr = _arr_arg; \
        size_t _index = _index_arg; \
        assert(_index < _arr->size); \
        \
        memmove( \
            &_arr->data[_index], &_arr->data[_index + 1], \
            (_arr->size - _index - 1) * sizeof(*_arr->data) \
        ); \
        \
        --_arr->size; \
        (void)0; \
    })

#define linc_array_delete(_arr_arg, _index_arg) \
    LINC_ARRAY_DELETE_IMP( \
        (_arr_arg), (_index_arg), \
        linc_array_delete_arr, \
        linc_array_delete_index \
    )

#define LINC_ARRAY_INSERT_IMP(_arr_arg, _index_arg, _arr, _index) \
    ({ \
        __auto_type _arr = (_arr_arg); \
        size_t _index = (_index_arg); \
        assert(_index <= _arr->size); \
        \
        linc_array_reserve(_arr, \
                _arr->size + 1); \
        \
        memmove( \
            &_arr->data[_index + 1], &_arr->data[_index], \
            (_arr->size - _index) * sizeof(*_arr->data) \
        ); \
        \
        ++_arr->size; \
        &(_arr->data[_index]); \
    })

#define linc_array_insert(_arr_arg, _index_arg) \
    LINC_ARRAY_INSERT_IMP( \
        (_arr_arg), (_index_arg), \
        linc_array_insert_arr, \
        linc_array_insert_index \
    )

#endif
