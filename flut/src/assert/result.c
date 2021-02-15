#include <stddef.h>
#include <stdbool.h>

#include <fllib/Std.h>
#include <fllib/Mem.h>
#include <fllib/Cstring.h>

#include "result.h"

FlutAssertResult* flut_assert_result_new(void) {
    return fl_malloc(sizeof(struct FlutAssertResult));
}

void flut_assert_result_free(FlutAssertResult *result)
{
    if (result->message)
        fl_cstring_free(result->message);

    if (result->assertion)
        fl_cstring_free(result->assertion);

    fl_free(result);
}
