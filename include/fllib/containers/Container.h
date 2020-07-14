#ifndef FL_CONTAINER_H
#define FL_CONTAINER_H

#include <stdbool.h>
#include "../Types.h"

/*
 * Type: void(*FlContainerCleanupFn)(void *key)
 *
 * This module provides the fl_container_cleaner_pointer function
 * that simply frees the value pointer by {obj}.
 *
 */
typedef void(*FlContainerCleanupFn)(void *obj);

/*
 * Function: fl_container_cleaner_pointer
 *
 * Frees the memory allocated in the region obj is pointing to
 *
 * void* obj - Object to free its memory
 *
 * {return: void}
 *
 */
void fl_container_cleaner_pointer(void *obj);

/* -------------------------------------------------------------
* {datatype: void(*FlContainerAllocatorFn)(FlByte **dest, const FlByte *src)}
* -------------------------------------------------------------
* Functions that allocates and writes the sequence of bytes in {src} to the 
* destination object {dest}. This function MUST allocate the 
* memory before writing to the object.
* By default containers work with pointers.
* In order to use the actual value behind the pointers, the caller
* must provide an allocator function to know how to save/restore the
* value from within the container.
* This module provides a family of allocator functions with the name structure
* fl_container_allocator_*. These functions are:
*   string
*   int
*   char
*   sizet
* -------------------------------------------------------------
*/
typedef void(*FlContainerAllocatorFn)(FlByte **dest, const FlByte *src);

/* -------------------------------------------------------------
* {function: fl_container_allocator_string}
* -------------------------------------------------------------
* Copies the char* pointed by {src} into {dest} by allocating
* space to make {src} and the NULL terminator fit in {dest}
* -------------------------------------------------------------
* {param: FlByte** dest} Destination object. It is allocated by this function
* {param: const FlByte* src} Sequence of bytes containing a NULL terminated string
* -------------------------------------------------------------
* {return: void}
* -------------------------------------------------------------
*/
void fl_container_allocator_string(FlByte **dest, const FlByte *src);

/* -------------------------------------------------------------
* {function: fl_container_allocator_int}
* -------------------------------------------------------------
* Copies the integer pointed by {src} into {dest} by allocating
* space to fit an int.
* -------------------------------------------------------------
* {param: FlByte** dest} Destination object. It is allocated by this function
* {param: const FlByte* src} Pointer to an integer
* -------------------------------------------------------------
* {return: void}
* -------------------------------------------------------------
*/
void fl_container_allocator_int(FlByte **dest, const FlByte *src);

/* -------------------------------------------------------------
* {function: fl_container_allocator_char}
* -------------------------------------------------------------
* Copies the char pointed by {src} into {dest} by allocating
* space to fit a char.
* -------------------------------------------------------------
* {param: FlByte** dest} Destination object. It is allocated by this function
* {param: const FlByte* src} Pointer to a char
* -------------------------------------------------------------
* {return: void}
* -------------------------------------------------------------
*/
void fl_container_allocator_char(FlByte **dest, const FlByte *src);

/* -------------------------------------------------------------
* {function: fl_container_allocator_sizet}
* -------------------------------------------------------------
* Copies the size_t pointed by {src} into {dest} by allocating
* space to fit a size_t object.
* -------------------------------------------------------------
* {param: FlByte** dest} Destination object. It is allocated by this function
* {param: const FlByte* src} Pointer to a size_t object
* -------------------------------------------------------------
* {return: void}
* -------------------------------------------------------------
*/
void fl_container_allocator_sizet(FlByte **dest, const FlByte *src);

/* -------------------------------------------------------------
* {datatype: void(*FlContainerWriterFn)(FlByte **dest, const FlByte *src)}
* -------------------------------------------------------------
* Functions that writes the sequence of bytes in {src} to the 
* destination object {dest}. This function MUST NOT allocate dest's memory.
* By default containers work with pointers.
* In order to use the actual value behind the pointers, the caller
* must provide a writer function to know how to save the value within the container.
* This module provides a family of writer functions with the name structure
* fl_container_allocator_*. These functions are:
*   string
*   int
*   char
*   sizet
* -------------------------------------------------------------
*/
typedef void(*FlContainerWriterFn)(FlByte *dest, const FlByte *src, size_t size);

void fl_container_writer(FlByte *dest, const FlByte *src, size_t size);

/*
 * Type: bool(*FlContainerEqualsFn)(const FlByte *val1, const FlByte *val2)
 *
 * Function that compares two sequence of bytes.
 * This module provides a family of equality comparer functions with the name structure
 * fl_container_equals_*. These functions are:
 *   pointer
 *   string
 *   int
 *   char
 *   sizet
 *
 */
typedef bool(*FlContainerEqualsFn)(const FlByte *val1, const FlByte *val2);

/*
 * Function: fl_container_equals_pointer
 *
 * Compares the two keys by just applying the equality operator
 *
 * const FlByte* val1 - One of the keys to be compared
 * const FlByte* val2 - Another key to be compared
 *
 * {return: bool} True if the keys are equals
 *
 */
bool fl_container_equals_pointer(const FlByte *val1, const FlByte *val2);

/*
 * Function: fl_container_equals_pointer
 *
 * Compares the two keys by comparing the string they are pointing
 * to using the strcmp function. The strings must be null terminated.
 *
 * const FlByte* val1 - One of the keys to be compared
 * const FlByte* val2 - Another key to be compared
 *
 * {return: bool} True if the keys are equals
 *
 */
