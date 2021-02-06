#include <stddef.h>
#include <stdbool.h>

#include <fllib/Std.h>
#include <fllib/Mem.h>
#include <fllib/Cstring.h>

#include "assert.h"
#include "assert/asserts.h"
#include "context.h"
#include "test.h"

#define FLUT__TEST_PREFIX "flut__test_"
#define FLUT__TEST_PREFIX_LENGTH 11

FlutAssertUtils* flut_assert_utils_new(void)
{
    FlutAssertUtils *asserts = fl_malloc(sizeof(FlutAssertUtils));

    // NULL
    asserts->null = flut__assert_null;
    asserts->not_null = flut__assert_not_null;

    // bool
    asserts->is_true = flut__assert_bool_true;
    asserts->is_false = flut__assert_bool_false;

    // size_t
    asserts->size_t.equals = flut__assert_size_t_equals;
    asserts->size_t.not_equals = flut__assert_size_t_not_equals;

    // str
    asserts->str.equals = flut__assert_str_equals;
    asserts->str.equals_n = flut__assert_str_equals_n;
    asserts->str.not_equals = flut__assert_str_not_equals;
    asserts->str.length = flut__assert_str_length;

    return asserts;
}

void flut_assert_utils_free(FlutAssertUtils *asserts)
{
    fl_free(asserts);
}


bool flut_assert_result(FlutContext *ctx, FlutAssertResult *result)
{
    bool success = result->success;
    // Remove the "flut__test_" part
    const char *function_name = strstr(result->funcname, FLUT__TEST_PREFIX);
    if (function_name == NULL) {
        function_name = result->funcname;
    } else {
        function_name = result->funcname + FLUT__TEST_PREFIX_LENGTH;
    }

    if (success) {
        printf(" |  |- PASS: %s\n", result->assertion);
    } else {
        printf(" |  |- FAIL: %s\n", result->assertion);
        printf(" |  |        In %s:%llu:%s\n", result->filename, result->line, function_name);
        printf(" |  |           %s\n", result->message);
    }

    flut_assert_result_free(result);

    if (success)
        return true;

    ctx->failed = true;
    Throw(&ctx->context, FL_TEST_FAILURE, NULL);
}
