#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "Test.h"

struct FlTestSuite
{
    const char *name;
    size_t ntests;
    const FlTest *tests;
};

FlTestSuite* _fl_test_suite_new(const char *name, const FlTest tests[], size_t ntests)
{
    FlTestSuite *t = calloc(1, sizeof(FlTestSuite));
    t->name = name;
    t->ntests = ntests;
    t->tests = tests;
    return t;
}

void fl_test_suite_delete(FlTestSuite* suite)
{
    free(suite);
}

/* -------------------------------------------------------------
 * Running tests
 * -------------------------------------------------------------
 */
#define TEST_FAILURE 1
#define TEST_EXCEPTION 2

static FlTryContext testctx;

#ifdef _WIN32
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

bool fl_expect(const char* name, bool condition)
{
    printf("    |-- %s: %s\n", name, (condition ? "Pass" : "Fail"));
    if (!condition)
        Throw(&testctx, TEST_FAILURE);
    return true;
}

void fl_test_suite_run(FlTestSuite *suite)
{
    #ifdef _WIN32
    FlWinExceptionHandler prevh = fl_winex_global_handler_set(exception_filter);
    #endif
    printf("============================\n");
    printf("Test Suite: %s\n", suite->name);
    printf("----------------------------\n");
    for (int i=0; i < suite->ntests; i++)
    {
        printf(" - Test Case: %s\n", suite->tests[i].name);
        Try (&testctx)
            suite->tests[i].run();  
        Catch (TEST_FAILURE)
            printf("Test failure: %s\n", testctx.message);
        Catch (TEST_EXCEPTION)
            printf("Test exception: %s\n", testctx.message);
        EndTry;
    }
    printf("----------------------------\n");
    printf("Summary\n");
    printf("----------------------------\n");
    printf(" - Succesful Tests: %d\n", 6);
    printf(" - Failed Tests: %d\n", 1);
    printf("    |-- %s\n", "Fail Misserably");
    #ifdef _WIN32
    fl_winex_global_handler_set(prevh);
    #endif
}