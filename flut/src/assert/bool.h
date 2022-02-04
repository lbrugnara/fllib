#ifndef FL_TEST_ASSERT_BOOL_H
#define FL_TEST_ASSERT_BOOL_H

#include <stdbool.h>
#include "result.h"
#include "assertion.h"

#define flut_assert_is_true(expression)         \
do {                                            \
    flut_assertion_explainv(                       \
        flut__assert_bool_true((expression)),   \
        "Expression `%s` should be true",       \
        #expression                             \
    );                                          \
} while (0)

#define flut_assert_is_false(expression)        \
do {                                            \
    flut_assertion_explainv(                       \
        flut__assert_bool_false((expression)),  \
        "Expression `%s` should be false",      \
        #expression                             \
    );                                          \
} while (0)

FlutAssertResult* flut__assert_bool_true(bool condition);
FlutAssertResult* flut__assert_bool_false(bool condition);

#endif /* FL_TEST_ASSERT_BOOL_H */
