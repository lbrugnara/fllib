#ifndef FL_ARRAY_H
#define FL_ARRAY_H

/* =============================================================
 * {module: Array}
 * =============================================================
 * Provides a simple mechanism to allocate arrays on heap, keep 
 * track of its size and implements some manipulation and 
 * searching functions.
 * All the module's functions ASSUME the array they are manipulating
 * was allocated using this module, unless the function docs tells
 * otherwise.
 * -------------------------------------------------------------
 */

#include "Types.h"
#include "containers/Vector.h"

#define FlArray void*
#define arrayof(type) (type*)

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

/* -------------------------------------------------------------
 * {function: fl_array_new}
 * -------------------------------------------------------------
 * Allocate space for {n} elements of {size} bytes each
 * -------------------------------------------------------------
 * {param: size_t size} Size of the type of the array
 * {param: size_t n} Number of elements of {size} bytes to allocate
 * -------------------------------------------------------------
 * {return: void*} Pointer to the allocated memory
 * -------------------------------------------------------------
 */
void* fl_array_new(size_t size, size_t n);

/* -------------------------------------------------------------
 * {function: fl_array_delete}
 * -------------------------------------------------------------
 * Releases the memory allocated with {fl_array_new}
 * -------------------------------------------------------------
 * {param: void *array} A pointer to the memory to be released
 * -------------------------------------------------------------
 * {return: void}
 * -------------------------------------------------------------
 */
void fl_array_delete(void *array);

/* -------------------------------------------------------------
 * {function: fl_array_delete_h}
 * -------------------------------------------------------------
 * Releases the memory allocated for {array} using {delete_handler}
 * to release the memory used by each element.
 * The handler function MUST free all the memory used by each
 * element.
 * -------------------------------------------------------------
 * {param: void *array} A pointer to the memory to be realeased
 * {param: void (*)(FlByte*) delete_handler} Handler function to release the memory for each element of array
 * -------------------------------------------------------------
 * {return: void}
 * -------------------------------------------------------------
 */
void fl_array_delete_h(void *array, void (*delete_handler)(FlByte*));

/* -------------------------------------------------------------
 * {function: fl_array_resize}
 * -------------------------------------------------------------
 * Reallocate memory (previously allocated with {fl_array_new})
 * in {array}. The new size will be {n} * sizeof(type) where
 * type is the data type used to allocate the {array}
 * -------------------------------------------------------------
 * {param: void *array} A pointer to the memory to be reallocated
 * {param: size_t n} Number of elements to allocate in {array}
 * -------------------------------------------------------------
 * {return: void*} A pointer to the new allocated memory
 * -------------------------------------------------------------
 */
void* fl_array_resize(void *array, size_t n);

/* -------------------------------------------------------------
 * {function: fl_array_length}
 * -------------------------------------------------------------
 * Returns the number of elements allocated in array
 * -------------------------------------------------------------
 * {param: void *array} Target array
 * -------------------------------------------------------------
 * {return: size_t} Number of elements array can contain
 * -------------------------------------------------------------
 */
size_t fl_array_length(const void *array);

/* -------------------------------------------------------------
 * {function: fl_array_contains}
 * -------------------------------------------------------------
 * Search for {needle} in {array}. This function MUST be used
 * only with arrays allocated with {fl_array_new}.
 * -------------------------------------------------------------
 * {param: const void *array} Target array to look for {needle}
 * {param: const void *needle} Pointer to the memory to search in {array}
 * -------------------------------------------------------------
 * {return: bool} True if needle is in array
 * -------------------------------------------------------------
 */
bool fl_array_contains(const void *array, const void *needle);

/* -------------------------------------------------------------
 * {function: fl_array_contains_n}
 * -------------------------------------------------------------
 * Search for {needle} (of {needlesize} bytes) in the {nelems}
 * elements of {array}.
 * -------------------------------------------------------------
 * {param: const void *array} Target array to look for {needle}
 * {param: size_t nelems} Number of elements of {array}
 * {param: const void *needle} Pointer to the memory to search in {array}
 * {param: size_t needlesize} Size of {needle}
 * -------------------------------------------------------------
 * {return: bool} True if needle is in array
 * -------------------------------------------------------------
 */
bool fl_array_contains_n(const void *array, size_t nelems, const void *needle, size_t needlesize);

/* -------------------------------------------------------------
 * {function: fl_array_to_vector}
 * -------------------------------------------------------------
 * Creates an {FlVector} using as source of elements {array}
 * -------------------------------------------------------------
 * {param: const void *array} Array of elements
 * -------------------------------------------------------------
 * {return: FlVector`} Pointer to a new vector with the array 
 * elements on it
 * -------------------------------------------------------------
 */
FlVector fl_array_to_vector(const void *array);

#endif /* FL_ARRAY_H */
