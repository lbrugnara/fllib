#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include <fllib/Error.h>
#include <fllib/Mem.h>
#include <fllib/os/Signal.h>
#include <fllib/os/Timer.h>

#include "context.h"
#include "suite.h"
#include "test.h"


struct FlContext test_restore_context_compat = FL_CTX_STATIC_INIT;

/*
 * Function: flut_suite_new
 *
 * Creates a test suite
 *
 * const char* name - Suite name
 * const FlutTestCase[] tests - Array of tests that compose the suite
 * size_t length - Number of tests that compose the suite
 *
 * {return: FlutSuite} Suite pointer
 *
 */
FlutSuite *flut_suite_new(const char *name)
{
    FlutSuite *suite = fl_malloc(sizeof(struct FlutSuite));

    if (suite == NULL)
        return NULL;

    suite->name = fl_cstring_dup(name);
    suite->tests = fl_array_new(sizeof(FlutTestCase), 0);

    return suite;
}

/*
 * Function: flut_suite_free
 *
 * Delete a suite freeing its memory
 *
 * FlutSuite suite - Target suite to be removed
 *
 * {return: void}
 *
 */
void flut_suite_free(FlutSuite *suite)
{
    fl_cstring_free(suite->name);
    fl_cstring_free(suite->description);

    for (size_t i = 0; i < fl_array_length(suite->tests); i++) {
        fl_cstring_free(suite->tests[i].name);
    }

    fl_array_free(suite->tests);

    fl_free(suite);
}

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
static void test_signal_handler(int sign)
{
    char msg[FL_CTX_MSG_SIZE];
    snprintf(msg, FL_CTX_MSG_SIZE, "Signal %d", sign);
    Throw(&test_restore_context_compat, FL_TEST_FAILURE, msg);
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
static LONG WINAPI exception_filter(EXCEPTION_POINTERS *ExceptionInfo)
{
    if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_NONCONTINUABLE_EXCEPTION)
        return EXCEPTION_CONTINUE_SEARCH; // Let Win32 resolve
    // An error condition occured, so take the brief description and Throw the exception
    // to the Try()
    char message[FL_CTX_MSG_SIZE];
    fl_winex_message_get(ExceptionInfo->ExceptionRecord->ExceptionCode, message, FL_CTX_MSG_SIZE);
    Throw(&test_restore_context_compat, FL_TEST_EXCEPTION, message);
    return EXCEPTION_CONTINUE_EXECUTION;
}
#endif

void flut_suite_run(FlutSuite *suite, FlutSuiteResult *result)
{
#ifdef _WIN32
    FlWinExceptionHandler global_handler = fl_winex_global_handler_set(exception_filter);
#endif
    fl_signal_global_handler_set(test_signal_handler);

    printf("TEST SUITE: %s - %s\n", suite->name, suite->description);
    printf(" |\n");

    size_t failed_tests = 0;
    size_t number_of_tests = fl_array_length(suite->tests);

    for (size_t i = 0; i < number_of_tests; i++) {
        printf(" |- TEST CASE: %s\n", suite->tests[i].name);

        FlutContext *test_context = flut_context_new();
        volatile bool failed = false;

        FlTimer *timer = fl_timer_create();
        fl_timer_start(timer);

        Try(&test_context->context)
        {
            Try(&test_restore_context_compat)
            {
                suite->tests[i].run(test_context);

                if (test_context->failed) {
                    failed_tests++;
                    failed = true;
                }
            }
            Rest
            {
                struct FlContextFrame frame = test_restore_context_compat.frames[test_restore_context_compat.fp];
                Throw(&test_context->context, frame.exception, frame.message);
            }
            EndTry;
        }
        Catch(FL_TEST_EXCEPTION)
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

        if (failed) {
            struct FlContextFrame *frame = fl_ctx_frame_last(&(test_context->context));
            printf(" |  +- CASE FAILED");
            if (frame->message[0] != '\0')
                printf(": %s", frame->message);
        } else {
            printf(" |  +- CASE PASSED: %ld ms", fl_timer_elapsed_ms(timer));
        }

        if (i < number_of_tests - 1)
            printf("\n |");

        printf("\n");

        result->elapsed += fl_timer_elapsed_ms(timer);

        fl_timer_free(timer);
        flut_context_free(test_context);
    }

    printf(" |\n +- PASSED TESTS: %zu/%zu\n", (number_of_tests - failed_tests), number_of_tests);

    result->ran = true;
    result->passed_tests = number_of_tests - failed_tests;

#ifdef _WIN32
    fl_winex_global_handler_set(global_handler);
#endif
}
