#ifndef FL_ITERATOR_H
#define FL_ITERATOR_H

#include <stdio.h>
#include <stdlib.h>
#include "../Types.h"
#include "../Std.h"
#include "../Mem.h"
#include "Container.h"
#include "List.h"

typedef struct FlIterator FlIterator;
typedef struct FlVectorIterator FlVectorIterator;
typedef struct FlListIterator FlListIterator;

struct FlEachHandle
{
	FlUint isValid : 1;
	FlContainer *c;
	FlIterator *it;
	FlIterator *end;
};

/**
 * Using nested {for}s we can limit the scope of {uservar} to avoid pollution, weird but meets the requirements
 */
#define IN ,
#define FOREACH(vartype, expression) FOREACH_ELEM(expression, vartype)

#define FOREACH_ELEM(uservar, container, vartype)  																					\
	for (struct FlEachHandle handle = { 1, (FlContainer*)container, fl_iterator_start(handle.c), fl_iterator_end(handle.c)};		\
																		handle.isValid; 											\
																		(fl_free(handle.it), fl_free(handle.end), handle.isValid=0)) \
		for (vartype uservar;																										\
				(fl_iterator_next(handle.it), !fl_iterator_is_end(handle.it, handle.c)); /* nothing */)								\
			/* Hack: Assign value and discard result */																				\
			if ((uservar = flm_iterator_value(handle.it, vartype), true) && !fl_iterator_has_value(handle.it))						\
				flm_vexit(ERR_FATAL, "Pointer " #vartype "* in "#container"[%zd] is NULL", fl_iterator_current_index(handle.it));	\
			else if (!fl_iterator_is_end(handle.end, handle.c))																		\
				flm_exit(ERR_FATAL, "Container "#container" was modified during iteration"); 										\
			else

/* ITERATORS */

/**
 * Returns a FlIterator to the first element of {container}. The iterator pointer is set
 * before the first element of {container}.
 */
FlIterator* fl_iterator_start(void *container);

/**
 * Returns a FlIterator to the last element of {container}. The iterator pointer points
 * beyond the last element in {container}
 */
FlIterator* fl_iterator_end(void *container);

/**
 * Moves the {FlIterator} pointer to the next element of the {FlIterator} iterator
 */
FlPointer fl_iterator_next(FlIterator *it);

/**
 * Moves the {FlIterator} pointer to the previous element of the {FlIterator} iterator
 */
FlPointer fl_iterator_prev(FlIterator *it);

/**
 * Returns a pointer to the element {FlIterator} is pointing to
 */
FlPointer fl_iterator_value(FlIterator *it);

/**
 * Check if {it} is the end iterator of {container} 
 */
bool fl_iterator_is_end(FlIterator *it, void* container);

/**
 * Compares {FlIterator}s {i1} and {12} to check if they are pointing to the same element
 */
bool fl_iterator_equals(FlIterator *i1, FlIterator *i2);

/**
 * Checks if {it} is pointing to some value different from NULL
 */
bool fl_iterator_has_value(FlIterator *it);

/**
 * Returns a number representing an index in a {container} type
 */
size_t fl_iterator_current_index(FlIterator *it);

/**
 * Returns the value of the element {FlIterator} {it} is pointing to, casting it to
 * {type}
 */
#define flm_iterator_value(it, type) *(type*)fl_iterator_value(it)

#endif /* FL_ITERATOR_H */
