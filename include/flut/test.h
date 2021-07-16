#ifndef FLUT_TEST_H
#define FLUT_TEST_H

/*
 * About: FlutTestCase
 *  Allow the definition of runnable test cases
 */

#include <stdbool.h>

/*
 * Define: FLUT_TEST_CONTEXT
 *  This can be used to refer to the FlutContext parameter that exists within all the tests. It
 *  can be used to pass the context to another function
 *
 * Notes:
 *  - <FLUT_TEST_CONTEXT> exists within all the tests defined with <flut_test>
 * 
 * See:
 *  - <FLUT_TEST_CONTEXT_PARAM> for an example
 */
#define FLUT_TEST_CONTEXT flut__internal_ctx

/*
 * Define: FLUT_TEST_CONTEXT_PARAM
 *  This is the complete signature of the FlutContext parameter that exists within all the tests.
 *  It can be used to define a parameter of a function that expects a FlutContext
 *
 * Example:
 * --- C
 *  static void my_helper_func(FLUT_TEST_CONTEXT_PARAM, int n) {
 *      flut_assert_int_is_equals(15, n);
 *  }
 * 
 *  flut_test(my_test) {
 *      my_helper_func(FLUT_TEST_CONTEXT, 15);
 *  }
 * ---
 */
#define FLUT_TEST_CONTEXT_PARAM FlutContext *flut__internal_ctx

/*
 * Macro: flut_test
 *  Defines a new test
 *
 * Parameters:
 *  test_name - The name of the test that can be used to run it or to register it in a suite
 *
 * Returns:
 *  This macro is not an expression, it doesn't return a value
 *
 * Notes:
 *  - The name must be a valid C name function
 * 
 * See:
 *  - <flut_suite_register_test>: to register the test in a suite
 * 
 * --- Prototype
 *      flut_test(test_name)
 * ---
 * 
 * Example:
 * --- C
 *  flut_test(test_name) {
 *      // Make assertions here
 *  }
 * ---
 */
#define flut_test(test_name)       void flut__test_##test_name(FLUT_TEST_CONTEXT_PARAM)

/*
 * Macro: flut_describev
 *  Creates an execution frame, when an assert within the describe block fails, the frame skips the rest of assertions but
 *  it still continues within the test case defined with <flut_test>
 *
 * Parameters:
 *  message - A printf-like format message describing what is going on in the frame within the test case
 *  ... - The arguments to be applied to the format message
 *
 * Returns:
 *  This macro is not an expression, it doesn't return a value
 *
 * --- Prototype
 *      flut_describev(const char *message, ...)
 * ---
 * 
 * Example:
 * --- C
 *  flut_test(my_test) {
 *      flut_describev("Here goes the %s", "Message") {
 *          // Asserts my fail here
 *      }
 *      // Either on success or failure of the assertions within the describe block
 *      // the execution continues here
 *  }
 * ---
 */
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

/*
 * Macro: flut_describe
 *  Creates an execution frame, when an assert within the describe block fails, the frame skips the rest of assertions but
 *  it still continues within the test case defined with <flut_test>
 *
 * Parameters:
 *  message - A message describing what is going on in the frame within the test case
 *
 * Returns:
 *  This macro is not an expression, it doesn't return a value
 *
 * --- Prototype
 *      flut_describe(const char *message)
 * ---
 * 
 * Example:
 * --- C
 *  flut_test(my_test) {
 *      flut_describe("Here goes the message") {
 *          // Asserts my fail here
 *      }
 *      // Either on success or failure of the assertions within the describe block
 *      // the execution continues here
 *  }
 * ---
 */
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
