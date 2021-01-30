#ifndef FLUT_SUITE_H
#define FLUT_SUITE_H

/*
 * Struct: FlutSuite
 *  Represents a collection of test cases
 */

#include "context.h"
#include "map.h"
#include "test.h"
#include <fllib/Std.h>
#include <stdbool.h>
#include <stddef.h>

#define FLUT__TAKE_FIRST_PARAMETER(a, ...) a
#define FLUT__VA_ARGS_IDENTITY(...) __VA_ARGS__
#define FLUT__IDENTITY(X) X
#define FLUT__REMOVE_PARENTHESES(args_enclosed_in_paren) FLUT__IDENTITY( FLUT__VA_ARGS_IDENTITY args_enclosed_in_paren )

#define FLUT__TEST_DEFINITION(test_id, test_description, test_body)                                                     \
    static char *flut_suite_test_##test_id##_description = test_description;                                            \
    static void flut_suite_test_##test_id(FlutContext *flut__internal_ctx, FlutAssertUtils *assert)                     \
        FLUT__REMOVE_PARENTHESES(test_body)

#define FLUT__REGISTER_TEST(test_id)                                                                            \
    {                                                                                                                   \
        size_t cur_size = fl_array_length(cases);                                                                       \
        cases = fl_array_resize(cases, cur_size + 1);                                                                   \
        cases[cur_size].name = flut_suite_test_##test_id##_description;                                                 \
        cases[cur_size].run = &flut_suite_test_##test_id;                                                               \
    }

#define flut_test(test_id, test_description, ...) test_id, test_description, (__VA_ARGS__)

#define flut_define_suite(suite_id, description, ...)                                                                   \
    FLUT_MAP_3_AT_TIME_WITH_SPACE(FLUT__TEST_DEFINITION, __VA_ARGS__)                                                   \
    FlutSuite *flut_suite_##suite_id()                                                                                  \
    {                                                                                                                   \
        FlutTestCase *cases = fl_array_new(sizeof(FlutTestCase), 0);                                                    \
        FLUT_MAP(FLUT__REGISTER_TEST, FLUT_MAP_3_AT_TIME(FLUT__TAKE_FIRST_PARAMETER, __VA_ARGS__))              \
        FlutSuite *suite = flut_suite_new(#suite_id, description, cases, fl_array_length(cases));                       \
        fl_array_free(cases);                                                                                           \
        return suite;                                                                                                   \
    } FlutSuite *flut_suite_##suite_id()

#define flut_add_suite(suite_id)                                                                                        \
    {                                                                                                                   \
        extern FlutSuite *flut_suite_##suite_id();                                                                      \
        FlutSuite *suite_id##_suite = flut_suite_##suite_id();                                                          \
        suites = fl_array_append(suites, &suite_id##_suite);                                                            \
    }

/**
 * Notes:
 *  - Deprecated: Use flut_add_suite(suite_id) or flut_run_suites
 */
#define flut_register_suite(suite_id)                                                                                  \
    (void)0);                                                                                                          \
    {                                                                                                                  \
        extern FlutSuite *flut_suite_##suite_id();                                                                     \
        FlutSuite *suite_id##_suite = flut_suite_##suite_id();                                                         \
        suites = fl_array_append(suites, &suite_id##_suite);                                                           \
    }((void)0

/**
 * Notes:
 *  - Deprecated: Use flut_add_suite(suite_id) or flut_run_suites
 */
#define flut_suite(name, ...)                                                                                          \
    (void)0);                                                                                                          \
    {                                                                                                                  \
        FlutSuite *suite = flut_suite_new(name,                                                                        \
                                          name,                                                                        \
                                          ((const FlutTestCase[]){ __VA_ARGS__ }),                                     \
                                          sizeof(((const FlutTestCase[]){ __VA_ARGS__ })) /                            \
                                            sizeof(((const FlutTestCase[]){ __VA_ARGS__ }))[0]);                       \
        suites = fl_array_append(suites, &suite);                                                                      \
    }((void)0

typedef struct FlutSuite
{
    const char *id;
    const char *description;
    FlutTestCase *tests;
} FlutSuite;

typedef struct FlutSuiteResult
{
    FlutSuite *suite;
    size_t passedTests;
    long elapsed;
    bool ran;
} FlutSuiteResult;

/*
 * Function: flut_suite_new
 *  Creates a new test suite that includes all the provided tests and can be identified by the
 *  provided id
 *
 * Parameters:
 *  id - The id to identify the suite
 *  descr - A message describing the suite
 *  tests - An array of test cases
 *  length - The number of test cases in the *tests* array
 *
 * Returns:
 *  FlutSuite* - An opaque pointer to a test suite
 *
 * Notes:
 *  -   The object returned by this function must be freed using the <flut_suite_free> function.
 *
 * See:
 *  -   <flut_suite_free>
 */
FlutSuite *flut_suite_new(const char *id, const char *descr, const FlutTestCase *tests, size_t length);

/*
 * Function: flut_suite_free
 *  Releases the memory used by the test suite object
 *
 * Parameters:
 *  suite - Test suite object
 *
 * Returns:
 *  This function does not return a value
 *
 * See:
 *  -   <flut_suite_new>
 */
void flut_suite_free(FlutSuite *suite);

/*
 * Function: flut_suite_run
 *  This function runs all the test cases in the test suite and prints a summary of the result to
 *  the standard output
 *
 * Parameters:
 *  suite - The test suite
 *  result - The object that will contain information about the test execution
 *
 * Returns:
 *  void - This function does not return a value
 *
 */
void flut_suite_run(FlutSuite *suite, FlutSuiteResult *result);

#endif
