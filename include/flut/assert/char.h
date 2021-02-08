#ifndef FL_TEST_ASSERT_CHAR_H
#define FL_TEST_ASSERT_CHAR_H

#include <stdbool.h>
#include "result.h"
#include "assertion.h"

#define flut_assert_char_is_equals(expected, actual)                                    \
do {                                                                                    \
    flut_assertion_vexplain(                                                               \
        flut__assert_char_equals((expected), (actual)),                                 \
        "Char expression `%s` should be equals to char expression `%s` ('%c' == '%c')", \
        #actual, #expected, (actual), (expected)                                        \
    );                                                                                  \
} while (0)
#define flut_assert_char_is_not_equals(expected, actual)                                    \
do {                                                                                        \
    flut_assertion_vexplain(                                                                   \
        flut__assert_char_not_equals((expected), (actual)),                                 \
        "Char expression `%s` should not be equals to char expression `%s` ('%c' != '%c')", \
        #actual, #expected, (actual), (expected)                                            \
    );                                                                                      \
} while (0)

FlutAssertResult* flut__assert_char_equals(char expected, char actual);
FlutAssertResult* flut__assert_char_not_equals(char expected, char actual);

#endif /* FL_TEST_ASSERT_CHAR_H */
