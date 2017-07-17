#ifndef FL_LIST_H
#define FL_LIST_H

#include "../Types.h"
#include "Iterator.h"

typedef struct FlListNode* FlListNode;
typedef struct FlList* FlList;

/**
 * {@dtsize} DataType size for the new list
 *
 * Creates an empty list for elements of {dtsize} size
 */
FlList fl_list_new(size_t dtsize);

/**
 * Returns the number of elements in {list}
 */
size_t fl_list_length(FlList list);

/**
 * Returns the size of the data type that {list} accepts
 */
size_t fl_list_dtsize(FlList list);

/**
 * {@list} Target list to add a new value
 * {@elem} Element to add to the list.
 * Return a pointer to the inserted element
 */
void* fl_list_add(FlList list, const void *elem);

/**
 * {@list} Target list to insert a new value
 * {@elem} Element to add to the list.
 * Insert {elem} in the position {pos} of {list}
 * Return a pointer to the inserted element
 */
void* fl_list_insert(FlList list, const void *elem, size_t pos);

/**
 * Returns a pointer to the element in the {index} index of {list}
 */
void* fl_list_get(FlList list, size_t index);

/**
 * Returns a pointer to the FlListNode element in the {index} index of {list}
 */
FlListNode fl_list_get_node(FlList list, size_t index);

/**
 * If {elem} exists in {list}, returns a pointer to the element
 */
void* fl_list_find(FlList list, const void *elem);

/**
 * If {elem} exists in {list}, returns a pointer to the element.
 * To find {elem} this functions uses a {handler} function
 *
 * {handler} receives a current element {ce}, an {elem} to look for and the size in bytes of each {ce}
 */
void* fl_list_find_h(FlList list, bool (*handler)(const void *ce, const void *elem, size_t dtsize), const void *elem);

/**
 * Remove from {list}, the first element in {list} copying it into {dest}
 */
bool fl_list_shift(FlList list, void *dest);

/**
 * Insert {elem} in the first position of {list}
 */
void* fl_list_unshift(FlList list, const void *elem);

/**
 * Remove from {list}, the last element in {list} copying it into {dest}
 */
bool fl_list_pop(FlList list, void *dest);

/**
 * Return true if {list} contains {elem}
 */
bool fl_list_contains(FlList list, const void *elem);

/**
 * Append {list2} elements at the end of {list}
 */
void fl_list_concat(FlList list, FlList list2);

/**
 * Creates a new FlList that will contain all the elements of
 * {list} and {list2}
 */
FlList fl_list_merge(FlList list, FlList list2);

/**
 * Removes the {pos}-th element {list}.
 * The removed element will be copied in {dest}
 */
bool fl_list_remove(FlList list, size_t pos, void *dest);

/**
 * Removes from {list} the element that found with {comparer}.
 * The removed element will be copied in {dest}
 */
bool fl_list_remove_h(FlList list, bool (*comparer)(const void *celem, const void *elem, size_t dtsize), const void *elem, void *dest);

/**
 * Remove all elements from the {list}
 */
void fl_list_clear(FlList list);

/**
 * Remove all elements from the {list} using {delete_handler} to release the memory
 */
void fl_list_clear_h(FlList list, void (*delete_handler)(FlByte*));

/**
 * Free the memory reserved for {list}
 */
void fl_list_delete(FlList list);

/**
 * Free the memory reserved for {list} using {delete_handler} for each element.
 * The handler MUST free the memory used by each element
 */
void fl_list_delete_h(FlList list, void (*delete_handler)(FlByte*));

/**
 * Returns a pointer to the next element of {node}
 */
FlListNode fl_list_node_next(FlListNode node);

/**
 * Returns a pointer to the previous element of {node}
 */
FlListNode fl_list_node_prev(FlListNode node);

/**
 * Returns a pointer to data saved in {node}
 */
FlByte* fl_list_node_data(FlListNode node);

/* -------------------------------------------------------------
 * {function: fl_list_start}
 * -------------------------------------------------------------
 * Returns an {FlIterator} that points to the first element in {param: list}
 * -------------------------------------------------------------
 * {param: const FlList list} Target list to be pointed by the iterator
 * -------------------------------------------------------------
 * {return: FlIterator} Iterator pointing to the first element in {param: list}
 * -------------------------------------------------------------
 */
FlIterator fl_list_start(const FlList list);

/* -------------------------------------------------------------
 * {function: fl_list_end}
 * -------------------------------------------------------------
 * Returns an {FlIterator} that points to the past-last-nth element in {param: list}
 * The FlIterator returned by this function MUST NOT be dereferenced by fl_iterator_value.
 * -------------------------------------------------------------
 * {param: const FlList list} Target list to be pointed by the iterator
 * -------------------------------------------------------------
 * {return: FlIterator} Iterator pointing beyond the last element in {param: list}
 * -------------------------------------------------------------
 */
FlIterator fl_list_end(const FlList list);

#endif /* FL_LIST_H */
