#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include <math.h>
#include "../Mem.h"
#include "../Array.h"
#include "BinaryHeap.h"

#define flm_binheap_capacity(heap) fl_array_length((heap)->nodes)
#define flm_binheap_element_size(heap) fl_array_element_size((heap)->nodes)

struct FlBinaryHeap {
    FlContainerCompareFn comparer;
    FlContainerWriterFn writer;
    FlContainerCleanupFn cleaner;
    FlByte *nodes;
    double growth_factor;
    size_t max_capacity;
    size_t length;
    FlBinaryHeapType type;
};

static inline size_t calculate_growth(FlBinaryHeap *heap, size_t reqcapacity)
{
    // If the heap has reached its max capacity, we have nothing to do here
    size_t capacity = fl_binheap_capacity(heap);
    if (capacity == heap->max_capacity)
        return capacity;

    // Apply growth factor until get required capacity or the maximum one, whichever
    // happens first
    size_t newcapacity = capacity;
    do {
        newcapacity = (newcapacity > ceil(heap->max_capacity / heap->growth_factor))
                            ? heap->max_capacity
                            : ceil(newcapacity * heap->growth_factor);
    } while (reqcapacity > newcapacity && newcapacity != heap->max_capacity);

    return newcapacity;
}

bool resize_heap(FlBinaryHeap *heap, size_t capacity)
{
    // Check if the number of bytes will overflow
    size_t element_size = fl_array_element_size(heap->nodes);
    if (fl_std_uint_mult_overflow(element_size, capacity, SIZE_MAX))
        return false;

    heap->nodes = fl_array_resize(heap->nodes, element_size * capacity);

    return heap->nodes != NULL;
}

static int binheap_compare(FlBinaryHeap *heap, FlByte *first, FlByte *second)
{
    int result = heap->comparer(first, second);

    if (heap->type == FL_BINHEAP_MIN)
    {
        switch (result)
        {
            case INT_MAX:
                result = INT_MIN;
                break;

            case INT_MIN:
                result = INT_MAX;
                break;

            default:
                result *= -1;
        }
    }

    return result;
}

static bool binheap_exchange(FlBinaryHeap *heap, size_t child_index, size_t parent_index)
{
    if (heap->writer == NULL)
    {
        void** nodes = (void**) heap->nodes;
        void *child = nodes[child_index];
        void *parent = nodes[parent_index];
        
        if (binheap_compare(heap, child, parent) <= 0)
            return false;

        void *tmp = nodes[child_index];
        nodes[child_index] = nodes[parent_index];
        nodes[parent_index] = tmp;
    }
    else
    {
        size_t element_size = flm_binheap_element_size(heap);
        FlByte *child = heap->nodes + element_size * child_index;
        FlByte *parent = heap->nodes + element_size * parent_index;

        if (binheap_compare(heap, child, parent) <= 0)
            return false;

        fl_scoped_resource(FlByte*, tmp, fl_calloc(1, element_size), fl_free(tmp)) {
            memcpy(tmp, child, element_size);
            memcpy(child, parent, element_size);
            memcpy(parent, tmp, element_size);
        }
    }

    return true;
}

static size_t binheap_swim(FlBinaryHeap *heap, size_t target_index)
{
    while (target_index > 0)
    {
        size_t parent_index = (int) floor((target_index - 1) / 2);

        if (!binheap_exchange(heap, target_index, parent_index))
            break;

        target_index = parent_index;
    }

    return target_index;
}

static void binheap_sink(FlBinaryHeap *heap, size_t target_index)
{
    size_t element_size = flm_binheap_element_size(heap);

    size_t child_index = 0;

    while ((child_index = 2 * target_index + 1) < heap->length)
    {
        FlByte *left_child = heap->nodes + element_size * child_index;

        if (child_index < heap->length - 1)
        {
            FlByte *right_child = heap->nodes + element_size * (child_index + 1);

            // If the right child is greater than the left one, take the right child's index
            if (right_child && binheap_compare(heap, right_child, left_child) >= 0)
                child_index++;
        }

        if (!binheap_exchange(heap, child_index, target_index))
            break;

        target_index = child_index;
    }
}

