#ifndef FLUT_CONTEXT_H
#define FLUT_CONTEXT_H

#include <stdbool.h>
#include <fllib/Error.h>
#include "assert.h"

typedef struct FlutContext {
    struct FlContext context;
} FlutContext;

FlutContext* flut_context_new(void);
void flut_context_free(FlutContext *context);

#endif
