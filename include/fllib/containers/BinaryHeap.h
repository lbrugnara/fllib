#ifndef FL_BINARY_HEAP_H
#define FL_BINARY_HEAP_H

#include <stdbool.h>
#include <stdint.h>
#include "Container.h"

typedef struct FlBinaryHeap FlBinaryHeap;

typedef enum FlBinaryHeapType {
    FL_BINHEAP_MAX,
    FL_BINHEAP_MIN
} FlBinaryHeapType;

/*
 * Struct: struct FlBinaryHeapArgs
 *  This struct lets the caller configure the behavior of a binary heap instance when passed
 *  to the <fl_binheap_new_args> function.
 * 
 * Members:
 *  <FlContainerCompareFn> comparer: Function that compares items of the binary heap
 *  <FlContainerCleanupFn> cleaner: Function that, if present, frees the memory used by an item when removed from the binary heap
 *  <FlContainerWriterFn> writer: Function that, if present, writes an item into the binary heap.
 *  <size_t> element_size: The size of the elements to store in the heap. It is required if the <FlContainerWriterFn> is provided.
 *  <size_t> capacity: The initial capacity of the heap. If it is not provided, the default value is 15
 *  <size_t> max_capacity: If specified, the heap will limit the number of elements that can be stored to this number
 *  <double> growth_factor: Factor to apply in growth of the internal vector
 *  <FlBinaryHeapType> type: A binary heap can be a max-heap (FL_BINHEAP_MAX) or a min-heap (FL_BINHEAP_MIN). Default is max-heap
 */
struct FlBinaryHeapArgs {
    FlContainerCompareFn comparer;
    FlContainerCleanupFn cleaner;
    FlContainerWriterFn writer;
    size_t element_size;
    size_t capacity;
    size_t max_capacity;
    double growth_factor;
    FlBinaryHeapType type;
};

/*
 * Function: fl_binheap_new
 *  Creates a new instance of a binary heap
 *
 * Parameters:
 *  <FlContainerCompareFn> comparer: Function that compares items of the binary heap. If it is NULL the comparison happens between pointers
 *  <FlContainerCleanupFn> cleaner: Function that frees the memory used by an item when removed from the binary heap. 
 *      If it is null, the memory is not freed on item removal
 *
 * Returns:
 *  <FlBinaryHeap>*: A pointer to a binary heap object
 *
 * Notes:
 *  The object returned by this function must be freed using the
 *  <fl_binheap_free> function.
 */
FlBinaryHeap* fl_binheap_new(FlContainerCompareFn comparer, FlContainerCleanupFn cleaner);

/*
 * Function: fl_binheap_new_args
 *  Creates a new instance of a binary heap using the provided <struct FlBinaryHeapArgs> object to configure the behavior
 *
 * Parameters:
 *  <struct FlBinaryHeapArgs> args - Arguments to build a new binary heap
 *
 * Returns:
 *  <FlBinaryHeap>*: A pointer to a binary heap object
 *
 * Notes:
 *  The object returned by this function must be freed using the
 *  <fl_binheap_free> function.
 */
FlBinaryHeap* fl_binheap_new_args(struct FlBinaryHeapArgs args);

/*
 * Function: fl_binheap_free
 *  Releases the memory used by the binary heap object. If an <FlContainerCleanupFn> was provided on heap creation, the memory
 *  used by the items is released too using that function.
 *
 * Parameters:
 *  <FlBinaryHeap> *heap: The heap object to free
 *
 * Returns:
 *  This function does not return a value
 */
void fl_binheap_free(FlBinaryHeap *heap);

/*
 * Function: fl_binheap_insert
 *  Stores a new item into the binary heap
 *
 * Parameters:
 *  <FlBinaryHeap> *heap: The heap object
 *  <const void> *item: The item to store into the heap
 *
 * Returns:
 *  <void>*: Returns a pointer to the stored item.
 *
 * Notes:
 *  If the heap object was created using an <FlContainerWriterFn> the returned pointer points to
 *  the newly allocated object within the heap, otherwise the pointer is equals to the *item* parameter
 */
void* fl_binheap_insert(FlBinaryHeap *heap, const void *item);

