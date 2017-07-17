#ifndef FL_ERROR_H
#define FL_ERROR_H
/* =============================================================
 * {module: Error}
 * =============================================================
 * Provides common data structures, functions and macros that
 * can be used by any module and user libary for error handling.
 * Current support:
 *  - Error handling through {FlError} objects and the {fl_error_*} functions familiy
 *  - Assertion and a exit() wrapper for quick-leave-on-error situations
 * -------------------------------------------------------------
 */

#include <stddef.h>
#include <setjmp.h>
#include "Types.h"

#define FL_ERROR_MSG_MAX_SIZE 256
#define FL_ERROR_TRYCONTEXT_MAX_MSG_SIZE 256

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
typedef struct FlError
{
    int id;
    char message[FL_ERROR_MSG_MAX_SIZE];
} FlError;

/* -------------------------------------------------------------
 * {function: fl_error_push}
 * -------------------------------------------------------------
 * Creates an {FlError} with the specified {id} and message and push it
 * in the error queue
 * -------------------------------------------------------------
 * {param: int id} ID of the error. This is generic, each module can create its own mapping for error codes
 * {param: const char *format} Error message. Accepts 3 format specifiers: %d, %c and %s
 * {param: ... arguments} Additional arguments used to replace format specifiers in {format}
 * -------------------------------------------------------------
 * {return: FlError} Created error
 * -------------------------------------------------------------
 */
void fl_error_push(int id, const char *format, ...);

/* -------------------------------------------------------------
* {function: fl_error_last}
* -------------------------------------------------------------
* Returns the most recent error. In a threaded environment, it is
* the most recent error for the current thread id ({ref: fl_thread_current_id})
* -------------------------------------------------------------
* {return: FlError} FlError object with its id and message
* -------------------------------------------------------------
*/
FlError fl_error_last();

/* -------------------------------------------------------------
* {function: fl_errno_str}
* -------------------------------------------------------------
* Populates {buf} with an error message based on errnum (errno) through
* one of the strerror functions family
. The target buffer must allocate the needed space.
* -------------------------------------------------------------
* {param: int errnum} Errno
* {param: char* buf} Destination buffer
* {param: size_t len} Max number of bytes to be write  in {buf}
* -------------------------------------------------------------
* {return: char*} String containing the error message represented by errno
* -------------------------------------------------------------
*/
char* fl_errno_str(int errnum, char* buf, size_t len);

/* -------------------------------------------------------------
 * {function: fl_exit}
 * -------------------------------------------------------------
 * Writes a message to {stderr} and call exit(-1)
 * -------------------------------------------------------------
 * {param: FlErrorType errtype} Type of error to show in the message
 * {param: const char *format} Contains the text to be write to {stderr}
 * {param: ... arguments} Additional arguments used to replace format specifiers in {format}
 * -------------------------------------------------------------
 * {return: void}
 * -------------------------------------------------------------
 */
void fl_exit(FlErrorType errtype, const char *format, ...);

/* -------------------------------------------------------------
 * {macro: flm_exit}
 * -------------------------------------------------------------
 * Provides a verbose error message
 * -------------------------------------------------------------
 * {param: FlErrorType errtype} Type of error
 * {param: const char *message} Error message
 * -------------------------------------------------------------
 */
#define flm_exit(errtype, message) fl_exit(errtype, "in ["__FILE__ ":" FL_LINE "|%s] " message, __func__)

/* -------------------------------------------------------------
 * {macro: flm_vexit}
 * -------------------------------------------------------------
 * Provides a verbose error message
 * -------------------------------------------------------------
 * {param: FlErrorType errtype} Type of error
 * {param: const char *format} Error message with format specifers support
 * {param: ... arguments} Additional arguments used to replace format specifiers in {format}
 * -------------------------------------------------------------
 */
#define flm_vexit(errtype, format,...) fl_exit(errtype, "in ["__FILE__ ":" FL_LINE "|%s] " format, __func__, __VA_ARGS__)

