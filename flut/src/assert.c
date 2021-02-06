#include <stddef.h>
#include <stdbool.h>

#include <fllib/Std.h>
#include <fllib/Mem.h>
#include <fllib/Cstring.h>

#include "assert.h"
#include "assert/asserts.h"
#include "context.h"
#include "test.h"

FlutAssertUtils* flut_assert_utils_new(void)
{
    FlutAssertUtils *asserts = fl_malloc(sizeof(FlutAssertUtils));

    // NULL
    asserts->null = flut_assert_null;
    asserts->not_null = flut_assert_not_null;

    // bool
    asserts->is_true = flut_assert_bool_is_true;
    asserts->is_false = flut_assert_bool_is_false;

    // size_t
    asserts->size_t.equals = flut_assert_size_t_equals;
    asserts->size_t.not_equals = flut_assert_size_t_not_equals;

    // str
    asserts->str.equals = flut_assert_str_equals;
    asserts->str.equals_n = flut_assert_str_equals_n;
    asserts->str.not_equals = flut_assert_str_not_equals;
    asserts->str.length = flut_assert_str_length;

    return asserts;
}

void flut_assert_utils_free(FlutAssertUtils *asserts)
{
    fl_free(asserts);
}


bool flut_assert_result(FlutContext *ctx, FlutAssertResult *result)
{
    bool success = result->success;
    // Remove the -probably- "assert->" part
    const char *assertion = strstr(result->assertion, "->");
    if (assertion != NULL) {
        assertion += 2;
    } else {
        assertion = result->assertion;
    }

    if (success) {
        printf(" |  |- PASS: %s\n", assertion);
    } else {
        printf(" |  |- FAIL: %s\n", assertion);
        printf(" |  |        In %s:%llu:%s\n", result->filename, result->line, result->funcname);
        printf(" |  |           %s\n", result->message);
    }

    flut_assert_result_free(result);

    if (success)
        return true;

    ctx->failed = true;
    Throw(&ctx->context, FL_TEST_FAILURE, NULL);
}
