#include <linc/array.h>

#include <assert.h>

#include "unity_fixture.h"

TEST_GROUP(array);

/************ Shared utils ************/

LINC_ARRAY_TYPE(int);

static linc_array_int arr;

static void validate_array(linc_array_int *arr, size_t size, size_t cap) {
    TEST_ASSERT_NOT_NULL(arr);
    TEST_ASSERT_EQUAL_size_t(size, arr->size);
    TEST_ASSERT_EQUAL_size_t(cap, arr->cap);
}

/************ Fixture ************/

TEST_SETUP(array) {
    arr = (linc_array_int)LINC_ARRAY_INIT;
    validate_array(&arr, 0, 0);
}

TEST_TEAR_DOWN(array) {
    linc_array_free(&arr);
}

/************ Tests ************/

TEST(array, reserve_min_lte_cap_is_no_op) {
    linc_array_reserve(&arr, 0);
    validate_array(&arr, 0, 0);
}

TEST(array, reserve_empty_array) {
    linc_array_reserve(&arr, 10);
    validate_array(&arr, 0, 10);
}

TEST(array, reserve_non_empty_array) {
    /* initial allocation ... realloc(NULL, size) */
    linc_array_reserve(&arr, 10);
    validate_array(&arr, 0, 10);
    /* actual realloc call ... realloc (data, size) */
    linc_array_reserve(&arr, 20);
    validate_array(&arr, 0, 20);
}

TEST(array, reserve_realloc_preserves_data) {
    int *p = linc_array_push(&arr);
    TEST_ASSERT_NOT_NULL(p);
    validate_array(&arr, 1, 2);

    arr.data[0] = 100;

    p = linc_array_push(&arr);
    TEST_ASSERT_NOT_NULL(p);
    validate_array(&arr, 2, 2);

    TEST_ASSERT_EQUAL_INT(100, arr.data[0]);
}

TEST(array, push_returns_new_element) {
    int *p = linc_array_push(&arr);
    TEST_ASSERT_EQUAL_PTR(&arr.data[0], p);
    validate_array(&arr, 1, 2);
}

TEST(array, delete_only_element) {
    linc_array_push(&arr);
    validate_array(&arr, 1, 2);

    linc_array_delete(&arr, 0);
    validate_array(&arr, 0, 2);
}

TEST(array, delete_first_element) {
    int *p1 = linc_array_push(&arr);
    validate_array(&arr, 1, 2);
    *p1 = 1;

    int *p2 = linc_array_push(&arr);
    validate_array(&arr, 2, 2);
    *p2 = 2;

    linc_array_delete(&arr, 0);
    validate_array(&arr, 1, 2);

    TEST_ASSERT_EQUAL_INT(arr.data[0], 2);
}

TEST(array, delete_last_element) {
    int *p1 = linc_array_push(&arr);
    validate_array(&arr, 1, 2);
    *p1 = 1;

    int *p2 = linc_array_push(&arr);
    validate_array(&arr, 2, 2);
    *p2 = 2;

    linc_array_delete(&arr, 1);
    validate_array(&arr, 1, 2);

    TEST_ASSERT_EQUAL_INT(arr.data[0], 1);
}

TEST(array, delete_middle_element) {
    int *p1 = linc_array_push(&arr);
    validate_array(&arr, 1, 2);
    *p1 = 1;

    int *p2 = linc_array_push(&arr);
    validate_array(&arr, 2, 2);
    *p2 = 2;

    int *p3 = linc_array_push(&arr);
    validate_array(&arr, 3, 4);
    *p3 = 3;

    linc_array_delete(&arr, 1);
    validate_array(&arr, 2, 4);

    TEST_ASSERT_EQUAL_INT(arr.data[0], 1);
    TEST_ASSERT_EQUAL_INT(arr.data[1], 3);
}

TEST(array, inserting_element) {
    *linc_array_insert(&arr, 0) = 25;
    validate_array(&arr, 1, 2);
    TEST_ASSERT_EQUAL_INT(25, arr.data[0]);
}

/************ Test runner ************/

TEST_GROUP_RUNNER(array) {
    RUN_TEST_CASE(array, reserve_min_lte_cap_is_no_op);
    RUN_TEST_CASE(array, reserve_empty_array);
    RUN_TEST_CASE(array, reserve_non_empty_array);
    RUN_TEST_CASE(array, reserve_realloc_preserves_data);
    RUN_TEST_CASE(array, push_returns_new_element);
    RUN_TEST_CASE(array, delete_only_element);
    RUN_TEST_CASE(array, delete_first_element);
    RUN_TEST_CASE(array, delete_last_element);
    RUN_TEST_CASE(array, delete_middle_element);
    RUN_TEST_CASE(array, inserting_element);
}