/*
 * Function: fl_binheap_get_root
 *  Returns the heap's root element. Based on the <FlContainerEqualsfunction> the heap can be configured as a maximum-oriented
 *  or a minimum-oriented heap, which means the root element will be the item with the greatest priority or the lowest priorioty
 *  respectively.
 *
 * Parameters:
 *  <FlBinaryHeap> *heap: The heap object
 *
 * Returns:
 *  <void>*: If the heap size is greater than 0, a pointer to the root element, otherwise it returns NULL;
 */
void* fl_binheap_get_root(FlBinaryHeap *heap);

/*
 * Function: fl_binheap_remove_root
 *  Removes the root element from the heap. Based on the <FlContainerEqualsfunction> the heap can be configured as a maximum-oriented
 *  or a minimum-oriented heap, which means the root element will be the item with the greatest priority or the lowest priorioty
 *  respectively.
 *
 * Parameters:
 *  <FlBinaryHeap> *heap: The heap object
 *  <bool> clean: If clean is *true* and the <FlContainerCleanupFn> was provided on heap creation, the memory of the root element
 *                is realeased, otherwise the memory must be freed by the caller.
 *
 * Returns:
 *  <bool>: *true* if the root element exists and it is removed, otherwise this function returns *false*.
 */
bool fl_binheap_remove_root(FlBinaryHeap *heap, bool clean);

/*
 * Function: fl_binheap_search
 *  Searches an item within the heap using the <FlContainerCompareFn> if it is present. Otherwise the comparison
 *  happens between pointers using the <fl_container_compare_pointer> function.
 *
 * Parameters:
 *  <FlBinaryHeap> *heap: The heap object
 *  <const void> *item: The item to search within the heap
 *
 * Returns:
 *  <void>*: A pointer to the item within the heap if it exists, otherwise it returns NULL
 */
void* fl_binheap_search(FlBinaryHeap *heap, const void *item);

/*
 * Function: fl_binheap_remove
 *  This function removes the item from the heap if it exists.
 *
 * Parameters:
 *  <FlBinaryHeap> *heap: The heap object
 *  <const void> *item: The item to search and remove from the heap
 *  <bool> clean: If clean is *true* and the <FlContainerCleanupFn> was provided on heap creation, the memory of the root element
 *                is realeased, otherwise the memory must be freed by the caller.
 *
 * Returns:
 *  <bool>: *true* if the root element exists and it is removed, otherwise this function returns *false*.
 */
bool fl_binheap_remove(FlBinaryHeap *heap, const void *item, bool clean);

/*
 * Function: fl_binheap_length
 *  Returns the length of the heap.
 *
 * Parameters:
 *  <FlBinaryHeap> *heap: The heap object
 *
 * Returns:
 *  <size_t>: The binary heap length
 */
size_t fl_binheap_length(FlBinaryHeap *heap);

/*
 * Function: fl_binheap_height
 *  Returns the height of the heap.
 *
 * Parameters:
 *  <FlBinaryHeap> *heap: The heap object
 *
 * Returns:
 *  <size_t>: The binary heap height
 */
size_t fl_binheap_height(FlBinaryHeap *heap);

/*
 * Function: fl_binheap_element_size
 *  Returns the size of the element that can be stored in the heap object
 *
 * Parameters:
 *  <FlBinaryHeap> *heap: The heap object
 *
 * Returns:
 *  <size_t>: Size of the element type the heap can store
 */
size_t fl_binheap_element_size(FlBinaryHeap *heap);

/*
 * Function: fl_binheap_growth_factor
 *  Returns the growth factor of the heap's internal vector
 *
 * Parameters:
 *  <FlBinaryHeap> *heap: The heap object
 *
 * Returns:
 *  <double>: growth factor of the heap's internal vector
 */
double fl_binheap_growth_factor(FlBinaryHeap *heap);

/*
 * Function: fl_binheap_capacity
 *  Returns the current capacity of the heap
 *
 * Parameters:
 *  <FlBinaryHeap> *heap: The heap object
 *
 * Returns:
 *  size_t: Heap capacity 
 */
size_t fl_binheap_capacity(FlBinaryHeap *heap);

/*
 * Function: fl_binheap_max_capacity
 *  Returns the maximum number of items the heap object can contain
 *
 * Parameters:
 *  <FlBinaryHeap> *heap: The heap object
 *
 * Returns:
 *  size_t - Maximum capacity
 */
size_t fl_binheap_max_capacity(FlBinaryHeap *heap);

#endif /* FL_BINARY_HEAP_H */
