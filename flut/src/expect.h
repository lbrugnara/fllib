#ifndef FLUT_EXPECT_H
#define FLUT_EXPECT_H

#include "context.h"
#include <fllib/Cstring.h>
#include <fllib/Std.h>
#include <stdbool.h>


#define flut_do_assert(result, call, format, ...)                                                                      \
    ((result) = call,                                                                                                  \
     (result)->filename = __FILE__,                                                                                    \
     (result)->funcname = __func__,                                                                                    \
     (result)->line = __LINE__,                                                                                        \
     (result)->assertion = fl_cstring_vdup(format, __VA_ARGS__),                                                       \
     (result))

#define flut_expect(ctx, assertion)                                                                                    \
    do {                                                                                                               \
        FlutAssertResult *result = NULL;                                                                               \
        flut_expect_result(ctx, flut_do_assert(result, assertion, "%s", #assertion));                                  \
    } while (0)

#define flut_expect_vexplain(ctx, assertion, format, ...)                                                              \
    do {                                                                                                               \
        FlutAssertResult *result = NULL;                                                                               \
        flut_expect_result(ctx, flut_do_assert(result, assertion, format, __VA_ARGS__));                               \
    } while (0)

#define flut_expect_explain(ctx, assertion, message) flut_expect_vexplain(ctx, assertion, "%s", message)

/**
 * Notes:
 *  - Deprecated: Adopt flut_describe and use flut_expect_explain instead
 */
bool flut_expect_compat(const char *name, bool condition);

/**
 * Notes:
 *  - Deprecated: Adopt flut_describe and use flut_expect_vexplain instead
 */
bool flut_vexpect_compat(bool conditionResult, const char *format, ...);

bool flut_expect_result(FlutContext *ctx, FlutAssertResult *result);

#endif /* FLUT_EXPECT_H */
