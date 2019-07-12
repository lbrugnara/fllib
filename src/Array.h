#ifndef FL_ARRAY_H
#define FL_ARRAY_H

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

/*
 * Type: FlArray
 *  An FlArray is a pointer to the allocated memory
 *
 */
typedef void *FlArray;

#define arrayof(type) (type*)

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
 *  The pointer returned by this function MUST BE freed with the <fl_array_delete> function
 *
 */
FlArray fl_array_new(size_t size, size_t n);

/*
 * Function: fl_array_delete
 *  Releases the memory allocated with the <fl_array_new> function
 *
 * Parameters:
 *  const FlArray array - A pointer to the memory to be freed
 *
 * Return:
 *  void - This function does not return a value
 *
 */
void fl_array_delete(const FlArray array);

/*
 * Function: fl_array_delete_each
 *  Releases the memory allocated for an *array* using *delete_handler*
 *  to release the memory used by each element within the array.
 *  It is up to the handler function to free all the memory used by each
 *  element.
 *
 * Parameters:
 *  const FlArray array - A pointer to the memory to be realeased
 *  void (*)(FlByte*) delete_handler - Handler function to release the memory for each element of array
 *
 * Return:
 *  void - This function does not return a value.
 *
 */
void fl_array_delete_each(const FlArray array, void (*delete_handler)(FlByte*));

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
 *  FlArray array - A pointer to the memory to be reallocated
 *  size_t n - Number of elements to allocate in *array*
 *
 * Return:
 *  FlArray - A pointer to the new allocated memory, or NULL on failure
 *
 */
FlArray fl_array_resize(FlArray array, size_t n);

/*
 * Function: fl_array_length
 *  Returns the number of elements the *array* contains
 *
 * Parameters:
 *  FlArray array - Target array to get its capacity
 *
 * Return:
 *  size_t - Number of elements the array can contain
 *
 */
size_t fl_array_length(const FlArray array);

/*
 * Function: fl_array_contains
 *  Search for *needle* in *array*. This function MUST be used
 *  only with arrays allocated with <fl_array_new>.
 *
 * Parameters:
 *  const FlArray array - Target array to look for *needle*
 *  const void *needle - Pointer to the memory to search in *array*
 *
 * Return:
 *  bool - *true* if *needle* is present in the *array*
 *
 */
bool fl_array_contains(const FlArray array, const void *needle);

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
 *  const FlArray array - Array of elements
 *
 * Return:
 *  FlVector - Pointer to a new vector with the array's elements on it
 *
 */
FlVector fl_array_to_vector(const FlArray array);

#endif /* FL_ARRAY_H */
