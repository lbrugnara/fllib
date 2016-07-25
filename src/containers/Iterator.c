/* =============================================================
 * {module: Iterator}
 * =============================================================
 * {FlIterator} is merely a group of functions and a data type
 * that only the container module knows how they work together
 * to return elements of the collection.
 * The container module has to create an own iterator implementation
 * referenced by {FlIterator} as the "target"
 * -------------------------------------------------------------
 */

#include "../Std.h"
#include "../Mem.h"
#include "Iterator.h"
#include "Vector.h"
#include "Dictionary.h"

/* -------------------------------------------------------------
 * {datatype: struct FlIterator}
 * -------------------------------------------------------------
 * Represents an Iterator that contains the data and functions
 * necessaries to traverse a container. All the information
 * MUST be provided by the particular implementation made by
 * a container module.
 * -------------------------------------------------------------
 * {member: FlIteratorType type} The type of iterator.
 * {member: FlPointer itdata} The container needs to implement a data structure to keep track of the state of the iterator
 * {member: FlIteratorMove next} Moves the iterator to the next element in the collection
 * {member: FlIteratorMove prev} Moves the iterator to the previous element in the collection
 * {member: FlIteratorValue value} Retrieves the value of the current elment
 * {member: FlIteratorEquals equals} Comparision function to check if two iterators point to the same element in the collection
 * {member: FlIteratorPosition isstart} Checks if an iterator is pointing to the first element in the collection
 * {member: FlIteratorPosition isend} Checks if an iterator is pointing to the past-the-last-nth element in the collection
 * {member: FlIteratorDelete delete} Frees the memory used by {itdata}
 * -------------------------------------------------------------
 */
struct FlIterator {
    FlIteratorType type;
    FlPointer target;
    FlIteratorMove next;
    FlIteratorMove prev;
    FlIteratorValue value;
    FlIteratorPosition isstart;
    FlIteratorEquals equals;
    FlIteratorPosition isend;
    FlIteratorDelete delete;
};

FlIterator* fl_iterator_new(
    FlIteratorType type, 
    FlPointer itdata, 
    FlIteratorMove next, 
    FlIteratorMove prev, 
    FlIteratorValue value, 
    FlIteratorEquals equals, 
    FlIteratorPosition isstart, 
    FlIteratorPosition isend, 
    FlIteratorDelete delete_handler
)
{
    FlIterator *it = fl_calloc(1, sizeof(FlIterator));
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

/* -------------------------------------------------------------
 * {function: fl_iterator_delete}
 * -------------------------------------------------------------
 * Releases the memory used by {FlIterator} and calls the
 * {FlIteratorDelete} function to request to the container
 * module implementing the iterator to release the memory
 * allocated in the process.
 * -------------------------------------------------------------
 * {param: FlIterator* it} Target iterator to clean up
 * -------------------------------------------------------------
 * {return: void}
 * -------------------------------------------------------------
 */
void fl_iterator_delete(FlIterator *it)
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

FlPointer fl_iterator_value(FlIterator *it)
{
    return it->value(it->target);
}

bool fl_iterator_equals(FlIterator *it1, FlIterator *it2)
{
    return it1->type == it2->type && it1->equals(it1->target, it2->target);
}

bool fl_iterator_is_start(FlIterator *it, FlPointer container)
{
    return it->isstart(it->target, container);
}

bool fl_iterator_is_end(FlIterator *it, FlPointer container)
{
    return it->isend(it->target, container);
}
