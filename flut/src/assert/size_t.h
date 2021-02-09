#ifndef FL_TEST_ASSERT_SIZE_T_H
#define FL_TEST_ASSERT_SIZE_T_H

#include <stddef.h>
#include "result.h"
#include "assertion.h"

#define flut_assert_size_t_is_equals(expected, actual)                                          \
do {                                                                                            \
    flut_assertion_explainv(                                                                    \
        flut__assert_size_t_equals((expected), (actual)),                                       \
        "size_t expression `%s` should be equals to size_t expression `%s` (%zu == %zu)",       \
        #actual, #expected, (size_t) (actual), (size_t) (expected)                              \
    );                                                                                          \
} while (0)

#define flut_assert_size_t_is_not_equals(expected, actual)                                      \
do {                                                                                            \
    flut_assertion_explainv(                                                                    \
        flut__assert_size_t_not_equals((expected), (actual)),                                   \
        "size_t expression `%s` should not be equals to size_t expression `%s` (%zu != %zu)",   \
        #actual, #expected, (size_t) (actual), (size_t) (expected)                              \
    );                                                                                          \
} while(0)

#define flut_assert_size_t_is_greater_than(expected, actual)                                    \
do {                                                                                            \
    flut_assertion_explainv(                                                                    \
        flut__assert_size_t_gt((expected), (actual)),                                           \
        "size_t expression `%s` should be greater than size_t expression `%s` (%zu > %zu)",     \
        #actual, #expected, (size_t) (actual), (size_t) (expected)                              \
    );                                                                                          \
} while (0)

#define flut_assert_size_t_is_greater_than_or_equals(expected, actual)                                      \
do {                                                                                                        \
    flut_assertion_explainv(                                                                                \
        flut__assert_size_t_gte((expected), (actual)),                                                      \
        "size_t expression `%s` should be greater than or equals to size_t expression `%s` (%zu >= %zu)",   \
        #actual, #expected, (size_t) (actual), (size_t) (expected)                                          \
    );                                                                                                      \
} while (0)

#define flut_assert_size_t_is_lesser_than(expected, actual)                                 \
do {                                                                                        \
    flut_assertion_explainv(                                                                \
        flut__assert_size_t_lt((expected), (actual)),                                       \
        "size_t expression `%s` should be lesser than size_t expression `%s` (%zu < %zu)",  \
        #actual, #expected, (size_t) (actual), (size_t) (expected)                          \
    );                                                                                      \
} while (0)

#define flut_assert_size_t_is_lesser_than_or_equals(expected, actual)                                       \
do {                                                                                                        \
    flut_assertion_explainv(                                                                                \
        flut__assert_size_t_lte((expected), (actual)),                                                      \
        "size_t expression `%s` should be lesser than or equals to size_t expression `%s` (%zu <= %zu)",    \
        #actual, #expected, (size_t) (actual), (size_t) (expected)                                          \
    );                                                                                                      \
} while (0)

FlutAssertResult* flut__assert_size_t_equals(size_t expected, size_t actual);
FlutAssertResult* flut__assert_size_t_not_equals(size_t expected, size_t actual);
FlutAssertResult* flut__assert_size_t_gt(size_t expected, size_t actual);
FlutAssertResult* flut__assert_size_t_gte(size_t expected, size_t actual);
FlutAssertResult* flut__assert_size_t_lt(size_t expected, size_t actual);
FlutAssertResult* flut__assert_size_t_lte(size_t expected, size_t actual);

#endif /* FL_TEST_ASSERT_SIZE_T_H */
