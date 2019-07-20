#ifndef FL_DEFER_H
#define FL_DEFER_H

#include "Types.h"

struct FlDeferStmt {
    struct FlDeferStmt *prev;
    jmp_buf entry_point;
};

struct FlDeferScope {
    struct FlDeferStmt *call_chain;
    jmp_buf exit_point;
    enum { 
        FL_DEFER_STARTED = 0,
        FL_DEFER_LEAVING,
        FL_DEFER_ENDED
    } state;
};

#define defer_scope                                                                                         \
    for (                                                                                                   \
        /* initialization */                                                                                \
        struct FlDeferScope _fl_defer_scope_ = { .state = FL_DEFER_STARTED, .call_chain = NULL };           \
        /* condition: get in only if the state is "started (0)" */                                          \
        _fl_defer_scope_.state == FL_DEFER_STARTED;                                                         \
        /* increment (use comma operator) */                                                                \
        (                                                                                                   \
            /* Flag scope as "leaving (1)" only if it is "started (0)" */                                   \
            _fl_defer_scope_.state = FL_DEFER_LEAVING,                                                      \
            /* Jump to the next stacked statement, or to the scope's exit */                                \
            longjmp(_fl_defer_scope_.call_chain                                                             \
                ? _fl_defer_scope_.call_chain->entry_point                                                  \
                : _fl_defer_scope_.exit_point, 1)                                                           \
        )                                                                                                   \
    )                                                                                                       \
        /* Save the exit point */                                                                           \
        if (setjmp(_fl_defer_scope_.exit_point) != 0) {                                                     \
            /* longjmp will jump here after traversing the deferred statement's */                          \
            /* list (if any). We break the loop, for that we flag the scope as "exited (2)" */              \
            _fl_defer_scope_.state = FL_DEFER_ENDED;                                                        \
            break;                                                                                          \
        }                                                                                                   \
        else


#define defer_exit()                                                                                        \
do {                                                                                                        \
    /* Check current state to make sure it is valid */                                                      \
    switch (_fl_defer_scope_.state)                                                                         \
    {                                                                                                       \
        case FL_DEFER_LEAVING:                                                                              \
            flm_assert(false, "defer_exit cannot be called twice nor be called within a defer statement");  \
            break;                                                                                          \
        case FL_DEFER_ENDED:                                                                                \
            flm_assert(false, "Scope has ended, defer_exit cannot be used"); break;                         \
        default: break;                                                                                     \
    }                                                                                                       \
    /* Flag scope as "leaving (1)" only if it is "started (0)" */                                           \
    _fl_defer_scope_.state = FL_DEFER_LEAVING;                                                              \
    longjmp(_fl_defer_scope_.call_chain                                                                     \
        ? _fl_defer_scope_.call_chain->entry_point                                                          \
        : _fl_defer_scope_.exit_point, 1);                                                                  \
} while (0)

#define defer_return                                                                                        \
    /* The outer for calls setjmp to change the scope's exit point (to our return statement). */            \
    /* If the inner for's longjmp call succeed, we will jump back here, the fldefjmp will be */             \
    /* updated to the longjmp's value. */                                                                   \
    for (int fldefjmp = 0; (fldefjmp = setjmp(_fl_defer_scope_.exit_point), 1) ;)                           \
        /* The inner for will check two conditions to know if it needs to avoid longjmp: */                 \
        /*  1- fldefjmp different to 0: We already executed longjmp, we called the deferred statements */   \
        /*     (if any) and we need to leave */                                                             \
        /*  2- If the scope's state is different to STARTED it means we are in a not valid state */         \
        /*      so we need to keep that into account */                                                     \
        /* If the longjmp is avoided, we simply continue to the inner-most for. In case we */               \
        /* need to take the longjmp, we will jump before continuing to the inner-most for */                \
        for (;                                                                                              \
            fldefjmp != 0 || _fl_defer_scope_.state != FL_DEFER_STARTED                                     \
                ? 1                                                                                         \
                : (_fl_defer_scope_.state = FL_DEFER_LEAVING, longjmp(_fl_defer_scope_.call_chain           \
                    ? _fl_defer_scope_.call_chain->entry_point                                              \
                    : _fl_defer_scope_.exit_point, 1), 0);)                                                 \
            for (;                                                                                          \
                /* In case of an error state, we abort with an error. If not, the condition */              \
                /* will be true, and the return statement will be executed  */                              \
                fldefjmp == 0 &&_fl_defer_scope_.state != FL_DEFER_STARTED                                  \
                    ? (flm_exit(ERR_FATAL, "defer_exit cannot be called twice nor"                          \
                        " be called within a defer statement"), 0)                                          \
                    : 1;)                                                                                   \
                return 


#define defer_statements                                                                                    \
    /* This for will be skipped the first time it is called, and the body */                                \
    /* will be accessible just by a longjmp call to the buffer we are     */                                \
    /* evaluation at the "condition" part. */                                                               \
    for (                                                                                                   \
        /* init: "Capture" the newly created statement, if not we will lose */                              \
        /* it on following statements' "pushes"  */                                                         \
        struct FlDeferStmt *self                                                                            \
            = _fl_defer_scope_.call_chain                                                                   \
            = &(struct FlDeferStmt) { .prev = _fl_defer_scope_.call_chain };                                \
        /* condition: We will skip the for's body in the first setjmp call */                               \
        setjmp(self->entry_point) != 0;                                                                     \
        /* increment (use comma operator) */                                                                \
        (                                                                                                   \
            /* The increment condition is the "deferred statement" that has been */                         \
            /* executed. We need to move to the previous "deferred statemet" */                             \
            _fl_defer_scope_.call_chain = self->prev,                                                       \
            /* If there is still a call in the stack, jump to it, if not, jump to */                        \
            /* the scope's exit point */                                                                    \
            longjmp(_fl_defer_scope_.call_chain                                                             \
                ? _fl_defer_scope_.call_chain->entry_point                                                  \
                : _fl_defer_scope_.exit_point, 1)                                                           \
        )                                                                                                   \
    )

#define defer_expression(expression)                                                                        \
    flm_assert(_fl_defer_scope_.state == FL_DEFER_STARTED,                                                  \
        "Cannot stack a deferred statement within a deferred block.");                                      \
    /* Add the new deferred statement to the stack, and skip the if's body in the first call */             \
    /* to setjmp */                                                                                         \
    if (setjmp((_fl_defer_scope_.call_chain                                                                 \
        = &(struct FlDeferStmt) { .prev = _fl_defer_scope_.call_chain })->entry_point) != 0)                \
    {                                                                                                       \
        /* evaluate the expression */                                                                       \
        (expression);                                                                                       \
        /* Remove the statement from the stack by overriding it with the previous */                        \
        /* stacked statement */                                                                             \
        _fl_defer_scope_.call_chain = _fl_defer_scope_.call_chain->prev;                                    \
        /* If the previous statement exists, jump to it. If not, jump to the */                             \
        /* scope's exit point */                                                                            \
        longjmp(_fl_defer_scope_.call_chain                                                                 \
            ? _fl_defer_scope_.call_chain->entry_point : _fl_defer_scope_.exit_point, 1);                   \
    }

#endif /*FL_DEFER_H */
