#ifndef FLUT_EXPECT_H
#define FLUT_EXPECT_H

#include "context.h"
#include "assert.h"
#include <fllib/Cstring.h>
#include <fllib/Std.h>
#include <stdbool.h>

/**
 * Notes:
 *  - Deprecated: Adopt flut_describe and use flut_assert_explain instead
 */
bool flut_expect_compat(const char *name, bool condition);

/**
 * Notes:
 *  - Deprecated: Adopt flut_describe and use flut_assert_vexplain instead
 */
bool flut_vexpect_compat(bool conditionResult, const char *format, ...);

#endif /* FLUT_EXPECT_H */
