#include <stddef.h>
#include <stdbool.h>

#include <fllib/Std.h>
#include <fllib/Mem.h>
#include <fllib/Cstring.h>

#include "result.h"

FlutAssertResult* flut__assert_size_t_equals(size_t expected, size_t actual)
{
    struct FlutAssertResult *result = flut_assert_result_new();

    result->success = expected == actual;

    if (!result->success) {
        result->message = fl_cstring_vdup("Expecting size_t value to be equals to %zu, actually %zu", expected, actual);
    }

    return result;
}

FlutAssertResult* flut__assert_size_t_not_equals(size_t expected, size_t actual)
{
    struct FlutAssertResult *result = flut_assert_result_new();

    result->success = expected != actual;

    if (!result->success) {
        result->message = fl_cstring_vdup("Expecting size_t value to be not-equals to %zu, actually %zu", expected, actual);
    }

    return result;
}

FlutAssertResult* flut__assert_size_t_gt(size_t expected, size_t actual) {
    struct FlutAssertResult *result = flut_assert_result_new();

    result->success = actual > expected;

    if (!result->success) {
        result->message = fl_cstring_vdup("Expecting size_t value to be greater than %zu, actually %zu", expected, actual);
    }

    return result;
}
FlutAssertResult* flut__assert_size_t_gte(size_t expected, size_t actual) {
    struct FlutAssertResult *result = flut_assert_result_new();

    result->success = actual >= expected;

    if (!result->success) {
        result->message = fl_cstring_vdup("Expecting size_t value to be greater than or equals to %zu, actually %zu", expected, actual);
    }

    return result;
}
FlutAssertResult* flut__assert_size_t_lt(size_t expected, size_t actual) {
    struct FlutAssertResult *result = flut_assert_result_new();

    result->success = actual < expected;

    if (!result->success) {
        result->message = fl_cstring_vdup("Expecting size_t value to be lesser than %zu, actually %zu", expected, actual);
    }

    return result;
}
FlutAssertResult* flut__assert_size_t_lte(size_t expected, size_t actual) {
    struct FlutAssertResult *result = flut_assert_result_new();

    result->success = actual <= expected;

    if (!result->success) {
        result->message = fl_cstring_vdup("Expecting size_t value to be lesser than or equals to %zu, actually %zu", expected, actual);
    }

    return result;
}
