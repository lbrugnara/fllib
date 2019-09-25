#include <stdarg.h>
#include <string.h>

#include "Error.h"
#include "Mem.h"
#include "Std.h"
#include "threading/Thread.h"
#include "Cstring.h"

#include "containers/Hashtable.h"

#ifndef FL_ERROR_QUEUE_MAX
    #define FL_ERROR_QUEUE_MAX 1
#endif

#ifndef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif

/*
 * The FlErrQueue struct is used as a circular queue, through FL_ERROR_QUEUE_MAX
 * the user can se the maximum number of errors to "save".
 * This module is intended to be used in a multi-threaded environment, in that
 * case, each thread has its own FL_ERROR_QUEUE_MAX restriction.
 *
 */
typedef struct FlErrQueue {
    int last;
    struct FlError errors[FL_ERROR_QUEUE_MAX];
} FlErrQueue;

/*
 * Errors is an FlHashtable<FlThreadId, FlErrQueue> that will contains
 * up to FL_ERROR_QUEUE_MAX errors for each thread.
 *
 */
FlHashtable Errors;

FlMutex ErrMutex = FL_MUTEX_INIT_STATIC;

void push_error(FlErrQueue *queue, struct FlError error)
{
    flm_assert(queue != NULL, "FlErrQueue must not be NULL");
    if (queue->last == FL_ERROR_QUEUE_MAX)
        queue->last = 0;
    queue->errors[queue->last++] = error;
}

struct FlError last_error(FlErrQueue *queue)
{
    return queue->errors[queue->last-1];
}

unsigned long thread_id_hash(const FlByte *key)
{
    return *(const FlThreadId*)key;
}

bool thread_id_comparer(const FlByte *key1, const FlByte *key2)
{
    return *(const FlThreadId*)key1 == *(const FlThreadId*)key2;
}

FlThreadId* ttop(FlThreadId tid)
{
    FlThreadId *p = fl_malloc(sizeof(FlThreadId));
    *p = tid;
    return p;
}

void cleanup(void)
{
    fl_hashtable_free(Errors);
    fl_mutex_destroy(&ErrMutex);
}

void fl_error_push(int id, const char *format, ...)
{
    // Prepare the new error
    struct FlError error = {0};
    error.id = id;
    va_list args;
    va_start(args, format);
    char *str = fl_cstring_vadup(format, args);
    va_end(args);
    // Cap the message to FL_ERROR_MSG_SIZE
    size_t length = min(FL_ERROR_MSG_SIZE, strlen(str) + 1);
    memcpy(error.message, str, length);
    error.message[length-1] = FL_EOS;
    fl_cstring_free(str);

    // Sync access to Errors hashtable
    fl_mutex_lock(&ErrMutex);
    if (Errors == NULL)
    {
        // Create the Dict if it doesn't exist
        Errors = fl_hashtable_new_args((struct FlHashtableArgs){
            .hash_function = thread_id_hash,
            .key_comparer = thread_id_comparer,
            .key_cleaner = fl_container_cleaner_pointer,
            .value_cleaner = fl_container_cleaner_pointer
        });
        // Register the cleaning function
        atexit(cleanup);
    }

    // Get the current ID
    FlThreadId currentid = fl_thread_current_id();
    // If there is no FlErrQueue registered for this thread, create it
    if (!fl_hashtable_has_key(Errors, &currentid))
    {
        FlErrQueue *q = fl_calloc(1, sizeof(FlErrQueue));
        fl_hashtable_set(Errors, ttop(currentid), q);
    }

    // Get the thread's FlErrQueue
    FlErrQueue *queue = (FlErrQueue*)fl_hashtable_get(Errors, &currentid);

    // Push the error
    push_error(queue, error);

    // Release the lock
    fl_mutex_unlock(&ErrMutex);
}

/*
 * Returns the thread's last registered error
 *
 */
struct FlError fl_error_last()
{
    FlThreadId currentid = fl_thread_current_id();

    if (Errors == NULL || !fl_hashtable_has_key(Errors, &currentid))
        return (struct FlError){ 0 };

    FlErrQueue *queue = (FlErrQueue*)fl_hashtable_get(Errors, &currentid);    

    return last_error(queue);
}

/*
 * This function wraps the strerror function to use when available the
 * secure version, if it is not available, just copy up to {len} bytes
 * of the msg in the destination buffer.
 *
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

/*
 * Calls exit(-1) but before send formatted error message to the
 * stderror
 *
 */
void fl_exit(enum FlErrorType errtype, const char *format, ...)
{
    char *errtypemsg;
    switch(errtype)
    {
        case ERR_FATAL:
            errtypemsg = fl_cstring_dup("FATAL ERROR: ");
            break;
        case ERR_UNIMPLEMENTED:
            errtypemsg = fl_cstring_dup("UNIMPLEMENTED ERROR: ");
            break;
        default:
            errtypemsg = fl_cstring_dup("UNKNOWN ERROR: ");
    }

    va_list args;
    va_start(args, format);
    char *msg = fl_cstring_dup(format);
    fl_cstring_append(&msg, "\n");
    fl_cstring_append(&errtypemsg, msg);
    vfprintf(stderr, errtypemsg, args);
    va_end(args);
    fl_cstring_free(msg);
    fl_cstring_free(errtypemsg);
    exit(-1);
}

struct FlContext* fl_ctx_new(void)
{
    struct FlContext *ctx = fl_malloc(sizeof(struct FlContext));

    return ctx;
}


void fl_ctx_free(struct FlContext *ctx)
{
    fl_free(ctx);
}
