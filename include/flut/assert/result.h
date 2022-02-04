#ifndef FLUT_ASSERT_RESULT_H
#define FLUT_ASSERT_RESULT_H

#include <stddef.h>
#include <stdbool.h>
#include <fllib/Cstring.h>

typedef struct FlutAssertResult {
    const char *filename;
    const char *funcname;
    unsigned long long line;
    const char *message;
    const char *assertion;
    bool success;
} FlutAssertResult;

#define flut_set_assert_resultv(result_ptr, assert_result, assert_message_fmt, ...) \
    (result_ptr)->success = assert_result; (result_ptr)->message = fl_cstring_vdup(assert_message_fmt, __VA_ARGS__)

#define flut_set_assert_result(result_ptr, assert_result, assert_message) \
    flut_set_assert_resultv(result_ptr, assert_result, "%s", assert_message)

FlutAssertResult* flut_assert_result_new(void);
void flut_assert_result_free(FlutAssertResult *result);

#endif /* FLUT_ASSERT_RESULT_H */
