/*
 * file: Tests
 *
 * This is a basic module to run unit tests. It is very basic and
 * it was intended to be used as soon as possible, it will need
 * some refactoring to make it more pleasant to work with, robust
 * and extensible..
 *
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include <fllib.h>
#include "Test.h"

/*
 * Type: struct FlTestSuite
 *
 * Represents a test suite that contains a set of tests to run
 *
 * {member: char* name} Suite name
 * {member: size_t ntests} Number of tests that compose the suite
 * {member: FlTest* tests} Pointer to the set of tests
 *
 */
struct FlTestSuite
{
    const char *name;
    size_t ntests;
    const FlTest *tests;
};

struct FlTestSuiteResult {
    FlTestSuite *suite;
    size_t passedTests;
    long elapsed;
    bool ran;
};

/*
 * Function: fl_test_suite_new
 *
 * Creates a test suite
 *
 * const char* name - Suite name
 * const FlTest[] tests - Array of tests that compose the suite
 * size_t ntests - Number of tests that compose the suite
 *
 * {return: FlTestSuite} Suite pointer
 *
 */
FlTestSuite fl_test_suite_new(const char *name, const FlTest *tests, size_t ntests)
{
    FlTestSuite t = fl_malloc(sizeof(struct FlTestSuite));

    if (t == NULL)
        return NULL;

    t->name = name;
    t->ntests = ntests;
    t->tests = tests;
    return t;
}

/*
 * Function: fl_test_suite_free
 *
 * Delete a suite freeing its memory
 *
 * FlTestSuite suite - Target suite to be removed
 *
 * {return: void}
 *
 */
void fl_test_suite_free(FlTestSuite suite)
{
    fl_free(suite);
}

/*
 * Running tests
 *
 */
enum {
    TEST_FAILURE = 1,
    TEST_EXCEPTION
};

/*
 * {variable: struct FlContext testctx}
 *
 * Will keep the state of the program before running a test
 * to be able to go back a continue the execution
 *
 */
static struct FlContext testctx = FL_CTX_STATIC_INIT;

/*
 * Function: test_signal_handler
 *
 * Signal handler function for POSIX platforms. When a signal
 * is raised, this handler will trigger an exception.
 *
 * int sign - Signal number
 *
 * {return: void}
 *
 */
void test_signal_handler(int sign)
{
    char msg[FL_CTX_MSG_SIZE];
    snprintf(msg, FL_CTX_MSG_SIZE, "Signal %d", sign);
    Throw(&testctx, TEST_FAILURE, msg);
}

#ifdef _WIN32
#include "../src/os/WinEx.h"
/*
 * Function: exception_filter
 *
 * Exception filter function for Windows platforms. Capture an 
 * exception raised by Win32 API. When that happens, it throws
 * an exception.
 *
 * EXCEPTION_POINTERS ExceptionInfo - Information about the exception
 *
 * {return: LONG} Returns EXCEPTION_CONTINUE_EXECUTION to resume the execution
 *  or EXCEPTION_CONTINUE_SEARCH when the program can't continue
 *
 */
LONG WINAPI exception_filter(EXCEPTION_POINTERS * ExceptionInfo)
{
    if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_NONCONTINUABLE_EXCEPTION)
        return EXCEPTION_CONTINUE_SEARCH; // Let Win32 resolve
    // An error condition occured, so take the brief description and Throw the exception
    // to the Try()
    char message[FL_CTX_MSG_SIZE];
    fl_winex_message_get(ExceptionInfo->ExceptionRecord->ExceptionCode, message, FL_CTX_MSG_SIZE);
    Throw(&testctx, TEST_EXCEPTION, message);
    return EXCEPTION_CONTINUE_EXECUTION;
}
#endif

/*
 * Function: fl_expect
 *
 * Simple functions that throws an exception when the condition
 * is false
 *
 * const char* descr - Description of the conditions to check
 * bool conditionResult - Condition result
 *
 * {return: bool} true if conditionResult is valid, throws exception if not
 *
 */
bool fl_expect(const char* descr, bool conditionResult)
{
    if (!conditionResult)
    {
        printf(" |-- Failed: %s\n", descr);
        Throw(&testctx, TEST_FAILURE, descr);
    }
    printf(" |-- Passed: %s\n", descr);
    return true;
}

/*
 * Function: fl_expect
 *
 * Simple functions that throws an exception when the condition
 * is false
 *
 * const char* descr - Description of the conditions to check
 * bool conditionResult - Condition result
 *
 * {return: bool} true if conditionResult is valid, throws exception if not
 *
 */
