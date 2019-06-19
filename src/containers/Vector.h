#ifndef FL_VECTOR_H
#define FL_VECTOR_H

#include "../Types.h"
#include "Iterator.h"

typedef struct FlVector* FlVector;

#define FlVectorOf(type)    FlVector

/*
 * Function: fl_vector_new
 *
 * Creates an FlVector with capacity for {nelem} of size {dtsize}
 *
 * size_t dtsize - Size of the data type to be stored
 * size_t nelem - Initial capacity
 *
 * {return: FlVector} Handle to the created vector
 *
 */
FlVector fl_vector_new(size_t dtsize, size_t nelem);

/*
 * Function: fl_vector_length
 *
 * Returns the number of elements stored in the vector {vector}
 *
 * FlVector vector - Vector handle
 *
 * {return: size_t} Number of elements currently stored in {vector}
 *
 */
size_t fl_vector_length(FlVector vector);

/*
 * Function: fl_vector_capacity
 *
 * Returns the current max capacity of the vector
 *
 * FlVector vector - Vector handle
 *
 * {return: size_t} Current capacity of {vector}
 *
 */
size_t fl_vector_capacity(FlVector vector);

/*
 * Function: fl_vector_dtsize
 *
 * Returns the size of the data type stored in {vector}
 *
 * FlVector vector - Vector handle
 *
 * {return: size_t} Vector's data type size
 *
 */
size_t fl_vector_dtsize(FlVector vector);

/*
 * Function: fl_vector_add
 *
 * Adds {elem} into {vector}. It n-bytes of elem into vector,
 * n-bytes is the amount of bytes in the data type size stored into
 * vector
 *
 * FlVector vector} Vector to store {elem -
 * const void *elem} Target {elem} to be added into {vector -
 *
 * {return: void*} Pointer to the added element
 *
 */
void* fl_vector_add(FlVector vector, const void *elem);

/*
 * Function: fl_vector_insert
 *
 * Inserts {elem} in the position {pos} of {vector}
 *
 * FlVector vector} Target vector to insert {elem -
 * const void *elem} Element to be inserted into {vector -
 * size_t pos - Position to inser the new element
 *
 * {return: void*} Pointer to the inserted element
 *
 */
void* fl_vector_insert(FlVector vector, const void *elem, size_t pos);

/*
 * Function: fl_vector_resize
 *
 * Resizes the {vector} so that it contains {nelem} elements.
 * If the new capacity is smaller than the current length, the elements
 * beyond {nelem} position will be lost.
 *
 * FlVector vector - Target vector to be resized
 * size_t nelem - New capacity
 *
 * {return: void}
 *
 */
void fl_vector_resize(FlVector vector, size_t nelem);

/**
 * Returns a pointer to the element in the {index} index of {vector}
 */
/*
 * Function: fl_vector_get
 *
 * Returns a pointer to the element in the {index} index of {vector}
 *
 * FlVector vector - Target vector
 * size_t index - Index of the element to be retrieved
 *
 * {return: void*} Pointer to the element stored at index {index}
 *
 */
void* fl_vector_get(FlVector vector, size_t index);

/*
 * Function: fl_vector_first
 *
 * Returns a pointer to the first element in the {vector}
 *
 * FlVector vector - Target vector
 *
 * {return: void*} Pointer to the first element stored in the vector.
 * If vector is empty, this function returns NULL
 *
 */
void *fl_vector_first(FlVector vector);

/*
 * Function: fl_vector_last
 *
 * Returns a pointer to the last element in the {vector}
 *
 * FlVector vector - Target vector
 *
 * {return: void*} Pointer to the last element stored in the vector.
 * If vector is empty, this function returns NULL
 *
 */
void *fl_vector_last(FlVector vector);

/*
 * Function: fl_vector_shift
 *
 * Removes the first element in {vector}. If {dest} is not null,
 * the element to be removed will be copied into it.
 *
 * FlVector vector - Vector to remove its first element
 * void *dest - Destination to copy the element to be removed
 *
 * {return: bool} If vector length is 0, this function returns 0,
 * if not true
 *
 */
bool fl_vector_shift(FlVector vector, void *dest);

/**
 * Insert {elem} in the first position of {vector}
 */
/*
 * Function: fl_vector_unshift
 *
 * Inserts {elem} in the first position of {vector}
 *
 * FlVector vector} Vector where to insert the {elem -
 * const void *} Element to be inserted into {vector -
 *
 * {return: void*} Pointer to the inserted element
 *
 */
void* fl_vector_unshift(FlVector vector, const void *elem);

/**
 * Remove from {vector}, the last element copying it into {dest}
 */
/*
 * Function: fl_vector_pop
 *
 * Removes the last element in {vector}. If {dest} is not null,
 * the element to be removed will be copied into it.
 *
 * FlVector vector - Vector to remove its last element
 * void *dest - If present, the removed element will be copied into it
 *
 * {return: bool} false if vector length is 0, true otherwise
 *
 */
bool fl_vector_pop(FlVector vector, void *dest);

/**
 * Return true if {vector} contains {elem}
 */
