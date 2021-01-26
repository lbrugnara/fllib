#ifndef FLUT_H
#define FLUT_H

#include "assert.h"
#include "context.h"
#include "expect.h"
#include "suite.h"
#include "test.h"
#include <fllib/Array.h>

/**
 * Notes:
 *  - Deprecated: Use flut_run_suites instead
 */
#define flut_run_tests(argc, argv, ...)                                                                                \
    do {                                                                                                               \
        FlutSuite **suites = fl_array_new(sizeof(FlutSuite *), 0);                                                     \
        ((void)0, __VA_ARGS__, (void)0);                                                                               \
        flut_run(argc, argv, suites, fl_array_length(suites));                                                         \
        fl_array_free_each_pointer(suites, (FlArrayFreeElementFunc)flut_suite_free);                                   \
    } while (0)

#define flut_run_suites(argc, argv, ...)                                                                               \
    do {                                                                                                               \
        FlutSuite **suites = fl_array_new(sizeof(FlutSuite *), 0);                                                     \
        FLUT_MAP(flut_add_suite, __VA_ARGS__)                                                                          \
        flut_run(argc, argv, suites, fl_array_length(suites));                                                         \
        fl_array_free_each_pointer(suites, (FlArrayFreeElementFunc)flut_suite_free);                                   \
    } while (0)

/*
 * Function: flut_run
 *  This function runs all the test cases of each test suite and prints a summary of the result to
 *  the standard output
 *
 * Parameters:
 *  argc - Program's argument count
 *  argv - Program's argument vector
 *  suites - An array of test suites
 *  length - The number of test suites
 *
 * Returns:
 *  void - This function does not return a value
 *
 * Notes:
 *
 */
void flut_run(int argc, char **argv, FlutSuite **suites, size_t length);

#endif /* FLUT_H */
