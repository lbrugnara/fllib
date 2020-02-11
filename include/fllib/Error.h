#ifndef FL_ERROR_H
#define FL_ERROR_H
/*
 * File: Error
 *  Provides common data structures, functions and macros that
 *  can be used by any module and user libary for error handling.
 *  
 *  It currently supports:
 * 
 *  - Error handling through the *FlError* object and the *fl_error_* functions family
 *  - Assertions and an exit() wrapper for quick-leave-on-error situations
 *  - A primitive, basic, better you know what you are doing, try-catch mechanism using setjmp/longjmp
 *
 */

#include <stddef.h>
#include <setjmp.h>
#include "Types.h"


/*
 * Define: FL_ERROR_MSG_SIZE
 * ===== C =====
 *  #define FL_ERROR_MSG_SIZE 256
 * =============
 * 
 *  Maximum length of an error message in a <struct FlError> object
 *
 */
#define FL_ERROR_MSG_SIZE 256

/*
 * Define: FL_CTX_MSG_SIZE
 * ===== C =====
 *  #define FL_CTX_MSG_SIZE 256
 * =============
 * 
 *  Maximum length of an error message in a <struct FlContext> object
 *
 */
#define FL_CTX_MSG_SIZE 256

/*
 * Define: FL_CTX_FRAME_NUM
 * ===== C =====
 *  #define FL_CTX_FRAME_NUM 256
 * =============
 * 
 *  Maximum number of <struct FlContextFrame> objects a <struct FlContext>
 *  can contain
 *
 */
#define FL_CTX_FRAME_NUM 256

/*
 * Type: enum FlErrorType
 *  Different types of error. Used to give a more verbose error messages.
 * 
 * ===== C =====
 *  enum FlErrorType
 *  {
 *      ERR_FATAL,
 *      ERR_UNIMPLEMENTED
 *  };
 *
 */
enum FlErrorType {
    ERR_FATAL,
    ERR_UNIMPLEMENTED
};

/*
 * Type: struct FlError
 *  Represents an error object. It could be used by any module to give information about errors
 * 
 * ===== C =====
 * typedef struct FlError
 * {
 *     int id;
 *     char message[FL_ERROR_MSG_SIZE];
 * } FlError;
 *
 */
struct FlError
{
    int id;
    char message[FL_ERROR_MSG_SIZE];
};

/*
 * Function: fl_error_push
 *  Creates a <struct FlError> with the specified *id* and message and push it to the error queue
 *
 * Parameters:
 *  int id - ID of the error. This is generic, each module can create its own mapping for error codes
 *  const char *format - Error message. Accepts 3 format specifiers: *%d*, *%c* and *%s*
 *  ... arguments - Additional arguments used to replace format specifiers in *format*
 *
 * Return:
 *  void - This function does not return a value
 *
 */
void fl_error_push(int id, const char *format, ...);

/*
 * Function: fl_error_last
 *  Returns the most recent error. In a threaded environment, it is
 *  the most recent error for the current thread id (can get it using <fl_thread_current_id>)
 *
 * Parameters:
 *  None.
 * 
 * Return:
 *  <struct FlError> - error object with its id and message
 *
 */
struct FlError fl_error_last(void);

/*
 * Function: fl_errno_str
 *  Populates *buffer* with an error message based on the *errnum* (errno) through
 *  one of the *strerror* functions family
 * 
 * Parameters:
 *  int errnum - Errno
 *  char* buffer - Destination buffer
 *  size_t len - Max number of bytes to write in *buffer*
 *
 * Return:
 *  char* - Pointer to the *buffer* object
 * 
 * Notes:
 *  The target buffer must allocate the needed space.
 *
 */
char* fl_errno_str(int errnum, char* buffer, size_t len);

