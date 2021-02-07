#ifndef FL_TEST_ASSERT_SIZE_T_H
#define FL_TEST_ASSERT_SIZE_T_H

#include <stddef.h>
#include "result.h"

FlutAssertResult* flut__assert_size_t_equals(size_t expected, size_t actual);
FlutAssertResult* flut__assert_size_t_not_equals(size_t expected, size_t actual);
FlutAssertResult* flut__assert_size_t_gt(size_t expected, size_t actual);
FlutAssertResult* flut__assert_size_t_gte(size_t expected, size_t actual);
FlutAssertResult* flut__assert_size_t_lt(size_t expected, size_t actual);
FlutAssertResult* flut__assert_size_t_lte(size_t expected, size_t actual);

#endif /* FL_TEST_ASSERT_SIZE_T_H */