bool fl_vexpect(bool conditionResult, const char* format, ...)
{
    char descr[FL_CTX_MSG_SIZE];
    va_list args;
    va_start(args, format);
    vsnprintf(descr, FL_CTX_MSG_SIZE, format, args);
    va_end(args);

    if (!conditionResult)
    {
        printf(" |-- Failed: %s\n", descr);
        Throw(&testctx, TEST_FAILURE, descr);
    }
    printf(" |-- Passed: %s\n", descr);
    return true;
}

/*
 * Function: run_suite
 *
 * Run a suite of tests
 *
 * FlTestSuite suite - Target suite to be ran
 *
 * {return: size_t} number of passed tests
 *
 */
void run_suite(FlTestSuite suite, struct FlTestSuiteResult *result)
{
    #ifdef _WIN32
    FlWinExceptionHandler global_handler = fl_winex_global_handler_set(exception_filter);
    #endif
    fl_signal_global_handler_set(test_signal_handler);

    printf("============================\n");
    printf("Test Suite: %s\n", suite->name);
    printf("----------------------------\n");

    size_t failed_tests = 0;

    for (size_t i=0; i < suite->ntests; i++)
    {
        printf(" +-- Test Case: %s\n", suite->tests[i].name);

        volatile bool failed = false;

        FlTimer timer = fl_timer_create();
        fl_timer_start(timer);

        Try (&testctx)
        {
            suite->tests[i].run();
        }
        Catch(TEST_EXCEPTION)
        {
            failed_tests++;
            failed = true;
        }
        Rest
        {
            failed_tests++;
            failed = true;
        }
        EndTry;
        
        fl_timer_end(timer);

        if (failed)
        {
            printf(" +-- Assertion failed: %s\n\n", fl_ctx_last_frame(&testctx)->message);
        }
        else
        {
            printf(" +-- OK: %ld ms\n\n", fl_timer_elapsed_ms(timer));
        }

        result->elapsed += fl_timer_elapsed_ms(timer);

        fl_timer_free(timer);
    }
    
    result->ran = true;
    result->passedTests = suite->ntests - failed_tests;

    printf("\n");

    #ifdef _WIN32
    fl_winex_global_handler_set(global_handler);
    #endif
}

bool should_run_suite(struct FlTestSuite *suite, int argc, char **argv)
{
    if (argc == 1)
        return true;

    if (argc == 2 && flm_cstring_equals("all", argv[1]))
        return true;

    size_t suite_name_length = strlen(suite->name);
    for (int j=1; j < argc; j++)
    {
        size_t arg_length = strlen(argv[j]);
        
        if (arg_length != suite_name_length)
            continue;

        for (size_t k=0; k < suite_name_length; k++)
        {
            if (tolower(suite->name[k]) != tolower(argv[j][k]))
                break;

            if (k == suite_name_length - 1)
                return true;
        }
    }

    return false;
}

void fl_test_run_all(int argc, char **argv, FlTestSuite *suites)
{
    size_t number_of_suites = 0;
    for (;number_of_suites < SIZE_MAX && suites[number_of_suites]; number_of_suites++);

    struct FlTestSuiteResult *results = fl_array_new(sizeof(struct FlTestSuiteResult), number_of_suites);
    
    bool any = false;
    unsigned long long totalElapsedTime = 0;
    for (size_t i=0; i < number_of_suites; i++)
    {
        results[i].suite = &suites[i];
        results[i].ran = false;
        
        if (!should_run_suite(suites[i], argc, argv))
            continue;

        any = true;
        run_suite(suites[i], &results[i]);
        totalElapsedTime += results[i].elapsed;
    }

    if (!any)
    {
        printf("No tests have been run.\n");
        return;
    }

    size_t ntests = 0;
    size_t nptests = 0;
    printf("+--------------------------+--------------+------------+------------+\n");
    printf("| %-25s| %-12s | %-10s | %-10s |\n", "Suite", "Passed/Total", "Percentage", "Time (ms)");
    printf("+--------------------------+--------------+------------+------------+\n");
    for (size_t i=0; i < number_of_suites; i++)
    {
        if (!results[i].ran)
        {
            fl_test_suite_free(suites[i]);
            continue;
        }

        ntests += suites[i]->ntests;
        nptests += results[i].passedTests;
        printf("| %-25s| %6zu/%-5zu | %-2s%3.2f%%%-1s | %7ld ms |\n", suites[i]->name, results[i].passedTests, suites[i]->ntests, "", (results[i].passedTests / (float)suites[i]->ntests)*100, "", results[i].elapsed);
        fl_test_suite_free(suites[i]);
    }
    printf("+--------------------------+--------------+------------+------------+\n");
    printf("| %-25s| %6zu/%-5zu | %-2s%3.2f%%%-1s | %7llu ms |\n", "Total", nptests, ntests, "", (nptests/(float)ntests)*100, "", totalElapsedTime);
    printf("+--------------------------+--------------+------------+------------+\n");
    fl_array_free(results);
}
