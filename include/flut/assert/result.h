#ifndef FLUT_ASSERT_RESULT_H
#define FLUT_ASSERT_RESULT_H

#include <stddef.h>
#include <stdbool.h>

typedef struct FlutAssertResult {
    const char *filename;
    const char *funcname;
    unsigned long long line;
    const char *message;
    const char *assertion;
    bool success;
} FlutAssertResult;

void flut_assert_result_free(FlutAssertResult *result);

#endif /* FLUT_ASSERT_RESULT_H */
