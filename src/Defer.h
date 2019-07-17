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
};

#define defer_scope_return if (setjmp(scope.exit) == 0) longjmp(scope.call_chain ? scope.call_chain->stmt_entry : scope.exit, 1); else return

#define defer_scope \
    for (struct FlDeferScope scope = { .call_chain = NULL, .exit = { 0 } }; scope.call_chain == NULL; longjmp(scope.call_chain ? scope.call_chain->stmt_entry : scope.exit, 1)) \
        if (setjmp(scope.exit) != 0) \
        { \
            break; \
        } else 

#define defer_statements \
    scope.call_chain = &(struct FlDeferStmt) { .prev = scope.call_chain, .stmt_entry = { 0 } }; \
    for (struct FlDeferStmt *this = scope.call_chain; \
        setjmp(this->stmt_entry) != 0 ; \
        (scope.call_chain = this->prev, longjmp(scope.call_chain ? scope.call_chain->stmt_entry : scope.exit, 1)))

#define defer_statement(statement) \
    scope.call_chain = &(struct FlDeferStmt) { .prev = scope.call_chain, .stmt_entry = { 0 } }; \
    if (setjmp(scope.call_chain->stmt_entry) != 0) \
    { \
        statement; \
        scope.call_chain = scope.call_chain->prev; \
        longjmp(scope.call_chain ? scope.call_chain->stmt_entry : scope.exit, 1); \
    }


#endif /*FL_DEFER_H */
