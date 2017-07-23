#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "Std.h"
#include "Mem.h"

void* fl_malloc(size_t size)
{
	void *dst = calloc(1, size);
	if (dst == NULL)
		flm_exit(ERR_FATAL, "Out of memory");
	return dst;
}

void fl_free(void *ptr)
{
	flm_assert(ptr != NULL, "void* to free cannot be NULL");
	free(ptr);
}

void* fl_calloc(size_t nmemb, size_t size)
{
	void *dst = calloc(nmemb, size);
	if (dst == NULL)
		flm_exit(ERR_FATAL, "Out of memory");
	return dst;
}

void* fl_realloc(void *ptr, size_t size)
{
	flm_assert(ptr != NULL, "void *to realloc cannot be NULL");
	void *tmp = realloc(ptr, size);
	if (tmp == NULL)
		flm_exit(ERR_FATAL, "Out of memory");
	ptr = tmp;
	return ptr;
}
