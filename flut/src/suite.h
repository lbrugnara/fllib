#ifndef FLUT_SUITE_H
#define FLUT_SUITE_H

/*
 * Struct: FlutSuite
 *  Represents a collection of test cases
 */

#include <stddef.h>
#include <stdbool.h>
#include <fllib/Std.h>
#include "test.h"

#define flut_suite(name, ...) flut_suite_new(name, ((const FlutTestCase[]){__VA_ARGS__}), sizeof(((const FlutTestCase[]){__VA_ARGS__})) / sizeof(((const FlutTestCase[]){__VA_ARGS__}))[0])


typedef struct FlutSuite {
    const char *name;
    FlutTestCase *tests;
} FlutSuite;


typedef struct FlutSuiteResult {
    FlutSuite *suite;
    size_t passedTests;
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
FlutSuite* flut_suite_new(const char* name, const FlutTestCase *tests, size_t length);

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
