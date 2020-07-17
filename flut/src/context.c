#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <fllib/Mem.h>
#include "context.h"
#include "test.h"

static FlutContext base_context = { 
    .context = FL_CTX_STATIC_INIT,
};

FlutContext* flut_context_new(void)
{
    FlutContext *ctx = fl_malloc(sizeof(FlutContext));
    memcpy(ctx, &base_context, sizeof(FlutContext));

    return ctx;
}

void flut_context_free(FlutContext *context)
{
    fl_free(context);
}
