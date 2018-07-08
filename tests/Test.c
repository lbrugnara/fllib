/* =============================================================
* {module: Tests}
* =============================================================
* This is a basic module to run unit tests. It is very basic and
* it was intended to be used as soon as possible, it will need
* some refactoring to make it more pleasant to work with, robust
* and extensible..
* -------------------------------------------------------------
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <fllib.h>
#include "Test.h"

/* -------------------------------------------------------------
 * {datatype: struct FlTestSuite}
 * -------------------------------------------------------------
 * Represents a test suite that contains a set of tests to run
 * -------------------------------------------------------------
 * {member: char* name} Suite name
 * {member: size_t ntests} Number of tests that compose the suite
 * {member: FlTest* tests} Pointer to the set of tests
 * -------------------------------------------------------------
 */
struct FlTestSuite
{
    const char *name;
    size_t ntests;
    const FlTest *tests;
};

/* -------------------------------------------------------------
 * {function: fl_test_suite_new}
 * -------------------------------------------------------------
 * Creates a test suite
 * -------------------------------------------------------------
 * {param: const char* name} Suite name
 * {param: const FlTest[] tests} Array of tests that compose the suite
 * {param: size_t ntests} Number of tests that compose the suite
 * -------------------------------------------------------------
 * {return: FlTestSuite} Suite pointer
 * -------------------------------------------------------------
 */
FlTestSuite fl_test_suite_new(const char *name, const FlTest *tests, size_t ntests)
{
    FlTestSuite t = calloc(1, sizeof(struct FlTestSuite));
    t->name = name;
    t->ntests = ntests;
    t->tests = tests;
    return t;
}

/* -------------------------------------------------------------
 * {function: fl_test_suite_delete}
 * -------------------------------------------------------------
 * Delete a suite freeing its memory
 * -------------------------------------------------------------
 * {param: FlTestSuite suite} Target suite to be removed
 * -------------------------------------------------------------
 * {return: void}
 * -------------------------------------------------------------
 */
void fl_test_suite_delete(FlTestSuite suite)
{
    free(suite);
}

/* -------------------------------------------------------------
 * Running tests
 * -------------------------------------------------------------
 */
enum {
    TEST_FAILURE = 1,
    TEST_EXCEPTION
};

/* -------------------------------------------------------------
 * {variable: FlTryContext testctx}
 * -------------------------------------------------------------
 * Will keep the state of the program before running a test
 * to be able to go back a continue the execution
 * -------------------------------------------------------------
 */
static FlTryContext testctx;

/* -------------------------------------------------------------
 * {function: sighandler}
 * -------------------------------------------------------------
 * Signal handler function for POSIX platforms. When a signal
 * is raised, this handler will trigger an exception.
 * -------------------------------------------------------------
 * {param: int sign} Signal number
 * -------------------------------------------------------------
 * {return: void}
 * -------------------------------------------------------------
 */
void sighandler(int sign)
{
    Throw(&testctx, TEST_FAILURE);
}

#ifdef _WIN32
#include "../src/os/WinEx.h"
/* -------------------------------------------------------------
 * {function: exception_filter}
 * -------------------------------------------------------------
 * Exception filter function for Windows platforms. Capture an 
 * exception raised by Win32 API. When that happens, it throws
 * an exception.
 * -------------------------------------------------------------
 * {param: EXCEPTION_POINTERS ExceptionInfo} Information about the exception
 * -------------------------------------------------------------
 * {return: LONG} Returns EXCEPTION_CONTINUE_EXECUTION to resume the execution
 *  or EXCEPTION_CONTINUE_SEARCH when the program can't continue
 * -------------------------------------------------------------
 */
LONG WINAPI exception_filter(EXCEPTION_POINTERS * ExceptionInfo)
{
    if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_NONCONTINUABLE_EXCEPTION)
        return EXCEPTION_CONTINUE_SEARCH; // Let Win32 resolve
    // An error condition occured, so take the brief description and Throw the exception
    // to the Try()
    fl_winex_message_get(ExceptionInfo->ExceptionRecord->ExceptionCode, testctx.message);
    Throw(&testctx, TEST_EXCEPTION);
    return EXCEPTION_CONTINUE_EXECUTION;
}
#endif

/* -------------------------------------------------------------
 * {function: fl_expect}
 * -------------------------------------------------------------
 * Simple functions that throws an exception when the condition
 * is false
 * -------------------------------------------------------------
 * {param: const char* descr} Description of the conditions to check
 * {param: bool conditionResult} Condition result
 * -------------------------------------------------------------
 * {return: bool} true if conditionResult is valid, throws exception if not
 * -------------------------------------------------------------
 */
