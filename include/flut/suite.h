#ifndef FLUT_SUITE_H
#define FLUT_SUITE_H

/*
 * About: FlutSuite
 *  Represents a collection of test cases
 */

#include "context.h"
#include "map.h"
#include "test.h"
#include <fllib/Std.h>
#include <stdbool.h>
#include <stddef.h>

#define FLUT__SUITE_PTR flut__internal_suite

/*
 * Macro: flut_suite
 *  Defines a new suite with the provided name
 *
 * Parameters:
 *  suite_name - The name of the suite
 *
 * Returns:
 *  This macro is not an expression, it doesn't return a value
 *
 * Notes:
 *  - The name must be a valid C function name
 * 
 * --- Prototype
 *      flut_suite(suite_name)
 * ---
 * 
 * Example:
 * --- C
 *  flut_suite(suite_name) {
 *      flut_suite_description("My suite");
 *      flut_suite_register_test(my_test, "My test");
 *  }
 * ---
 * 
 * See:
 *  - <flut_suite_description>: to set the suite description
 *  - <flut_suite_register_test>: to register a test in the suite
 */
#define flut_suite(suite_name)     void flut_suite_##suite_name(FlutSuite *FLUT__SUITE_PTR)

/*
 * Macro: flut_suite_description
 *  This macro must be used within a <flut_suite> block to set the description of the suite
 *
 * Parameters:
 *  suite_description - The description of the suite
 *
 * Notes:
 *  - This macro can only be used within a <flut_suite> block
 * 
 * --- Prototype
 *      flut_suite_description(const char *suite_description)
 * ---
 */
#define flut_suite_description(suite_description) FLUT__SUITE_PTR->description = fl_cstring_dup(suite_description)

/*
 * Macro: flut_suite_register_test
 *  Includes the test identified by the provided name in the suite where it is being registered
 *
 * Parameters:
 *  test_name - The name of the test
 *  test_description - The description of the test
 *
 * Returns:
 *  This macro is not an expression, it doesn't return a value
 *
 * Notes:
 *  - This macro is valid only within a <flut_suite> block
 * 
 * See:
 *  - <flut_suite>: to define a suite and register a test within it
 *  - <flut_test>: To define a new test
 * 
 * --- Prototype
 *      flut_suite_register_test(test_name, const char *test_description)
 * ---
 */
#define flut_suite_register_test(test_name, test_description)                               \
    {                                                                                       \
        size_t cur_size = fl_array_length(FLUT__SUITE_PTR->tests);                          \
        FLUT__SUITE_PTR->tests = fl_array_resize(FLUT__SUITE_PTR->tests, cur_size + 1);     \
        FLUT__SUITE_PTR->tests[cur_size].name = fl_cstring_dup(test_description);           \
        void flut__test_##test_name(FlutContext*);                                          \
        FLUT__SUITE_PTR->tests[cur_size].run = &flut__test_##test_name;                     \
    } (void)0

/*
 * Type: struct FlutSuite
 *  This struct contains information about a test suite.
 * 
 * Members:
 *  name - The name of the suite
 *  description - The description of the test performed within the suite
 *  tests - The set of tests to be run within the suite
 * 
 * --- Prototype
 * typedef struct FlutSuite
 * {
 *     const char *name;
 *     const char *description;
 *     FlutTestCase *tests;
 * } FlutSuite;
 * ---
 */
typedef struct FlutSuite
{
    const char *name;
    const char *description;
    FlutTestCase *tests;
} FlutSuite;

/*
 * Type: struct FlutSuiteResult
 *  This struct contains information about the result of running one suite of tests
 * 
 * Members:
 *  suite - The test suite object
 *  passed_tests - The number of tests that succeed
 *  elapsed - The elapsed time running all the tests within the suite in miliseconds
 *  ran - true if the suite has been run, otherwise false
 * 
 * --- Prototype
 * typedef struct FlutSuiteResult
 * {
 *     FlutSuite *suite;
 *     size_t passed_tests;
 *     long elapsed;
 *     bool ran;
 * } FlutSuiteResult;
 * ---
 */
typedef struct FlutSuiteResult
{
    FlutSuite *suite;
    size_t passed_tests;
    long elapsed;
    bool ran;
} FlutSuiteResult;

/*
 * Function: flut_suite_new
 *  Creates a new test suite that includes all the provided tests and can be identified by the
 *  provided name
 *
 * Parameters:
 *  name - The name to identify the suite
 *
 * Returns:
 *  <FlutSuite>* - An opaque pointer to a test suite
 *
 * Notes:
 *  -   The object returned by this function must be freed using the <flut_suite_free> function.
 *
 * See:
 *  -   <flut_suite_free>
 */
FlutSuite *flut_suite_new(const char *name);

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
