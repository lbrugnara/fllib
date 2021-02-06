#ifndef FLUT_TEST_H
#define FLUT_TEST_H

#include <stdbool.h>

/**
 * Notes:
 *  - Deprecated: Use flut_describe or flut_vdescribe instead
 */
#define flut_println(descr)                                                                                             \
    printf("%s", " |  |-------------------------------------------------------------------------\n");                   \
    printf("%s", " |  |  ");                                                                                            \
    printf("%s", descr);                                                                                                \
    printf("%s", "\n");                                                                                                 \
    printf("%s", " |  |-------------------------------------------------------------------------\n");                   \

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
