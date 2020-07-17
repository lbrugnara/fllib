#ifndef FL_ARRAY_H
#define FL_ARRAY_H

#include <stdbool.h>

/*
 * Type: FlArray
 *  An FlArray is a pointer to the allocated memory
 *
 */
typedef void FlArray;

/*
 * File: Array
 *  Provides a simple mechanism to allocate arrays on heap, keep track of its size and 
 *  implements some manipulation and searching functions. 
 * 
 * Notes:
 *  All the module's functions ASSUME the array they are manipulating was allocated 
 *  using the functions in this module, unless the function documentation tells otherwise.
 *  The signature of each function clearly states if it specifically expects <FlArray> 
 *  or if it accepts any type of array (including <FlArray>), and based on that it is safe 
 *  to assume that functions that expect an <FlArray> works only with arrays allocated with <fl_array_new> 
 *  and functions that expect a pointer work with both types of arrays.
 *
 */

#include "Types.h"
#include "containers/Vector.h"

#define arrayof(type) (type*)

typedef void(*FlArrayFreeElementFunc)(void*);

/*
 * Macro: flm_array_length
 * ===== C =====
 *  #define flm_array_length(x)  (sizeof(x) / sizeof((x)[0]))
 * =============
 * 
 *  Obtains the length of a C array that is allocated on the stack
 *  *important: This MUST NOT be used with heap allocated arrays,
 *  use <fl_array_new> and <fl_array_length> instead*
 *
 * Parameters:
 *  mixed[] x - Stack allocated array
 *
 */
#define flm_array_length(x)  (sizeof(x) / sizeof((x)[0]))

/*
 * Macro: flm_array_cmp
 * ===== C =====
 *  #define flm_array_cmp memcmp
 * =============
 *
 *  Syntactic sugar for the memcmp function
 *
 */
#define flm_array_cmp memcmp

/*
 * Function: fl_array_new
 *  Allocates space for *n* elements of *size* bytes each and return
 *  a pointer to it.
 *
 * Parameters:
 *  size_t size - Size of the type of the array
 *  size_t n - Number of elements of *size* bytes to allocate
 *
 * Return:
 *  FlArray - Pointer to the allocated memory
 * 
 * Notes:
 *  The pointer returned by this function MUST BE freed with the <fl_array_free> function
 *
 */
FlArray* fl_array_new(size_t size, size_t n);

/*
 * Function: fl_array_free
 *  Releases the memory allocated with the <fl_array_new> function
 *
 * Parameters:
 *  FlArray * const array - A pointer to the memory to be freed
 *
 * Return:
 *  void - This function does not return a value
 *
 */
void fl_array_free(FlArray *array);

/*
 * Function: fl_array_free_each
 *  Releases the memory allocated for an *array* using *item_free_func*
 *  to release the memory used by each element within the array.
 *  It is up to the handler function to free all the memory used by each
 *  element.
 *
 * Parameters:
 *  FlArray * const array - A pointer to the memory to be realeased
 *  FlArrayFreeElementFunc item_free_func - Handler function to release the memory for each element of array
 *
 * Return:
 *  void - This function does not return a value.
 *
 */
void fl_array_free_each(FlArray *array, FlArrayFreeElementFunc item_free_func);

void fl_array_free_each_pointer(FlArray *array, FlArrayFreeElementFunc item_free_func);

/*
 * Function: fl_array_resize
 *  Reallocates memory (previously allocated with <fl_array_new>)
 *  in *array*. The new size will be *n* * *sizeof(type)* where
 *  type is the data type used to allocate the *array*. Similarly
 *  to the <fl_realloc> function, if the reallocation fails, this
 *  function frees the memory used by *array*, that way
 *  the following snippet of code can be safely used (as far as *array*
 *  does not point to a memory section that needs to be freed too):
 * 
 * ===== C =====
 *  array = fl_array_resize(array, size);
 * =============
 *
 * Parameters:
 *  FlArray *array - A pointer to the memory to be reallocated
 *  size_t n - Number of elements to allocate in *array*
 *
 * Return:
 *  FlArray - A pointer to the new allocated memory, or NULL on failure
 *
 */
