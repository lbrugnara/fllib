#ifndef FLUT_TEST_H
#define FLUT_TEST_H

#include <stdbool.h>

#define FLUT__VA_ARGS_IDENTITY(...) __VA_ARGS__
#define FLUT__IDENTITY(X) X
#define FLUT__REMOVE_PARENTHESES(args_enclosed_in_paren) FLUT__IDENTITY( FLUT__VA_ARGS_IDENTITY args_enclosed_in_paren )

#define FLUT__TEST_DEFINITION(test_id, test_description, test_body)                                                     \
    static char *flut_suite_test_##test_id##_description = test_description;                                            \
    static void flut_suite_test_##test_id(FlutContext *flut__internal_ctx, FlutAssertUtils *assert)                     \
        FLUT__REMOVE_PARENTHESES(test_body)

#define flut_test(test_id, test_description, ...) test_id, test_description, (__VA_ARGS__)

#define flut_long_test(test_id, test_description, test_body) test_id, test_description, test_body

/**
 * Notes:
 *  - Deprecated: Use flut_describe or flut_vdescribe instead
 */
#define flut_println(descr)                                                                                             \
    printf("%s", " |  |-------------------------------------------------------------------------\n");                   \
    printf("%s", " |  |  ");                                                                                            \
    printf("%s", descr);                                                                                                \
    printf("%s", "\n");                                                                                                 \
    printf("%s", " |  |-------------------------------------------------------------------------\n");

#define flut_vdescribe(format, ...)                                                                                     \
    printf("%s", " |  |-------------------------------------------------------------------------\n");                   \
    printf("%s", " |  |  ");                                                                                            \
    printf(format, __VA_ARGS__);                                                                                        \
    printf("%s", "\n");                                                                                                 \
    printf("%s", " |  |-------------------------------------------------------------------------\n");                   \
    fl_ctx_frame_push(&(flut__internal_ctx)->context);                                                                  \
    if (fl_ctx_frame_activate(&(flut__internal_ctx)->context) != 0) {                                                   \
        fl_ctx_frame_pop(&(flut__internal_ctx)->context);                                                               \
    } else                                                                                                              \
        for (int fp = (&(flut__internal_ctx)->context)->fp; (&(flut__internal_ctx)->context)->fp == fp; fl_ctx_frame_pop(&(flut__internal_ctx)->context))

#define flut_describe(message) flut_vdescribe("%s", message)

#define flut_host_is_windows() (flut__internal_ctx->os == FLUT_OS_WIN)
#define flut_host_is_linux() (flut__internal_ctx->os == FLUT_OS_LINUX)

/*
 * Running tests
 *
 */
enum
{
    FL_TEST_FAILURE = 1,
    FL_TEST_EXCEPTION
};

typedef void (*FlutTestCaseFnCompat)();

typedef struct
{
    char *name;
    FlutTestCaseFnCompat run;
} FlutTestCase;

#endif /* FLUT_TEST_H */
