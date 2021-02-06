#ifndef FL_TEST_ASSERT_INT_H
#define FL_TEST_ASSERT_INT_H

#include <stddef.h>
#include "result.h"

FlutAssertResult* flut__assert_int_equals(int expected, int actual);
FlutAssertResult* flut__assert_int_not_equals(int expected, int actual);

#endif /* FL_TEST_ASSERT_INT_H */
