#include <stddef.h>
#include <stdbool.h>

#include <fllib/Std.h>
#include <fllib/Mem.h>
#include <fllib/Cstring.h>

#include "result.h"

FlutAssertResult* flut_assert_size_t_equals(size_t expected, size_t actual)
{
    struct FlutAssertResult *result = fl_malloc(sizeof(struct FlutAssertResult));

    result->success = expected == actual;

    if (!result->success) {
        result->message = fl_cstring_vdup("Expecting size_t value to be equals to %zu, actually %zu", expected, actual);
    }

    return result;
}

FlutAssertResult* flut_assert_size_t_not_equals(size_t expected, size_t actual)
{
    struct FlutAssertResult *result = fl_malloc(sizeof(struct FlutAssertResult));

    result->success = expected != actual;

    if (!result->success) {
        result->message = fl_cstring_vdup("Expecting size_t value to be not-equals to %zu, actually %zu", expected, actual);
    }

    return result;
}
