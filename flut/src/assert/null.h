#ifndef FL_TEST_ASSERT_NULL_H
#define FL_TEST_ASSERT_NULL_H

#include "result.h"
#include "assertion.h"

#define flut_assert_is_null(obj)                \
do {                                            \
    flut_assertion_explainv(                    \
        flut__assert_null((obj)),               \
        "Expression `%s` should be null",       \
        #obj                                    \
    );                                          \
} while (0)

#define flut_assert_is_not_null(obj)            \
do {                                            \
    flut_assertion_explainv(                    \
        flut__assert_not_null((obj)),           \
        "Expression `%s` should not be null",   \
        #obj                                    \
    );                                          \
} while (0)

FlutAssertResult* flut__assert_null(void *obj);
FlutAssertResult* flut__assert_not_null(void *obj);

#endif /* FL_TEST_ASSERT_NULL_H */