/*
 * Function: fl_exit
 *  Writes a message to the *stderr* and calls exit(-1)
 *
 * Parameters:
 *  enum FlErrorType errtype - Type of error to show in the message
 *  const char *format - Contains the text to write to *stderr*
 *  ... arguments - Additional arguments used to replace format specifiers in *format*
 *
 * Return:
 *  This function does never return
 *
 */
void fl_exit(enum FlErrorType errtype, const char *format, ...);

/*
 * Macro: flm_exit
 * ===== C =====
 *  #define flm_exit(errtype, message) fl_exit(errtype, "in ["__FILE__ ":" FL_LINE "|%s] " message, __func__)
 * =============
 *
 *  Writes a message (that includes the file name, the line number, and the calling function) to the *stderr*
 *  and calls exit(-1)
 *
 * Parameters:
 *  enum FlErrorType errtype - Type of error to show in the message
 *  const char *message - Error message
 * 
 * Notes:
 *  Because this macro calls <fl_exit>, it does never return.
 *
 */
#define flm_exit(errtype, message) fl_exit(errtype, "in ["__FILE__ ":" FL_LINE "|%s] " message, __func__)

/*
 * Macro: flm_vexit
 * ===== C =====
 *  #define flm_vexit(errtype, format,...) fl_exit(errtype, "in ["__FILE__ ":" FL_LINE "|%s] " format, __func__, __VA_ARGS__)
 * =============
 *
 *  Writes a formatted message (that includes the file name, the line number, and the calling function) to the *stderr*
 *  and calls exit(-1)
 *
 * Parameters:
 *  enum FlErrorType errtype - Type of error
 *  const char *format - Error message with format specifers support
 *  ... arguments - Additional arguments used to replace format specifiers in *format*
 * 
 * Notes:
 *  Because this macro calls <fl_exit>, it does never return.
 *
 */
#define flm_vexit(errtype, format,...) fl_exit(errtype, "in ["__FILE__ ":" FL_LINE "|%s] " format, __func__, __VA_ARGS__)

#ifdef FL_DEBUG
     /*
     * Macro: flm_assert
     * ===== C =====
     *  #define flm_assert(condition, error) ((condition) ? (flm_exit(ERR_FATAL, "Condition '" #condition "' failed. " error), 0) : 1)
     * =============
     *  Checks the *condition*, and if it is false, it calls the <fl_exit> function with the
     *  message error defined in *error*
     *
     * Parameters:
     *  bool condition - Expression that returns a boolean value
     *  const char *error - Error message
     * 
     * Notes:
     *  - If the condition evaluates to false, the macro calls <fl_exit> therefore in that situation it does not return
     *  - This macro evaluates only when the <FL_DEBUG> flag evaluates to 1
     *
     */
    #define flm_assert(condition, error) (!(condition) ? (flm_exit(ERR_FATAL, "Condition '" #condition "' failed. " error), 0) : 1)

     /*
      * Macro: flm_vassert
      * ===== C =====
      *  #define flm_vassert(condition, error, ...) (!(condition) ? (flm_vexit(ERR_FATAL, "Condition '" #condition "' failed. " error, __VA_ARGS__), 0) : 1)
      * =============
      *  Checks the *condition*, and if it is false, it calls the <fl_exit> function with the
      *  message error defined in *error* formatted with the additional arguments.
      *
      * Parameters:
      *     bool condition - Expression that returns a boolean value
      *     const char *error - Error message that supports format specifiers
      *     ... arguments - Additional arguments used to replace format specifiers in *error*
      *
      * Notes:
      *  - If the condition evaluates to false, the macro calls <fl_exit> therefore in that situation it does not return
      *  - This macro evaluates only when the <FL_DEBUG> flag evaluates to 1
      */
    #define flm_vassert(condition, error, ...) (!(condition) ? (flm_vexit(ERR_FATAL, "Condition '" #condition "' failed. " error, __VA_ARGS__), 0) : 1)
#else
    /* Mute the assertions */
    #define flm_assert(condition, error) 1
    #define flm_vassert(condition, error, ...) 1
#endif

