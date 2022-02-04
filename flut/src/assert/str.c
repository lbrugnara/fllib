#include <stdbool.h>
#include <stddef.h>

#include <fllib/Cstring.h>
#include <fllib/Mem.h>
#include <fllib/Std.h>

#include "result.h"

FlutAssertResult* flut__assert_str_equals(const char *expected, const char *actual, bool free_mem)
{
    struct FlutAssertResult *result = flut_assert_result_new();

    result->success = strcmp(expected, actual) == 0;

    if (!result->success) {
        result->message = fl_cstring_vdup("Expecting string to be equals to \"%s\", actually \"%s\"", expected, actual);
    }

    if (free_mem)
        fl_cstring_free(actual);

    return result;
}

FlutAssertResult* flut__assert_str_equals_n(const char *expected, const char *actual, size_t n, bool free_mem)
{
    struct FlutAssertResult *result = flut_assert_result_new();

    result->success = strncmp(expected, actual, n) == 0;

    if (!result->success) {
        result->message = fl_cstring_vdup("Expecting string to be equals to \"%s\", actually \"%s\"", expected, actual);
    }

    if (free_mem)
        fl_cstring_free(actual);

    return result;
}

FlutAssertResult* flut__assert_str_not_equals(const char *expected, const char *actual, bool free_mem)
{
    struct FlutAssertResult *result = flut_assert_result_new();

    result->success = strcmp(expected, actual) != 0;

    if (!result->success) {
        result->message = fl_cstring_vdup("Expecting string to be not-equals to %s, actually %s", expected, actual);
    }

    if (free_mem)
        fl_cstring_free(actual);

    return result;
}

FlutAssertResult* flut__assert_str_length(size_t expected_length, const char *str, bool free_mem)
{
    struct FlutAssertResult *result = flut_assert_result_new();

    result->success = str != NULL && strlen(str) == expected_length;

    if (!result->success) {
        if (str == NULL) {
            result->message = fl_cstring_vdup("Expecting string to have length %zu, actually NULL", expected_length);
        } else {
            result->message = fl_cstring_vdup("Expecting string to have length %zu, actually %zu", expected_length, strlen(str));
        }
    }

    if (free_mem) {
        fl_cstring_free(str);
    }

    return result;
}
