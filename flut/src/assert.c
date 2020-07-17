#include <stddef.h>
#include <stdbool.h>

#include <fllib/Std.h>
#include <fllib/Mem.h>
#include <fllib/Cstring.h>

#include "assert.h"
#include "assert/asserts.h"

FlutAssertUtils* flut_assert_utils_new(void)
{
    FlutAssertUtils *asserts = fl_malloc(sizeof(FlutAssertUtils));

    // NULL
    asserts->null = flut_assert_null;
    asserts->not_null = flut_assert_not_null;

    // bool
    asserts->is_true = flut_assert_bool_is_true;
    asserts->is_false = flut_assert_bool_is_false;

    // size_t
    asserts->size_t.equals = flut_assert_size_t_equals;

    return asserts;
}

void flut_assert_utils_free(FlutAssertUtils *asserts)
{
    fl_free(asserts);
}
