#ifndef FL_STD_H
#define FL_STD_H

#include <setjmp.h>

/* =============================================================
 * {module: Std}
 * =============================================================
 * Provides common data structures, functions and macros that
 * can be used by any module and user libary.
 * Current support:
 *  - Error handling through {FlError} objects and the {fl_error_*} functions familiy
 *  - Assertion and a exit() wrapper for quick-leave-on-error situations
 *  - Utility functions like {fl_copy} or macros like the {FLBIT} familiy intended to be used to handle modules flags
 * -------------------------------------------------------------
 */

#include "Types.h"

/* -------------------------------------------------------------
 * {datatype: enum FlErrorType}
 * -------------------------------------------------------------
 * Different types of error. Used to give a more verbose error
 * messages.
 * -------------------------------------------------------------
 */
typedef enum
{
    ERR_FATAL,
    ERR_UNIMPLEMENTED
} FlErrorType;

/* -------------------------------------------------------------
 * {datatype: typedef struct FlError}
 * -------------------------------------------------------------
 * Represents an error object. It could be used by any module
 * to give information about errors
 * -------------------------------------------------------------
 */
typedef struct FlError FlError;

/* -------------------------------------------------------------
 * {function: fl_error_set}
 * -------------------------------------------------------------
 * Allocates an {FlError} with the specified {id} and message
 * -------------------------------------------------------------
 * {param: FlError** error} A pointer to a memory location where this function will allocate the memory for the error object
 * {param: int id} ID of the error. This is generic, each module can create its own mapping for error codes
 * {param: const FlCstr format} Error message. Accepts format 3 specifiers: %d, %c and %s
 * {param: ... arguments} Additional arguments used to replace format specifiers in {format}
 * -------------------------------------------------------------
 * {return: void}
 * -------------------------------------------------------------
 */
void fl_error_set(FlError **error, int id, const FlCstr format, ...);

/* -------------------------------------------------------------
 * {function: fl_error_get_id}
 * -------------------------------------------------------------
 * Returns the ID of an {FlError} object
 * -------------------------------------------------------------
 * {param: FlError* error} Target error
 * -------------------------------------------------------------
 * {return: int} ID of the error object
 * -------------------------------------------------------------
 */
int fl_error_get_id(FlError *error);

/* -------------------------------------------------------------
 * {function: fl_error_get_message}
 * -------------------------------------------------------------
 * Returns the message of an {FlError} object
 * -------------------------------------------------------------
 * {param: FlError* error} Target error
 * -------------------------------------------------------------
 * {return: FlCstr} Message of the error object
 * -------------------------------------------------------------
 */
FlCstr fl_error_get_message(FlError *error);

/* -------------------------------------------------------------
 * {function: fl_error_delete}
 * -------------------------------------------------------------
 * Releases the memory allocated in {error}
 * -------------------------------------------------------------
 * {param: FlError* error} Target object to release its memory
 * -------------------------------------------------------------
 * {return: void}
 * -------------------------------------------------------------
 */
void fl_error_delete(FlError *error);


/* -------------------------------------------------------------
 * {function: fl_has_error}
 * -------------------------------------------------------------
 * Check if the memory pointed by {error} is allocated by an error
 * -------------------------------------------------------------
 * {param: FlError** error} A pointer that points to a location where could exist an {FlError} object allocated
 * -------------------------------------------------------------
 * {return: bool} True if the memory is allocated
 * -------------------------------------------------------------
 */
static inline bool fl_has_error(FlError **error)
{
    return error != NULL && *error != NULL;
}

/* -------------------------------------------------------------
 * {function: fl_exit}
 * -------------------------------------------------------------
 * Writes a message to {stderr} and call exit(-1)
 * -------------------------------------------------------------
 * {param: FlErrorType errtype} Type of error to show in the message
 * {param: const FlCstr format} Contains the text to be write to {stderr}
 * {param: ... arguments} Additional arguments used to replace format specifiers in {format}
 * -------------------------------------------------------------
 * {return: void}
 * -------------------------------------------------------------
 */
void fl_exit(FlErrorType errtype, const FlCstr format, ...);

/* -------------------------------------------------------------
 * {function: fl_copy}
 * -------------------------------------------------------------
 * Allocates and returns a new pointer to an element of {nbytes}
 * bytes and copies the content of {var} in the allocated memory
 * This is a memcpy-like function but it creates the destination
 * pointer.
 * -------------------------------------------------------------
 * {param: const FlPointer var} A pointer to the memory to be copied in the destination pointer
 * {param: size_t nbytes} Number of bytes to copy from {var} in destination pointer
 * -------------------------------------------------------------
 * {return: FlPointer} A pointer to the allocated memory
 * -------------------------------------------------------------
 */
