#include <linc/log.h>

#include <unistd.h>
#include <fcntl.h>

#include "unity_fixture.h"

TEST_GROUP(log);

/************ Shared utils ************/

static char log_buf[8192];

void info_sink(const linc_log_ctx *ctx, void *_) {
    TEST_ASSERT_NOT_NULL(ctx);
    TEST_ASSERT_EQUAL_INT(LINC_LOG_INFO, ctx->level);
    snprintf(log_buf, 8192, "INFO %s", ctx->msg);
}

void err_sink(const linc_log_ctx *ctx, void *_) {
    TEST_ASSERT_NOT_NULL(ctx);
    TEST_ASSERT_EQUAL_INT(LINC_LOG_ERROR, ctx->level);
    snprintf(log_buf, 8192, "ERROR %s", ctx->msg);
}

void warn_sink(const linc_log_ctx *ctx, void *_) {
    TEST_ASSERT_NOT_NULL(ctx);
    TEST_ASSERT_EQUAL_INT(LINC_LOG_WARN, ctx->level);
    snprintf(log_buf, 8192, "WARN %s", ctx->msg);
}

void data_sink(const linc_log_ctx *ctx, void *user) {
    TEST_ASSERT_NOT_NULL(ctx);
    TEST_ASSERT_NOT_NULL(user);

    TEST_ASSERT_EQUAL_INT(LINC_LOG_INFO, ctx->level);
    TEST_ASSERT_EQUAL_INT(99, *(int *)user);
    snprintf(log_buf, 8192, "%s%d", ctx->msg, *(int *)user);
}

static linc_log log_sink;

/************ Fixture ************/

TEST_SETUP(log) {
    log_sink =(linc_log){0};
    log_buf[0] = '\0';
}

TEST_TEAR_DOWN(log) {
}

/************ Tests ************/

TEST(log, simple_err_log) {
    log_sink.sink = err_sink;
    linc_log_write(log_sink, LINC_LOG_ERROR, LINC_SITE, "hi");
    TEST_ASSERT_EQUAL_STRING("ERROR hi", log_buf);
}

TEST(log, simple_warn_log) {
    log_sink.sink = warn_sink;
    linc_log_write(log_sink, LINC_LOG_WARN, LINC_SITE, "hi");
    TEST_ASSERT_EQUAL_STRING("WARN hi", log_buf);
}

TEST(log, simple_info_log) {
    log_sink.sink = info_sink;
    linc_log_write(log_sink, LINC_LOG_INFO, LINC_SITE, "hi");
    TEST_ASSERT_EQUAL_STRING("INFO hi", log_buf);
}

TEST(log, special_chars_are_normalized) {
    log_sink.sink = info_sink;
    linc_log_write(log_sink, LINC_LOG_INFO, LINC_SITE, "\n\t");
    TEST_ASSERT_EQUAL_STRING("INFO \\n\\t", log_buf);
}

TEST(log, passing_user_data) {
    linc_log_write(
        ((linc_log){.sink = data_sink, .user = &(int){99}}),
        LINC_LOG_INFO,
        LINC_SITE,
        "DATA = "
    );

    TEST_ASSERT_EQUAL_STRING("DATA = 99", log_buf);
}

TEST(log, changing_global_default) {
    log_sink.sink = info_sink;
    linc_log_set_default(log_sink);

    LINC_LOG_INFO("hi");
    TEST_ASSERT_EQUAL_STRING("INFO hi", log_buf);
}

/************ Test runner ************/

TEST_GROUP_RUNNER(log) {
    RUN_TEST_CASE(log, simple_info_log);
    RUN_TEST_CASE(log, simple_err_log);
    RUN_TEST_CASE(log, simple_info_log);
    RUN_TEST_CASE(log, special_chars_are_normalized);
    RUN_TEST_CASE(log, passing_user_data);
    RUN_TEST_CASE(log, changing_global_default);
}
