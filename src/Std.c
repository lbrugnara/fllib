#include <stdarg.h>
#include "Mem.h"
#include "Std.h"
#include "Cstr.h"

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
struct FlError
{
	int id;
	FlCstr message;
};

void
fl_error_set(FlError **error, int id, const FlCstr format, ...)
{
	if (error == NULL)
		return;
	FlError *err = fl_calloc(1, sizeof(FlError));
	err->id = id;
	va_list args;
    va_start(args, format);
    err->message = fl_cstr_vadup(format, args);
    va_end(args);
    *error = err;
}

int
fl_error_get_id(FlError *error)
{
	flm_assert(error != NULL, "Error must not be NULL");
	return error->id;
}

FlCstr
fl_error_get_message(FlError *error)
{
	flm_assert(error != NULL, "Error must not be NULL");
	if (error->message == NULL)
		return NULL;
	return error->message;
}

void
fl_error_delete(FlError *error)
{
	flm_assert(error != NULL, "Error must not be NULL");
	if (error->message)
		fl_free(error->message);
	fl_free(error);
}

FlPointer
fl_copy(const FlPointer var, size_t nbytes)
{
	flm_assert(var != NULL, "const FlPointer var cannot be NULL");
	FlPointer dest = fl_calloc(1, nbytes);
	memcpy(&dest, &var, nbytes);
	return dest;
}


void
fl_exit(FlErrorType errtype, const FlCstr format, ...)
{
	FlCstr errtypemsg;
	switch(errtype)
	{
		case ERR_FATAL:
			errtypemsg = fl_cstr_dup("FATAL ERROR");
			break;
		case ERR_UNIMPLEMENTED:
			errtypemsg = fl_cstr_dup("UNIMPLEMENTED ERROR");
			break;
		default:
			errtypemsg = fl_cstr_dup("UNKNOWN ERROR");
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