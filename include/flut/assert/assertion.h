#ifndef FLUT_ASSERTION_H
#define FLUT_ASSERTION_H

#include <stddef.h>
#include <stdbool.h>
#include "result.h"
#include "../context.h"

#define flut_do_assertion(assertion_result, call_retval, format, ...)       \
    ((assertion_result) = call_retval,                                      \
     (assertion_result)->filename = __FILE__,                               \
     (assertion_result)->funcname = __func__,                               \
     (assertion_result)->line = __LINE__,                                   \
     (assertion_result)->assertion = fl_cstring_vdup(format, __VA_ARGS__),  \
     (assertion_result))

#define flut_assertion_explainv(assertion, format, ...)                                                                     \
    do {                                                                                                                    \
        FlutAssertResult *result = NULL;                                                                                    \
        flut_assertion_process_result(flut__internal_ctx, flut_do_assertion(result, assertion, format, __VA_ARGS__));       \
    } while (0)

#define flut_assertion_explain(assertion, message) flut_assertion_explainv(assertion, "%s", message)

#define flut_explain(message, assertion)                                                                                \
    assertion;                                                                                                          \
    printf("%s", " |  |  ");                                                                                            \
    printf("      Explanation: %s", message);                                                                           \
    printf("%s", "\n")

bool flut_assertion_process_result(FlutContext *ctx, FlutAssertResult *result);

#endif /* FLUT_ASSERTION_H */