/*
 * Function: fl_vector_contains
 *
 * Returns true of {vector} contains {elem}.
 *
 * FlVector vector} Vector where to search {elem -
 * const void *elem - Element to search in vector
 *
 * {return: bool} true if {elem} is present in {vector}
 *
 */
bool fl_vector_contains(FlVector vector, const void *elem);

/*
 * Function: fl_vector_concat
 *
 * Appends {vector2} elements at the end of {vector}
 *
 * FlVector vector} Vector where to insert the elements of {vector2 -
 * FlVector vector2} Vector where to retrieve its elements to be inserted into {vector -
 *
 * {return: void}
 *
 */
void fl_vector_concat(FlVector vector, FlVector vector2);

/*
 * Function: fl_vector_concat
 *
 * Creates a new vector with the elements of {vector} and {vector2}
 *
 * FlVector vector - First vector to be copied in the new vector
 * FlVector vector2} Second vector to be copied (after {vector -) in the new vector
 *
 * {return: FlVector} New vector that contains {vector}'s elements and {vector2}'s elements
 *
 */
FlVector fl_vector_merge(FlVector vector, FlVector vector2);

/**
 * Removes the {pos}-th element in {vector}.
 * The removed element will be copied in {dest} if it's different from NULL
 */
/*
 * Function: fl_vector_remove
 *
 * Removes the element at position {pos}, and if {dest} is not null,
 * the target element will be copied into it.
 *
 * FlVector vector} Vector where to remove element at position {pos -
 * size_t pos - Index of the element to be removed
 * void *dest - Element to be removed will be copied to it if it is not null
 *
 * {return: bool} If {pos} is greater than the current {vector}'s length,
 * this function returns false. Otherwise it returns true.
 *
 */
bool fl_vector_remove(FlVector vector, size_t pos, void *dest);

/**
 * Remove all elements from the vector
 */
/*
 * Function: fl_vector_clear
 *
 * Removes all the elements from {vector}
 *
 * FlVector vector - Vector to be cleared
 *
 * {return: void}
 *
 */
void fl_vector_clear(FlVector vector);

/**
 * Free the memory reserved for vector 
 */
/*
 * Function: fl_vector_delete
 *
 * Deletes {vector} releaseing its memory
 *
 * FlVector vector - Vector to be deleted
 *
 * {return: void}
 *
 */
void fl_vector_delete(FlVector vector);

/**
 * Free the memory reserved for {vector} using {delete_handler} for each element.
 * The handler MUST free the memory used by each element
 */
/*
 * Function: fl_vector_delete_h
 *
 * Deletes the memory of {vector} calling {delete_handler} for
 * each element in the vector. {delete_handler} MUST free the memory
 * used by each element.
 *
 * FlVector vector - Vector to be deleted
 * void (*)(FlByte*) delete_handler - Function to be called for each element to release the elemen's memory
 *
 * {return: void}
 *
 */
void fl_vector_delete_h(FlVector vector, void (*delete_handler)(FlByte*));

/*
 * Function: fl_vector_delete_ptrs
 *
 * This function will delete {vector} and call free for each element
 * in vector. It is intended to be used for vectors of pointers, similar
 * to use fl_vector_delete_h with a function that calls free on each element
 *
 * FlVector vector - Vector to delete releasing its memory and the memory of the elements
 * it contains
 *
 * {return: void}
 *
 */
void fl_vector_delete_ptrs(FlVector vector);

void* fl_vector_to_array(FlVector vector);

/*
 * Macro: flm_vector_get
 *
 * Handy macro to retrieve the value at position {index} from {vector}
 *
 * FlVector vector - Target vector
 * size_t index - Index of the element to be retrieved
 *
 * {return: void*} Element stored at index {index}
 *
 */
#define flm_vector_get(vector, dtype, index) (*(dtype*)fl_vector_get(vector, index))

/*
 * Macro: flm_vector_addl
 *
 * Add a literal value ({elem}) of type {type} into {vector}
 *   Example: 
 *	    flm_vector_addl(v, char, 'c');
 *	    flm_vector_addl(v, int, 99);
 *
 * FlVector vector - Vector where to add the literal value
 * type} Type of {elem -
 * type elem} Literal value to be added to {vector -
 *
 */
#define flm_vector_addl(vector, type, elem) \
do {    									\
    type var = elem;						\
    fl_vector_add(vector, &var);            \
} while (0)

/* Datatype specifics getters/setters. (Can be extended by each datatype created in the same way here) */

/*
 * Function: fl_vector_begin
 *
 * Returns an {FlIterator} that points to the first element in vector -
 *
 * const FlVector vector - Vector handle to be pointed by the iterator
 *
 * {return: FlIterator} Iterator pointing to the first element in vector -
 *
 */
FlIterator fl_vector_begin(const FlVector vector);

/*
 * Function: fl_vector_end
 *
 * Returns an {FlIterator} that points to the past-last-nth element in vector -
 * The FlIterator returned by this function MUST NOT be dereferenced by fl_iterator_value.
 *
 * const FlVector vector - Vector handle to be pointed by the iterator
 *
 * {return: FlIterator} Iterator pointing beyond the last element in vector -
 *
 */
FlIterator fl_vector_end(const FlVector vector);

#endif /* FL_VECTOR_H */
