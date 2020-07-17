#ifndef FLUT_TEST_H
#define FLUT_TEST_H

#include <stdbool.h>

#define flut_println(descr) printf("\n- %s\n", (descr));

/*
* Running tests
*
*/
enum {
    FL_TEST_FAILURE = 1,
    FL_TEST_EXCEPTION
};

typedef void(*FlutTestCaseFnCompat)();

typedef struct
{
    char *name;
    FlutTestCaseFnCompat run;
} FlutTestCase;

#define flut_vdescribe(ctx, format, ...)                                            \
    printf("%s", "\n- ");                                                           \
    printf(format, __VA_ARGS__);                                                    \
    printf("%s", "\n");                                                             \
    fl_ctx_frame_push(&(ctx)->context);                                             \
    if (fl_ctx_frame_activate(&(ctx)->context) != 0) {                              \
        fl_ctx_frame_pop(&(ctx)->context);                                          \
    }                                                                               \
    else for (                                                                      \
        int fp = (&(ctx)->context)->fp;                                             \
        (&(ctx)->context)->fp == fp;                                                \
        fl_ctx_frame_pop(&(ctx)->context)                                           \
    )

#define flut_describe(ctx, message) flut_vdescribe(ctx, "%s", message)

#endif /* FLUT_TEST_H */
