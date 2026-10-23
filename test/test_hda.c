#include <linc/hda.h>

#include "unity_fixture.h"

TEST_GROUP(hda);

/************ Fixture ************/

int *numbers;

TEST_SETUP(hda) {
    numbers = LINC_HDA_INIT;
}

TEST_TEAR_DOWN(hda) {
    linc_hda_free(numbers);
}

/************ Tests ************/

TEST(hda, push_element_to_null_pointer) {
    linc_hda_push(numbers, 1);

    TEST_ASSERT_EQUAL_size_t(1, linc_hda_size(numbers));
    TEST_ASSERT_EQUAL_size_t(2, linc_hda_cap(numbers));
    TEST_ASSERT_EQUAL_INT(1, numbers[0]);
}

TEST(hda, push_two_elements) {
    linc_hda_push(numbers, 1);

    TEST_ASSERT_EQUAL_size_t(1, linc_hda_size(numbers));
    TEST_ASSERT_EQUAL_size_t(2, linc_hda_cap(numbers));
    TEST_ASSERT_EQUAL_INT(1, numbers[0]);

    linc_hda_push(numbers, 10);

    TEST_ASSERT_EQUAL_size_t(2, linc_hda_size(numbers));
    TEST_ASSERT_EQUAL_size_t(2, linc_hda_cap(numbers));
    TEST_ASSERT_EQUAL_INT(10, numbers[1]);
}

TEST(hda, push_many_elements) {
    for (int num = 1; num <= 100000; ++num)
        linc_hda_push(numbers, num);

    for (int i = 0; i < 100000; ++i)
        TEST_ASSERT_EQUAL_INT(i + 1, numbers[i]);

    TEST_ASSERT_EQUAL_size_t(100000, linc_hda_size(numbers));
    TEST_ASSERT_EQUAL_size_t(131072, linc_hda_cap(numbers));
}

TEST(hda, two_dimensional_array) {
    int **grid = nullptr;

    int total_rows = 50;
    int total_cols = 100;

    for (int row = 0; row < total_rows; ++row) {
        linc_hda_push(grid, nullptr);

        for (int col = 0; col < total_cols; ++col)
            linc_hda_push(grid[row], row * col);
    }

    TEST_ASSERT_EQUAL_INT(100, grid[10][10]);
    TEST_ASSERT_EQUAL_INT(4851, grid[49][99]);

    TEST_ASSERT_EQUAL_size_t(50, linc_hda_size(grid));
    TEST_ASSERT_EQUAL_size_t(64, linc_hda_cap(grid));

    TEST_ASSERT_EQUAL_size_t(100, linc_hda_size(*grid));
    TEST_ASSERT_EQUAL_size_t(128, linc_hda_cap(*grid));

    for (int row = 0; row < total_rows; ++row)
        linc_hda_free(grid[row]);

    linc_hda_free(grid);
    TEST_ASSERT_NULL(grid);
}

TEST(hda, delete_span_of_elements_from_start) {
    linc_hda_push(numbers, 1);
    linc_hda_push(numbers, 2);
    linc_hda_push(numbers, 3);

    linc_hda_delete(numbers, 0, 2);

    TEST_ASSERT_EQUAL_size_t(1, linc_hda_size(numbers));
    TEST_ASSERT_EQUAL_size_t(4, linc_hda_cap(numbers));
    TEST_ASSERT_EQUAL_INT(3, numbers[0]);
}

TEST(hda, delete_last_element) {
    linc_hda_push(numbers, 1);
    linc_hda_delete(numbers, 0, 1);

    TEST_ASSERT_EQUAL_size_t(0, linc_hda_size(numbers));
    TEST_ASSERT_EQUAL_size_t(2, linc_hda_cap(numbers));
}

TEST(hda, delete_elements_to_end) {
    linc_hda_push(numbers, 1);
    linc_hda_push(numbers, 2);
    linc_hda_push(numbers, 3);

    linc_hda_delete(numbers, 1, 2);

    TEST_ASSERT_EQUAL_size_t(1, linc_hda_size(numbers));
    TEST_ASSERT_EQUAL_size_t(4, linc_hda_cap(numbers));
    TEST_ASSERT_EQUAL_INT(1, numbers[0]);
}