/*
 * Type: struct FlContextStorage
 *  Keeps track of the context storage when used with <fl_ctx_storage_save> to 
 *  later be restored with <fl_ctx_storage_restore>
 * 
 * Members:
 *  env - *jmp_buf* used by setjmp and longjmp functions. The buffer shouldn't be manipulated by hand
 *
 *  ===== C =====
 *  struct FlContextStorage {
 *      jmp_buf env;
 *  };
 *
 */
struct FlContextStorage {
  jmp_buf env;
};

/*
 * Type: struct FlContextFrame
 *  Represents a frame within a <struct FlContext> object.
 * 
 * Members:
 *  ctx - context's storage
 *  exception - A value greater than 0 indicates the frame threw an exception
 *  message - A message describing the exception
 * 
 * ===== C ===== 
 *  struct FlContextFrame {
 *      struct FlContextStorage ctx;
 *      int exception;
 *      char message[FL_CTX_MSG_SIZE];
 *  };
 */
struct FlContextFrame {
    struct FlContextStorage ctx;
    int exception;
    char message[FL_CTX_MSG_SIZE];
};

/*
 * Type: struct FlContext
 *  A context object keeps track of multiple virtual frames to "unwind the stack" and keep track of
 *  nested errors
 *
 * Members:
 *  frames - Context "stack"
 *  fp - Current frame pointer
 *
 * ===== C =====
 *  struct FlContext {
 *      struct FlContextFrame frames[FL_CTX_FRAME_NUM];
 *      unsigned short fp;
 *  };
 */
struct FlContext {
    struct FlContextFrame frames[FL_CTX_FRAME_NUM];
    unsigned short fp;
};

/*
 * Macro: FL_CTX_STATIC_INIT
 * ===== C =====
 *  #define FL_CTX_STATIC_INIT (struct FlContext){ .fp = 0 }
 * =============
 * 
 *  Initializes a <struct FlContext> object in the stack.
 * 
 * Notes:
 *  For a heap allocated <struct FlContext> object use <fl_ctx_new> and <fl_ctx_free>
 * 
 */
#define FL_CTX_STATIC_INIT {0}

/*
 * Function: fl_ctx_new
 *  Allocates a <struct FlContext> object in heap and returns a pointer to it.
 *  The context returned by this function must be destroyed using the <fl_ctx_free>
 *  function.
 *
 * Parameters:
 *  None.
 *
 * Returns:
 *  <struct FlContext>* - Pointer to the allocated object.
 * 
 * Notes:
 *  For a stack allocated <struct FlContext> object use <FL_CTX_STATIC_INIT>
 *
 */
struct FlContext* fl_ctx_new(void);

/*
 * Function: fl_ctx_free
 *  Frees the memory allocated by the <struct FlContext> object.
 *
 * Parameters:
 *  ctx - Context object to be freed.
 *
 * Returns:
 *  void - This function does not return a value
 */
void fl_ctx_free(struct FlContext *ctx);

/*
 * Macro: fl_ctx_frame
 * ===== C =====
 *  #define fl_ctx_frame(tryctx) ((tryctx)->frames + ((tryctx)->fp - 1))
 * =============
 * 
 *  Returns a pointer to the current active <struct FlContextFrame> in the <struct FlContext>
 *  (the one that belongs to a Try-EndTry block)
 *  
 */
#define fl_ctx_frame(tryctx) ((tryctx)->frames + ((tryctx)->fp - 1))

/*
 * Macro: fl_ctx_last_frame
 * ===== C =====
 *  #define fl_ctx_last_frame(tryctx) ((tryctx)->frames + (tryctx)->fp)
 * =============
 * 
 *  Returns a pointer to the <struct FlContextFrame> that was previously active in the <struct FlContext>
 *  before the current frame (the one that could have thrown an exception)
 *  
 */
#define fl_ctx_last_frame(tryctx) ((tryctx)->frames + ((tryctx)->fp))

