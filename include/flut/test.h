#ifndef FLUT_TEST_H
#define FLUT_TEST_H

#include <stdbool.h>

/**
 * Notes:
 *  - Deprecated: Use flut_describe or flut_vdescribe instead
 */
#define flut_println(descr)                                                                                            \
    printf("%s", " |  |-------------------------------------------------------------------------\n");                  \
    printf("%s", " |  |  ");                                                                                           \
    printf("%s", descr);                                                                                               \
    printf("%s", "\n");                                                                                                \
    printf("%s", " |  |-------------------------------------------------------------------------\n");

#define flut_define_test(suite_id, test_id, descr)                                                                     \
    char *flut_suite_##suite_id##_test_##test_id##_description = descr;                                                \
    void flut_suite_##suite_id##_test_##test_id(FlutContext *ctx, FlutAssertUtils *assert)

#define flut_register_test(suite_id, test_id)                                                                          \
    {                                                                                                                  \
        extern char *flut_suite_##suite_id##_test_##test_id##_description;                                             \
        extern void flut_suite_##suite_id##_test_##test_id(FlutContext *, FlutAssertUtils *);                          \
        size_t cur_size = fl_array_length(cases);                                                                      \
        cases = fl_array_resize(cases, cur_size + 1);                                                                  \
        cases[cur_size].name = flut_suite_##suite_id##_test_##test_id##_description;                                   \
        cases[cur_size].run = &flut_suite_##suite_id##_test_##test_id;                                                 \
    }

#define flut_vdescribe(ctx, format, ...)                                                                               \
    printf("%s", " |  |-------------------------------------------------------------------------\n");                  \
    printf("%s", " |  |  ");                                                                                           \
    printf(format, __VA_ARGS__);                                                                                       \
    printf("%s", "\n");                                                                                                \
    printf("%s", " |  |-------------------------------------------------------------------------\n");                  \
    fl_ctx_frame_push(&(ctx)->context);                                                                                \
    if (fl_ctx_frame_activate(&(ctx)->context) != 0) {                                                                 \
        fl_ctx_frame_pop(&(ctx)->context);                                                                             \
    } else                                                                                                             \
        for (int fp = (&(ctx)->context)->fp; (&(ctx)->context)->fp == fp; fl_ctx_frame_pop(&(ctx)->context))

#define flut_describe(ctx, message) flut_vdescribe(ctx, "%s", message)

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
