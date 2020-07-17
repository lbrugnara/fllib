#ifndef FL_TEST_ASSERTS_H
#define FL_TEST_ASSERTS_H

#include <stddef.h>
#include <stdbool.h>
#include "result.h"

FlutAssertResult* flut_assert_null(void *obj);
FlutAssertResult* flut_assert_not_null(void *obj);
FlutAssertResult* flut_assert_bool_is_true(bool condition);
FlutAssertResult* flut_assert_bool_is_false(bool condition);
FlutAssertResult* flut_assert_size_t_equals(size_t expected, size_t actual);

#endif /* FL_TEST_ASSERTS_H */
