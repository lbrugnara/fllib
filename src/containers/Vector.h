#ifndef FL_VECTOR_H
#define FL_VECTOR_H

#include "../Types.h"
#include "Iterator.h"

/**
 * Vector struct does not implement
 * any specific member.
 */
typedef struct FlVector* FlVector;

/**
 * {@dtsize} DataType size for the new vector
 * {@nelem} Initial allocation for {nelem} elements
 *
 * Reserve memory for {nelem} elements of {dtsize} size each
 */
FlVector fl_vector_new(size_t dtsize, size_t nelem);

/**
 * Returns the number of elements in {vector}
 */
size_t fl_vector_length(FlVector vector);

/**
 * Returns the number of elements {vector} can contains without
 * a new allocation
 */
size_t fl_vector_capacity(FlVector vector);

/**
 * Returns the size of the data type that {vector} accepts
 */
size_t fl_vector_dtsize(FlVector vector);

/**
 * {@vector} Target vector to add a new value
 * {@elem} Element to add to the {vector}.
 * Copy {elem} content in {vector}.
 * Return a pointer to the inserted element
 */
FlPointer fl_vector_add(FlVector vector, const FlPointer elem);

/**
 * {@vector} Target vector to insert a new value
 * {@elem} Element to insert to the {vector}.
 * Insert {elem} in the position {pos} of {vector}
 * Return a pointer to the inserted element
 */
FlPointer fl_vector_insert(FlVector vector, const FlPointer elem, size_t pos);

/**
 * Increases {vector} capacity by {nelem}
 */
void fl_vector_resize(FlVector vector, size_t nelem);

/**
 * Returns a pointer to the element in the {index} index of {vector}
 */
FlPointer fl_vector_get(FlVector vector, size_t index);

void fl_vector_qsort(FlVector vector, int (*comparer)(const void *, const void*));

FlPointer fl_vector_bsearch(FlVector vector, const void *needle, int (*comparer)(const void *, const void*));

/**
 * Remove from {vector}, the first element in {vector} copying it into {dest}
 */
bool fl_vector_shift(FlVector vector, FlPointer dest);

/**
 * Insert {elem} in the first position of {vector}
 */
FlPointer fl_vector_unshift(FlVector vector, const FlPointer elem);

/**
 * Remove from {vector}, the last element copying it into {dest}
 */
bool fl_vector_pop(FlVector vector, FlPointer dest);

/**
 * Return true if {vector} contains {elem}
 */
bool fl_vector_contains(FlVector vector, const FlPointer elem);

/**
 * Append {vector2} elements at the end of {vector}
 */
void fl_vector_concat(FlVector vector, FlVector vector2);

/**
 * Creates a new FlVector that will contain all the elements of
 * {vector} and {vector2}
 */
FlVector fl_vector_merge(FlVector vector, FlVector vector2);

/**
 * Removes the {pos}-th element in {vector}.
 * The removed element will be copied in {dest} if it's different from NULL
 */
bool fl_vector_remove(FlVector vector, size_t pos, FlPointer dest);

/**
 * Remove all elements from the vector
 */
void fl_vector_clear(FlVector vector);

/**
 * Free the memory reserved for vector 
 */
void fl_vector_delete(FlVector vector);

/**
 * Free the memory reserved for {vector} using {delete_handler} for each element.
 * The handler MUST free the memory used by each element
 */
void fl_vector_delete_h(FlVector vector, void (*delete_handler)(FlByte*));

/** 
 * Release the memory used by each element in {vector} and also the memory used
 * by {FlVector} struct
 */
void fl_vector_delete_ptrs(FlVector vector);

#define flm_vector_new(dtype, nelem) fl_vector_new(sizeof(dtype), nelem)
#define flm_vector_get(vector, dtype, index) (*(dtype*)fl_vector_get(vector, index))

/**
 * Add a literal element {elem} of type {type} into {vector}
 *
 * Example: 
 *	flm_vector_addl(v, char, 'c');
 *	flm_vector_addl(v, int, 99);
 */
#define flm_vector_addl(vector, type, elem) \
do {    									\
    type var = elem;						\
    fl_vector_add(vector, &var);            \
} while (0)

/* Datatype specifics getters/setters. (Can be extended by each datatype created in the same way here) */

/**
 * Creates a copy of {src} and adds the new copy to {vector}
 */
void fl_vector_add_cstr(FlVector vector, const FlCstr src);

/**
 * Creates a copy of {src} and insert it in the first position of {vector}
 */
void fl_vector_unshift_cstr(FlVector vector, const FlCstr src);

/* -------------------------------------------------------------
 * {function: fl_vector_start}
 * -------------------------------------------------------------
 * Returns an {FlIterator} that points to the first element in {param: vector}
 * -------------------------------------------------------------
 * {param: const FlVector vector} Target vector to be pointed by the iterator
 * -------------------------------------------------------------
 * {return: FlIterator} Iterator pointing to the first element in {param: vector}
 * -------------------------------------------------------------
 */
FlIterator fl_vector_start(const FlVector vector);

/* -------------------------------------------------------------
 * {function: fl_vector_end}
 * -------------------------------------------------------------
 * Returns an {FlIterator} that points to the past-last-nth element in {param: vector}
 * The FlIterator returned by this function MUST NOT be dereferenced by fl_iterator_value.
 * -------------------------------------------------------------
 * {param: const FlVector vector} Target vector to be pointed by the iterator
 * -------------------------------------------------------------
 * {return: FlIterator} Iterator pointing beyond the last element in {param: vector}
 * -------------------------------------------------------------
 */
FlIterator fl_vector_end(const FlVector vector);

#endif /* FL_VECTOR_H */
