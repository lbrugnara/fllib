#ifndef FL_TEST_ASSERT_CHAR_H
#define FL_TEST_ASSERT_CHAR_H

#include <stdbool.h>
#include "result.h"

FlutAssertResult* flut__assert_char_equals(char expected, char actual);
FlutAssertResult* flut__assert_char_not_equals(char expected, char actual);

#endif /* FL_TEST_ASSERT_CHAR_H */