bool fl_expect(const char* descr, bool conditionResult)
{
    if (!conditionResult)
    {
        printf(" |-- Failed: %s\n", descr);
        fl_cstring_copy_n(testctx.message, descr, FL_ERROR_TRYCONTEXT_MAX_MSG_SIZE);
        Throw(&testctx, TEST_FAILURE);
    }
    printf(" |-- Passed: %s\n", descr);
    return true;
}

/* -------------------------------------------------------------
 * {function: fl_expect}
 * -------------------------------------------------------------
 * Simple functions that throws an exception when the condition
 * is false
 * -------------------------------------------------------------
 * {param: const char* descr} Description of the conditions to check
 * {param: bool conditionResult} Condition result
 * -------------------------------------------------------------
 * {return: bool} true if conditionResult is valid, throws exception if not
 * -------------------------------------------------------------
 */
bool fl_vexpect(bool conditionResult, const char* format, ...)
{
    char descr[FL_ERROR_TRYCONTEXT_MAX_MSG_SIZE];
    va_list args;
    va_start(args, format);
    vsnprintf(descr, FL_ERROR_TRYCONTEXT_MAX_MSG_SIZE, format, args);
    va_end(args);

    if (!conditionResult)
    {
        printf(" |-- Failed: %s\n", descr);
        fl_cstring_copy_n(testctx.message, descr, FL_ERROR_TRYCONTEXT_MAX_MSG_SIZE);
        Throw(&testctx, TEST_FAILURE);
    }
    printf(" |-- Passed: %s\n", descr);
    return true;
}

/* -------------------------------------------------------------
 * {function: fl_test_suite_run}
 * -------------------------------------------------------------
 * Run a suite of tests
 * -------------------------------------------------------------
 * {param: FlTestSuite suite} Target suite to be ran
 * -------------------------------------------------------------
 * {return: size_t} number of passed tests
 * -------------------------------------------------------------
 */
size_t fl_test_suite_run(FlTestSuite suite)
{
    #ifdef _WIN32
    FlWinExceptionHandler prevh = fl_winex_global_handler_set(exception_filter);
    #endif
    fl_signal_global_handler_set(sighandler);
    printf("============================\n");
    printf("Test Suite: %s\n", suite->name);
    printf("----------------------------\n");
    size_t failedTests = 0;
    for (size_t i=0; i < suite->ntests; i++)
    {
        printf(" # Test Case: %s\n", suite->tests[i].name);
        Try (&testctx)
        {
            suite->tests[i].run();
            printf(" [Result] success\n\n");
        }
        Catch(TEST_EXCEPTION)
        {
            failedTests++;
            printf(" [Result] fail: %s\n\n", testctx.message);
        }
        Rest
        {
            failedTests++;
            printf(" [Result] fail: %s\n\n", testctx.message);
        }
        EndTry;
    }
    printf("\n");
    #ifdef _WIN32
    fl_winex_global_handler_set(prevh);
    #endif
    return suite->ntests - failedTests;
}

struct FlSuiteResult {
    FlTestSuite *suite;
    size_t passedTests;
};

void fl_test_run_all(FlTestSuite *suites)
{
    size_t number_of_suites = 0;
    for (;number_of_suites < SIZE_MAX && suites[number_of_suites]; number_of_suites++);

    struct FlSuiteResult *results = fl_array_new(sizeof(struct FlSuiteResult), number_of_suites);
    for (size_t i=0; i < number_of_suites; i++)
    {
        results[i].suite = &suites[i];
        results[i].passedTests = fl_test_suite_run(suites[i]);
    }

    size_t ntests = 0;
    size_t nptests = 0;
    printf("+--------------------------+--------------+------------+\n");
    printf("| %-25s| %-12s | %-10s |\n", "Suite", "Passed/Total", "Percentage");
    printf("+--------------------------+--------------+------------+\n");
    for (size_t i=0; i < number_of_suites; i++)
    {
        ntests += suites[i]->ntests;
        nptests += results[i].passedTests;
        printf("| %-25s| %6zu/%-5zu | %-2s%3.2f%%%-1s |\n", suites[i]->name, results[i].passedTests, suites[i]->ntests, "", (results[i].passedTests / (float)suites[i]->ntests)*100, "");
        fl_test_suite_delete(suites[i]);
    }
    printf("+--------------------------+--------------+------------+\n");
    printf("| %-25s| %6zu/%-5zu | %-2s%3.2f%%%-1s |\n", "Total", nptests, ntests, "", (nptests/(float)ntests)*100, "");
    printf("+--------------------------+--------------+------------+\n");
    fl_array_delete(results);
}
