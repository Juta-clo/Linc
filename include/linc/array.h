#ifndef LINC_ARRAY_H
#define LINC_ARRAY_H

#include <stdlib.h>
#include <string.h>

#define LINC_ARRAY_INIT {0}

#define LINC_ARRAY_TYPE(type) \
    typedef struct { \
        type *data; \
        size_t size; \
        size_t cap; \
    } linc_array_##type;

#define linc_array_free(_arr) \
    ({ \
        __auto_type linc_array_free_arr = (_arr); \
        \
        linc_array_free_imp(linc_array_free_arr->data); \
        *linc_array_free_arr = (typeof(*linc_array_free_arr)){0}; \
    })

#define linc_array_reserve(_arr, _min) \
    ({ \
        __auto_type linc_array_reserve_arr = (_arr); \
        size_t linc_array_reserve_min = (_min); \
        \
        linc_array_reserve_arr->data = linc_array_reserve_imp( \
            linc_array_reserve_arr->data, \
            sizeof(*linc_array_reserve_arr->data), \
            linc_array_reserve_min, \
            &linc_array_reserve_arr->cap \
        ); \
    })

#define linc_array_push(_arr) \
    ({ \
        __auto_type linc_array_push_arr = (_arr); \
        \
        linc_array_reserve(linc_array_push_arr, \
                linc_array_push_arr->size + 1); \
        &(linc_array_push_arr->data[linc_array_push_arr->size++]); \
    })

#define linc_array_delete(_arr, _index) \
    ({ \
        __auto_type linc_array_delete_arr = (_arr); \
        size_t linc_array_delete_index = (_index); \
        assert(linc_array_delete_index < linc_array_delete_arr->size); \
        \
        memmove( \
            &linc_array_delete_arr->data[linc_array_delete_index], \
            &linc_array_delete_arr->data[linc_array_delete_index + 1], \
            (linc_array_delete_arr->size - linc_array_delete_index - 1) * \
                sizeof(*linc_array_delete_arr->data) \
        ); \
        --linc_array_delete_arr->size; \
    })

#define linc_array_insert(_arr, _index) \
    ({ \
        __auto_type linc_array_insert_arr = (_arr); \
        size_t linc_array_insert_index = (_index); \
        assert(linc_array_insert_index <= linc_array_insert_arr->size); \
        \
        linc_array_reserve(linc_array_insert_arr, \
                linc_array_insert_arr->size + 1); \
        memmove( \
            &linc_array_insert_arr->data[linc_array_insert_index + 1], \
            &linc_array_insert_arr->data[linc_array_insert_index], \
            (linc_array_insert_arr->size - linc_array_insert_index) * \
                sizeof(*linc_array_insert_arr->data) \
        ); \
        ++linc_array_insert_arr->size; \
        &(linc_array_insert_arr->data[linc_array_insert_index]); \
    })

void linc_array_free_imp(void *data);
void *linc_array_reserve_imp(void *data, size_t el_sz, size_t min, size_t *cap);

#endif
