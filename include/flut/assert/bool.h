#ifndef FL_TEST_ASSERT_BOOL_H
#define FL_TEST_ASSERT_BOOL_H

#include <stdbool.h>
#include "result.h"

FlutAssertResult* flut_assert_bool_is_true(bool condition);
FlutAssertResult* flut_assert_bool_is_false(bool condition);

#endif /* FL_TEST_ASSERT_BOOL_H */
