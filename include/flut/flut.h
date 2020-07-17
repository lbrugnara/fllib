#ifndef FLUT_H
#define FLUT_H

#include "assert.h"
#include "expect.h"
#include "context.h"
#include "test.h"
#include "suite.h"

#define flut_run_tests(argc, argv, ...) do { FlutSuite *suites[] = { __VA_ARGS__ }; flut_run(argc, argv, suites, flm_array_length(suites)); } while (0)

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
