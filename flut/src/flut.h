#ifndef FLUT_H
#define FLUT_H

#include "assert/asserts.h"
#include "context.h"
#include "expect.h"
#include "suite.h"
#include "test.h"
#include <fllib/Array.h>

#define FLUT_CURRENT_CTX flut__internal_ctx

#define flut_define_test(test_id)       void flut__test_##test_id(FlutContext *FLUT_CURRENT_CTX)

#define flut_define_suite(suite_id)     void flut_suite_##suite_id(FlutSuite *flut__internal_suite)

#define flut_suite_description(message) flut__internal_suite->description = fl_cstring_dup(message)

#define flut_suite_register_test(test_id, test_description)                                                             \
    {                                                                                                                   \
        size_t cur_size = fl_array_length(flut__internal_suite->tests);                                                 \
        flut__internal_suite->tests = fl_array_resize(flut__internal_suite->tests, cur_size + 1);                       \
        flut__internal_suite->tests[cur_size].name = fl_cstring_dup(test_description);                                  \
        void flut__test_##test_id(FlutContext*);                                                                        \
        flut__internal_suite->tests[cur_size].run = &flut__test_##test_id;                                              \
    } (void)0

/**
 * Notes:
 *  - Deprecated: Use flut_run_suites instead
 */
#define flut_run_tests(argc, argv, result_ptr, ...)                                                                    \
    do {                                                                                                               \
        FlutSuite **suites = fl_array_new(sizeof(FlutSuite *), 0);                                                     \
        ((void)0, __VA_ARGS__, (void)0);                                                                               \
        *(result_ptr) = flut_run(argc, argv, suites, fl_array_length(suites));                                         \
        fl_array_free_each_pointer(suites, (FlArrayFreeElementFunc)flut_suite_free);                                   \
    } while (0)


#define FLUT__ADD_SUITE(suite_id)                                                                                       \
    {                                                                                                                   \
        extern FlutSuite *flut_suite_##suite_id();                                                                      \
        FlutSuite *suite_id##_suite = flut_suite_##suite_id();                                                          \
        suites = fl_array_append(suites, &suite_id##_suite);                                                            \
    }

#define flut_run_suites(argc, argv, ...)                                                                               \
    do {                                                                                                               \
        FlutSuite **suites = fl_array_new(sizeof(FlutSuite *), 0);                                                     \
        FLUT_MAP(FLUT__ADD_SUITE, __VA_ARGS__)                                                                         \
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
 *  bool - True if all tests succeed, otherwise it returns false
 *
 * Notes:
 *
 */
bool flut_run(int argc, char **argv, FlutSuite **suites, size_t length);

#endif /* FLUT_H */
