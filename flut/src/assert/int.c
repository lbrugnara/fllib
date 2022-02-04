#include <stddef.h>
#include <stdbool.h>

#include <fllib/Std.h>
#include <fllib/Mem.h>
#include <fllib/Cstring.h>

#include "result.h"

FlutAssertResult* flut__assert_int_equals(int expected, int actual)
{
    struct FlutAssertResult *result = flut_assert_result_new();

    result->success = expected == actual;

    if (!result->success) {
        result->message = fl_cstring_vdup("Expecting int value to be equals to %d, actually %d", expected, actual);
    }

    return result;
}

FlutAssertResult* flut__assert_int_not_equals(int expected, int actual)
{
    struct FlutAssertResult *result = flut_assert_result_new();

    result->success = expected != actual;

    if (!result->success) {
        result->message = fl_cstring_vdup("Expecting int value to be not-equals to %d, actually %d", expected, actual);
    }

    return result;
}

FlutAssertResult* flut__assert_int_gt(int expected, int actual) {
    struct FlutAssertResult *result = flut_assert_result_new();

    result->success = actual > expected;

    if (!result->success) {
        result->message = fl_cstring_vdup("Expecting int value to be greater than %d, actually %d", expected, actual);
    }

    return result;
}
FlutAssertResult* flut__assert_int_gte(int expected, int actual) {
    struct FlutAssertResult *result = flut_assert_result_new();

    result->success = actual >= expected;

    if (!result->success) {
        result->message = fl_cstring_vdup("Expecting int value to be greater than or equals to %d, actually %d", expected, actual);
    }

    return result;
}
FlutAssertResult* flut__assert_int_lt(int expected, int actual) {
    struct FlutAssertResult *result = flut_assert_result_new();

    result->success = actual < expected;

    if (!result->success) {
        result->message = fl_cstring_vdup("Expecting int value to be lesser than %d, actually %d", expected, actual);
    }

    return result;
}
FlutAssertResult* flut__assert_int_lte(int expected, int actual) {
    struct FlutAssertResult *result = flut_assert_result_new();

    result->success = actual <= expected;

    if (!result->success) {
        result->message = fl_cstring_vdup("Expecting int value to be lesser than or equals to %d, actually %d", expected, actual);
    }

    return result;
}