/*
 * Macro: fl_ctx_storage_save
 * ===== C =====
 *  #define fl_ctx_storage_save(ctx) (setjmp((ctx)->env))
 * =============
 * 
 *  Saves the state of the program into the <struct FlContextStorage> object
 *  that can be used with the <fl_ctx_storage_restore> macro.
 *
 * Parameters:
 *  ctx - A <struct FlContextStorage> object to save the state of the program
 * 
 * Return:
 *  int - Same rules as the setjmp function
 *
 */
#define fl_ctx_storage_save(ctx) (setjmp((ctx)->env))

/*
 * Macro: fl_ctx_storage_restore
 * ===== C =====
 *  #define fl_ctx_storage_restore(ctx, v) (longjmp((ctx)->env, v))
 * =============
 *
 *  Restores the state of the program using the provided <struct FlContextStorage> and uses
 *  the *v* value as the return value for the "fake" call to the <fl_ctx_storage_save> macro
 *
 * Parameters:
 *  ctx - Contains the state of the program in a <struct FlContextStorage> to be restored after this call
 * 
 * Return:
 *  This macro calls longjmp, therefore it does not return.
 * 
 * Notes:
 *  Because the longjmp will jump to the previous setjmp call (or <fl_ctx_storage_save>), the latter will return the value
 *  *v* on the "fake" call
 *
 */
#define fl_ctx_storage_restore(ctx, v) (longjmp((ctx)->env, v))

/*
 * About: Try-Catch
 *  Through the usage of the setjmp/longjmp functions family *fllib* implements a basic
 *  and primitive exceptions-like mechanism to handle errors. Because of the nature of the
 *  setjmp/longjmp functions, it is recommended to use this only if you are sure of what you are doing.
 * 
 *  The test suite of the *fllib* library uses this mechanism to recover on failures, you can check the
 *  usage there to get a working example of this.
 */

/*
 * Macro: Try
 *  The <Try> macro expects a pointer to a <struct FlContext> to save the state of the
 *  program, and push a new <struct FlContextFrame> onto the stack before evaluating the <Try> block.
 *  If the <Try> block "throws an exception", the program execution will be restored to this point, where
 *  the added frame will be removed from the stack, and the execution will "fall" in one of the <Catch>
 *  or the <Rest> blocks (if any). Either the <Try> block throws or not,  the <Finally> block, if present,
 *  will be called (with the exceptions mentioned in the <Finally> documentation).
 *
 * Parameters:
 *  tryctx - Pointer to a <struct FlContext> object to save the state of the program
 * 
 * ===== C =====
 *  #define Try(tryctx)                                                         \
 *  do                                                                          \
 *  {                                                                           \
 *      if ((tryctx)->fp >= FL_CTX_FRAME_NUM)                                   \
 *          fl_exit(ERR_FATAL, "Frame limit reached");                          \
 *      (tryctx)->fp++; // push frame onto the stack                            \
 *      fl_ctx_frame(tryctx)->exception=0;                                      \
 *      memset((void*)fl_ctx_frame(tryctx)->message, 0, FL_CTX_MSG_SIZE);       \
 *      int res = fl_ctx_storage_save(&fl_ctx_frame(tryctx)->ctx);              \
 *      if (res != 0) (tryctx)->fp--; // pop frame off the stack                \
 *      switch(res)                                                             \
 *      {                                                                       \
 *          case 0:                                                             \
 *          {
 *  //          The body of the try goes here
 *  //      }
 */
