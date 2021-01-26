#ifndef FLUT_CONTEXT_H
#define FLUT_CONTEXT_H

#include "assert.h"
#include <fllib/Error.h>
#include <stdbool.h>

typedef struct FlutContext
{
    struct FlContext context;
    bool failed;
} FlutContext;

FlutContext *flut_context_new(void);
void flut_context_free(FlutContext *context);

#endif
