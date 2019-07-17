#ifndef FL_DEFER_H
#define FL_DEFER_H

#include "Types.h"

/*
 * Type: struct FlDeferStmt
 * 
 * ===== C =====
 *  struct FlDeferStmt {
 *      struct FlDeferStmt *prev;
 *      void *object;
 *      void (*call)(void*);
 *  };
 *
 */
struct FlDeferStmt {
    struct FlDeferStmt *prev;
    jmp_buf stmt_entry;
};

struct FlDeferScope {
    struct FlDeferStmt *call_chain;
    jmp_buf exit;
    void *tmpptr;
    short leaving;
};

#define defer_end() \
    if (scope.leaving) {\
        flm_assert(!scope.leaving, "Invalid statement within a defer statement or expression") \
    } else \
        if (setjmp(scope.exit) == 0) \
            longjmp(scope.call_chain ? scope.call_chain->stmt_entry : scope.exit, 1);

#define defer_end_return defer_end() else return

#define defer_end_and defer_end() else 



#define defer_enable_in_function { defer_enable_in_block

#define defer_enable_in_block \
    for (struct FlDeferScope scope = { .leaving = 0, .call_chain = NULL, .tmpptr = NULL }; \
        scope.call_chain == NULL; \
        (scope.leaving = 1, longjmp(scope.call_chain ? scope.call_chain->stmt_entry : scope.exit, 1))) \
        if (setjmp(scope.exit) != 0) { \
            break; \
        } else 

#define defer_statements \
    scope.call_chain = &(struct FlDeferStmt) { .prev = scope.call_chain }; \
    for (struct FlDeferStmt *this = scope.call_chain; \
        setjmp(this->stmt_entry) != 0 ; \
        (scope.call_chain = this->prev, longjmp(scope.call_chain ? scope.call_chain->stmt_entry : scope.exit, 1)))

#define defer_expression(expression) \
    scope.call_chain = &(struct FlDeferStmt) { .prev = scope.call_chain }; \
    if (setjmp(scope.call_chain->stmt_entry) != 0) \
    { \
        expression; \
        scope.call_chain = scope.call_chain->prev; \
        longjmp(scope.call_chain ? scope.call_chain->stmt_entry : scope.exit, 1); \
    }


#endif /*FL_DEFER_H */
