#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "Vector.h"
#include "../Mem.h"
#include "../Cstring.h"
#include "../Array.h"

struct FlVector {
    FlContainerWriterFn writer;
    FlContainerCleanupFn cleaner;
    double growth_factor;
    size_t element_size;
    size_t capacity;
    size_t max_capacity;
    size_t length;
    FlByte *data;
};

static inline bool calculate_growth(FlVector *vector, size_t required_capacity, size_t *granted_capacity_ptr)
{
    if (granted_capacity_ptr == NULL) {
        return false;
    }

    // If the vector has reached its max capacity, we have nothing to grant
    if (vector->capacity == vector->max_capacity) {
        *granted_capacity_ptr = 0;
        return false;
    }

    // Apply growth factor until get required capacity or the maximum one, whichever
    // happens first
    size_t granted_capacity = vector->capacity;
    do {
        granted_capacity = ceil(granted_capacity * vector->growth_factor);
        if (granted_capacity >= vector->max_capacity) {
            // No more room, break the loop
            granted_capacity = vector->max_capacity;
            break;
        }
    } while (granted_capacity < required_capacity);

    *granted_capacity_ptr = granted_capacity;
    return granted_capacity >= required_capacity;
}

bool resize_vector(FlVector *vector, size_t capacity)
{
    // Check if the number of bytes will overflow
    if (fl_std_uint_mult_overflow(vector->element_size, capacity, SIZE_MAX)) {
        return false;
    }

    // Try to reallocate 
    void *tmp = fl_realloc(vector->data, vector->element_size * capacity);
    if (!tmp) {
        return false;
    }

    // On success update the vector
    vector->capacity = capacity;
    vector->data = tmp;

    return true;
}

FlVector* fl_vector_new(void) 
{
    return fl_vector_new_args((struct FlVectorArgs){
        .capacity = 10,
        .cleaner = NULL,
        .writer = NULL,
        .growth_factor = 0.0, // default
        .element_size = 0, // default
        .max_capacity = 0 // default
    });
}

FlVector* fl_vector_new_args(struct FlVectorArgs args)
{
    flm_assert(args.element_size > 0 || args.writer == NULL, "Element size must be greater than 0");

    FlVector *vector = fl_calloc(1, sizeof(struct FlVector));

    vector->length = 0;

    vector->writer = args.writer != NULL ? args.writer : fl_container_writer;
    vector->cleaner = args.cleaner != NULL ? args.cleaner : NULL;

    vector->growth_factor = args.growth_factor != 0.0 ? args.growth_factor : 2.0;
    vector->element_size = args.element_size > 0 ? args.element_size : sizeof(void*);

    if (args.capacity == 0)
        args.capacity = 1;

    vector->capacity = args.capacity;
    vector->max_capacity = args.max_capacity != 0 ? args.max_capacity : SIZE_MAX / vector->element_size;

    if (vector->capacity > vector->max_capacity)
    {
        fl_vector_free(vector);
        return NULL;
    }

    if (fl_std_uint_mult_overflow(vector->max_capacity, vector->element_size, SIZE_MAX))
    {
        fl_vector_free(vector);
        return NULL;
    }

    vector->data = fl_calloc(vector->capacity, vector->element_size);

	return vector;
}

size_t fl_vector_length(FlVector *vector)
{
    return vector->length;
}

FlContainerCleanupFn fl_vector_cleanup_fn_get(FlVector *vector)
{
    return vector->cleaner;
}

FlContainerCleanupFn fl_vector_cleanup_fn_set(FlVector *vector, FlContainerCleanupFn cleanup_fn)
{
    FlContainerCleanupFn old_cleanup_fn = vector->cleaner;
    vector->cleaner = cleanup_fn;
    return old_cleanup_fn;
}

double fl_vector_growth_factor(FlVector *vector)
{
    return vector->growth_factor;
}

size_t fl_vector_capacity(FlVector *vector)
{
    return vector->capacity;
}

size_t fl_vector_max_capacity(FlVector *vector)
{
    return vector->max_capacity;
}

size_t fl_vector_element_size(FlVector *vector)
{
    return vector->element_size;
}

bool fl_vector_add(FlVector *vector, const void *element) 
{
    size_t required_capacity = vector->length + 1;

    if (required_capacity > vector->capacity)
    {
        size_t granted_capacity = 0;

        if (!calculate_growth(vector, required_capacity, &granted_capacity)) {
            return false;
        }

        if (!resize_vector(vector,  granted_capacity)) {
            return false;
        }
    }

    // Get the target slot
    void *retval = vector->data + vector->length * vector->element_size;

    // Write the element to the slot
    vector->writer(retval, element, vector->element_size);
    
    // Update the length
    vector->length++;
    
    return true;
}

