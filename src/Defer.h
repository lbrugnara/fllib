#ifndef FL_DEFER_H
#define FL_DEFER_H

#include "Types.h"

/*
 * Type: struct FlDeferCallObj
 * 
 * ===== C =====
 *  struct FlDeferCallObj {
 *      struct FlDeferCallObj *prev;
 *      void *object;
 *      void (*call)(void*);
 *  };
 *
 */
struct FlDeferCallObj {
    struct FlDeferCallObj *prev;
    void *object;
    void (*call)(void*);
};

struct FlDeferScope {
    struct FlDeferCallObj *call_chain;
    void *tmpptr;
};

static inline void fl_defer_scope_close(struct FlDeferScope *scope)
{
    if (!scope->call_chain)
        return;

    struct FlDeferCallObj *deferred_call = scope->call_chain;
    while (deferred_call)
    {
        deferred_call->call(deferred_call->object);
        deferred_call = deferred_call->prev;
    }
}

#define defer_return do { fl_defer_scope_close(&scope); return; } while (0)
#define defer_return_value(expr) do { fl_defer_scope_close(&scope); return expr; } while (0)

#define defer_scope \
    for (struct FlDeferScope scope = { .call_chain = NULL }; scope.call_chain == NULL; fl_defer_scope_close(&scope))

#define defer_call(func, obj) \
    scope.call_chain = &(struct FlDeferCallObj) {   \
        .object = obj,  \
        .call = (void(*)(void*))func,   \
        .prev = scope.call_chain    \
    }


#endif /*FL_DEFER_H */
