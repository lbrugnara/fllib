#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "context.h"
#include "expect.h"
#include "test.h"
#include <fllib/Error.h>


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
bool flut_expect_compat(const char *descr, bool conditionResult)
{
    if (!conditionResult) {
        printf(" |  |- FAIL: %s\n", descr);
        Throw(&test_restore_context_compat, FL_TEST_FAILURE, descr);
    }
    printf(" |  |- PASS: %s\n", descr);
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
bool flut_vexpect_compat(bool conditionResult, const char *format, ...)
{
    char descr[FL_CTX_MSG_SIZE];
    va_list args;
    va_start(args, format);
    vsnprintf(descr, FL_CTX_MSG_SIZE, format, args);
    va_end(args);

    if (!conditionResult) {
        printf(" |  |- FAIL: %s\n", descr);
        Throw(&test_restore_context_compat, FL_TEST_FAILURE, descr);
    }
    printf(" |  |- PASS: %s\n", descr);
    return true;
}
