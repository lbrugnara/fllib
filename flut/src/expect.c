#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdarg.h>

#include <fllib/Error.h>
#include "expect.h"
#include "context.h"
#include "test.h"

extern struct FlContext test_restore_context_compat;

/*
 * Function: flut_expect_compat
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
bool flut_expect_compat(const char* descr, bool conditionResult)
{
    if (!conditionResult)
    {
        printf("    FAIL: %s\n", descr);
        Throw(&test_restore_context_compat, FL_TEST_FAILURE, descr);
    }
    printf("    PASS: %s\n", descr);
    return true;
}

/*
 * Function: flut_expect_compat
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
bool flut_vexpect_compat(bool conditionResult, const char* format, ...)
{
    char descr[FL_CTX_MSG_SIZE];
    va_list args;
    va_start(args, format);
    vsnprintf(descr, FL_CTX_MSG_SIZE, format, args);
    va_end(args);

    if (!conditionResult)
    {
        printf("    FAIL: %s\n", descr);
        Throw(&test_restore_context_compat, FL_TEST_FAILURE, descr);
    }
    printf("    PASS: %s\n", descr);
    return true;
}

bool flut_expect_result(FlutContext *ctx, FlutAssertResult *result)
{
    bool success = result->success;
    // Remove the -probably- "assert->" part
    const char *assertion = strstr(result->assertion, "->");
    if (assertion != NULL) 
    {
        assertion += 2;
    }
    else
    {
        assertion = result->assertion;
    }

    char description[FL_CTX_MSG_SIZE];
    snprintf(description, FL_CTX_MSG_SIZE, "%s:%llu (%s) condition \"%s\"\n\t\t%s", 
        result->filename, 
        result->line, 
        result->funcname, 
        assertion,
        (success ? "" : result->message));

    printf("    %s: %s\n", (success ? "PASS" : "FAIL"), assertion);

    flut_assert_result_free(result);

    if (success)
        return true;

    Throw(&ctx->context, FL_TEST_FAILURE, description);
}