#define Try(tryctx)                                                         \
do                                                                          \
{                                                                           \
    if ((tryctx)->fp >= FL_CTX_FRAME_NUM)                                   \
        fl_exit(ERR_FATAL, "Frame limit reached");                          \
    (tryctx)->fp++; /* push frame onto the stack */                         \
    fl_ctx_frame(tryctx)->exception=0;                                      \
    memset((void*)fl_ctx_frame(tryctx)->message, 0, FL_CTX_MSG_SIZE);       \
    int res = fl_ctx_storage_save(&fl_ctx_frame(tryctx)->ctx);                  \
    if (res != 0) (tryctx)->fp--; /* pop frame off the stack */             \
    switch(res)                                                             \
    {                                                                       \
        case 0:                                                             \
        {
//          The body of the try goes here
//      }


/*
 * Macro: Catch
 *  The <Catch> macro represents a catch clause that expects for a particular
 *  error value
 *
 * Parameters:
 *  int excp - The value (*int*) the catch expects in order to go through the case body
 * 
 * ===== C =====
 *  #define Catch(excp) \
 *              break;  \
 *          }           \
 *          case excp:  \
 *          {
 *  //          Each catch has its case
 *  //      }
 */
#define Catch(excp) \
            break;  \
        }           \
        case excp:  \
        {
//          Each catch has its case
//      }


/*
 * Define: Rest
 *  The <Rest> macro represents a "catch-all" block where all the
 *  "exceptions" that do not have an appropiate <Catch> clause will
 *  end.
 * 
 * ===== C =====
 *  #define Rest        \
 *              break;  \
 *          }           \
 *          default:    \
 *          {
 *  //          All the "exceptions" that do not have a 
 *  //          particular case can be handled in a "catch all"
 *  //      }
 */
#define Rest        \
            break;  \
        }           \
        default:    \
        {
//          All the "exceptions" that do not have a 
//          particular case can be handled in a "catch all"
//      }


/*
 * Define: Finally
 *  The <Finally> macro represents a block of code that will be executed
 *  always independently of what happened in the <Try> and <Catch> clauses
 *  (as long as the blocks of these clauses do not jump or exit/abort, so 
 *  be careful, again, this is a basic and primitive mechanism)
 *
 * ===== C =====
 *  #define Finally \
 *          }       \
 *      }           \
 *      {           \
 *          {
 */
#define Finally \
        }       \
    }           \
    {           \
        {

/*
 * Define: EndTry
 *  The <EndTry> clause simply closes the whole block.
 *
 * ===== C =====
 *  #define EndTry  \
 *          }       \
 *      }           \
 *  } while (0)
 */
#define EndTry  \
        }       \
    }           \
} while (0)

/*
 * Macro: Throw
 *  Sets an "exception-like" state into the <struct FlContext> object and then
 *  restores the program to the state saved in a previous usage of the <Try> macro,
 *  where the active <struct FlContextFrame> will be popped out.
 *
 * Parameters:
 *  <struct FlContext> ctx - Pointer to the context with the saved state to restore (saved in a previous <Try> usage)
 *  int ex - Represents an error condition. Is the value that setjmp will return
 *  char *msg - Message to be copied into the frame's message member
 *
 * ===== C =====
 *  #define Throw(tryctx, ex, msg)                                                          \
 *  do                                                                                      \
 *  {                                                                                       \
 *      flm_assert(ex != 0, "setjmp will return 1 after longjmp because int val == 0");     \
 *      fl_ctx_frame(tryctx)->exception = ex;                                               \
 *      if (msg)                                                                            \
 *      {                                                                                   \
 *          size_t msglength = strlen(msg);                                                 \
 *          memcpy(fl_ctx_frame(tryctx)->message, msg, msglength < FL_CTX_MSG_SIZE          \
 *              ? msglength : FL_CTX_MSG_SIZE);                                             \
 *      }                                                                                   \
 *      fl_ctx_storage_restore(&fl_ctx_frame(tryctx)->ctx, ex);                             \
 *  } while (0)
 */
