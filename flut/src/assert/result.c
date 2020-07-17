#include <stddef.h>
#include <stdbool.h>

#include <fllib/Std.h>
#include <fllib/Mem.h>
#include <fllib/Cstring.h>

#include "result.h"

void flut_assert_result_free(FlutAssertResult *result)
{
    if (result->message)
        fl_cstring_free(result->message);

    if (result->assertion)
        fl_cstring_free(result->assertion);

    fl_free(result);
}
