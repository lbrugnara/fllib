#include <stdarg.h>
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

#ifndef FL_ERROR_QUEUE
    #define FL_ERROR_QUEUE 1
#endif

typedef struct FlErrQueue {
    int last;
    FlError errors[FL_ERROR_QUEUE];
} FlErrQueue;

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

void delete_errors_h(FlByte* ptr)
{
    FlKeyValuePair kvp = *(FlKeyValuePair*)ptr;
    FlErrQueue** queueptr = fl_kvp_get_val(kvp);
    free(*queueptr);
    fl_kvp_delete(kvp);
    free(ptr);
}

void delete_errors()
{
    fl_dictionary_delete_h(Errors, delete_errors_h);
}

void
fl_error_push(int id, const FlCstr format, ...)
{
    struct FlError error = {0};
    error.id = id;
    va_list args;
    va_start(args, format);
    FlCstr str = fl_cstr_vadup(format, args);
    va_end(args);
    size_t max = 256;
    size_t l = strlen(str) + 1;
    memcpy(error.message, str, max < l ? max : l);
    error.message[(max < l ? max : l)-1] = FL_EOS;
    fl_cstr_delete(str);
    fl_mutex_lock(&ErrMutex);
    if (Errors == NULL)
    {
        Errors = fl_dictionary_new(sizeof(FlThreadId), sizeof(FlErrQueue*));
        atexit(delete_errors);
    }
    FlThreadId currentid = fl_thread_current_id();
    if (!fl_dictionary_contains_key(Errors, &currentid))
    {
        FlErrQueue *q = fl_malloc(sizeof(FlErrQueue));
        fl_dictionary_add(Errors, &currentid, &q);
    }
    FlErrQueue *queue = *(FlErrQueue**)fl_dictionary_get_val(Errors, &currentid);
    push_error(queue, error);
    fl_mutex_unlock(&ErrMutex);
}

FlError
fl_error_last()
{
    FlThreadId currentid = fl_thread_current_id();
    if (!fl_dictionary_contains_key(Errors, &currentid))
    {
        return (FlError){ 0 };
    }
    FlErrQueue *queue = *(FlErrQueue**)fl_dictionary_get_val(Errors, &currentid);    
    return last_error(queue);
}

void
fl_exit(FlErrorType errtype, const FlCstr format, ...)
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