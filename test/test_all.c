#include "unity_fixture.h"

void run_all(void) {
    RUN_TEST_GROUP(args);
    RUN_TEST_GROUP(array);
    RUN_TEST_GROUP(string);
    RUN_TEST_GROUP(hda);
    RUN_TEST_GROUP(hds);
    RUN_TEST_GROUP(log);
    RUN_TEST_GROUP(trie);
    RUN_TEST_GROUP(xsys);
}

int main(int argc, const char **argv) {
    return UnityMain(argc, argv, run_all);
}