#ifdef FL_DEBUG
     /* -------------------------------------------------------------
     * {macro: flm_assert}
     * -------------------------------------------------------------
     * Checks {condition}, and if it is false, calls {fl_exit} with the
     * message error defined in {error}
     * -------------------------------------------------------------
     * {param: bool condition} Expression that returns a boolean value
     * {param: const char *error} Error message
     * -------------------------------------------------------------
     */
    #define flm_assert(condition, error) if (!(condition)) (flm_exit(ERR_FATAL, "Condition '" #condition "' failed. " error));

     /* -------------------------------------------------------------
      * {macro: flm_vassert}
      * -------------------------------------------------------------
      * Checks {condition}, and if it is false, calls {fl_exit} with the
      * message error defined in {error}.
      * -------------------------------------------------------------
      * {param: bool condition} Expression that returns a boolean value
      * {param: const char *error} Error message that supports format specifiers
      * {param: ... arguments} Additional arguments used to replace format specifiers in {error}
      * -------------------------------------------------------------
      */
    #define flm_vassert(condition, error, ...) if (!(condition)) (flm_vexit(ERR_FATAL, "Condition '" #condition "' failed. " error, __VA_ARGS__));
#else
    /* Mute the assertions */
    #define flm_assert(condition, error)
    #define flm_vassert(condition, error, ...)
#endif

/* -------------------------------------------------------------
 * {datatype: struct FlContext}
 * -------------------------------------------------------------
 * Has a jmp_buf member to save and restore the state of the
 * program using setjmp and longjmp. It is not opaque because
 * we need to access jmp_buf directly
 * -------------------------------------------------------------
 * {member: jmp_buf env} State of the program
 * -------------------------------------------------------------
 */
typedef struct {
  jmp_buf env;
} FlContext;

/* -------------------------------------------------------------
 * {macro: fl_context_save}
 * -------------------------------------------------------------
 * Saves the state of the program into an FlContext
 * -------------------------------------------------------------
 * {param: FlContext ctx} Context to save the state of the program
 * -------------------------------------------------------------
 */
#define fl_context_save(ctx)    (setjmp((ctx)->env))

/* -------------------------------------------------------------
 * {macro: fl_context_restore}
 * -------------------------------------------------------------
 * Restores the state of the program using the provided FlContext
 * -------------------------------------------------------------
 * {param: FlContext ctx} Contains the state of the program
 * to be restored after this call
 * -------------------------------------------------------------
 */
#define fl_context_restore(ctx, v) (longjmp((ctx)->env, v))

/* -------------------------------------------------------------
 * Error handling with setjmp/longjmp
 * -------------------------------------------------------------
 * Provides a mechanism to handle error conditions using the 
 * (evil?) sjlj. (Yup, like goto, nope, also non-local jumps)
 */

/* -------------------------------------------------------------
 * {datatype: struct FlTryContext}
 * -------------------------------------------------------------
 * Contains information about the context (using jmp_buf through FlContext)
 * and information about the "exception". It is not opaque because
 * we need to access jmp_buf directly through FlContext
 * -------------------------------------------------------------
 * {member: FlContext ctx} Saves the info to restore the program state 
 * {member: int exception} Value to be passed to longjmp. Represents an exception (General purpouse usage)
 * {member: char[] message} Contains a brief description about the exception
 * -------------------------------------------------------------
 */
typedef struct
{
    FlContext ctx;
    int exception;
    char message[FL_ERROR_TRYCONTEXT_MAX_MSG_SIZE];
} FlTryContext;

/* -------------------------------------------------------------
 * Try...Catch...Rest...Finally...EndTry. Do not run away!
 * -------------------------------------------------------------
 * Remove all the defines and you will understand this ;)
 */
#define Try(tryctx)                                             \
do                                                              \
{                                                               \
    (tryctx)->exception=0;                                      \
    memset((tryctx)->message, 0, FL_ERROR_TRYCONTEXT_MAX_MSG_SIZE);  \
    int res = fl_context_save(&(tryctx)->ctx);                  \
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
//          Notice the two braces in Finally, intended
//          to close the switch, and the opening  brace
//          to match the "}" in EndTry.
//          Closing the switch makes this piece of code 
//          to run always, either after the normal execution
//          or after the exception handling process
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
#define Throw(tryctx, ex)                                                            \
do                                                                                   \
{                                                                                    \
    flm_assert(ex != 0, "setjmp will return 1 after longjmp because int val == 0");  \
    (tryctx)->exception = ex;                                                        \
    fl_context_restore(&(tryctx)->ctx, ex);                                          \
} while (0)

#endif /* FL_ERROR_H */
