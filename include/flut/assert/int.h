#ifndef FL_TEST_ASSERT_INT_H
#define FL_TEST_ASSERT_INT_H

#include <stddef.h>
#include "result.h"
#include "assertion.h"

#define flut_assert_int_is_equals(expected, actual)                                         \
do {                                                                                        \
    flut_assertion_explainv(                                                                \
        flut__assert_int_equals((expected), (actual)),                                      \
        "Integer expression `%s` should be equals to integer expression `%s` (%d == %d)",   \
        #actual, #expected, (int) (actual), (int) (expected)                                \
    );                                                                                      \
} while (0)

#define flut_assert_int_is_not_equals(expected, actual)                                         \
do {                                                                                            \
    flut_assertion_explainv(                                                                    \
        flut__assert_int_not_equals((expected), (actual)),                                      \
        "Integer expression `%s` should not be equals to integer expression `%s` (%d != %d)",   \
        #actual, #expected, (int) (actual), (int) (expected)                                    \
    );                                                                                          \
} while (0)

#define flut_assert_int_is_greater_than(expected, actual)                                       \
do {                                                                                            \
    flut_assertion_explainv(                                                                    \
        flut__assert_int_gt((expected), (actual)),                                              \
        "Integer expression `%s` should be greater than integer expression `%s` (%d > %d)",     \
        #actual, #expected, (int) (actual), (int) (expected)                                    \
    );                                                                                          \
} while (0)

#define flut_assert_int_is_greater_than_or_equals(expected, actual)                                         \
do {                                                                                                        \
    flut_assertion_explainv(                                                                                \
        flut__assert_int_gte((expected), (actual)),                                                         \
        "Integer expression `%s` should be greater than or equals to integer expression `%s` (%d >= %d)",   \
        #actual, #expected, (int) (actual), (int) (expected)                                                \
    );                                                                                                      \
} while (0)

#define flut_assert_int_is_lesser_than(expected, actual)                                        \
do {                                                                                            \
    flut_assertion_explainv(                                                                    \
        flut__assert_int_lt((expected), (actual)),                                              \
        "Integer expression `%s` should be lesser than integer expression `%s` (%d < %d)",      \
        #actual, #expected, (int) (actual), (int) (expected)                                    \
    );                                                                                          \
} while (0)

#define flut_assert_int_is_lesser_than_or_equals(expected, actual)                                          \
do {                                                                                                        \
    flut_assertion_explainv(                                                                                \
        flut__assert_int_lte((expected), (actual)),                                                         \
        "Integer expression `%s` should be lesser than or equals to integer expression `%s` (%d <= %d)",    \
        #actual, #expected, (int) (actual), (int) (expected)                                                \
    );                                                                                                      \
} while (0)

FlutAssertResult* flut__assert_int_equals(int expected, int actual);
FlutAssertResult* flut__assert_int_not_equals(int expected, int actual);
FlutAssertResult* flut__assert_int_gt(int expected, int actual);
FlutAssertResult* flut__assert_int_gte(int expected, int actual);
FlutAssertResult* flut__assert_int_lt(int expected, int actual);
FlutAssertResult* flut__assert_int_lte(int expected, int actual);

#endif /* FL_TEST_ASSERT_INT_H */
