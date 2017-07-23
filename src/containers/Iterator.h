#ifndef FL_ITERATOR_H
#define FL_ITERATOR_H

/* =============================================================
 * {module: Iterator}
 * =============================================================
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
 * -------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include "../Types.h"
#include "../Std.h"
#include "../Mem.h"

/* -------------------------------------------------------------
 * {datatype: struct FlIterator FlIterator}
 * -------------------------------------------------------------
 * Typedef of the opaque struct FlIterator
 * -------------------------------------------------------------
 */
typedef struct FlIterator* FlIterator;

/* -------------------------------------------------------------
 * {datatype: enum FlIteratorType}
 * -------------------------------------------------------------
 * Contains all the type of iterators registred by {FlIterator}
 * module.
 * Current support: {FlVector}, {FlList} and {FlDictionary}
 * -------------------------------------------------------------
 */
typedef enum {
    IT_VECTOR,
    IT_LIST,
    IT_DICTIONARY
} FlIteratorType;

/* -------------------------------------------------------------
 * {datatype: void (*)(void*) FlIteratorMove}
 * -------------------------------------------------------------
 * Represents a function that moves the current iterator to 
 * another element in the collection.
 * -------------------------------------------------------------
 */
typedef void (*FlIteratorMove)(void *it);

/* -------------------------------------------------------------
 * {datatype: void* (*)(void*) FlIteratorValue}
 * -------------------------------------------------------------
 * Represents a function that retrieves the value of an element
 * -------------------------------------------------------------
 */
typedef void* (*FlIteratorValue)(void *it);

/* -------------------------------------------------------------
 * {datatype: bool (*)(void*, void*) FlIteratorEquals}
 * -------------------------------------------------------------
 * Compares two iterators to check if both are pointing to the
 * same element in the collection,
 * -------------------------------------------------------------
 */
typedef bool (*FlIteratorEquals)(void *it1, void *it2);

/* -------------------------------------------------------------
 * {datatype: void (*)(void*) FlIteratorDelete}
 * -------------------------------------------------------------
 * It is the routine called to free the memory used by the
 * specific implementation made by a container module
 * -------------------------------------------------------------
 */
typedef void (*FlIteratorDelete)(void *it);

/* -------------------------------------------------------------
 * {datatype: bool (*)(void*, void*) FlIteratorPosition}
 * -------------------------------------------------------------
 * Is a function that check the position of the element pointed
 * by {it} in {container}
 * -------------------------------------------------------------
 */
typedef bool (*FlIteratorPosition)(void *it, void *container);


/* -------------------------------------------------------------
 * {function: fl_iterator_new}
 * -------------------------------------------------------------
 * This function will create a new iterator for a specific container
 * type. The container module is responsible for all the routines
 * that are necessary to instantaite and use an {FlIterator}
 * -------------------------------------------------------------
 * {param: FlIteratorType type} Type of {FlIterator}
 * {param: void *itdata} Is the data that the container module uses to manipulate the iteration process.
 *	It contains all the information necessary to move the iterator, retrieve its value, compare and check it.
 * {param: FlIteratorMove next} Moves the iterator to the next element in the collection
 * {param: FlIteratorMove prev} Moves the iterator to the previous element in the collection
 * {param: FlIteratorValue value} Retrieves the value of the current elment
 * {param: FlIteratorEquals equals} Comparision function to check if two iterators point to the same element in the collection
 * {param: FlIteratorPosition isstart} Checks if an iterator is pointing to the first element in the collection
 * {param: FlIteratorPosition isend} Checks if an iterator is pointing to the past-the-last-nth element in the collection
 * {param: FlIteratorDelete delete} Frees the memory used by {itdata}
 * -------------------------------------------------------------
 * {return: FlIterator} Iterator to traverse a collection of the type {FlIteratorType}
 * -------------------------------------------------------------
 */
FlIterator fl_iterator_new(
	FlIteratorType type, 
	void *itdata, 
	FlIteratorMove next, 
	FlIteratorMove prev, 
	FlIteratorValue value, 
	FlIteratorEquals equals, 
	FlIteratorPosition isstart, 
	FlIteratorPosition isend, 
	FlIteratorDelete delete
);

/* -------------------------------------------------------------
 * {function: fl_iterator_delete}
 * -------------------------------------------------------------
 * Releases the memory used by {FlIterator}.
 * -------------------------------------------------------------
 * {param: FlIterator it} Target iterator to clean up
 * -------------------------------------------------------------
 * {return: void}
 * -------------------------------------------------------------
 */
void fl_iterator_delete(FlIterator it);

/* -------------------------------------------------------------
 * {function: fl_iterator_next}
 * -------------------------------------------------------------
 * Moves the iterator to the next element in the collection
 * -------------------------------------------------------------
 * {param: FlIterator it} Target iterator
 * -------------------------------------------------------------
 * {return: FlIterator} Reference to the target iterator
 * -------------------------------------------------------------
 */
FlIterator fl_iterator_next(FlIterator it);

/* -------------------------------------------------------------
 * {function: fl_iterator_prev}
 * -------------------------------------------------------------
 * Moves the iterator to the previous element in the collection
 * -------------------------------------------------------------
 * {param: FlIterator it} Target iterator
 * -------------------------------------------------------------
 * {return: FlIterator} Reference to the target iterator
 * -------------------------------------------------------------
 */
FlIterator fl_iterator_prev(FlIterator it);

/* -------------------------------------------------------------
 * {function: fl_iterator_value}
 * -------------------------------------------------------------
 * Returns the value of the current pointed element
 * -------------------------------------------------------------
 * {param: FlIterator it} Target iterator
 * -------------------------------------------------------------
 * {return: void*} Pointer to the element data
 * -------------------------------------------------------------
 */
void* fl_iterator_value(FlIterator it);

/* -------------------------------------------------------------
 * {macro: flm_iterator_value}
 * -------------------------------------------------------------
 * Function {fl_iterator_value} returns a pointer to the 
 * element that needs to be dereferenced. This macro makes
 * this more friendly by taking a data type and doing the
 * casting.
 * -------------------------------------------------------------
 * {param: FlIterator it} Pointer to an interator
 * {param: DataType Keyword type} C data type
 * -------------------------------------------------------------
 */
#define flm_iterator_value(it, type) *(type*)fl_iterator_value(it)

/* -------------------------------------------------------------
 * {function: fl_iterator_is_end}
 * -------------------------------------------------------------
 * Check if the iterator is pointing 1 element beyond the last
 * element (actually, not an element)
 * -------------------------------------------------------------
 * {param: FlIterator it} Target iterator
 * {param: void *container} Collection of elements
 * -------------------------------------------------------------
 * {return: bool}
 * -------------------------------------------------------------
 */
bool fl_iterator_is_end(FlIterator it, void *container);

/* -------------------------------------------------------------
 * {function: fl_iterator_equals}
 * -------------------------------------------------------------
 * Check if both iterators are pointing to the same element
 * in the container
 * -------------------------------------------------------------
 * {param: FlIterator i1} Target iterator
 * {param: FlIterator i2} Target iterator
 * -------------------------------------------------------------
 * {return: bool}
 * -------------------------------------------------------------
 */
bool fl_iterator_equals(FlIterator i1, FlIterator i2);

#endif /* FL_ITERATOR_H */
