#ifndef FL_DEFER_H
#define FL_DEFER_H

#include <limits.h>

/*
 * Type: struct FlDeferStack
 * 	Keeps track of the deferred expressions and statements.
 * 
 * ===== C =====
 * typedef struct FlDeferStack
 * {
 * 		volatile unsigned char ptr;
 *      volatile unsigned char max;
 *      jmp_buf *buffers;
 * };
 * 
 */
struct FlDeferStack {
    jmp_buf *buffers;
    volatile unsigned char ptr;
    volatile unsigned char max;
};

/*
 * Define: FL_DEFER_SCOPE_STACK_SIZE
 *  The default size of the deferred stack when the <defer_scope> macro is
 *  used.
 *
 */
#define FL_DEFER_SCOPE_STACK_SIZE 21

/*
 * Define: FL_DEFER_MAX_STACK_SIZE
 *  Maximum size of the deferred stack for both <defer_scope> and <defer_stack>
 *
 */
#define FL_DEFER_MAX_STACK_SIZE UCHAR_MAX


/*
 * Macro: FL_DEFER_STACK_INIT
 *  Initializes a new deferred stack with the provided size. This macro static asserts the size
 *  of the stack to make sure it is not greater than <FL_DEFER_MAX_STACK_SIZE>
 *
 * Parameters:
 *  size - Size of the stack to be created
 *
 */
#define FL_DEFER_STACK_INIT(size) struct FlDeferStack fl__defer__stack__ = \
    { .ptr = 0, .max = size, .buffers = (jmp_buf[size < FL_DEFER_MAX_STACK_SIZE ? size : -1]) { 0 } }

/*
 * Define: FL_DEFER_SET_JMP
 *  Calls setjmp to fill the current buffer and increments the pointer to point to the next available buffer
 *
 */
#define FL_DEFER_SET_JMP setjmp(fl__defer__stack__.buffers[fl__defer__stack__.ptr++])

/*
 * Macro: FL_DEFER_SET_JMP_AT
 *  Calls setjmp on the specifc buffer pointed by *i*
 *
 * Parameters:
 *  i - Index of the buffer to be filled
 *
 */
#define FL_DEFER_SET_JMP_AT(i) setjmp(fl__defer__stack__.buffers[(i)])

/*
 * Define: FL_DEFER_JMP
 *  Moves the stack pointer back to the last saved buffer and then jumps to it
 *
 */
#define FL_DEFER_JMP longjmp(fl__defer__stack__.buffers[--fl__defer__stack__.ptr], 1)

/*
 * Define: FL_DEFER_CHECK_SIZE
 *  Ensures the stack does not overflows
 *
 */
#define FL_DEFER_CHECK_SIZE  flm_vassert((fl__defer__stack__.ptr < fl__defer__stack__.max), \
    "Deferred stack has reached its maximum capacity (%d)", (int)fl__defer__stack__.max - 1)

/*
 * Define: defer_scope
 *  Creates a deferred stack using the default size (<FL_DEFER_SCOPE_STACK_SIZE>)
 *
 */
#define defer_scope                                                             \
    for (FL_DEFER_STACK_INIT(FL_DEFER_SCOPE_STACK_SIZE); FL_DEFER_SET_JMP == 0; FL_DEFER_JMP)

/*
 * Macro: defer_stack
 *  Creates a deferred stack of size *size*. It static asserts that the value 
 *  of *size* to be lesser than <FL_DEFER_MAX_STACK_SIZE>
 * 
 * Parameters:
 *  size - Size of the stack
 *
 */
#define defer_stack(size)                                                       \
    for (FL_DEFER_STACK_INIT(size + 1); FL_DEFER_SET_JMP == 0; FL_DEFER_JMP)

/*
 * Macro: defer_expression
 *  The *expression* is enqueued to be called before leaving the deferred scope.
 *
 */
#define defer_expression(expression)                                            \
    do {                                                                        \
        FL_DEFER_CHECK_SIZE;                                                    \
        if (FL_DEFER_SET_JMP != 0)                                              \
        {                                                                       \
            (expression);                                                       \
            FL_DEFER_JMP;                                                       \
        }                                                                       \
    } while (0)

/*
 * Macro: defer_statements
 *  The statements within the <defer_statements> block will be enqueued to be called
 *  before leaving the deferred scope.
 *  Even though the deferred statements and expressions are called in the inverse order 
 *  they were registered (LIFO), the statements *within* the <defer_statements> block will
 *  be evaluated in a FIFO order
 *
 */
#define defer_statements                                                        \
    for (FL_DEFER_CHECK_SIZE; FL_DEFER_SET_JMP != 0; FL_DEFER_JMP)              \

/*
 * Macro: defer_break
 *  It jumps out of the deferred scope, but before leaving it calls all the deferred statements.
 *
 */
#define defer_break() do { FL_DEFER_JMP; } while (0)

/*
 * Macro: defer_return
 *  Changes the scope's exit point to be the return statement, but before leaving, it calls
 *  all the deferred statements.
 *
 */
#define defer_return                                                            \
    for (;;FL_DEFER_JMP)                                                        \
        for (;FL_DEFER_SET_JMP_AT(0) != 0;)                                     \
            return

#endif /*FL_DEFER_H */