#define Throw(tryctx, ex, msg)                                                          \
do                                                                                      \
{                                                                                       \
    flm_assert(ex != 0, "setjmp will return 1 after longjmp because int val == 0");     \
    fl_ctx_frame(tryctx)->exception = ex;                                               \
    if (msg != NULL)                                                                            \
    {                                                                                   \
        size_t msglength = strlen(msg);                                                 \
        memcpy(fl_ctx_frame(tryctx)->message, msg, msglength < FL_CTX_MSG_SIZE          \
            ? msglength : FL_CTX_MSG_SIZE);                                             \
    }                                                                                   \
    fl_ctx_storage_restore(&fl_ctx_frame(tryctx)->ctx, ex);                                 \
} while (0)

/*
 * About: Gluing it all together
 *  Following is an example of how it looks before and after the preprocessing stage to
 *  better understand how this works:
 * 
 * ===== C =====
 *  struct FlContext tryctx = FL_CTX_STATIC_INIT;
 *  Try(&tryctx)
 *  {
 *      // some code
 *      Throw(&tryctx, 1, "Exception One");
 *  }
 *  Catch(1)
 *  {
 *      // do something if exception is of type 1
 *  }
 *  Catch(2)
 *  {
 *      // do something if exception is of type 2
 *  }
 *  Rest
 *  {
 *      // do something if exception is other than type 1 or 2
 *  }
 *  Finally
 *  {
 *      // do something independently of the exception type
 *  }
 *  EndTry;
 * =============
 * 
 * The -simplified- macro expansion will result in the following code:
 * 
 * ===== C =====
 *  // -----------------------------------------------------------------------------
 *  // Try(&tryctx)
 *  struct FlContext tryctx = FL_CTX_STATIC_INIT;
 *  do
 *  {
 *      if ((&tryctx)->fp >= FL_CTX_FRAME_NUM)
 *          fl_exit(ERR_FATAL, "Frame limit reached");
 * 
 *      (&tryctx)->fp++; // push frame onto the stack
 * 
 *      fl_ctx_frame(&tryctx)->exception=0;
 *      memset((void*)fl_ctx_frame(&tryctx)->message, 0, FL_CTX_MSG_SIZE);
 * 
 *      int res = fl_ctx_storage_save(&fl_ctx_frame(&tryctx)->ctx);
 * 
 *      // If it is non-0, is the context restore action, pop the frame off the stack
 *      if (res != 0) 
 *          (&tryctx)->fp--;
 * 
 *      switch(res)
 *      {
 *          case 0:
 *          {
 *            // some code
 *   
 * // -----------------------------------------------------------------------------
 * // Throw(&tryctx, 1, "Exception One");
 *            do
 *            {
 *                  flm_assert(1 != 0, "setjmp will return 1 after longjmp because int val == 0");
 *                  fl_ctx_frame(tryctx)->exception = 1;
 *                  if ("Exception One")
 *                  {
 *                      size_t msglength = strlen("Exception One");
 *                      memcpy(fl_ctx_frame(tryctx)->message, "Exception One", msglength < FL_CTX_MSG_SIZE
 *                          ? msglength : FL_CTX_MSG_SIZE);
 *                  }
 *                  fl_ctx_storage_restore(&fl_ctx_frame(tryctx)->ctx, 1);
 *            } while (0);
 * 
 * // -----------------------------------------------------------------------------
 * // Catch(1):
 *              break;
 *          }
 *          case 1:
 *          {
 *              // do something if exception is of type 1
 * 
 * // -----------------------------------------------------------------------------
 * // Catch(2):
 *              break;
 *          }
 *          case 2:
 *          {
 *              // do something if exception is of type 2
 *
 * // ----------------------------------------------------------------------------- 
 * // Rest:
 *              break;
 *          }
 *          default:
 *          {
 *              // do something if exception is other than type 1 or 2
 * 
 * // -----------------------------------------------------------------------------
 * // Finally:
 *          }
 *      }
 *      {
 *          {
 *              // do something either there is an exception or not, independently of the exception type
 * 
 * // -----------------------------------------------------------------------------
 * // EndTry:
 *          }
 *      }
 *  } while (0);
 * =============
 * 
 */

#endif /* FL_ERROR_H */
