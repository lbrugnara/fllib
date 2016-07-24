#ifndef FL_ARRAY_H
#define FL_ARRAY_H

#include "Types.h"
#include "containers/Vector.h"

/**
 * Obtains the length of a C array that is allocated on the stack
 * NOTE: This MUST NOT be used with heap allocated arrays, use
 * fl_array_new and fl_array_length instead.
 */
#define flm_array_length(x)  (sizeof(x) / sizeof((x)[0]))
#define flm_array_cmp memcmp

/**
 * Creates a new array of {n} elements of {size} bytes each.
 * Returns a pointer to the allocated memory
 */
FlPointer fl_array_new(size_t size, size_t n);

/**
 * Release the memory allocated by {fl_array_new}
 */
void fl_array_delete(FlPointer array);

/**
 * Free the memory reserved for {array} using {delete_handler} for each element.
 * The handler MUST free the memory used by each element
 */
void fl_array_delete_h(FlPointer array, void (*delete_handler)(FlByte*));

/**
 * Realloc the memory allocated with {fl_array_new} to {n} elements (the size is the one specified in the new function)
 */
FlPointer fl_array_resize(FlPointer array, size_t n);

/**
 * Returns the length of {array}
 */
size_t fl_array_length(const FlPointer array);

/**
 * Search for {needle} in {array}.
 * This function SHOULD be used only with arrays allocated using {fl_array_new} (this function
 * takes the number of elements and the size of each element from the FlArrayHeader struct
 * that is created on {fl_array_new})
 */
bool fl_array_contains(const FlPointer array, const FlPointer needle);

/**
 * Search for {needle} in {array}. 
 * {nelems} is the number of elements of {array}
 * {needlesize} is the number of bytes to use in compare.
 * This could be use with stack allocated arrays and heap allocated arrays.
 */
bool fl_array_contains_n(const FlPointer array, size_t nelems, const FlPointer needle, size_t needlesize);


FlVector* fl_array_to_vector(const FlPointer array);

#endif /* FL_ARRAY_H */
