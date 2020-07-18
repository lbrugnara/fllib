#include <stddef.h>
#include <stdbool.h>

#include <fllib/Std.h>
#include <fllib/Mem.h>
#include <fllib/Cstring.h>

#include "result.h"

FlutAssertResult* flut_assert_null(void *obj)
{
    struct FlutAssertResult *result = fl_malloc(sizeof(struct FlutAssertResult));

    result->success = obj == NULL;

    if (!result->success) {
        result->message = fl_cstring_dup("Expecting expression to be NULL, actually not NULL");
    }

    return result;
}

FlutAssertResult* flut_assert_not_null(void *obj)
{
    struct FlutAssertResult *result = fl_malloc(sizeof(struct FlutAssertResult));

    result->success = obj != NULL;

    if (result->success) {
        result->message = fl_cstring_dup("Expecting expression to be not NULL, actually NULL");
    }

    return result;
}
