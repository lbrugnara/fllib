#include <string.h>
#include <stdarg.h>
#include "Mem.h"
#include "Std.h"
#include "Cstring.h"

void* fl_copy(const void *var, size_t nbytes)
{
	flm_assert(var != NULL, "const void *var cannot be NULL");
	void *dest = fl_calloc(1, nbytes);
	memcpy(dest, var, nbytes);
	return dest;
}
