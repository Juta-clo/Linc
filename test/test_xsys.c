#include <linc/xsys.h>
#include <linc.h>

#include <errno.h> // IWYU pragma: export
#include <stdlib.h>
#include <sys/wait.h>

#include "unity_fixture.h"

TEST_GROUP(xsys);

/************ Fixture ************/

typedef struct {
    void(* handler)(void);
} target_wrapper;

static const char *expected_sys_name;

static void test_user_handler(void) {
    TEST_PASS();
}

target_wrapper wrapper = { test_user_handler };

static void test_fatal(const linc_fatal_ctx *ctx, void *user) {
    TEST_ASSERT_NOT_NULL(ctx);
    TEST_ASSERT_NOT_NULL(ctx->msg);

    TEST_ASSERT_EQUAL_STRING(expected_sys_name, ctx->msg);
    TEST_ASSERT_EQUAL_INT(EBADF, ctx->saved_errno);
    TEST_ASSERT_EQUAL_INT(getpid(), ctx->pid);
    TEST_ASSERT_EQUAL_INT(getppid(), ctx->ppid);
    TEST_ASSERT_EQUAL_INT(getpgid(0), ctx->pgid);

    ((target_wrapper *)user)->handler();
}

linc_fatal_target test_target;

TEST_SETUP(xsys) {
    linc_fatal_set_target(
        test_target = (linc_fatal_target) {
            .handler = test_fatal,
            .user = &wrapper
        }
    );

    expected_sys_name = nullptr;
}

TEST_TEAR_DOWN(xsys) {
}

/************ Tests ************/

TEST(xsys, verify_xsys_ctx_data) {
    expected_sys_name = "close";
    xclose(-1);
    TEST_FAIL_MESSAGE("xclose call returned instead of running handler");
}

TEST(xsys, process_aborts_with_default_handler) {
    linc_fatal_set_target((linc_fatal_target){0});

    int cpid;

    switch ((cpid = fork())) {
    case -1:
        TEST_FAIL_MESSAGE("fork() failed");
    case 0:
        if (!freopen("/dev/null", "w", stderr))
            _exit(EXIT_FAILURE);

        xclose(-1);
        _exit(EXIT_FAILURE);
    default:
        break;
    }

    int wstat;
    if (waitpid(cpid, &wstat, 0) == -1)
        TEST_FAIL_MESSAGE("waitpid() failed");

    TEST_ASSERT_EQUAL_INT(1, WIFSIGNALED(wstat));
    TEST_ASSERT_EQUAL_INT(SIGABRT, WTERMSIG(wstat));
}

/************ Test runner ************/

TEST_GROUP_RUNNER(xsys) {
    RUN_TEST_CASE(xsys, verify_xsys_ctx_data);
    RUN_TEST_CASE(xsys, process_aborts_with_default_handler);
}
