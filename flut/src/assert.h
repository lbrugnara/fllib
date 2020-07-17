#ifndef FLUT_ASSERT_UTILS_H
#define FLUT_ASSERT_UTILS_H

#include <stddef.h>
#include <stdbool.h>
#include "assert/result.h"

typedef struct FlutAssertUtils {
    FlutAssertResult* (*null)(void *obj);
    FlutAssertResult* (*not_null)(void *obj);
    FlutAssertResult* (*is_true)(bool condition);
    FlutAssertResult* (*is_false)(bool condition);
    struct {
        FlutAssertResult* (*equals)(size_t expected, size_t actual);
    } size_t;
} FlutAssertUtils;

FlutAssertUtils* flut_assert_utils_new(void);
void flut_assert_utils_free(FlutAssertUtils *asserts);

#endif /* FLUT_ASSERT_UTILS_H */