FlPointer fl_copy(const FlPointer var, size_t nbytes);

/* -------------------------------------------------------------
 * {macro: flm_exit}
 * -------------------------------------------------------------
 * Provides a verbose error message
 * -------------------------------------------------------------
 * {param: FlErrorType errtype} Type of error
 * {param: const FlCstr message} Error message
 * -------------------------------------------------------------
 */
#define flm_exit(errtype, message) fl_exit(errtype, " in ["__FILE__ ":" FL_LINE "|%s] " message, __func__)

/* -------------------------------------------------------------
 * {macro: flm_vexit}
 * -------------------------------------------------------------
 * Provides a verbose error message
 * -------------------------------------------------------------
 * {param: FlErrorType errtype} Type of error
 * {param: const FlCstr format} Error message with format specifers support
 * {param: ... arguments} Additional arguments used to replace format specifiers in {format}
 * -------------------------------------------------------------
 */
#define flm_vexit(errtype, format,...) fl_exit(errtype, " in ["__FILE__ ":" FL_LINE "|%s] " format, __func__, __VA_ARGS__)

#ifdef FL_DEBUG
     /* -------------------------------------------------------------
     * {macro: flm_assert}
     * -------------------------------------------------------------
     * Checks {condition}, and if it is false, calls {fl_exit} with the
     * message error defined in {error}
     * -------------------------------------------------------------
     * {param: bool condition} Expression that returns a boolean value
     * {param: const FlCstr error} Error message
     * -------------------------------------------------------------
     */
    #define flm_assert(condition, error) if (!(condition)) (flm_exit(ERR_FATAL, error));

     /* -------------------------------------------------------------
      * {macro: flm_vassert}
      * -------------------------------------------------------------
      * Checks {condition}, and if it is false, calls {fl_exit} with the
      * message error defined in {error}.
      * -------------------------------------------------------------
      * {param: bool condition} Expression that returns a boolean value
      * {param: const FlCstr error} Error message that supports format specifiers
      * {param: ... arguments} Additional arguments used to replace format specifiers in {error}
      * -------------------------------------------------------------
      */
    #define flm_vassert(condition, error, ...) if (!(condition)) (flm_vexit(ERR_FATAL, error, __VA_ARGS__));
#else
    #define flm_assert(condition, error)
    #define flm_vassert(condition, error, ...)
#endif

/* -------------------------------------------------------------
 * {macro: FLBIT}
 * -------------------------------------------------------------
 * Set an n-th bit
 * -------------------------------------------------------------
 * {param: int n} Bit number
 * -------------------------------------------------------------
 */
#define FLBIT(n)            (1 << n)

/* -------------------------------------------------------------
 * {macro: FLBIT_ON}
 * -------------------------------------------------------------
 * Makes an OR between {t} and {b}. Intended to be use with
 * {FLBIT} for single bits (see examples).
 * -------------------------------------------------------------
 * {param: mixed t} Target value to apply OR
 * {param: mixed b} Source value to use in OR with {t}
 * -------------------------------------------------------------
 * {example}
 *  int n = 0;
 *  FLBIT_ON(n, FLBIT(2));
 *  printf("%d", n); //-> "4"
 *  FLBIT_ON(n, FLBIT(3));
 *  printf("%d", n); //-> "12"
 */
#define FLBIT_ON(t,b)       ((t) |= (b))

/* -------------------------------------------------------------
 * {macro: FLBIT_OFF}
 * -------------------------------------------------------------
 * Makes an AND between {t} and NOT({b}). Intended to be use with
 * {FLBIT} for single bits (see examples)
 * -------------------------------------------------------------
 * {param: mixed t} Target value to apply AND
 * {param: mixed b} Source value to apply NOT and use in AND with {t}
 * -------------------------------------------------------------
 * {example}
 *  int n = 12;
 *  FLBIT_OFF(n, FLBIT(3));
 *  printf("%d", n); //-> "4"
 *  FLBIT_OFF(n, FLBIT(2));
 *  printf("%d", n); //-> "0"
 */
#define FLBIT_OFF(t,b)      ((t) &= ~(b))

/* -------------------------------------------------------------
 * {macro: FLBIT_IS_ON}
 * -------------------------------------------------------------
 * Intended to be used with {FLBIT}, check if bit {b} of {t} is 1
 * -------------------------------------------------------------
 * {param: mixed t} Target value to check bit {b}
 * {param: mixed b} Bit to check in {t}
 * -------------------------------------------------------------
 */
