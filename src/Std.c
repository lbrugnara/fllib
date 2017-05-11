#include <string.h>
#include <stdarg.h>
#include "Mem.h"
#include "Std.h"
#include "Cstr.h"

FlPointer
fl_copy(const FlPointer var, size_t nbytes)
{
	flm_assert(var != NULL, "const FlPointer var cannot be NULL");
	FlPointer dest = fl_calloc(1, nbytes);
	memcpy(dest, var, nbytes);
	return dest;
}
