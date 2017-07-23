#ifndef FL_MEM_H
#define FL_MEM_H

#include <stddef.h>
#include "Types.h"

/**
 * These functions are wrappers for allocation/deallocation functions. Currently they
 * do not do much, only simple assertions and checks for bad (re)allocation.
 */

void* fl_malloc(size_t size);
void  fl_free(void *ptr);
void* fl_calloc(size_t nmemb, size_t size);
void* fl_realloc(void *ptr, size_t size);

#endif /* FL_MEM_H */
