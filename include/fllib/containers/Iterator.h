#ifndef FL_ITERATOR_H
#define FL_ITERATOR_H

/*
 * file: Iterator
 *
 * Provides a simple interface to allow any type of container to
 * implement iterators to traverse the collection of elements.
 * Each container has to implement internally the functions
 * to traverse its collection, a function to create {FlIterator}s
 * and a function to delete them.
 * The functions needed by {FlIterator} are
 *	- Navigate to next element and to previous element
 *	- Retrieve the current element value
 *	- Check if two iterators point to the same element
 *	- Check if an iterator is the first element or the past-the-last-nth element
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "../Types.h"
#include "../Std.h"
#include "../Mem.h"

/*
 * Type: struct FlIterator FlIterator
 *
 * Typedef of the opaque struct FlIterator
 *
 */
typedef struct FlIterator FlIterator;

/*
 * Type: enum FlIteratorType
 *
 * Contains all the type of iterators registred by {FlIterator}
 * module.
 * Current support: {FlVector}, {FlList} and {FlDictionary}
 *
 */
typedef enum {
	IT_SPAN,
	IT_VECTOR,
	IT_LIST,
	IT_DICTIONARY
} FlIteratorType;

/*
 * Type: void (*)(void*) FlIteratorMoveFunction
 *
 * Represents a function that moves the current iterator to 
 * another element in the collection.
 *
 */
typedef void (*FlIteratorMoveFunction)(void *it);

/*
 * Type: void* (*)(void*) FlIteratorValueFunction
 *
 * Represents a function that retrieves the value of an element
 *
 */
typedef void* (*FlIteratorValueFunction)(void *it);

/*
 * Type: bool (*)(void*, void*) FlIteratorEqualsFunction
 *
 * Compares two iterators to check if both are pointing to the
 * same element in the collection,
 *
 */
typedef bool (*FlIteratorEqualsFunction)(void *it1, void *it2);

/*
 * Type: void (*)(void*) FlIteratorFreeFunction
 *
 * It is the routine called to free the memory used by the
 * specific implementation made by a container module
 *
 */
typedef void (*FlIteratorFreeFunction)(void *it);

/*
 * Type: bool (*)(void*, void*) FlIteratorPositionFunction
 *
 * Is a function that check the position of the element pointed
 * by {it} in {container}
 *
 */
typedef bool (*FlIteratorPositionFunction)(void *it, void *container);


/*
 * Function: fl_iterator_new
 *
 * This function will create a new iterator for a specific container
 * type. The container module is responsible for all the routines
 * that are necessary to instantaite and use an {FlIterator}
 *
 * FlIteratorType type} Type of {FlIterator -
 * void *itdata - Is the data that the container module uses to manipulate the iteration process.
 *	It contains all the information necessary to move the iterator, retrieve its value, compare and check it.
 * FlIteratorMoveFunction next - Moves the iterator to the next element in the collection
 * FlIteratorMoveFunction prev - Moves the iterator to the previous element in the collection
 * FlIteratorValueFunction value - Retrieves the value of the current elment
 * FlIteratorEqualsFunction equals - Comparision function to check if two iterators point to the same element in the collection
 * FlIteratorPositionFunction isstart - Checks if an iterator is pointing to the first element in the collection
 * FlIteratorPositionFunction isend - Checks if an iterator is pointing to the past-the-last-nth element in the collection
 * FlIteratorFreeFunction delete - Frees the memory used by *itdata*
 *
 * {return: FlIterator} Iterator to traverse a collection of the type {FlIteratorType}
 *
 */
FlIterator* fl_iterator_new(
	FlIteratorType type, 
	void *itdata, 
	FlIteratorMoveFunction next, 
	FlIteratorMoveFunction prev, 
	FlIteratorValueFunction value, 
	FlIteratorEqualsFunction equals, 
	FlIteratorPositionFunction isstart, 
	FlIteratorPositionFunction isend, 
	FlIteratorFreeFunction delete
);

/*
 * Function: fl_iterator_free
 *
 * Releases the memory used by {FlIterator}.
 *
 * FlIterator it - Target iterator to clean up
 *
 * {return: void}
 *
 */
void fl_iterator_free(FlIterator *it);

/*
 * Function: fl_iterator_next
 *
 * Moves the iterator to the next element in the collection
 *
 * FlIterator *it - Target iterator
 *
 * {return: FlIterator} Reference to the target iterator
 *
 */
FlIterator* fl_iterator_next(FlIterator *it);

/*
 * Function: fl_iterator_prev
 *
 * Moves the iterator to the previous element in the collection
 *
 * FlIterator *it - Target iterator
 *
 * {return: FlIterator*} Reference to the target iterator
 *
 */
FlIterator* fl_iterator_prev(FlIterator *it);

/*
 * Function: fl_iterator_value
 *
 * Returns the value of the current pointed element
 *
 * FlIterator *it - Target iterator
 *
 * {return: void*} Pointer to the element data
 *
 */
void* fl_iterator_value(FlIterator *it);

/*
 * Macro: flm_iterator_value
 *
 * Function {fl_iterator_value} returns a pointer to the 
 * element that needs to be dereferenced. This macro makes
 * this more friendly by taking a data type and doing the
 * casting.
 *
 * FlIterator *it - Pointer to an interator
 * DataType Keyword type - C data type
 *
 */
#define flm_iterator_value(it, type) *(type*)fl_iterator_value(it)

/*
 * Function: fl_iterator_is_start
 *
 * Check if the iterator is pointing 1 element
 *
 * FlIterator *it - Target iterator
 * void *container - Collection of elements
 *
 * {return: bool} True if iterator points to the 1 element in the container
 *
 */
bool fl_iterator_is_start(FlIterator *it, void *container);

/*
 * Function: fl_iterator_is_end
 *
 * Check if the iterator is pointing to the nth+1 element (actually, not an element)
 *
 * FlIterator *it - Target iterator
 * void *container - Collection of elements
 *
 * {return: bool} True if iterator points to the nth+1 element in the container
 *
 */
bool fl_iterator_is_end(FlIterator *it, void *container);

/*
 * Function: fl_iterator_equals
 *
 * Check if both iterators are pointing to the same element
 * in the container
 *
 * FlIterator *i1 - Target iterator
 * FlIterator *i2 - Target iterator
 *
 * {return: bool}
 *
 */
bool fl_iterator_equals(FlIterator *i1, FlIterator *i2);

#endif /* FL_ITERATOR_H */
