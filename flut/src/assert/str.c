#include <stdbool.h>
#include <stddef.h>

#include <fllib/Cstring.h>
#include <fllib/Mem.h>
#include <fllib/Std.h>

#include "result.h"

FlutAssertResult* flut_assert_str_equals(const char *expected, const char *actual, bool free_mem)
{
    struct FlutAssertResult *result = fl_malloc(sizeof(struct FlutAssertResult));

    result->success = strcmp(expected, actual) == 0;

    if (!result->success) {
        result->message = fl_cstring_vdup("Expecting string to be equals to \"%s\", actually \"%s\"", expected, actual);
    }

    if (free_mem)
        fl_cstring_free(actual);

    return result;
}

FlutAssertResult* flut_assert_str_equals_n(const char *expected, const char *actual, size_t n, bool free_mem)
{
    struct FlutAssertResult *result = fl_malloc(sizeof(struct FlutAssertResult));

    result->success = strncmp(expected, actual, n) == 0;

    if (!result->success) {
        result->message = fl_cstring_vdup("Expecting string to be equals to \"%s\", actually \"%s\"", expected, actual);
    }

    if (free_mem)
        fl_cstring_free(actual);

    return result;
}

FlutAssertResult* flut_assert_str_not_equals(const char *expected, const char *actual, bool free_mem)
{
    struct FlutAssertResult *result = fl_malloc(sizeof(struct FlutAssertResult));

    result->success = strcmp(expected, actual) != 0;

    if (!result->success) {
        result->message = fl_cstring_vdup("Expecting string to be not-equals to %s, actually %s", expected, actual);
    }

    if (free_mem)
        fl_cstring_free(actual);

    return result;
}
