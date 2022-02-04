#include <string.h>
#include "Error.h"
#include "Mem.h"
#include "Std.h"

void* fl_copy(const void *var, size_t nbytes)
{
	flm_assert(var != NULL, "const void *var cannot be NULL");
	void *dest = fl_calloc(1, nbytes);

	if (!dest)
		return NULL;

	memcpy(dest, var, nbytes);
	return dest;
}
