#include <linc/hds.h>
#include <linc/hda.h>

#include "unity_fixture.h"

TEST_GROUP(hds);

/************ Fixture ************/

char *hds;

TEST_SETUP(hds) {
    hds = LINC_HDS_INIT;
}

TEST_TEAR_DOWN(hds) {
    linc_hds_free(hds);
}

/************ Tests ************/

/* use lower-level hda_size function to assert malloc size */

TEST(hds, append_strings) {
    linc_hds_append(hds, "hello, ");
    TEST_ASSERT_EQUAL_STRING("hello, ", hds);
    TEST_ASSERT_EQUAL_size_t(sizeof("hello, "), linc_hda_size(hds));

    linc_hds_append(hds, "world!");
    TEST_ASSERT_EQUAL_STRING("hello, world!", hds);
    TEST_ASSERT_EQUAL_size_t(sizeof("hello, world!"), linc_hda_size(hds));
}

TEST(hds, copy_and_append) {
    linc_hds_copy(hds, "hello, ");
    TEST_ASSERT_EQUAL_STRING("hello, ", hds);
    TEST_ASSERT_EQUAL_size_t(sizeof("hello, "), linc_hda_size(hds));

    linc_hds_append(hds, "wonderful ");
    TEST_ASSERT_EQUAL_STRING("hello, wonderful ", hds);
    TEST_ASSERT_EQUAL_size_t(sizeof("hello, wonderful "), linc_hda_size(hds));

    linc_hds_append(hds, "world!");
    TEST_ASSERT_EQUAL_STRING("hello, wonderful world!", hds);
    TEST_ASSERT_EQUAL_size_t(sizeof("hello, wonderful world!"), linc_hda_size(hds));

    linc_hds_copy(hds, "");
    TEST_ASSERT_EQUAL_STRING("", hds);
    TEST_ASSERT_EQUAL_size_t(sizeof(""), linc_hda_size(hds));
}

TEST(hds, append_n_chars) {
    linc_hds_append_n(hds, "abcdef", 3);
    TEST_ASSERT_EQUAL_STRING("abc", hds);
    TEST_ASSERT_EQUAL_size_t(sizeof("abc"), linc_hda_size(hds));

    linc_hds_append_n(hds, "defghi", 3);
    TEST_ASSERT_EQUAL_STRING("abcdef", hds);
    TEST_ASSERT_EQUAL_size_t(sizeof("abcdef"), linc_hda_size(hds));
}

TEST(hds, copy_n_chars) {
    linc_hds_copy_n(hds, "abcdef", 3);
    TEST_ASSERT_EQUAL_STRING("abc", hds);
    TEST_ASSERT_EQUAL_size_t(sizeof("abc"), linc_hda_size(hds));

    linc_hds_copy_n(hds, "defghi", 3);
    TEST_ASSERT_EQUAL_STRING("def", hds);
    TEST_ASSERT_EQUAL_size_t(sizeof("def"), linc_hda_size(hds));
}

TEST(hds, append_fmt) {
    int x = 3;
    const char *s = "hi";

    linc_hds_printf(hds, "int: %d string: %s", x, s);
    TEST_ASSERT_EQUAL_STRING("int: 3 string: hi", hds);
    TEST_ASSERT_EQUAL_size_t(sizeof("int: 3 string: hi"), linc_hda_size(hds));
}

TEST(hds, append_to_empty_string_that_was_non_empty) {
    linc_hds_append(hds, "abc");
    linc_hds_copy(hds, "");
    linc_hds_append(hds, "def");

    TEST_ASSERT_EQUAL_STRING("def", hds);
    TEST_ASSERT_EQUAL_size_t(sizeof("def"), linc_hda_size(hds));
}

TEST(hds, self_append) {
    linc_hds_append(hds, "abc");
    linc_hds_append(hds, hds);

    TEST_ASSERT_EQUAL_STRING("abcabc", hds);
    TEST_ASSERT_EQUAL_size_t(sizeof("abcabc"), linc_hda_size(hds));
}

/************ Test runner ************/

TEST_GROUP_RUNNER(hds) {
    RUN_TEST_CASE(hds, append_strings);
    RUN_TEST_CASE(hds, copy_and_append);
    RUN_TEST_CASE(hds, append_n_chars);
    RUN_TEST_CASE(hds, copy_n_chars);
    RUN_TEST_CASE(hds, append_fmt);
    RUN_TEST_CASE(hds, append_to_empty_string_that_was_non_empty);
    RUN_TEST_CASE(hds, self_append);
}
