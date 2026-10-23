#include <linc/string.h>

#include "unity_fixture.h"

/* TODO: add test for n versions of str funcs */

TEST_GROUP(string);

/************ Shared utils ************/

static linc_string str;

static void validate_str(char *exp_cstr, size_t exp_len, size_t exp_cap) {
    TEST_ASSERT_EQUAL_size_t(exp_len, str.len);
    TEST_ASSERT_EQUAL_size_t(exp_cap, str.cap);
    TEST_ASSERT_EQUAL_STRING(exp_cstr, str.cstr);
}

/************ Fixture ************/

TEST_SETUP(string) {
    str = LINC_STRING_INIT;
    validate_str("", 0, 0);
}

TEST_TEAR_DOWN(string) {
    linc_string_free(&str);
}

/************ Tests ************/

TEST(string, is_c_string_after_push) {
    linc_string_push(&str, 'a');
    validate_str("a", 1, 2);

    linc_string_push(&str, 'b');
    validate_str("ab", 2, 4);

    linc_string_push(&str, 'c');
    validate_str("abc", 3, 4);
}

TEST(string, concat_right_after_init) {
    linc_string_append(&str, "abc");
    validate_str("abc", 3, 4);
}

TEST(string, concat_with_non_empty_string) {
    linc_string_set(&str, "abc");
    validate_str("abc", 3, 4);

    linc_string_append(&str, "def");
    validate_str("abcdef", 6, 8);
}

TEST(string, string_formated_print) {
    int x = 1;
    float y = 1.5;
    char *s = "Equation";

    linc_string_appendf(&str, "%s: %d + %.1f = %.1f", s, x, y, x + y);
    validate_str("Equation: 1 + 1.5 = 2.5", 23, 24);
}

TEST(string, setting_appending_clearing_and_pushing) {
    linc_string_set(&str, "foobar");
    validate_str("foobar", 6, 7);

    linc_string_append_n(&str, "beeboo", 3);
    validate_str("foobarbee", 9, 14);

    linc_string_clear(&str);
    validate_str("", 0, 14);

    for (size_t i = 0; i < 5; ++i)
        linc_string_push(&str, 'x');

    validate_str("xxxxx", 5, 14);

    linc_string_set_n(&str, "xxxxxxxxxxxxxxxX", 15);
    validate_str("xxxxxxxxxxxxxxx", 15, 28);
}

TEST(string, freeing_leaves_cstr_valid) {
    linc_string_free(&str);
    validate_str("", 0, 0);

    linc_string_push(&str, 'x');
    validate_str("x", 1, 2);

    linc_string_free(&str);
    validate_str("", 0, 0);

    linc_string_push(&str, 'x');
    validate_str("x", 1, 2);
}

/************ Test runner ************/

TEST_GROUP_RUNNER(string) {
    RUN_TEST_CASE(string, is_c_string_after_push);
    RUN_TEST_CASE(string, concat_right_after_init);
    RUN_TEST_CASE(string, concat_with_non_empty_string);
    RUN_TEST_CASE(string, string_formated_print);
    RUN_TEST_CASE(string, setting_appending_clearing_and_pushing);
    RUN_TEST_CASE(string, freeing_leaves_cstr_valid);
}