FlArray* fl_array_resize(FlArray *array, size_t n);

/*
 * Function: fl_array_length
 *  Returns the number of elements the *array* contains
 *
 * Parameters:
 *  FlArray *array - Target array to get its capacity
 *
 * Return:
 *  size_t - Number of elements the array can contain
 *
 */
size_t fl_array_length(const FlArray * const array);

/*
 * Function: fl_array_element_size
 *  Returns the size of the elements the array can hold
 *
 * Parameters:
 *  FlArray *array - Target array
 *
 * Return:
 *  size_t - Size of the elements the array can hold
 *
 */
size_t fl_array_element_size(const FlArray * const array);

/*
 * Function: fl_array_combine
 *  Combines the 2 arrays by chaining the elements from the *src_array*
 *  at the end of the *dest_array*. Similarly
 *  to the <fl_realloc> function, if the resize operation fails, this
 *  function frees the memory used by *dest_array*, that way
 *  the following snippet of code can be safely used (as far as *dest_array*
 *  does not point to a memory section that needs to be freed too):
 * 
 * ===== C =====
 *  dest_array = fl_array_combine(dest_array, src_array);
 * =============
 * 
 * Parameters:
 * FlArray dest_array - Destination array that will contain the new elements
 * FlArray src_array - Source array from where to get the elements
 *
 * Returns:
 *  FlArray - A pointer to the destination array, or NULL on failure
 *
 */
FlArray* fl_array_combine(FlArray *dest_array, const FlArray * const src_array);

/*
 * Function: fl_array_append
 *  Appends an element to the end of the *array* making room for it. This
 *  function returns a pointer to the new location of the array. Similarly
 *  to the <fl_realloc> function, if the resize operation fails, this
 *  function frees the memory used by *array*, that way
 *  the following snippet of code can be safely used (as far as *array*
 *  does not point to a memory section that needs to be freed too):
 * 
 * ===== C =====
 *  array = fl_array_append(array, element);
 * =============
 * 
 * Parameters:
 * FlArray *array - Destination array that will contain the new element
 * const void *element - New element to append to the end of the *array*
 *
 * Returns:
 *  FlArray - A pointer to the destination array, or NULL on failure
 *
 */
FlArray* fl_array_append(FlArray *array, const void *element);

/*
 * Function: fl_array_contains
 *  Search for *needle* in *array*. This function MUST be used
 *  only with arrays allocated with <fl_array_new>.
 *
 * Parameters:
 *  FlArray * const array - Target array to look for *needle*
 *  const void *needle - Pointer to the memory to search in *array*
 *
 * Return:
 *  bool - *true* if *needle* is present in the *array*
 *
 */
bool fl_array_contains(const FlArray * const array, const void *needle);

/*
 * Function: fl_array_contains_n
 *  Search for *needle* (of *needlesize* bytes) in the *nelems*
 *  elements of *array*.
 *
 * Parameters>
 *  const void *array - Target array to look for *needle*
 *  size_t nelems - Number of elements of *array*
 *  const void *needle - Pointer to the memory to search in *array*
 *  size_t needlesize - Size of *needle*
 *
 * Return:
 *  bool - *true* if needle is present in the *array*
 * 
 * Notes:
 *  This function does not necessarily expect array to be an array allocated
 *  with <fl_array_new>, it can be safely used with stack or heap allocated arrays,
 *  including <FlArray>s.
 *
 */
bool fl_array_contains_n(const void *array, size_t nelems, const void *needle, size_t needlesize);

/*
 * Function: fl_array_to_vector
 *  Creates an <FlVector> using as the source of elements the provided *array*
 *
 * Parameters:
 *  FlArray * const array - Array of elements
 *
 * Return:
 *  FlVector* - Pointer to a new vector with the array's elements on it
 *
 */
struct FlVector* fl_array_to_vector(const FlArray * const array);

#endif /* FL_ARRAY_H */
