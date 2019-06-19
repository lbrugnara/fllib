#ifndef FL_MEM_H
#define FL_MEM_H

/*
 * File: Mem
 *  These functions are wrappers for allocation/deallocation functions. Currently they
 *  do not do much, only simple assertions and checks for bad (re)allocation.
 * 
 */

#include <stddef.h>
#include "Types.h"


/*
 * Function: fl_malloc
 *  Allocates *size* bytes of memory initialized to 0
 *
 * Parameters:
 *  size - Number of bytes to allocate
 *
 * Returns:
 *  void* - Pointer to the allocated memory, or <NULL> on failure.
 *
 */
void* fl_malloc(size_t size);


/*
 * Function: fl_free
 *  Frees the memory allocated in *ptr*
 *
 * Parameters:
 *  ptr - Pointer to memory allocated with one of the <Mem> functions
 *
 * Returns:
 *  void - This function does not return a value.
 *
 */
void  fl_free(void *ptr);


/*
 * Function: fl_calloc
 *  Allocates memory for *nmemb* elements of *size* bytes each one.
 *
 * Parameters:
 *  nmemb - Number of elements
 *  size - Size of each element
 *
 * Returns:
 *  void* - Pointer to the allocated memory, or <NULL> on failure.
 *
 */
void* fl_calloc(size_t nmemb, size_t size);


/*
 * Function: fl_realloc
 *  Reallocates the memory in *ptr* to fit *size* bytes. On failure
 *  it returns <NULL> and frees the memory allocated by *ptr*, that way
 *  the following snippet of code can be safely used (as far as *ptr*
 *  does not point to a memory section that needs to be freed too):
 * 
 * ===== C =====
 *  ptr = fl_realloc(ptr, size);
 * =============
 * 
 * Parameters:
 *  ptr - Pointer to reallocate its memory.
 *  size - Number of bytes to reallocate.
 *
 * Returns:
 *  void* - Pointer to the reallocated memory or <NULL> on failure.
 *
 */
void* fl_realloc(void *ptr, size_t size);

#endif /* FL_MEM_H */
