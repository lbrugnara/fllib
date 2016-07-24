#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "Std.h"
#include "Mem.h"

static size_t max_allocated_mem = 0;

void
fl_memtrack_add(size_t bytes)
{	
	max_allocated_mem += bytes;
}

size_t
fl_memtrack_get()
{
	return max_allocated_mem;
}

FlPointer 
fl_malloc(size_t size)
{
	FlPointer dst = malloc(size);
	if (dst == NULL)
		flm_exit(ERR_FATAL, "Out of memory");
	fl_memtrack_add(size);
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
	fl_memtrack_add(nmemb * size);
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
	fl_memtrack_add(size);
	return ptr;
}
