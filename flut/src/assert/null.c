#include <stddef.h>
#include <stdbool.h>

#include <fllib/Std.h>
#include <fllib/Mem.h>
#include <fllib/Cstring.h>

#include "result.h"

FlutAssertResult* flut__assert_null(void *obj)
{
    struct FlutAssertResult *result = flut_assert_result_new();

    result->success = obj == NULL;

    if (!result->success) {
        result->message = fl_cstring_dup("Expecting expression to be NULL, actually not NULL");
    }

    return result;
}

FlutAssertResult* flut__assert_not_null(void *obj)
{
    struct FlutAssertResult *result = flut_assert_result_new();

    result->success = obj != NULL;

    if (result->success) {
        result->message = fl_cstring_dup("Expecting expression to be not NULL, actually NULL");
    }

    return result;
}
