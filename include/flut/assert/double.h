#ifndef FL_TEST_ASSERT_DOUBLE_H
#define FL_TEST_ASSERT_DOUBLE_H

#include <stddef.h>
#include "result.h"
#include "assertion.h"

#define flut_assert_double_is_equals(expected, actual, epsilon)                         \
do {                                                                                    \
    flut_assertion_explainv(                                                            \
        flut__assert_double_equals((expected), (actual), (epsilon)),                    \
        "Double expression `%s` should be equals to double expression `%s` with an "    \
        "acceptable error of %f (|%f - %f| <= %f)",                                     \
        #actual, #expected, (epsilon), (actual), (expected), (epsilon)                  \
    );                                                                                  \
} while (0)

#define flut_assert_double_is_not_equals(expected, actual, epsilon)                         \
do {                                                                                        \
    flut_assertion_explainv(                                                                \
        flut__assert_double_not_equals((expected), (actual), (epsilon)),                    \
        "Double expression `%s` should not be equals to double expression `%s` with an "    \
        "acceptable error of %f (|%f - %f| > %f)",                                          \
        #actual, #expected, (epsilon), (actual), (expected), (epsilon)                      \
    );                                                                                      \
} while (0)

#define flut_assert_double_is_greater_than(expected, actual, epsilon)                       \
do {                                                                                        \
    flut_assertion_explainv(                                                                \
        flut__assert_double_gt((expected), (actual), (epsilon)),                            \
        "Double expression `%s` should be greater than double expression `%s` with an "     \
        "acceptable error of %f (%f > %f)",                                                 \
        #actual, #expected, (epsilon), (actual), (expected)                                 \
    );                                                                                      \
} while (0)

#define flut_assert_double_is_greater_than_or_equals(expected, actual, epsilon)                         \
do {                                                                                                    \
    flut_assertion_explainv(                                                                            \
        flut__assert_double_gte((expected), (actual), (epsilon)),                                       \
        "Double expression `%s` should be greater than or equals to double expression `%s` with an "    \
        "acceptable error of %f (%f >= %f)",                                                            \
        #actual, #expected, (epsilon), (actual), (expected)                                             \
    );                                                                                                  \
} while (0)

#define flut_assert_double_is_lesser_than(expected, actual, epsilon)                        \
do {                                                                                        \
    flut_assertion_explainv(                                                                \
        flut__assert_double_lt((expected), (actual), (epsilon)),                            \
        "Double expression `%s` should be lesser than double expression `%s` with an "      \
        "acceptable error of %f (%f < %f)",                                                 \
        #actual, #expected, (epsilon), (actual), (expected)                                 \
    );                                                                                      \
} while (0)

#define flut_assert_double_is_lesser_than_or_equals(expected, actual, epsilon)                              \
do {                                                                                                        \
    flut_assertion_explainv(                                                                                \
        flut__assert_double_lte((expected), (actual), (epsilon)),                                           \
        "Double expression `%s` should be lesser than or equals to double expression `%s` with an "         \
        "acceptable error of %f (%f <= %f)",                                                                \
        #actual, #expected, (epsilon), (actual), (expected)                                                 \
    );                                                                                                      \
} while (0)

FlutAssertResult* flut__assert_double_equals(double expected, double actual, double epsilon);
FlutAssertResult* flut__assert_double_not_equals(double expected, double actual, double epsilon);
FlutAssertResult* flut__assert_double_gt(double expected, double actual, double epsilon);
FlutAssertResult* flut__assert_double_gte(double expected, double actual, double epsilon);
FlutAssertResult* flut__assert_double_lt(double expected, double actual, double epsilon);
FlutAssertResult* flut__assert_double_lte(double expected, double actual, double epsilon);

#endif /* FL_TEST_ASSERT_DOUBLE_H */
