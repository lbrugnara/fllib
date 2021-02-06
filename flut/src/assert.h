#ifndef FLUT_ASSERT_UTILS_H
#define FLUT_ASSERT_UTILS_H

#include <stddef.h>
#include <stdbool.h>
#include "assert/asserts.h"
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

#define flut_assert_is_null(obj)        flut_assert_explain(flut__assert_null((obj)), "Expression `"#obj"` should be null")
#define flut_assert_is_not_null(obj)     flut_assert_explain(flut__assert_not_null((obj)), "Expression `"#obj"` should not be null")

#define flut_assert_is_true(expression)    flut_assert_explain(flut__assert_bool_true((expression)), "Expression `"#expression"` should be true")
#define flut_assert_is_false(expression)   flut_assert_explain(flut__assert_bool_false((expression)), "Expression `"#expression"` should be false")

#define flut_assert_pointer_is_equals(expected, actual)         \
    flut_assert_explain(flut__assert_ptr_equals((expected), (actual)), "Pointer `"#actual"` should be equals to pointer `" #expected "`")
#define flut_assert_pointer_is_not_equals(expected, actual)     \
    flut_assert_explain(flut__assert_ptr_not_equals((expected), (actual)), "Pointer `"#actual"` should not be equals to pointer `" #expected "`")

#define flut_assert_size_t_is_equals(expected, actual)         \
    flut_assert_vexplain(flut__assert_size_t_equals((expected), (actual)), "size_t `"#actual"` (%zu) should be equals to size_t `" #expected "` (%zu)", (actual), (expected))

#define flut_assert_size_t_is_not_equals(expected, actual)     \
    flut_assert_vexplain(flut__assert_size_t_not_equals((expected), (actual)), "size_t `"#actual"` (%zu) should not be equals to size_t `" #expected "` (%zu)", (actual), (expected))

#define flut_assert_int_is_equals(expected, actual)         \
    flut_assert_vexplain(flut__assert_int_equals((expected), (actual)), "Integer `"#actual"` (%zu) should be equals to integer `" #expected "` (%zu)", (actual), (expected))
#define flut_assert_int_is_not_equals(expected, actual)     \
    flut_assert_vexplain(flut__assert_int_not_equals((expected), (actual)), "Integer `"#actual"` (%zu) should not be equals to integer `" #expected "` (%zu)", (actual), (expected))

#define flut_assert_string_is_equals(expected, actual, shouldFreeActual)        \
    flut_assert_vexplain(flut__assert_str_equals((expected), (actual), (shouldFreeActual)), "String `"#actual"` (\"%s\") should be equals to string `" #expected "` (\"%s\")", (actual), (expected))

#define flut_assert_string_is_equals_n(expected, actual, length, shouldFreeActual)                                      \
    flut_assert_vexplain(flut__assert_str_equals_n((expected), (actual), (length), (shouldFreeActual)),                 \
        "%zu characters from string `"#actual"` (\"%.*s\") should be equals to string `" #expected "` (\"%.*s\")", (length), (length), (actual), (length), (expected))

#define flut_assert_string_is_not_equals(expected, actual, shouldFreeActual)    \
    flut_assert_vexplain(flut__assert_str_not_equals((expected), (actual), (shouldFreeActual)), "String `"#actual"` (\"%s\") should not be equals to string `" #expected "` (\"%s\")", (actual), (expected))

#define flut_assert_string_has_length(expected, string, shouldFreeString)       \
    flut_assert_vexplain(flut__assert_str_length((expected), (string), (shouldFreeString)), "String `"#string"` (\"%s\") should be %zu characters length", (string), (expected))

#define flut_unexpected(error_msg)                                                                          \
do {                                                                                                        \
    struct FlutAssertResult *result = fl_malloc(sizeof(struct FlutAssertResult));                           \
    result->success = false;                                                                                \
    result->message = fl_cstring_dup(error_msg);                                                            \
    flut_assert_result(flut__internal_ctx, flut_do_assert(result, result, "%s", "Unexpected scenario"));    \
} while (0)

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
