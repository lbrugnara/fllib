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

#define flut_assert_vexplain(assertion, format, ...)                                                                    \
    do {                                                                                                                \
        FlutAssertResult *result = NULL;                                                                                \
        flut_assert_result(flut__internal_ctx, flut_do_assert(result, assertion, format, __VA_ARGS__));                 \
    } while (0)

#define flut_assert_explain(assertion, message) flut_assert_vexplain(assertion, "%s", message)

// Null
#define flut_assert_is_null(obj)                \
do {                                            \
    flut_assert_vexplain(                       \
        flut__assert_null((obj)),               \
        "Expression `%s` should be null",       \
        #obj                                    \
    );                                          \
} while (0)

#define flut_assert_is_not_null(obj)            \
do {                                            \
    flut_assert_vexplain(                       \
        flut__assert_not_null((obj)),           \
        "Expression `%s` should not be null",   \
        #obj                                    \
    );                                          \
} while (0)

// Boolean
#define flut_assert_is_true(expression)         \
do {                                            \
    flut_assert_vexplain(                       \
        flut__assert_bool_true((expression)),   \
        "Expression `%s` should be true",       \
        #expression                             \
    );                                          \
} while (0)

#define flut_assert_is_false(expression)        \
do {                                            \
    flut_assert_vexplain(                       \
        flut__assert_bool_false((expression)),  \
        "Expression `%s` should be false",      \
        #expression                             \
    );                                          \
} while (0)

// Pointers
#define flut_assert_pointer_is_equals(expected, actual)         \
do {                                                            \
    flut_assert_vexplain(                                       \
        flut__assert_ptr_equals((expected), (actual)),          \
        "Pointer `%s` should be equals to pointer `%s`",        \
        #actual, #expected                                      \
    );                                                          \
} while (0)

#define flut_assert_pointer_is_not_equals(expected, actual)     \
do {                                                            \
    flut_assert_vexplain(                                       \
        flut__assert_ptr_not_equals((expected), (actual)),      \
        "Pointer `%s` should not be equals to pointer `%s`",    \
        #actual, #expected                                      \
    );                                                          \
} while (0)

// size_t
#define flut_assert_size_t_is_equals(expected, actual)                                          \
do {                                                                                            \
    flut_assert_vexplain(                                                                       \
        flut__assert_size_t_equals((expected), (actual)),                                       \
        "size_t expression `%s` should be equals to size_t expression `%s` (%zu == %zu)",       \
        #actual, #expected, (actual), (expected)                                                \
    );                                                                                          \
} while (0)

#define flut_assert_size_t_is_not_equals(expected, actual)                                      \
do {                                                                                            \
    flut_assert_vexplain(                                                                       \
        flut__assert_size_t_not_equals((expected), (actual)),                                   \
        "size_t expression `%s` should not be equals to size_t expression `%s` (%zu != %zu)",   \
        #actual, #expected, (actual), (expected)                                                \
    );                                                                                          \
} while(0)

#define flut_assert_size_t_is_greater_than(expected, actual)                                    \
do {                                                                                            \
    flut_assert_vexplain(                                                                       \
        flut__assert_size_t_gt((expected), (actual)),                                           \
        "size_t expression `%s` should be greater than size_t expression `%s` (%zu > %zu)",     \
        #actual, #expected, (actual), (expected)                                                \
    );                                                                                          \
} while (0)

#define flut_assert_size_t_is_greater_than_or_equals(expected, actual)                                      \
do {                                                                                                        \
    flut_assert_vexplain(                                                                                   \
        flut__assert_size_t_gte((expected), (actual)),                                                      \
        "size_t expression `%s` should be greater than or equals to size_t expression `%s` (%zu >= %zu)",   \
        #actual, #expected, (actual), (expected)                                                            \
    );                                                                                                      \
} while (0)

#define flut_assert_size_t_is_lesser_than(expected, actual)                                 \
do {                                                                                        \
    flut_assert_vexplain(                                                                   \
        flut__assert_size_t_lt((expected), (actual)),                                       \
        "size_t expression `%s` should be lesser than size_t expression `%s` (%zu < %zu)",  \
        #actual, #expected, (actual), (expected)                                            \
    );                                                                                      \
} while (0)

#define flut_assert_size_t_is_lesser_than_or_equals(expected, actual)                                       \
do {                                                                                                        \
    flut_assert_vexplain(                                                                                   \
        flut__assert_size_t_lte((expected), (actual)),                                                      \
        "size_t expression `%s` should be lesser than or equals to size_t expression `%s` (%zu <= %zu)",    \
        #actual, #expected, (actual), (expected)                                                            \
    );                                                                                                      \
} while (0)

// int
#define flut_assert_int_is_equals(expected, actual)                                         \
do {                                                                                        \
    flut_assert_vexplain(                                                                   \
        flut__assert_int_equals((expected), (actual)),                                      \
        "Integer expression `%s` should be equals to integer expression `%s` (%zu == %zu)", \
        #actual, #expected, (actual), (expected)                                            \
    );                                                                                      \
} while (0)

#define flut_assert_int_is_not_equals(expected, actual)                                         \
do {                                                                                            \
    flut_assert_vexplain(                                                                       \
        flut__assert_int_not_equals((expected), (actual)),                                      \
        "Integer expression `%s` should not be equals to integer expression `%s` (%zu != %zu)", \
        #actual, #expected, (actual), (expected)                                                \
    );                                                                                          \
} while (0)

#define flut_assert_int_is_greater_than(expected, actual)                                       \
do {                                                                                            \
    flut_assert_vexplain(                                                                       \
        flut__assert_int_gt((expected), (actual)),                                              \
        "Integer expression `%s` should be greater than integer expression `%s` (%zu > %zu)",   \
        #actual, #expected, (actual), (expected)                                                \
    );                                                                                          \
} while (0)