bool fl_container_equals_string(const FlByte *val1, const FlByte *val2);

/*
 * Function: fl_container_equals_int
 *
 * Compares the two keys by just applying the equality operator
 * on the dereferenced integers
 *
 * const FlByte* val1 - One of the keys to be compared
 * const FlByte* val2 - Another key to be compared
 *
 * {return: bool} True if the keys are equals
 *
 */
bool fl_container_equals_int(const FlByte *val1, const FlByte *val2);

/*
 * Function: fl_container_equals_char
 *
 * Compares the two keys by just applying the equality operator
 * on the dereferenced chars
 *
 * const FlByte* val1 - One of the keys to be compared
 * const FlByte* val2 - Another key to be compared
 *
 * {return: bool} True if the keys are equals
 *
 */
bool fl_container_equals_char(const FlByte *val1, const FlByte *val2);

/*
 * Function: fl_container_equals_size_t
 *
 * Compares the two keys by just applying the equality operator
 * on the dereferenced size_t objects
 *
 * const FlByte* val1 - One of the keys to be compared
 * const FlByte* val2 - Another key to be compared
 *
 * {return: bool} True if the keys are equals
 *
 */
bool fl_container_equals_sizet(const FlByte *val1, const FlByte *val2);

/*
 * Type: FlContainerCompareFn
 *  A function that compares two objects and returns an integer equals to 0 if both elements
 *  are equals, a negative integer if *obj1* is lesser than *obj2* or a positive integer if *obj1* is
 *  greater than *obj2*.
 * 
 * Parameters:
 *  <const FlByte> *obj1: First object to compare
 *  <const FlByte> *obj2: Second object to compare
 * 
 * Return:
 * <int>: A negative integer if *obj1* is lesser than *obj2*, a positive integer if *obj1* is greather
 *        than *obj2* or 0 if both objects are equals.
 */
typedef int(*FlContainerCompareFn)(const FlByte *obj1, const FlByte *obj2);

/*
 * Function: fl_container_compare_pointer
 *  A function that compares two pointers and returns 0 if both pointers are equals, a negative number if *obj1* 
 *  is lesser than *obj2* or a positive number if *obj1* is greater than *obj2*.
 * 
 * Parameters:
 *  <const FlByte> *obj1: First pointer to compare
 *  <const FlByte> *obj2: Second pointer to compare
 * 
 * Return:
 * <int>: A negative integer if *obj1* is lesser than *obj2*, a positive integer if *obj1* is greather
 *        than *obj2* or 0 if both pointers are equals.
 */
int fl_container_compare_pointer(const FlByte *obj1, const FlByte *obj2);

/*
 * Function: fl_container_compare_string
 *  A function that compares two strings and returns 0 if both strings are equals, a negative number if *obj1* 
 *  is lesser than *obj2* or a positive number if *obj1* is greater than *obj2*.
 * 
 * Parameters:
 *  <const FlByte> *obj1: First string to compare
 *  <const FlByte> *obj2: Second string to compare
 * 
 * Return:
 * <int>: A negative integer if *obj1* is lesser than *obj2*, a positive integer if *obj1* is greather
 *        than *obj2* or 0 if both strings are equals.
 */
int fl_container_compare_string(const FlByte *obj1, const FlByte *obj2);

/*
 * Function: fl_container_compare_int
 *  A function that compares two integers and returns 0 if both integers are equals, a negative number if *obj1* 
 *  is lesser than *obj2* or a positive number if *obj1* is greater than *obj2*.
 * 
 * Parameters:
 *  <const FlByte> *obj1: First integer to compare
 *  <const FlByte> *obj2: Second integer to compare
 * 
 * Return:
 * <int>: A negative integer if *obj1* is lesser than *obj2*, a positive integer if *obj1* is greather
 *        than *obj2* or 0 if both integers are equals.
 */
int fl_container_compare_int(const FlByte *obj1, const FlByte *obj2);

/*
 * Function: fl_container_compare_char
 *  A function that compares two chars and returns 0 if both chars are equals, a negative number if *obj1* 
 *  is lesser than *obj2* or a positive number if *obj1* is greater than *obj2*.
 * 
 * Parameters:
 *  <const FlByte> *obj1: First char to compare
 *  <const FlByte> *obj2: Second char to compare
 * 
 * Return:
 * <int>: A negative integer if *obj1* is lesser than *obj2*, a positive integer if *obj1* is greather
 *        than *obj2* or 0 if both chars are equals.
 */
int fl_container_compare_char(const FlByte *obj1, const FlByte *obj2);

/*
 * Function: fl_container_compare_sizet
 *  A function that compares two size_t objects and returns 0 if both size_t objects are equals, a negative number if *obj1* 
 *  is lesser than *obj2* or a positive number if *obj1* is greater than *obj2*.
 * 
 * Parameters:
 *  <const FlByte> *obj1: First size_t object to compare
 *  <const FlByte> *obj2: Second size_t object to compare
 * 
 * Return:
 * <int>: A negative integer if *obj1* is lesser than *obj2*, a positive integer if *obj1* is greather
 *        than *obj2* or 0 if both size_t objects are equals.
 */
int fl_container_compare_sizet(const FlByte *obj1, const FlByte *obj2);

#endif // FL_CONTAINER_H
