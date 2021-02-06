#include <stddef.h>
#include <stdbool.h>

#include <fllib/Std.h>
#include <fllib/Mem.h>
#include <fllib/Cstring.h>

#include "result.h"

FlutAssertResult* flut__assert_int_equals(int expected, int actual)
{
    struct FlutAssertResult *result = fl_malloc(sizeof(struct FlutAssertResult));

    result->success = expected == actual;

    if (!result->success) {
        result->message = fl_cstring_vdup("Expecting int value to be equals to %d, actually %d", expected, actual);
    }

    return result;
}

FlutAssertResult* flut__assert_int_not_equals(int expected, int actual)
{
    struct FlutAssertResult *result = fl_malloc(sizeof(struct FlutAssertResult));

    result->success = expected != actual;

    if (!result->success) {
        result->message = fl_cstring_vdup("Expecting int value to be not-equals to %d, actually %d", expected, actual);
    }

    return result;
}