#define FLBIT_IS_ON(t,b)    ((t) & (b))

/* -------------------------------------------------------------
 * {macro: FLBIT_IS_OFF}
 * -------------------------------------------------------------
 * Intended to be used with {FLBIT}, check if bit {b} of {t} is 0
 * -------------------------------------------------------------
 * {param: mixed t} Target value to check bit {b}
 * {param: mixed b} Bit to check in {t}
 * -------------------------------------------------------------
 */
#define FLBIT_IS_OFF(t,b)   (!((t) & (b)))

/* -------------------------------------------------------------
 * Error handling with setjmp/longjmp
 * -------------------------------------------------------------
 * Provides a mechanism to handle error conditions using the 
 * (evil?) sjlj. (Yup, like goto, nope, also non-local jumps)
 */

/* -------------------------------------------------------------
 * {datatype: struct FlTryContext}
 * -------------------------------------------------------------
 * Contains information about the context (using jmp_buf) and
 * information about the "exception". It is not opaque because
 * we need to access jmp_buf directly
 * -------------------------------------------------------------
 * {member: jmp_buf env} Saves the info to restore the program state 
 * {member: int exception} Value to be passed to longjmp. Represents an exception (General purpouse usage)
 * {member: char[] message} Contains a brief description about the exception
 * -------------------------------------------------------------
 */
#define EX_MSG_LENGTH 256
typedef struct
{
    jmp_buf env;
    int exception;
    char message[EX_MSG_LENGTH];
} FlTryContext;

/* -------------------------------------------------------------
 * {macro: fl_context_save}
 * -------------------------------------------------------------
 * Receives an FlTryContext, saves the state of the program in
 * jmp_buff and resets the values for its members exception
 * and message.
 * -------------------------------------------------------------
 * {param: FlTryContext ctx} Context to use in this try. It can
 * be passed through calls to return directly to where this call
 * was made
 * -------------------------------------------------------------
 */
#define fl_context_save(ctx)    ((ctx)->exception=0, memset((ctx)->message, 0, EX_MSG_LENGTH), setjmp((ctx)->env))

/* -------------------------------------------------------------
 * {macro: fl_context_restore}
 * -------------------------------------------------------------
 * Calls longjmp to return the program to the state saved in
 * jmp_buf, passing a parameter indicating an error situation
 * -------------------------------------------------------------
 * {param: FlTryContext ctx} Contains the state of the program
 * to be restored after this call
 * -------------------------------------------------------------
 */
#define fl_context_restore(ctx) (longjmp((ctx)->env, (int)(ctx)->exception))

/* -------------------------------------------------------------
 * Try...Catch...Rest...Finally...EndTry. Do not run away!
 * -------------------------------------------------------------
 * Remove all the defines and you will understand this ;)
 */
#define Try(ctx)                                                \
do                                                              \
{                                                               \
    int res = fl_context_save(ctx);                             \
    switch(res)                                                 \
    {                                                           \
        case 0:
//      {
//          The body of the try goes here
//      }
#define Catch(excp)                                             \
        break;                                                  \
        case excp:
//      {
//          Each catch has its case
//      }
#define Rest                                                    \
        break;                                                  \
        default:
//      {
//          All the "exceptions" that do not have a 
//          particular case can be handled in a "catch all"
//      }
#define Finally                                                 \
    }                                                           \
    {
//      {
//          Notice the two braces in finally, intended
//          to close the switch, and opening a new brace
//          to match the  old "}" in EndTry.
//          Closing the switch makes this pice of code to
//          run always after the normal execution or 
//          exception handling process through Catch and Rest
//      }
#define EndTry                                                  \
    }                                                           \
} while (0)

/* -------------------------------------------------------------
 * {macro: Throw}
 * -------------------------------------------------------------
 * Set an exception into an FlTryContext to restore the program
 * to the state saved in jmp_buf 
 * -------------------------------------------------------------
 * {param: FlTryContext ctx} Context with the saved state to restore
 * {param: int ex} Represents an error condition. Is the value that setjmp will return
 * -------------------------------------------------------------
 */
#define Throw(ctx, ex)                                                               \
do                                                                                   \
{                                                                                    \
    flm_assert(ex != 0, "setjmp will return 1 after longjmp because int val == 0");  \
    (ctx)->exception = ex;                                                           \
    fl_context_restore(ctx);                                                         \
} while (0)

#endif /* FL_STD_H */
