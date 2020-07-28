#ifndef FL_TEST_ASSERT_STR_H
#define FL_TEST_ASSERT_STR_H

#include <stdbool.h>
#include "result.h"

FlutAssertResult* flut_assert_str_equals(const char *expected, const char *actual, bool free_mem);
FlutAssertResult* flut_assert_str_equals_n(const char *expected, const char *actual, size_t n, bool free_mem);
FlutAssertResult* flut_assert_str_not_equals(const char *expected, const char *actual, bool free_mem);

#endif /* FL_TEST_ASSERT_STR_H */
