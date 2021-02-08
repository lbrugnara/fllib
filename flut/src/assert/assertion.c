#include <stddef.h>
#include <stdbool.h>

#include <fllib/Std.h>
#include <fllib/Mem.h>
#include <fllib/Cstring.h>

#include "assertion.h"
#include "../context.h"
#include "../test.h"

#define FLUT__TEST_PREFIX "flut__test_"
#define FLUT__TEST_PREFIX_LENGTH 11

bool flut_assertion_process_result(FlutContext *ctx, FlutAssertResult *result)
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
