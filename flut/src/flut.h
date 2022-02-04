#ifndef FLUT_H
#define FLUT_H

#include "assert/asserts.h"
#include "context.h"
#include "suite.h"
#include "test.h"
#include <fllib/Array.h>

/*
 * About: Flut
 *  A basic library for unit testing
 */

#define FLUT__REGISTER_SUITE(suite_id)                                                                                  \
    {                                                                                                                   \
        extern void flut_suite_##suite_id(FlutSuite *);                                                                 \
        FlutSuite *suite = flut_suite_new(#suite_id);                                                                   \
        flut_suite_##suite_id(suite);                                                                                   \
        suites = fl_array_append(suites, &suite);                                                                       \
    }

/*
 * Macro: flut_run_suites
 * 
 * --- Prototype
 *  flut_run_suites(int argc, char **argv, int *result_ptr, ...)
 * ---
 * 
 *  Receives a list with all the suites available and based on the CLI arguments run the requested ones
 *
 * Parameters:
 *  argc - The count of CLI arguments
 *  argv - The arguments vector
 *  result_ptr - A pointer to an int that will hold the result of the test execution
 *  ... - A list of suite identifiers
 *
 * Returns:
 *  This macro is not an expression
 * 
 * Example:
 * 
 * --- C
 *  #include <flut/flut.h>
 *
 *  int main(int argc, char **argv) 
 *  {
 *      bool success;
 *      flut_run_suites(
 *          argc,
 *          argv,
 *          &success,
 *          suite1,
 *          suite2
 *      );
 *      return success ? 0 : -1;
 *  }
 * ---
 **/
#define flut_run_suites(argc, argv, result_ptr, ...)                                                                   \
    do {                                                                                                               \
        FlutSuite **suites = fl_array_new(sizeof(FlutSuite *), 0);                                                     \
        FLUT_MAP(FLUT__REGISTER_SUITE, __VA_ARGS__)                                                                    \
        *(result_ptr) = flut_run(argc, argv, suites, fl_array_length(suites));                                         \
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
 */
bool flut_run(int argc, char **argv, FlutSuite **suites, size_t length);

#endif /* FLUT_H */
