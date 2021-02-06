#ifndef FL_TEST_ASSERT_BOOL_H
#define FL_TEST_ASSERT_BOOL_H

#include <stdbool.h>
#include "result.h"

FlutAssertResult* flut__assert_bool_true(bool condition);
FlutAssertResult* flut__assert_bool_false(bool condition);

#endif /* FL_TEST_ASSERT_BOOL_H */
