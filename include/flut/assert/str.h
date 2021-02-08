#ifndef FL_TEST_ASSERT_STR_H
#define FL_TEST_ASSERT_STR_H

#include <stdbool.h>
#include "result.h"
#include "assertion.h"

#define flut_assert_string_is_not_null(string, shouldFreeString)    \
do {                                                                \
    char *value = (char*) string;                                   \
    flut_assertion_vexplain(                                           \
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
    flut_assertion_vexplain(                                                                       \
        flut__assert_str_equals((expected), (value), (shouldFreeActual)),                       \
        "String expression `%s` should be equals to string expression `%s` (\"%s\" == \"%s\")", \
        #actual, #expected, (copy), (expected));                                                \
    fl_cstring_free(copy);                                                                      \
} while (0)

#define flut_assert_string_is_equals_n(expected, actual, length, shouldFreeActual)                                          \
do {                                                                                                                        \
    char *value = (char*) actual;                                                                                           \
    char *copy = fl_cstring_dup(value);                                                                                     \
    flut_assertion_vexplain(                                                                                                   \
        flut__assert_str_equals_n((expected), (value), (length), (shouldFreeActual)),                                       \
        "%zu character(s) from string expression `%s` should be equals to string expression `%s` (\"%.*s\" == \"%.*s\")",   \
        (length), #actual, #expected, (length), (copy), (length), (expected));                                              \
    fl_cstring_free(copy);                                                                                                  \
} while (0)

#define flut_assert_string_is_not_equals(expected, actual, shouldFreeActual)                        \
do {                                                                                                \
    char *value = (char*) actual;                                                                   \
    char *copy = fl_cstring_dup(value);                                                             \
    flut_assertion_vexplain(                                                                           \
        flut__assert_str_not_equals((expected), (value), (shouldFreeActual)),                       \
        "String expression `%s` should not be equals to string expression `%s` (\"%s\" != \"%s\")", \
        #actual, #expected, (copy), (expected));                                                    \
    fl_cstring_free(copy);                                                                          \
} while (0)

#define flut_assert_string_has_length(expected, string, shouldFreeString)                                                   \
do {                                                                                                                        \
    char *value = (char*) string;                                                                                           \
    char *copy = fl_cstring_dup(value);                                                                                     \
    flut_assertion_vexplain(                                                                                                   \
        flut__assert_str_length((expected), (value), (shouldFreeString)),                                                   \
        "String expression `%s` should be %zu character(s) length. Expression `%s` is %zu character(s) length (\"%s\")",    \
        #string, (expected), #string, strlen((copy)), (copy));                                                              \
    fl_cstring_free(copy);                                                                                                  \
} while (0)

FlutAssertResult* flut__assert_str_equals(const char *expected, const char *actual, bool free_mem);
FlutAssertResult* flut__assert_str_equals_n(const char *expected, const char *actual, size_t n, bool free_mem);
FlutAssertResult* flut__assert_str_not_equals(const char *expected, const char *actual, bool free_mem);
FlutAssertResult* flut__assert_str_length(size_t expected_length, const char *str, bool free_mem);

#endif /* FL_TEST_ASSERT_STR_H */
