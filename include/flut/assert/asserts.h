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
#define flut_passv(message_format, ...)     \
    printf("%s", " |  |- PASS: ");          \
    printf(message_format, __VA_ARGS__);    \
    printf("%s", "\n");

#define flut_pass(message)  flut_passv("%s", message)

// General
#define flut_unexpectedv(error_msg_format, ...)                                                                         \
do {                                                                                                                    \
    struct FlutAssertResult *result = flut_assert_result_new();                                       \
    result->success = false;                                                                                            \
    result->message = fl_cstring_vdup(error_msg_format, __VA_ARGS__);                                                   \
    flut_assertion_process_result(flut__internal_ctx, flut_do_assertion(result, result, "%s", "Unexpected scenario"));  \
} while (0)

#define flut_unexpected(error_msg)  flut_unexpectedv("%s", error_msg)

#endif /* FL_TEST_ASSERTS_H */
