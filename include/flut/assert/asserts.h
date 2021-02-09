#ifndef FL_TEST_ASSERTS_H
#define FL_TEST_ASSERTS_H

#include "bool.h"
#include "char.h"
#include "double.h"
#include "int.h"
#include "null.h"
#include "ptr.h"
#include "size_t.h"
#include "str.h"

// General
#define flut_unexpected(error_msg)                                                                          \
do {                                                                                                        \
    struct FlutAssertResult *result = fl_malloc(sizeof(struct FlutAssertResult));                           \
    result->success = false;                                                                                \
    result->message = fl_cstring_dup(error_msg);                                                            \
    flut_assertion_process_result(flut__internal_ctx, flut_do_assertion(result, result, "%s", "Unexpected scenario"));    \
} while (0)

#endif /* FL_TEST_ASSERTS_H */
