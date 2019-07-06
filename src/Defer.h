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
        FL_DEFER_RUNNING = 0,
        FL_DEFER_LEAVING,
        FL_DEFER_ENDED
    } state;
};


/*
 * Macro: FL_DEFER_SCOPE_INIT
 * ===== C =====
 *  #define FL_DEFER_SCOPE_INIT struct FlDeferScope _fl_defer_scope_ = { .state = FL_DEFER_RUNNING, .call_chain = NULL }
 * =============
 *  Initializes the defer scope
 *
 */
#define FL_DEFER_SCOPE_INIT struct FlDeferScope _fl_defer_scope_ = { .state = FL_DEFER_RUNNING, .call_chain = NULL }

/*
 * Macro: FL_DEFER_IS_RUNNING
 * ===== C =====
 *  #define FL_DEFER_IS_RUNNING _fl_defer_scope_.state == FL_DEFER_RUNNING
 * =============
 *  Expression that returns true if the scope's state is <FL_DEFER_RUNNING>
 *
 */
#define FL_DEFER_IS_RUNNING _fl_defer_scope_.state == FL_DEFER_RUNNING

/*
 * Macro: FL_DEFER_IS_EXIT_POINT
 * ===== C =====
 *  #define FL_DEFER_IS_EXIT_POINT (setjmp(_fl_defer_scope_.exit_point) != 0)
 * =============
 *  This macro "hides" the magic of the setjmp call by simply evaluating the
 *  setjmp's return value and return *true* only if the value is different from 0.
 *  When a call to longjmp happens on the scope's exit point, this macro will evaluate
 *  to 1, that way we are sure we are leaving the defer scope.
 *
 */
#define FL_DEFER_IS_EXIT_POINT (setjmp(_fl_defer_scope_.exit_point) != 0)

/*
 * Macro: FL_DEFER_SET_EXIT_POINT
 * ===== C =====
 *  #define FL_DEFER_SET_EXIT_POINT (setjmp(_fl_defer_scope_.exit_point) == 0)
 * =============
 *  This macro "hides" the magic of the setjmp call by simply evaluating the
 *  setjmp's return value and return *true* only if the value is equals to 0.
 *  The macro "sets" (or actually saves) the point where it is used as the
 *  future scope's exit point
 *
 */
#define FL_DEFER_SET_EXIT_POINT (setjmp(_fl_defer_scope_.exit_point) == 0)

/*
 * Macro: FL_DEFER_STMT_IS_ENTRY_POINT
 * ===== C =====
 *  #define FL_DEFER_STMT_IS_ENTRY_POINT (setjmp(_fl_defer_scope_.call_chain->entry_point) != 0)
 * =============
 *  This macro "hides" the magic of the setjmp call by simply evaluating the
 *  setjmp's return value and return *true* only if the value is different from 0.
 *  When a call to longjmp happens on the statement's entry point, this macro will evaluate
 *  to 1, that way we are sure we need to execute the deferred statement
 *
 */
#define FL_DEFER_STMT_IS_ENTRY_POINT (setjmp(_fl_defer_scope_.call_chain->entry_point) != 0)

/*
 * Macro: FL_DEFER_ASSERT_RUNNING
 * ===== C =====
 *  #define FL_DEFER_ASSERT_RUNNING flm_assert(FL_DEFER_IS_RUNNING, "defer_break cannot be called twice nor be called within a defer statement")
 * =============
 *  If the scope's state is different from FL_DEFER_RUNNING, the program will exit with an error. This macro is
 *  used to make sure all the defer's macro calls are correct
 *
 */
#define FL_DEFER_ASSERT_RUNNING flm_assert(FL_DEFER_IS_RUNNING, "defer_break cannot be called twice nor be called within a defer statement")

/*
 * Macro: FL_DEFER_JUMP
 * ===== C =====
 *  #define FL_DEFER_JUMP longjmp(_fl_defer_scope_.call_chain ? _fl_defer_scope_.call_chain->entry_point : _fl_defer_scope_.exit_point, FL_DEFER_LEAVING)
 * =============
 *  This macro calls longjmp to jump to the last enqueued statement (if any) or to the scope's exit point
 *
 */
#define FL_DEFER_JUMP longjmp(_fl_defer_scope_.call_chain ? _fl_defer_scope_.call_chain->entry_point : _fl_defer_scope_.exit_point, FL_DEFER_LEAVING)

