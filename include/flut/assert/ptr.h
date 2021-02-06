#ifndef FL_TEST_ASSERT_PTR_H
#define FL_TEST_ASSERT_PTR_H

#include <stddef.h>
#include "result.h"

FlutAssertResult* flut__assert_ptr_equals(void *expected, void *actual);
FlutAssertResult* flut__assert_ptr_not_equals(void *expected, void *actual);

#endif /* FL_TEST_ASSERT_PTR_H */