TEST(hda, insert_elements) {
    linc_hda_insert(numbers, 0, /*len*/3, { 1, 2, 3});

    TEST_ASSERT_EQUAL_size_t(3, linc_hda_size(numbers));
    TEST_ASSERT_EQUAL_size_t(3, linc_hda_cap(numbers));

    TEST_ASSERT_EQUAL_INT(1, numbers[0]);
    TEST_ASSERT_EQUAL_INT(2, numbers[1]);
    TEST_ASSERT_EQUAL_INT(3, numbers[2]);
}

TEST(hda, insert_elements_before_existing) {
    linc_hda_push(numbers, 4);
    linc_hda_insert(numbers, 0, 3, { 1, 2, 3 });

    TEST_ASSERT_EQUAL_size_t(4, linc_hda_size(numbers));
    TEST_ASSERT_EQUAL_size_t(4, linc_hda_cap(numbers));

    TEST_ASSERT_EQUAL_INT(1, numbers[0]);
    TEST_ASSERT_EQUAL_INT(2, numbers[1]);
    TEST_ASSERT_EQUAL_INT(3, numbers[2]);
    TEST_ASSERT_EQUAL_INT(4, numbers[3]);
}

TEST(hda, insert_elements_after_existing) {
    linc_hda_push(numbers, 1);
    linc_hda_insert(numbers, 1, 3, { 2, 3, 4 });

    TEST_ASSERT_EQUAL_size_t(4, linc_hda_size(numbers));
    TEST_ASSERT_EQUAL_size_t(4, linc_hda_cap(numbers));

    TEST_ASSERT_EQUAL_INT(1, numbers[0]);
    TEST_ASSERT_EQUAL_INT(2, numbers[1]);
    TEST_ASSERT_EQUAL_INT(3, numbers[2]);
    TEST_ASSERT_EQUAL_INT(4, numbers[3]);
}

TEST(hda, get_last_element) {
    linc_hda_push(numbers, 1);
    linc_hda_push(numbers, 2);

    int *num = linc_hda_last(numbers);
    TEST_ASSERT_EQUAL_INT(2, *num);
}

typedef struct {
    char *grades;
    int id;
} test_struct;

TEST(hda, push_struct_type) {
    test_struct *data = nullptr;;

    linc_hda_push(data, { .grades = nullptr, .id = 3 });
    linc_hda_push(linc_hda_last(data)->grades, 'A');
    linc_hda_push(linc_hda_last(data)->grades, 'B');

    TEST_ASSERT_EQUAL_INT(3, data->id);
    TEST_ASSERT_EQUAL_CHAR('A', data->grades[0]);
    TEST_ASSERT_EQUAL_CHAR('B', data->grades[1]);

    linc_hda_free(data->grades);
    linc_hda_free(data);
}

TEST(hda, hda_push_buf) {
    const char *s = "hello";
    unsigned char *data = nullptr;

    linc_hda_insert_buf(data, 0, 4, s);
    TEST_ASSERT_EQUAL_size_t(4, linc_hda_size(data));
    TEST_ASSERT_EQUAL_STRING_LEN("hell", data, 4);

    linc_hda_free(data);
}

/************ Test runner ************/

TEST_GROUP_RUNNER(hda) {
    RUN_TEST_CASE(hda, push_element_to_null_pointer);
    RUN_TEST_CASE(hda, push_two_elements);
    RUN_TEST_CASE(hda, push_many_elements);
    RUN_TEST_CASE(hda, two_dimensional_array);
    RUN_TEST_CASE(hda, delete_last_element);
    RUN_TEST_CASE(hda, delete_elements_to_end);
    RUN_TEST_CASE(hda, delete_span_of_elements_from_start);
    RUN_TEST_CASE(hda, insert_elements);
    RUN_TEST_CASE(hda, insert_elements_before_existing);
    RUN_TEST_CASE(hda, insert_elements_after_existing);
    RUN_TEST_CASE(hda, get_last_element);
    RUN_TEST_CASE(hda, push_struct_type);
    RUN_TEST_CASE(hda, hda_push_buf);
}
