#include <stdarg.h>
#include <string.h>

#include "Error.h"
#include "Mem.h"
#include "Std.h"
#include "threading/Thread.h"
#include "Cstr.h"

#include "containers/Dictionary.h"

/* -------------------------------------------------------------
 * {datatype: struct FlError}
 * -------------------------------------------------------------
 * Represents an error. There is no implementation restrictions,
 * each module or library can use {id} and {message} as it wish.
 * -------------------------------------------------------------
 * {member: int id} Error id or code. Module/library implementation defined
 * {member: FlCstr message} Is a brief message that contains information about the error
 * -------------------------------------------------------------
 */

/* -------------------------------------------------------------
* The FlErrQueue struct is used as a circular queue, through FL_ERROR_QUEUE
* the user can se the maximum number of errors to "save".
* This module is intended to be used in a multi-threaded environment, in that
* case, each thread has its own FL_ERROR_QUEUE restriction.
* -------------------------------------------------------------
*/
#ifndef FL_ERROR_QUEUE
    #define FL_ERROR_QUEUE 1
#endif

typedef struct FlErrQueue {
    int last;
    FlError errors[FL_ERROR_QUEUE];
} FlErrQueue;

/* -------------------------------------------------------------
* Errors is am FlDictionary<FlThreadId, FlErrQueue> that will containers
* up to FL_ERROR_QUEUE errors for each thread.
* -------------------------------------------------------------
*/
FlDictionary Errors/*<FlThreadId, FlErrQueue>*/;

FlMutex ErrMutex = FL_MUTEX_INIT_STATIC;

void push_error(FlErrQueue *queue, struct FlError error)
{
    flm_assert(queue != NULL, "FlErrQueue must not be NULL");
    if (queue->last == FL_ERROR_QUEUE)
        queue->last = 0;
    queue->errors[queue->last++] = error;
}

struct FlError last_error(FlErrQueue *queue)
{
    return queue->errors[queue->last-1];
}

/* -------------------------------------------------------------
* Function to release the memory used by FlErrQueue in the Errors
* dictionary
* -------------------------------------------------------------
*/
void delete_errors_h(FlByte* ptr)
{
    FlKeyValuePair kvp = *(FlKeyValuePair*)ptr;
    FlErrQueue** queueptr = fl_kvp_get_val(kvp);
    free(*queueptr);
    fl_kvp_delete(kvp);
    free(ptr);
}

void delete_errors(void)
{
    fl_dictionary_delete_h(Errors, delete_errors_h);
}

void fl_error_push(int id, const FlCstr format, ...)
{
    // Prepare the new error
    struct FlError error = {0};
    error.id = id;
    va_list args;
    va_start(args, format);
    FlCstr str = fl_cstr_vadup(format, args);
    va_end(args);
    // Cap the message to FL_ERROR_MSG_MAX_SIZE
    size_t length = min(FL_ERROR_MSG_MAX_SIZE, strlen(str) + 1);
    memcpy(error.message, str, length);
    error.message[length-1] = FL_EOS;
    fl_cstr_delete(str);

    // Sync access to Errors dictionary
    fl_mutex_lock(&ErrMutex);
    if (Errors == NULL)
    {
        // Create the Dict if it doesn't exist
        Errors = fl_dictionary_new(sizeof(FlThreadId), sizeof(FlErrQueue*));
        // Register the cleaning function
        atexit(delete_errors);
    }

    // Get the current ID
    FlThreadId currentid = fl_thread_current_id();
    // If there is no FlErrQueue registered for this thread, create it
    if (!fl_dictionary_contains_key(Errors, &currentid))
    {
        FlErrQueue *q = fl_malloc(sizeof(FlErrQueue));
        fl_dictionary_add(Errors, &currentid, &q);
    }

    // Get the thread's FlErrQueue
    FlErrQueue *queue = *(FlErrQueue**)fl_dictionary_get_val(Errors, &currentid);

    // Push the error
    push_error(queue, error);

    // Release the lock
    fl_mutex_unlock(&ErrMutex);
}

/* -------------------------------------------------------------
* Returns the thread's last registered error
* -------------------------------------------------------------
*/
FlError fl_error_last()
{
    FlThreadId currentid = fl_thread_current_id();
    if (!fl_dictionary_contains_key(Errors, &currentid))
    {
        return (FlError){ 0 };
    }
    FlErrQueue *queue = *(FlErrQueue**)fl_dictionary_get_val(Errors, &currentid);    
    return last_error(queue);
}

/* -------------------------------------------------------------
* This function wraps the strerror function to use when available the
* secure version, if it is not available, just copy up to {len} bytes
* of the msg in the destination buffer.
* -------------------------------------------------------------
*/
char* fl_errno_str(int errnum, char* buf, size_t len) 
{
    // If we have the secure version of strrerror use it, if not use strerror with its known 
    // issues in MT
    #if defined(_WIN32) && defined(__STDC_WANT_SECURE_LIB__)
    {
        strerror_s(buf, len, errnum);
    }
    #elif defined(__linux__) && (_XOPEN_SOURCE == 600 || _POSIX_C_SOURCE == 200112L && !defined(_GNU_SOURCE))
    {
        strerror_r(errnum, buf, len);
    }
    #else
    {
        char* msg = strerror(errnum);
        strncpy(buf, msg, len);
    }
    #endif
    return buf;
}

/* -------------------------------------------------------------
* Calls exit(-1) but before send formatted error message to the
* stderror
* -------------------------------------------------------------
*/
void fl_exit(FlErrorType errtype, const FlCstr format, ...)
{
    FlCstr errtypemsg;
    switch(errtype)
    {
        case ERR_FATAL:
            errtypemsg = fl_cstr_dup("FATAL ERROR: ");
            break;
        case ERR_UNIMPLEMENTED:
            errtypemsg = fl_cstr_dup("UNIMPLEMENTED ERROR: ");
            break;
        default:
            errtypemsg = fl_cstr_dup("UNKNOWN ERROR: ");
    }

    va_list args;
    va_start(args, format);
    FlCstr msg = fl_cstr_dup(format);
    fl_cstr_append(&msg, "\n");
    fl_cstr_append(&errtypemsg, msg);
    vfprintf(stderr, errtypemsg, args);
    va_end(args);
    fl_cstr_delete(msg);
    fl_cstr_delete(errtypemsg);
    exit(-1);
}