bool fl_vector_insert(FlVector *vector, const void *element, size_t index) {
    // Check resize
    if (index >= vector->capacity || vector->length >= vector->capacity) {
        // Expected new capacity per growth factor
        size_t granted_capacity = 0;

        if (!calculate_growth(vector, (index >= vector->capacity ? index : vector->length) + 1, &granted_capacity)) {
            return false;
        }

        if (!resize_vector(vector, granted_capacity)) {
            return false;
        }
    }

    // Zero out between the last element and the index if needed
    if (vector->length < index) {        
        memset(vector->data + vector->length * vector->element_size, 0, (index - vector->length) * vector->element_size);
    }

    // Calculate index' offset
    size_t offset = index * vector->element_size;

    // If the index is in between current elements, we need to move elements
    // to make space for the new element
    if (index < vector->length)
    {
        // Number of bytes to move "forward"
        size_t bytes = (vector->length - index) * vector->element_size;        

        // We need to move "forward" from the calculated offset...
        void *from = vector->data + offset;
        // ... to the "next" position to make room for the new element
        void *to = vector->data + (offset + vector->element_size);
        memmove(to, from, bytes);

        // Update the length taking into account the new element
        vector->length++;
    } 
    else 
    {
        // If the index is "beyond" the length, we just need to
        // update the length to reflect all the added padding along
        // with the new element
        vector->length = index + 1 ;
    }

    // Get the target slot
    void *retval = vector->data + offset;

    // Write the element to the slot
    vector->writer(retval, element, vector->element_size);

    return true;
}

void* fl_vector_ref_get(FlVector *vector, size_t index)
{
    if (index >= vector->length)
        return NULL;

    return vector->data + index * vector->element_size;
}

void *fl_vector_ref_first(FlVector *vector)
{
    if (vector->length == 0) {
        return NULL;
    }

    return vector->data;
}

void *fl_vector_ref_last(FlVector *vector)
{
    if (vector->length == 0) {
        return NULL;
    }

    return vector->data + (vector->length-1) * vector->element_size;
}

bool fl_vector_shift(FlVector *vector, void *dest)
{
    if (vector->length == 0)
        return false;

    if (dest != NULL)
    {
        // Write the first element to the destination pointer
        vector->writer(dest, vector->data, vector->element_size);
    }
    else if (vector->cleaner)
    {
        // If dest is null and there is a cleaner function, release the memory of the element
        vector->cleaner(*(void**)(vector->data));
    }

    // Decrease the element now before moving all the data
    vector->length--;

    // Move the items starting at position 1 to the left to "remove" the element at position 0
    memmove(vector->data, vector->data + vector->element_size, vector->element_size * vector->length);

    // Clear the memory of the past-nth element
    memset(vector->data + vector->element_size * vector->length, 0, vector->element_size);

    return true;
}

bool fl_vector_pop(FlVector *vector, void *dest)
{
    if (vector->length == 0)
        return false;

    vector->length--;
    
    size_t offset = vector->element_size * vector->length;

    if (dest != NULL)
    {
        vector->writer(dest, vector->data + offset, vector->element_size);
        return true;
    }

    if (vector->cleaner)
    {
        vector->cleaner(*(void**)(vector->data + offset));
    }

    return true;
}

bool fl_vector_contains(FlVector *vector, const void *needle)
{
    for (size_t i=0; i < vector->length; i++) {
        if (memcmp(vector->data + (i * vector->element_size), needle, vector->element_size) == 0) {
            return true;
        }
    }
    return false;
}

bool fl_vector_concat(FlVector *vector1, FlVector *vector2)
{
    // TODO: Check for all type of overflows

    size_t v1_length = vector1->element_size * vector1->length;
    size_t v2_length = vector2->element_size * vector2->length;

    if (vector1->length + vector2->length >= vector1->capacity) {
        if (!resize_vector(vector1, vector1->capacity + vector2->length))
            return false;
    }

    // Copy vector2 at the end of vector1
    memcpy(vector1->data + v1_length, vector2->data, v2_length);

    vector1->length += vector2->length;

    return true;
}

FlVector* fl_vector_merge(FlVector *vector1, FlVector *vector2)
{
    // TODO: Check for all type of overflows

    size_t dest_length = vector1->length + vector2->length;

    FlVector *dest_vector = fl_vector_new_args((struct FlVectorArgs) {
        .capacity = dest_length, 
        .cleaner = vector1->cleaner, 
        .writer = vector1->writer,
        .element_size = vector1->element_size,
        .growth_factor = vector1->growth_factor,
    });

    if (dest_vector == NULL)
        return NULL;

    size_t v1_length = vector1->element_size * vector1->length;
    size_t v2_length = vector2->element_size * vector2->length;

    // Copy vector1 to the destination vector
    memcpy(dest_vector->data, vector1->data, v1_length);

    // Copy vector2 to the destination vector
    memcpy(dest_vector->data + v1_length, vector2->data, v2_length);

    dest_vector->length = dest_length;

    return dest_vector;
}

