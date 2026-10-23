#include <linc/trie.h>

#include "unity_fixture.h"

TEST_GROUP(trie);

/************ Fixture ************/

linc_trie *tr;
linc_trie_status trie_stat;
int *data;

TEST_SETUP(trie) {
    tr = linc_trie_create();
}

TEST_TEAR_DOWN(trie) {
    linc_trie_destroy(tr);
}

/************ Tests ************/

TEST(trie, simple_exact_match) {
    linc_trie_add(tr, "abc", &(int){1}, sizeof(int));
    data = linc_trie_lookup(tr, "abc", &trie_stat);

    TEST_ASSERT_EQUAL_INT(TRIE_MATCH, trie_stat);
    TEST_ASSERT_EQUAL_INT(1, *data);
}

TEST(trie, lookup_with_no_match) {
    linc_trie_add(tr, "abc", &(int){1}, sizeof(int));
    data = linc_trie_lookup(tr, "def", &trie_stat);

    TEST_ASSERT_EQUAL_INT(TRIE_NONE, trie_stat);
    TEST_ASSERT_NULL(data);
}

TEST(trie, lookup_with_wait_match) {
    linc_trie_add(tr, "abcdef", &(int){1}, sizeof(int));
    data = linc_trie_lookup(tr, "abc", &trie_stat);

    TEST_ASSERT_EQUAL_INT(TRIE_WAIT, trie_stat);
    TEST_ASSERT_NULL(data);
}

TEST(trie, lookup_with_ambiguous_match) {
    linc_trie_add(tr, "abc", &(int){1}, sizeof(int));
    linc_trie_add(tr, "abcdef", &(int){2}, sizeof(int));
    data = linc_trie_lookup(tr, "abc", &trie_stat);

    TEST_ASSERT_EQUAL_INT(TRIE_AMBIG, trie_stat);
    TEST_ASSERT_EQUAL_INT(1, *data);
}

TEST(trie, add_and_lookup_empty_string) {
    linc_trie_add(tr, "", &(int){1}, sizeof(int));
    data = linc_trie_lookup(tr, "", &trie_stat);

    TEST_ASSERT_EQUAL_INT(TRIE_MATCH, trie_stat);
    TEST_ASSERT_EQUAL_INT(1, *data);

    linc_trie_add(tr, "x", &(int){2}, sizeof(int));
    data = linc_trie_lookup(tr, "x", &trie_stat);

    TEST_ASSERT_EQUAL_INT(TRIE_MATCH, trie_stat);
    TEST_ASSERT_EQUAL_INT(2, *data);
}

TEST(trie, lookup_empty_string_on_empty_trie) {
    linc_trie_lookup(tr, "", &trie_stat);

    TEST_ASSERT_EQUAL_INT(TRIE_NONE, trie_stat);
}

/************ Test runner ************/

TEST_GROUP_RUNNER(trie) {
    RUN_TEST_CASE(trie, simple_exact_match);
    RUN_TEST_CASE(trie, lookup_with_no_match);
    RUN_TEST_CASE(trie, lookup_with_wait_match);
    RUN_TEST_CASE(trie, lookup_with_ambiguous_match);
    RUN_TEST_CASE(trie, add_and_lookup_empty_string);
    RUN_TEST_CASE(trie, lookup_empty_string_on_empty_trie);
}
