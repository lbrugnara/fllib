#ifndef FL_CONTAINER_H
#define FL_CONTAINER_H

#include "../Types.h"

/* -------------------------------------------------------------
* {datatype: void(*FlContainerCleanupFunc)(void *key)}
* -------------------------------------------------------------
* This module provides the fl_container_cleaner_pointer function
* that simply frees the value pointer by {obj}.
* -------------------------------------------------------------
*/
typedef void(*FlContainerCleanupFunc)(void *obj);

/* -------------------------------------------------------------
* {function: fl_container_cleaner_pointer}
* -------------------------------------------------------------
* Frees the memory allocated in the region obj is pointing to
* -------------------------------------------------------------
* {param: void* obj} Object to free its memory
* -------------------------------------------------------------
* {return: void}
* -------------------------------------------------------------
*/
void fl_container_cleaner_pointer(void *obj);

/* -------------------------------------------------------------
* {datatype: void(*FlContainerWriterFunc)(FlByte **dest, const FlByte *src)}
* -------------------------------------------------------------
* Functions that writes the sequence of bytes in {src} to the 
* destination object {dest}. This function MUST allocate the 
* memory before writing to the object.
* By default containers work with pointers.
* In order to use the actual value behind the pointers, the caller
* must provide a writer function to know how to save/restore the
* value from within the container.
* This module provides a family of writers functions with the name structure
* fl_container_writer_*. These functions are:
*   string
*   int
*   char
*   sizet
* -------------------------------------------------------------
*/
typedef void(*FlContainerWriterFunc)(FlByte **dest, const FlByte *src);

/* -------------------------------------------------------------
* {function: fl_container_writer_string}
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
void fl_container_writer_string(FlByte **dest, const FlByte *src);

/* -------------------------------------------------------------
* {function: fl_container_writer_int}
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
void fl_container_writer_int(FlByte **dest, const FlByte *src);

/* -------------------------------------------------------------
* {function: fl_container_writer_char}
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
void fl_container_writer_char(FlByte **dest, const FlByte *src);

/* -------------------------------------------------------------
* {function: fl_container_writer_sizet}
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
void fl_container_writer_sizet(FlByte **dest, const FlByte *src);

/* -------------------------------------------------------------
* {datatype: bool(*FlContainerEqualsFunc)(const FlByte *val1, const FlByte *val2)}
* -------------------------------------------------------------
* Function that compares two sequence of bytes.
* This module provides a family of equality comparer functions with the name structure
* fl_container_equals_*. These functions are:
*   pointer
*   string
*   int
*   char
*   sizet
* -------------------------------------------------------------
*/
typedef bool(*FlContainerEqualsFunc)(const FlByte *val1, const FlByte *val2);

/* -------------------------------------------------------------
* {function: fl_container_equals_pointer}
* -------------------------------------------------------------
* Compares the two keys by just applying the equality operator
* -------------------------------------------------------------
* {param: const FlByte* val1} One of the keys to be compared
* {param: const FlByte* val2} Another key to be compared
* -------------------------------------------------------------
* {return: bool} True if the keys are equals
* -------------------------------------------------------------
*/
bool fl_container_equals_pointer(const FlByte *val1, const FlByte *val2);

/* -------------------------------------------------------------
* {function: fl_container_equals_pointer}
* -------------------------------------------------------------
* Compares the two keys by comparing the string they are pointing
* to using the strcmp function. The strings must be null terminated.
* -------------------------------------------------------------
* {param: const FlByte* val1} One of the keys to be compared
* {param: const FlByte* val2} Another key to be compared
* -------------------------------------------------------------
* {return: bool} True if the keys are equals
* -------------------------------------------------------------
*/
bool fl_container_equals_string(const FlByte *val1, const FlByte *val2);

/* -------------------------------------------------------------
* {function: fl_container_equals_int}
* -------------------------------------------------------------
* Compares the two keys by just applying the equality operator
* on the dereferenced integers
* -------------------------------------------------------------
* {param: const FlByte* val1} One of the keys to be compared
* {param: const FlByte* val2} Another key to be compared
* -------------------------------------------------------------
* {return: bool} True if the keys are equals
* -------------------------------------------------------------
*/
bool fl_container_equals_int(const FlByte *val1, const FlByte *val2);

/* -------------------------------------------------------------
* {function: fl_container_equals_char}
* -------------------------------------------------------------
* Compares the two keys by just applying the equality operator
* on the dereferenced chars
* -------------------------------------------------------------
* {param: const FlByte* val1} One of the keys to be compared
* {param: const FlByte* val2} Another key to be compared
* -------------------------------------------------------------
* {return: bool} True if the keys are equals
* -------------------------------------------------------------
*/
bool fl_container_equals_char(const FlByte *val1, const FlByte *val2);

/* -------------------------------------------------------------
* {function: fl_container_equals_size_t}
* -------------------------------------------------------------
* Compares the two keys by just applying the equality operator
* on the dereferenced size_t objects
* -------------------------------------------------------------
* {param: const FlByte* val1} One of the keys to be compared
* {param: const FlByte* val2} Another key to be compared
* -------------------------------------------------------------
* {return: bool} True if the keys are equals
* -------------------------------------------------------------
*/
bool fl_container_equals_sizet(const FlByte *val1, const FlByte *val2);

#endif // FL_CONTAINER_H
