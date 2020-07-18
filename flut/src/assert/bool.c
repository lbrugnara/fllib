#include <stddef.h>
#include <stdbool.h>

#include <fllib/Std.h>
#include <fllib/Mem.h>
#include <fllib/Cstring.h>

#include "result.h"

FlutAssertResult* flut_assert_bool_is_true(bool condition)
{
    struct FlutAssertResult *result = fl_malloc(sizeof(struct FlutAssertResult));

    result->success = condition;

    if (!result->success) {
        result->message = fl_cstring_dup("Expecting condition to be true, actually false");
    }

    return result;
}

FlutAssertResult* flut_assert_bool_is_false(bool condition)
{
    struct FlutAssertResult *result = fl_malloc(sizeof(struct FlutAssertResult));

    result->success = !condition;

    if (!result->success) {
        result->message = fl_cstring_dup("Expecting condition to be false, actually false");
    }

    return result;
}