FlVector* fl_vector_merge_args(struct FlVectorArgs args, FlVector *vector1, FlVector *vector2)
{
    // TODO: Check for all type of overflows

    size_t dest_length = vector1->length + vector2->length;

    // Override the capacity, if the provided one is smaller
    if (args.capacity < dest_length)
        args.capacity = dest_length;

    FlVector *dest_vector = fl_vector_new_args(args);

    if (dest_vector == NULL)
        return NULL;

    size_t v1_length = vector1->element_size * vector1->length;
    size_t v2_length = vector2->element_size * vector2->length;

    // Copy vector1 to the destination vector
    memcpy(dest_vector->data, vector1->data, v1_length);

    // Copy vector2 to the destination vector
    memcpy(dest_vector->data + v1_length, vector2->data, v2_length);

    dest_vector->length = dest_length;

    return dest_vector;
}

bool fl_vector_remove(FlVector *vector, size_t pos, void *dest)
{
    if (vector->length == 0 || pos >= vector->length)
        return false;

    size_t targetindex = vector->element_size * pos;
    if (dest != NULL)
    {
        vector->writer(dest, vector->data+targetindex, vector->element_size);
    }
    else if (vector->cleaner)
    {
        vector->cleaner(*(void**)(vector->data + targetindex));
    }
    
    size_t target_next_index = targetindex + vector->element_size;
    vector->length--;
    memmove(vector->data, vector->data + target_next_index, vector->element_size * vector->length);

    return true;
}

void fl_vector_free(FlVector *vector) 
{
    if (vector->cleaner)
    {
        size_t total = vector->length * vector->element_size;
        for (size_t i = 0; i < total; i += vector->element_size)
        {
            void **ptr = (void**)(vector->data + i);
            vector->cleaner(*ptr);
        }
    }
    fl_free(vector->data);
    fl_free(vector);
}

FlArray* fl_vector_to_array(FlVector *vector)
{
	FlArray *array = fl_array_new(vector->element_size, vector->length);
    memcpy(array, vector->data, vector->element_size * vector->length);
    return array;
}

/*
 * FlIterator support
 *
 */
typedef struct FlVectorIterator {
    FlByte* base;
    unsigned int current;
    size_t element_size;  
} FlVectorIterator;

static void it_next(void *it)
{
    FlVectorIterator *vit = (FlVectorIterator*)it;
    vit->current += vit->element_size;
}

static void it_prev(void *it)
{
    FlVectorIterator *vit = (FlVectorIterator*)it;
    vit->current -= vit->element_size;
}

static void* it_value(void *it)
{
    FlVectorIterator *vit = (FlVectorIterator*)it;
    return vit->base + vit->current;
}

static bool it_equals(void *it1, void *it2)
{
    FlVectorIterator *vi1 = (FlVectorIterator*)it1;
    FlVectorIterator *vi2 = (FlVectorIterator*)it2;
    return vi1->base+vi1->current == vi2->base + vi2->current;
}

static bool it_start(void *it, void *container)
{
    FlVectorIterator *vit = it;
    FlVector *vector = container;
    return vit->base + vit->current == vector->data;
}

static bool it_end(void *it, void *container)
{
    FlVectorIterator *vit = it;
    FlVector *vector = container;
    return vit->base + vit->current == vector->data + (vector->element_size * vector->length);
}

static void it_delete(void *it)
{
    fl_free(it);
}

FlIterator* fl_vector_begin(const FlVector *vector)
{
    FlVectorIterator *vector_it = fl_calloc(1, sizeof(struct FlVectorIterator));
    vector_it->base = (FlByte*)vector->data;
    vector_it->current = 0;
    vector_it->element_size = vector->element_size;
    return fl_iterator_new(IT_VECTOR, vector_it, &it_next, &it_prev, &it_value, &it_equals, &it_start, &it_end, &it_delete);
}

FlIterator* fl_vector_end(const FlVector *vector)
{
    FlVectorIterator *vector_it = fl_calloc(1, sizeof(struct FlVectorIterator));
    vector_it->base = (FlByte*)vector->data + (vector->element_size * vector->length);
    vector_it->current = 0;
    vector_it->element_size = vector->element_size;
    return fl_iterator_new(IT_VECTOR, vector_it, &it_next, &it_prev, &it_value, &it_equals, &it_start, &it_end, &it_delete);
}
