#ifndef FL_ARRAY_H
#define FL_ARRAY_H

/* =============================================================
 * {module: Array}
 * =============================================================
 * Provides a simple mechanism to allocate arrays on heap, keep 
 * track of its size and implements some manipulation and 
 * searching functions.
 * All the module's functions ASSUME the array they are manipulating
 * was allocated using this module, Unless the function docs tells
 * otherwise.
 * -------------------------------------------------------------
 */

#include "Types.h"
#include "containers/Vector.h"

/* -------------------------------------------------------------
 * {datatype: void* FlGenericArray}
 * -------------------------------------------------------------
 * Typedef used to give a more significant intention when a 
 * function's return type is intended to be an array of any type
 * -------------------------------------------------------------
 */
typedef void* FlGenericArray;

/* -------------------------------------------------------------
 * {macro: flm_array_length}
 * -------------------------------------------------------------
 * Obtains the length of a C array that is allocated on the stack
 * {important: This MUST NOT be used with heap allocated arrays,
 * use {fl_array_new} and {fl_array_length} instead}
 * -------------------------------------------------------------
 * {param: mixed[] x} Stack allocated array
 * -------------------------------------------------------------
 */
#define flm_array_length(x)  (sizeof(x) / sizeof((x)[0]))

/* -------------------------------------------------------------
 * {macro: flm_array_cmp}
 * -------------------------------------------------------------
 * Syntactic sugar for memcmp
 * -------------------------------------------------------------
 */
#define flm_array_cmp memcmp

/**
 * Creates a new array of {n} elements of {size} bytes each.
 * Returns a pointer to the allocated memory
 */
FlGenericArray fl_array_new(size_t size, size_t n);

/**
 * Release the memory allocated by {fl_array_new}
 */
void fl_array_delete(FlGenericArray array);

/**
 * Free the memory reserved for {array} using {delete_handler} for each element.
 * The handler MUST free the memory used by each element
 */
void fl_array_delete_h(FlGenericArray array, void (*delete_handler)(FlByte*));

/**
 * Realloc the memory allocated with {fl_array_new} to {n} elements (the size is the one specified in the new function)
 */
FlGenericArray fl_array_resize(FlGenericArray array, size_t n);

/**
 * Returns the length of {array}
 */
size_t fl_array_length(const FlGenericArray array);

/**
 * Search for {needle} in {array}.
 * This function SHOULD be used only with arrays allocated using {fl_array_new} (this function
 * takes the number of elements and the size of each element from the FlArrayHeader struct
 * that is created on {fl_array_new})
 */
bool fl_array_contains(const FlGenericArray array, const FlPointer needle);

/**
 * Search for {needle} in {array}. 
 * {nelems} is the number of elements of {array}
 * {needlesize} is the number of bytes to use in compare.
 * This could be use with stack allocated arrays and heap allocated arrays.
 */
bool fl_array_contains_n(const FlGenericArray array, size_t nelems, const FlPointer needle, size_t needlesize);

/* -------------------------------------------------------------
 * {function: fl_array_to_vector}
 * -------------------------------------------------------------
 * Creates an {FlVector} using as source of elements {array}
 * -------------------------------------------------------------
 * {param: const FlGenericArray array} Array of elements
 * -------------------------------------------------------------
 * {return: FlVector*} Pointer to a new vector with the array 
 * elements on it
 * -------------------------------------------------------------
 */
FlVector* fl_array_to_vector(const FlGenericArray array);

#endif /* FL_ARRAY_H */
