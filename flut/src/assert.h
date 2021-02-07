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

// Null
#define flut_assert_is_null(obj)        flut_assert_explain(flut__assert_null((obj)), "Expression `"#obj"` should be null")
#define flut_assert_is_not_null(obj)     flut_assert_explain(flut__assert_not_null((obj)), "Expression `"#obj"` should not be null")

// Boolean
#define flut_assert_is_true(expression)    flut_assert_explain(flut__assert_bool_true((expression)), "Expression `"#expression"` should be true")
#define flut_assert_is_false(expression)   flut_assert_explain(flut__assert_bool_false((expression)), "Expression `"#expression"` should be false")

// Pointers
#define flut_assert_pointer_is_equals(expected, actual)         \
    flut_assert_explain(flut__assert_ptr_equals((expected), (actual)), "Pointer `"#actual"` should be equals to pointer `" #expected "`")
#define flut_assert_pointer_is_not_equals(expected, actual)     \
    flut_assert_explain(flut__assert_ptr_not_equals((expected), (actual)), "Pointer `"#actual"` should not be equals to pointer `" #expected "`")

// size_t
#define flut_assert_size_t_is_equals(expected, actual)                                                                              \
do {                                                                                                                                \
    flut_assert_vexplain(                                                                                                           \
        flut__assert_size_t_equals((expected), (actual)),                                                                           \
        "size_t expression `"#actual"` should be equals to size_t expression `" #expected "` (%zu == %zu)", (actual), (expected)    \
    );                                                                                                                              \
} while (0)

#define flut_assert_size_t_is_not_equals(expected, actual)                                                                              \
do {                                                                                                                                    \
    flut_assert_vexplain(                                                                                                               \
        flut__assert_size_t_not_equals((expected), (actual)),                                                                           \
        "size_t expression `"#actual"` should not be equals to size_t expression `" #expected "` (%zu != %zu)", (actual), (expected)    \
    );                                                                                                                                  \
} while(0)

#define flut_assert_size_t_is_greater_than(expected, actual)                                                                            \
do {                                                                                                                                    \
    flut_assert_vexplain(                                                                                                               \
        flut__assert_size_t_gt((expected), (actual)),                                                                                   \
        "size_t expression `"#actual"` should be greater than size_t expression `" #expected "` (%zu > %zu)", (actual), (expected)      \
    );                                                                                                                                  \
} while (0)

#define flut_assert_size_t_is_greater_than_or_equals(expected, actual)                                                                              \
do {                                                                                                                                                \
    flut_assert_vexplain(                                                                                                                           \
        flut__assert_size_t_gte((expected), (actual)),                                                                                              \
        "size_t expression `"#actual"` should be greater than or equals to size_t expression `" #expected "` (%zu >= %zu)", (actual), (expected)    \
    );                                                                                                                                              \
} while (0)

#define flut_assert_size_t_is_lesser_than(expected, actual)                                                                             \
do {                                                                                                                                    \
    flut_assert_vexplain(                                                                                                               \
        flut__assert_size_t_lt((expected), (actual)),                                                                                   \
        "size_t expression `"#actual"` should be lesser than size_t expression `" #expected "` (%zu < %zu)", (actual), (expected)       \
    );                                                                                                                                  \
} while (0)

#define flut_assert_size_t_is_lesser_than_or_equals(expected, actual)                                                                               \
do {                                                                                                                                                \
    flut_assert_vexplain(                                                                                                                           \
        flut__assert_size_t_lte((expected), (actual)),                                                                                              \
        "size_t expression `"#actual"` should be lesser than or equals to size_t expression `" #expected "` (%zu <= %zu)", (actual), (expected)     \
    );                                                                                                                                              \
} while (0)

// int
#define flut_assert_int_is_equals(expected, actual)                                                                                 \
do {                                                                                                                                \
    flut_assert_vexplain(                                                                                                           \
        flut__assert_int_equals((expected), (actual)),                                                                              \
        "Integer expression `"#actual"` should be equals to integer expression `" #expected "` (%zu == %zu)", (actual), (expected)  \
    );                                                                                                                              \
} while (0)

#define flut_assert_int_is_not_equals(expected, actual)                                                                                 \
do {                                                                                                                                    \
    flut_assert_vexplain(                                                                                                               \
        flut__assert_int_not_equals((expected), (actual)),                                                                              \
        "Integer expression `"#actual"` should not be equals to integer expression `" #expected "` (%zu != %zu)", (actual), (expected)  \
    );                                                                                                                                  \
} while (0)

#define flut_assert_int_is_greater_than(expected, actual)                                                                               \
do {                                                                                                                                    \
    flut_assert_vexplain(                                                                                                               \
        flut__assert_int_gt((expected), (actual)),                                                                                      \
        "Integer expression `"#actual"` should be greater than integer expression `" #expected "` (%zu > %zu)", (actual), (expected)    \
    );                                                                                                                                  \
} while (0)

#define flut_assert_int_is_greater_than_or_equals(expected, actual)                                                                                 \
do {                                                                                                                                                \
    flut_assert_vexplain(                                                                                                                           \
        flut__assert_int_gte((expected), (actual)),                                                                                                 \
        "Integer expression `"#actual"` should be greater than or equals to integer expression `" #expected "` (%zu >= %zu)", (actual), (expected)  \
    );                                                                                                                                              \
} while (0)

#define flut_assert_int_is_lesser_than(expected, actual)                                                                                \
do {                                                                                                                                    \
    flut_assert_vexplain(                                                                                                               \
        flut__assert_int_lt((expected), (actual)),                                                                                      \
        "Integer expression `"#actual"` should be lesser than integer expression `" #expected "` (%zu < %zu)", (actual), (expected)     \
    );                                                                                                                                  \
} while (0)

#define flut_assert_int_is_lesser_than_or_equals(expected, actual)                                                                                  \
do {                                                                                                                                                \
    flut_assert_vexplain(                                                                                                                           \
        flut__assert_int_lte((expected), (actual)),                                                                                                 \
        "Integer expression `"#actual"` should be lesser than or equals to integer expression `" #expected "` (%zu <= %zu)", (actual), (expected)   \
    );                                                                                                                                              \
} while (0)

// char
#define flut_assert_char_is_equals(expected, actual)         \
    flut_assert_vexplain(flut__assert_char_equals((expected), (actual)), "Char expression `"#actual"` should be equals to char expression `" #expected "` ('%c' == '%c')", (actual), (expected))
#define flut_assert_char_is_not_equals(expected, actual)     \
    flut_assert_vexplain(flut__assert_char_not_equals((expected), (actual)), "Char expression `"#actual"` should not be equals to char expression `" #expected "` ('%c' != '%c')", (actual), (expected))

// string
#define flut_assert_string_is_equals(expected, actual, shouldFreeActual)                                                                    \
do {                                                                                                                                        \
    char *value = fl_cstring_dup(actual);                                                                                                   \
    flut_assert_vexplain(                                                                                                                   \
        flut__assert_str_equals((expected), (actual), (shouldFreeActual)),                                                                  \
        "String expression `"#actual"` should be equals to string expression `" #expected "` (\"%s\" == \"%s\")", (value), (expected));     \
    fl_cstring_free(value);                                                                                                                 \
} while (0)

#define flut_assert_string_is_equals_n(expected, actual, length, shouldFreeActual)                                                          \
do {                                                                                                                                        \
    char *value = fl_cstring_dup(actual);                                                                                                   \
    flut_assert_vexplain(                                                                                                                   \
        flut__assert_str_equals_n((expected), (actual), (length), (shouldFreeActual)),                                                      \
        "%zu character(s) from string expression `"#actual"` should be equals to string expression `" #expected "` (\"%.*s\" == \"%.*s\")", \
        (length), (length), (value), (length), (expected));                                                                                 \
    fl_cstring_free(value);                                                                                                                 \
} while (0)

#define flut_assert_string_is_not_equals(expected, actual, shouldFreeActual)    \
do {                                                                                                                                        \
    char *value = fl_cstring_dup(actual);                                                                                                   \
    flut_assert_vexplain(                                                                                                                   \
        flut__assert_str_not_equals((expected), (actual), (shouldFreeActual)),                                                              \
        "String expression `"#actual"` should not be equals to string expression `" #expected "` (\"%s\" != \"%s\")", (value), (expected)); \
    fl_cstring_free(value);                                                                                                                 \
} while (0)

#define flut_assert_string_has_length(expected, string, shouldFreeString)                                                                   \
do {                                                                                                                                        \
    char *value = fl_cstring_dup(string);                                                                                                   \
    flut_assert_vexplain(                                                                                                                   \
        flut__assert_str_length((expected), (string), (shouldFreeString)),                                                                  \
        "String expression `"#string"` should be %zu character(s) length. Expression `"#string"` is %zu character(s) length (\"%s\")",      \
        (expected), strlen((value)), (value));                                                                                              \
    fl_cstring_free(value);                                                                                                                 \
} while (0)

// General
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