#define flut_assert_int_is_greater_than_or_equals(expected, actual)                                         \
do {                                                                                                        \
    flut_assert_vexplain(                                                                                   \
        flut__assert_int_gte((expected), (actual)),                                                         \
        "Integer expression `%s` should be greater than or equals to integer expression `%s` (%zu >= %zu)", \
        #actual, #expected, (actual), (expected)                                                            \
    );                                                                                                      \
} while (0)

#define flut_assert_int_is_lesser_than(expected, actual)                                        \
do {                                                                                            \
    flut_assert_vexplain(                                                                       \
        flut__assert_int_lt((expected), (actual)),                                              \
        "Integer expression `%s` should be lesser than integer expression `%s` (%zu < %zu)",    \
        #actual, #expected, (actual), (expected)                                                \
    );                                                                                          \
} while (0)

#define flut_assert_int_is_lesser_than_or_equals(expected, actual)                                          \
do {                                                                                                        \
    flut_assert_vexplain(                                                                                   \
        flut__assert_int_lte((expected), (actual)),                                                         \
        "Integer expression `%s` should be lesser than or equals to integer expression `%s` (%zu <= %zu)",  \
        #actual, #expected, (actual), (expected)                                                            \
    );                                                                                                      \
} while (0)

// char
#define flut_assert_char_is_equals(expected, actual)                                    \
do {                                                                                    \
    flut_assert_vexplain(                                                               \
        flut__assert_char_equals((expected), (actual)),                                 \
        "Char expression `%s` should be equals to char expression `%s` ('%c' == '%c')", \
        #actual, #expected, (actual), (expected)                                        \
    );                                                                                  \
} while (0)
#define flut_assert_char_is_not_equals(expected, actual)                                    \
do {                                                                                        \
    flut_assert_vexplain(                                                                   \
        flut__assert_char_not_equals((expected), (actual)),                                 \
        "Char expression `%s` should not be equals to char expression `%s` ('%c' != '%c')", \
        #actual, #expected, (actual), (expected)                                            \
    );                                                                                      \
} while (0)

// string
#define flut_assert_string_is_not_null(string, shouldFreeString)    \
do {                                                                \
    char *value = (char*) string;                                   \
    flut_assert_vexplain(                                           \
        flut__assert_not_null((void*) (value)),                     \
        "String expression `%s` should not be null (\"%s\")",       \
        #string, (value)                                            \
    );                                                              \
    if (shouldFreeString) fl_cstring_free(value);                   \
} while (0)

#define flut_assert_string_is_equals(expected, actual, shouldFreeActual)                        \
do {                                                                                            \
    char *value = (char*) actual;                                                               \
    char *copy = fl_cstring_dup(value);                                                         \
    flut_assert_vexplain(                                                                       \
        flut__assert_str_equals((expected), (value), (shouldFreeActual)),                       \
        "String expression `%s` should be equals to string expression `%s` (\"%s\" == \"%s\")", \
        #actual, #expected, (copy), (expected));                                                \
    fl_cstring_free(copy);                                                                      \
} while (0)

#define flut_assert_string_is_equals_n(expected, actual, length, shouldFreeActual)                                          \
do {                                                                                                                        \
    char *value = (char*) actual;                                                                                           \
    char *copy = fl_cstring_dup(value);                                                                                     \
    flut_assert_vexplain(                                                                                                   \
        flut__assert_str_equals_n((expected), (value), (length), (shouldFreeActual)),                                       \
        "%zu character(s) from string expression `%s` should be equals to string expression `%s` (\"%.*s\" == \"%.*s\")",   \
        (length), #actual, #expected, (length), (copy), (length), (expected));                                              \
    fl_cstring_free(copy);                                                                                                  \
} while (0)

#define flut_assert_string_is_not_equals(expected, actual, shouldFreeActual)                        \
do {                                                                                                \
    char *value = (char*) actual;                                                                   \
    char *copy = fl_cstring_dup(value);                                                             \
    flut_assert_vexplain(                                                                           \
        flut__assert_str_not_equals((expected), (value), (shouldFreeActual)),                       \
        "String expression `%s` should not be equals to string expression `%s` (\"%s\" != \"%s\")", \
        #actual, #expected, (copy), (expected));                                                    \
    fl_cstring_free(copy);                                                                          \
} while (0)

#define flut_assert_string_has_length(expected, string, shouldFreeString)                                                   \
do {                                                                                                                        \
    char *value = (char*) string;                                                                                           \
    char *copy = fl_cstring_dup(value);                                                                                     \
    flut_assert_vexplain(                                                                                                   \
        flut__assert_str_length((expected), (value), (shouldFreeString)),                                                   \
        "String expression `%s` should be %zu character(s) length. Expression `%s` is %zu character(s) length (\"%s\")",    \
        #string, (expected), #string, strlen((copy)), (copy));                                                              \
    fl_cstring_free(copy);                                                                                                  \
} while (0)

// General
#define flut_unexpected(error_msg)                                                                          \
do {                                                                                                        \
    struct FlutAssertResult *result = fl_malloc(sizeof(struct FlutAssertResult));                           \
    result->success = false;                                                                                \
    result->message = fl_cstring_dup(error_msg);                                                            \
    flut_assert_result(flut__internal_ctx, flut_do_assert(result, result, "%s", "Unexpected scenario"));    \
} while (0)

bool flut_assert_result(FlutContext *ctx, FlutAssertResult *result);

#endif /* FLUT_ASSERT_UTILS_H */
