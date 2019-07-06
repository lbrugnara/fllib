#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "Std.h"
#include "Mem.h"

void* fl_malloc(size_t size)
{
	void *dst = calloc(1, size);
	
	if (dst)
		return dst;
	
	return NULL;
}

void fl_free(const void *ptr)
{
	free((void*)ptr);
}

void* fl_calloc(size_t nmemb, size_t size)
{
	void *dst = calloc(nmemb, size);

	if (dst)
		return dst;

	return NULL;
}

void* fl_realloc(void *ptr, size_t size)
{
	flm_assert(ptr != NULL, "pointer to realloc cannot be NULL");
	void *tmp = realloc(ptr, size);

	if (tmp)
		return tmp;

	fl_free(ptr);

	return NULL;
}
