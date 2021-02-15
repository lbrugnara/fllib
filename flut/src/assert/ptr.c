#include <stddef.h>
#include <stdbool.h>

#include <fllib/Std.h>
#include <fllib/Mem.h>
#include <fllib/Cstring.h>

#include "ptr.h"

FlutAssertResult* flut__assert_ptr_equals(void *expected, void *actual)
{
    struct FlutAssertResult *result = flut_assert_result_new();

    result->success = expected == actual;

    if (!result->success) {
        result->message = fl_cstring_vdup("Expecting pointers to be equals", expected, actual);
    }

    return result;
}

FlutAssertResult* flut__assert_ptr_not_equals(void *expected, void *actual)
{
    struct FlutAssertResult *result = flut_assert_result_new();

    result->success = expected != actual;

    if (!result->success) {
        result->message = fl_cstring_vdup("Expecting pointers to be not-equals", expected, actual);
    }

    return result;
}
