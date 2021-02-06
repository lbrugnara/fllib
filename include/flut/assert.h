#ifndef FLUT_ASSERT_UTILS_H
#define FLUT_ASSERT_UTILS_H

#include <stddef.h>
#include <stdbool.h>
#include "assert/result.h"
#include "context.h"

#define flut_do_assert(result, call, format, ...)                                                                       \
    ((result) = call,                                                                                                   \
     (result)->filename = __FILE__,                                                                                     \
     (result)->funcname = __func__,                                                                                     \
     (result)->line = __LINE__,                                                                                         \
     (result)->assertion = fl_cstring_vdup(format, __VA_ARGS__),                                                        \
     (result))

#define flut_assert(assertion)                                                                                          \
    do {                                                                                                                \
        FlutAssertResult *result = NULL;                                                                                \
        flut_assert_result(flut__internal_ctx, flut_do_assert(result, assertion, "%s", #assertion));                    \
    } while (0)

#define flut_assert_vexplain(assertion, format, ...)                                                                    \
    do {                                                                                                                \
        FlutAssertResult *result = NULL;                                                                                \
        flut_assert_result(flut__internal_ctx, flut_do_assert(result, assertion, format, __VA_ARGS__));                 \
    } while (0)

#define flut_assert_explain(assertion, message) flut_assert_vexplain(assertion, "%s", message)

typedef struct FlutAssertUtils {
    FlutAssertResult* (*null)(void *obj);
    FlutAssertResult* (*not_null)(void *obj);
    FlutAssertResult* (*is_true)(bool condition);
    FlutAssertResult* (*is_false)(bool condition);

    struct {
        FlutAssertResult* (*equals)(size_t expected, size_t actual);
        FlutAssertResult* (*not_equals)(size_t expected, size_t actual);
    } size_t;

    struct {
        FlutAssertResult* (*equals)(const char *expected, const char *actual, bool free_mem);
        FlutAssertResult* (*equals_n)(const char *expected, const char *actual, size_t n, bool free_mem);
        FlutAssertResult* (*not_equals)(const char *expected, const char *actual, bool free_mem);
        FlutAssertResult* (*length)(size_t expected_length, const char *str, bool free_mem);        
    } str;
} FlutAssertUtils;

FlutAssertUtils* flut_assert_utils_new(void);
bool flut_assert_result(FlutContext *ctx, FlutAssertResult *result);
void flut_assert_utils_free(FlutAssertUtils *asserts);

#endif /* FLUT_ASSERT_UTILS_H */
