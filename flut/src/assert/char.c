#include <stdbool.h>
#include <stddef.h>

#include <fllib/Cstring.h>
#include <fllib/Mem.h>
#include <fllib/Std.h>

#include "char.h"

FlutAssertResult* flut__assert_char_equals(char expected, char actual) {
    struct FlutAssertResult *result = flut_assert_result_new();

    result->success = expected == actual;

    if (!result->success) {
        result->message = fl_cstring_vdup("Expecting char to be equals to '%c', actually '%c'", expected, actual);
    }

    return result;
}

FlutAssertResult* flut__assert_char_not_equals(char expected, char actual) {
    struct FlutAssertResult *result = flut_assert_result_new();

    result->success = expected != actual;

    if (!result->success) {
        result->message = fl_cstring_vdup("Expecting string to be not-equals to '%c', actually '%c'", expected, actual);
    }

    return result;
}
