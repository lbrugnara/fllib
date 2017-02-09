#ifndef FL_MEM_H
#define FL_MEM_H

#include <stddef.h>
#include "Types.h"

/**
 * These functions are wrappers for allocation/deallocation functions. Currently they
 * do not do much, only simple assertions and checks for bad (re)allocation.
 */

FlPointer fl_malloc(size_t size);
void  fl_free(FlPointer ptr);
FlPointer fl_calloc(size_t nmemb, size_t size);
FlPointer fl_realloc(FlPointer ptr, size_t size);
size_t fl_memtrack_get();
#endif /* FL_MEM_H */
