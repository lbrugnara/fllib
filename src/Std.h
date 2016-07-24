#ifndef FL_STD_H
#define FL_STD_H

#include "Types.h"

typedef enum
{
	ERR_FATAL,
	ERR_UNIMPLEMENTED
} FlErrorType;

/**
 * Struct used to handle errors in fllib
 */
typedef struct FlError FlError;

/* Macros to call fl_exit using file name, line number and function */
#define flm_exit(errtype, format) fl_exit(errtype, " in ["__FILE__ ":" FL_LINE "|%s] " format, __func__)
#define flm_vexit(errtype, format,...) fl_exit(errtype, " in ["__FILE__ ":" FL_LINE "|%s] " format, __func__, __VA_ARGS__)

//#define flm_error_set(error, id, msg, ...) fl_error_set(error, id, "["__FILE__ ":" FL_LINE "|%s] %s", __func__, msg, __VA_ARGS__)

/**
 * Use flm_assert when FL_DEBUG is true to make assertions on expected values and results
 */
#ifdef FL_DEBUG
#	define flm_assert(condition, error) if (!(condition)) (flm_exit(ERR_FATAL, error));
#	define flm_vassert(condition, error, ...) if (!(condition)) (flm_vexit(ERR_FATAL, error, __VA_ARGS__));
#else
#	define flm_assert(condition, error)
#	define flm_vassert(condition, error, ...)
#endif

#define FLBIT(n) 			(1 << n)
#define FLBIT_ON(t,b)		((t) |= (b))
#define FLBIT_OFF(t,b)		((t) &= ~(b))
#define FLBIT_IS_ON(t,b) 	((t) & (b))
#define FLBIT_IS_OFF(t,b) 	(!((t) & (b)))

/**
 * Prints in stderr an error message and call exit(-1)
 */
void fl_exit(FlErrorType errtype, const FlCstr format, ...);

/**
 * Creates and returns a new pointer to an element of {nbytes} bytes
 * and copy the content of var in the element pointed by the
 * pointer. (memcpy like but creates the destination pointer)
 */
FlPointer fl_copy(const FlPointer var, size_t nbytes);


/**
 * Set an error {id} and an error {message} in {error}
 */
void fl_error_set(FlError **error, int id, const FlCstr format, ...);

/**
 * Returns the error ID of an {error}
 */
int fl_error_get_id(FlError *error);

/**
 * Returns a {message} that contains a brief description of the {error}
 */
FlCstr fl_error_get_message(FlError *error);

/**
 * Deletes an FlError freeing the memory used by the struct and the {desc}
 * memeber (an FlCstr)
 */
void fl_error_delete(FlError *error);

static inline bool fl_has_error(FlError **error)
{
	return error != NULL && *error != NULL;
}

#endif /* FL_STD_H */
