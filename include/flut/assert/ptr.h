#ifndef FL_TEST_ASSERT_PTR_H
#define FL_TEST_ASSERT_PTR_H

#include <stddef.h>
#include "result.h"
#include "assertion.h"

#define flut_assert_pointer_is_equals(expected, actual)         \
do {                                                            \
    flut_assertion_vexplain(                                       \
        flut__assert_ptr_equals((expected), (actual)),          \
        "Pointer `%s` should be equals to pointer `%s`",        \
        #actual, #expected                                      \
    );                                                          \
} while (0)

#define flut_assert_pointer_is_not_equals(expected, actual)     \
do {                                                            \
    flut_assertion_vexplain(                                       \
        flut__assert_ptr_not_equals((expected), (actual)),      \
        "Pointer `%s` should not be equals to pointer `%s`",    \
        #actual, #expected                                      \
    );                                                          \
} while (0)

FlutAssertResult* flut__assert_ptr_equals(void *expected, void *actual);
FlutAssertResult* flut__assert_ptr_not_equals(void *expected, void *actual);

#endif /* FL_TEST_ASSERT_PTR_H */
