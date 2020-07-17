#ifndef FLUT_EXPECT_H
#define FLUT_EXPECT_H

#include <stdbool.h>
#include <fllib/Std.h>
#include "context.h"

#define flut_do_assert(result, call, format, ...) (                 \
    (result) = call,                                                \
    (result)->filename = __FILE__,                                  \
    (result)->funcname = __func__,                                  \
    (result)->line = __LINE__,                                      \
    (result)->assertion = fl_cstring_vdup(format, __VA_ARGS__),     \
    (result)                                                        \
)

#define flut_expect(ctx, assertion) do {                                                \
    FlutAssertResult *result = NULL;                                                    \
    flut_expect_result(ctx, flut_do_assert(result, assertion, "%s", #assertion));      \
} while (0)

#define flut_expect_vexplain(ctx, assertion, format, ...) do {                          \
    FlutAssertResult *result = NULL;                                                    \
    flut_expect_result(ctx, flut_do_assert(result, assertion, format, __VA_ARGS__));   \
} while (0)

#define flut_expect_explain(ctx, assertion, message)    \
    fl_test_vaexpect(ctx, assertion, "%s", message)

bool flut_expect_compat(const char* name, bool condition);

bool flut_vexpect_compat(bool conditionResult, const char* format, ...);

bool flut_expect_result(FlutContext *ctx, FlutAssertResult *result);

#endif /* FLUT_EXPECT_H */