/*
 * Macro: FL_DEFER_JUMP_NEXT
 * ===== C =====
 *  #define FL_DEFER_JUMP_NEXT (_fl_defer_scope_.call_chain = _fl_defer_scope_.call_chain->prev, FL_DEFER_JUMP)
 * =============
 *  This macro removes an statement from the statements list and calls longjmp to jump to the next statement (if any)
 *  or to the scope's exit point
 *
 */
#define FL_DEFER_JUMP_NEXT (_fl_defer_scope_.call_chain =                                       \
    _fl_defer_scope_.call_chain->prev == _fl_defer_scope_.call_chain                            \
    ? NULL : _fl_defer_scope_.call_chain->prev, FL_DEFER_JUMP)

/*
 * Macro: FL_DEFER_ENQUEUE_STMT
 * ===== C =====
 *  #define FL_DEFER_ENQUEUE_STMT (_fl_defer_scope_.call_chain = &(struct FlDeferStmt) { .prev = _fl_defer_scope_.call_chain }, 1)
 * =============
 *  This macro enqueues a new deferred statement to the statements list. It uses the comma operator and returns a 1 to make sure it
 *  evaluates to *true*.
 *
 */
#define FL_DEFER_ENQUEUE_STMT (_fl_defer_scope_.call_chain = &(struct FlDeferStmt) { .prev = _fl_defer_scope_.call_chain }, 1)

/*
 * Macro: FL_DEFER_LEAVE
 * ===== C =====
 *  #define FL_DEFER_LEAVE (_fl_defer_scope_.state = FL_DEFER_LEAVING, FL_DEFER_JUMP)
 * =============
 *  Changes the scope's state to <FL_DEFER_LEAVING> to flag it so other statements can check if the scope
 *  is valid for certain operations, and then jumps to last enqueued statement or the scope's exit point
 *
 */
#define FL_DEFER_LEAVE (_fl_defer_scope_.state = FL_DEFER_LEAVING, FL_DEFER_JUMP)

/*
 * Macro: FL_DEFER_END
 * ===== C =====
 *  #define FL_DEFER_END (_fl_defer_scope_.state = FL_DEFER_ENDED)
 * =============
 *  Changes the scope's state to <FL_DEFER_ENDED> to reflect the scope is no longer valid for deferred
 *  operations.
 *
 */
#define FL_DEFER_END (_fl_defer_scope_.state = FL_DEFER_ENDED)


/*
 * Macro: defer_scope
 *  Initializes the deferred scope, sets the exit point, evaluates the scope's body and 
 *  before leave it runs all the deferred statements
 *
 */
#define defer_scope                                                                             \
    for (FL_DEFER_SCOPE_INIT; FL_DEFER_ASSERT_RUNNING; FL_DEFER_LEAVE)                          \
        if (FL_DEFER_IS_EXIT_POINT) {                                                           \
            FL_DEFER_END;                                                                       \
            break;                                                                              \
        }                                                                                       \
        else


/*
 * Macro: defer_break
 *  It jumps out of the deferred scope, but before leaving it calls all the deferred statements.
 *
 */
#define defer_break() if (FL_DEFER_ASSERT_RUNNING) { continue; } else {}


/*
 * Macro: defer_return
 *  Changes the scope's exit point to be the return statement, but before leaving, it calls
 *  all the deferred statements.
 *
 */
#define defer_return                                                                            \
    for (;;(FL_DEFER_ASSERT_RUNNING, FL_DEFER_LEAVE))                                           \
        for (;FL_DEFER_IS_EXIT_POINT;)                                                          \
            return

/*
 * Macro: defer_statements
 *  The statements within the <defer_statements> block will be enqueued to be called
 *  before leaving the deferred scope.
 *  Even though the deferred statements and expressions are called in the inverse order 
 *  they were registered (LIFO), the statements within the <defer_statements> block will
 *  be evaluated in a FIFO order
 *
 */
#define defer_statements                                                                        \
    for (FL_DEFER_ENQUEUE_STMT; FL_DEFER_STMT_IS_ENTRY_POINT; FL_DEFER_JUMP_NEXT)


/*
 * Macro: defer_expression
 *  The *expression* is enqueued to be called before leaving the deferred scope.
 *
 */
#define defer_expression(expression)                                                            \
    if (FL_DEFER_ASSERT_RUNNING && FL_DEFER_ENQUEUE_STMT)                                       \
    {                                                                                           \
        if (FL_DEFER_STMT_IS_ENTRY_POINT)                                                       \
        {                                                                                       \
            /* evaluate the expression */                                                       \
            (expression);                                                                       \
            FL_DEFER_JUMP_NEXT;                                                                 \
        }                                                                                       \
    }

#endif /*FL_DEFER_H */
