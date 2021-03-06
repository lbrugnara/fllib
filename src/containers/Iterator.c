/*
 * file: Iterator
 *
 * {FlIterator} is merely a group of functions and a data type
 * that only the container module knows how they work together
 * to return elements of the collection.
 * The container module has to create an own iterator implementation
 * referenced by {FlIterator} as the "target"
 *
 */

#include <stdbool.h>
#include "../Std.h"
#include "../Mem.h"
#include "Iterator.h"

/*
 * Type: struct FlIterator
 *
 * Represents an Iterator that contains the data and functions
 * necessaries to traverse a container. All the information
 * MUST be provided by the particular implementation made by
 * a container module.
 *
 * {member: FlIteratorType type} The type of iterator.
 * {member: void *itdata} The container needs to implement a data structure to keep track of the state of the iterator
 * {member: FlIteratorMoveFunction next} Moves the iterator to the next element in the collection
 * {member: FlIteratorMoveFunction prev} Moves the iterator to the previous element in the collection
 * {member: FlIteratorValueFunction value} Retrieves the value of the current elment
 * {member: FlIteratorEqualsFunction equals} Comparision function to check if two iterators point to the same element in the collection
 * {member: FlIteratorPositionFunction isstart} Checks if an iterator is pointing to the first element in the collection
 * {member: FlIteratorPositionFunction isend} Checks if an iterator is pointing to the past-the-last-nth element in the collection
 * {member: FlIteratorFreeFunction delete} Frees the memory used by {itdata}
 *
 */
struct FlIterator {
    FlIteratorType type;
    void *target;
    FlIteratorMoveFunction next;
    FlIteratorMoveFunction prev;
    FlIteratorValueFunction value;
    FlIteratorPositionFunction isstart;
    FlIteratorEqualsFunction equals;
    FlIteratorPositionFunction isend;
    FlIteratorFreeFunction delete;
};

FlIterator* fl_iterator_new(
    FlIteratorType type, 
    void *itdata, 
    FlIteratorMoveFunction next, 
    FlIteratorMoveFunction prev, 
    FlIteratorValueFunction value, 
    FlIteratorEqualsFunction equals, 
    FlIteratorPositionFunction isstart, 
    FlIteratorPositionFunction isend, 
    FlIteratorFreeFunction delete_handler
)
{
    FlIterator *it = fl_calloc(1, sizeof(struct FlIterator));
    it->type = type;
    it->target = itdata;
    it->next = next;
    it->prev = prev;
    it->value = value;
    it->equals = equals;
    it->isstart = isstart;
    it->isend = isend;
    it->delete = delete_handler;
    return it;
}

/*
 * Function: fl_iterator_free
 *
 * Releases the memory used by {FlIterator} and calls the
 * {FlIteratorFreeFunction} function to request to the container
 * module implementing the iterator to release the memory
 * allocated in the process.
 *
 * FlIterator *it - Target iterator to clean up
 *
 * {return: void}
 *
 */
void fl_iterator_free(FlIterator *it)
{
    it->delete(it->target);
    fl_free(it);    
}

FlIterator* fl_iterator_next(FlIterator *it)
{
    it->next(it->target);
    return it;
}

FlIterator* fl_iterator_prev(FlIterator *it)
{
    it->prev(it->target);
    return it;
}

void* fl_iterator_value(FlIterator *it)
{
    return it->value(it->target);
}

bool fl_iterator_equals(FlIterator *it1, FlIterator *it2)
{
    return it1->type == it2->type && it1->equals(it1->target, it2->target);
}

bool fl_iterator_is_start(FlIterator *it, void *container)
{
    return it->isstart(it->target, container);
}

bool fl_iterator_is_end(FlIterator *it, void *container)
{
    return it->isend(it->target, container);
}
