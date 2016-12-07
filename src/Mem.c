#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "Std.h"
#include "Mem.h"

FlPointer 
fl_malloc(size_t size)
{
	FlPointer dst = calloc(1, size);
	if (dst == NULL)
		flm_exit(ERR_FATAL, "Out of memory");
	return dst;
}

void
fl_free(FlPointer ptr)
{
	flm_assert(ptr != NULL, "FlPointer to free cannot be NULL");
	free(ptr);
}

FlPointer
fl_calloc(size_t nmemb, size_t size)
{
	FlPointer dst = calloc(nmemb, size);
	if (dst == NULL)
		flm_exit(ERR_FATAL, "Out of memory");
	return dst;
}

FlPointer 
fl_realloc(FlPointer ptr, size_t size)
{
	flm_assert(ptr != NULL, "FlPointer to realloc cannot be NULL");
	FlPointer tmp = realloc(ptr, size);
	if (tmp == NULL)
		flm_exit(ERR_FATAL, "Out of memory");
	ptr = tmp;
	return ptr;
}
