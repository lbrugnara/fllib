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

    
    #ifdef _WIN32
        ctx->os = FLUT_OS_WIN;
    #elif defined(__linux__)
        ctx->os = FLUT_OS_LINUX;
    #else
        ctx->os = FLUT_OS_UNK;
    #endif

    return ctx;
}

void flut_context_free(FlutContext *context)
{
    fl_free(context);
}
