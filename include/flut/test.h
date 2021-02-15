#ifndef FLUT_TEST_H
#define FLUT_TEST_H

#include <stdbool.h>

/*
 * About: FlutTestCase
 *  Allow the definition of runnable test cases
 */

#define FLUT_TEST_CONTEXT flut__internal_ctx
#define FLUT_TEST_CONTEXT_PARAM FlutContext *flut__internal_ctx

/*
 * Macro: flut_test
 *  Defines a new test
 *
 * Parameters:
 *  test_id - The ID of the test that can be used to run it or to register it in a suite
 *
 * Returns:
 *  This macro is not an expression, it doesn't return a value
 *
 * Notes:
 *  - The ID must be a valid C name function
 * 
 * See:
 *  - <flut_suite_register_test>: to register the test in a suit
 * 
 * Example:
 * --- C
 *  flut_test(test_id) {
 *      // Make assertions here
 *  }
 * ---
 */
#define flut_test(test_id)       void flut__test_##test_id(FLUT_TEST_CONTEXT_PARAM)

#define flut_describev(format, ...)                                                                                     \
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

#define flut_describe(message) flut_describev("%s", message)

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