FlBinaryHeap* fl_binheap_new(FlContainerCompareFn comparer, FlContainerCleanupFn cleaner)
{
    return fl_binheap_new_args((struct FlBinaryHeapArgs) {
        .comparer = comparer,
        .cleaner = cleaner,
        .writer = NULL,
        .capacity = 15,
        .element_size = sizeof(void*),
        .type = FL_BINHEAP_MAX
    });
}

FlBinaryHeap* fl_binheap_new_args(struct FlBinaryHeapArgs args)
{
    FlBinaryHeap *heap = fl_malloc(sizeof(struct FlBinaryHeap));

    heap->length = 0;
    heap->comparer = args.comparer != NULL ? args.comparer : fl_container_compare_pointer;
    heap->writer = args.writer != NULL ? args.writer : NULL;
    heap->cleaner = args.cleaner != NULL ? args.cleaner : NULL;
    heap->growth_factor = args.growth_factor != 0.0 ? args.growth_factor : 2.0;
    heap->type = args.type;

    size_t element_size = args.element_size > 0 ? args.element_size : sizeof(void*);
    size_t capacity = args.capacity == 0 ? 1 : args.capacity;
    
    heap->max_capacity = args.max_capacity != 0 ? args.max_capacity : SIZE_MAX / element_size;

    if (capacity > heap->max_capacity)
    {
        fl_binheap_free(heap);
        return NULL;
    }

    if (fl_std_uint_mult_overflow(heap->max_capacity, element_size, SIZE_MAX))
    {
        fl_binheap_free(heap);
        return NULL;
    }

    heap->nodes = fl_array_new(element_size, capacity);

    if (heap->nodes == NULL)
    {
        fl_binheap_free(heap);
        return NULL;
    }

    return heap;
}

void fl_binheap_free(FlBinaryHeap *heap)
{
    flm_assert(heap != NULL, "Heap object must not be NULL");

    if (heap->nodes != NULL)
    {
        if (heap->cleaner)
        {
            fl_array_free_each(heap->nodes, heap->cleaner);
        }
        else
        {
            fl_array_free(heap->nodes);
        }
        
    }

    fl_free(heap);
}

void* fl_binheap_insert(FlBinaryHeap *heap, const void *item)
{
    size_t required_capacity = heap->length + 1;

    if (required_capacity > flm_binheap_capacity(heap))
    {
        size_t new_capacity = calculate_growth(heap, required_capacity);

        if (new_capacity == heap->max_capacity)
            return NULL;

        if (!resize_heap(heap,  new_capacity))
            return NULL;
    }

    size_t element_size = flm_binheap_element_size(heap);
    void *bucket = NULL;
    if (heap->writer == NULL)
    {
        bucket = ((void**) heap->nodes)[heap->length] = (void*) item;
    }
    else
    {
        bucket = heap->nodes + heap->length * element_size;
        heap->writer(bucket, item, element_size);
    }
    
    heap->length++;

    size_t position_in_heap = binheap_swim(heap, heap->length - 1);
    bucket = heap->nodes + position_in_heap * element_size;

    return bucket;
}

void* fl_binheap_get_root(FlBinaryHeap *heap)
{
    return heap->length > 0 ? heap->nodes : NULL;
}

bool fl_binheap_remove_root(FlBinaryHeap *heap, bool clean)
{
    if (heap->length == 0)
        return false;

    binheap_exchange(heap, 0, heap->length - 1);
    heap->length--;
    binheap_sink(heap, 0);

    if (clean && heap->cleaner)
    {
        heap->cleaner(heap->nodes + heap->length * flm_binheap_element_size(heap));
    }

    return true;
}

void* fl_binheap_search(FlBinaryHeap *heap, const void *item)
{
    return NULL;
}

bool fl_binheap_remove(FlBinaryHeap *heap, const void *item, bool clean)
{
    return false;
}

size_t fl_binheap_length(FlBinaryHeap *heap)
{
    return heap->length;
}

size_t fl_binheap_height(FlBinaryHeap *heap)
{
    return 0;
}

size_t fl_binheap_element_size(FlBinaryHeap *heap)
{
    return flm_binheap_element_size(heap);
}

double fl_binheap_growth_factor(FlBinaryHeap *heap)
{
    return heap->growth_factor;
}

size_t fl_binheap_capacity(FlBinaryHeap *heap)
{
    return flm_binheap_capacity(heap);
}

size_t fl_binheap_max_capacity(FlBinaryHeap *heap)
{
    return heap->max_capacity;
}
