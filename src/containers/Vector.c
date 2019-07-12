#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "Vector.h"
#include "../Mem.h"
#include "../Cstring.h"
#include "../Array.h"

struct FlVector {
    FlContainerWriterFunction writer;
    FlContainerCleanupFunction cleaner;
    double growth_factor;
    size_t element_size;
    size_t capacity;
    size_t max_capacity;
    size_t length;
    FlByte *data;
};

static inline size_t calculate_growth(FlVector vector, size_t reqcapacity)
{
    // If the vector has reached its max capacity, we have nothing to do here
    if (vector->capacity == vector->max_capacity)
        return vector->capacity;

    // Apply growth factor until get required capacity or the maximum one, whichever
    // happens first
    size_t newcapacity = vector->capacity;
    do {
        newcapacity = (newcapacity > ceil(vector->max_capacity / vector->growth_factor))
                            ? vector->max_capacity
                            : ceil(newcapacity * vector->growth_factor);
    } while (reqcapacity > newcapacity && newcapacity != vector->max_capacity);

    return newcapacity;
}

bool resize_vector(FlVector vector, size_t capacity)
{
    // Check if the number of bytes will overflow
    if (fl_std_umult_wrap(vector->element_size, capacity, SIZE_MAX))
        return false;

    // Try to reallocate 
    void *tmp = fl_realloc(vector->data, vector->element_size * capacity);
    if (!tmp)
        return false;

    // On success update the vector
    vector->capacity = capacity;
    vector->data = tmp;

    return true;
}

FlVector fl_vector_new(size_t capacity, FlContainerCleanupFunction cleaner) 
{
    return fl_vector_new_args((struct FlVectorArgs){
        .capacity = capacity,
        .cleaner = cleaner,
        .writer = NULL,
        .growth_factor = 0.0, // default
        .element_size = 0, // default
        .max_capacity = 0 // default
    });
}

FlVector fl_vector_new_args(struct FlVectorArgs args)
{
    flm_assert(args.element_size > 0 || args.writer == NULL, "Element size must be greater than 0");

    FlVector vector = fl_calloc(1, sizeof(struct FlVector));

    vector->length = 0;

    vector->writer = args.writer != NULL ? args.writer : NULL;
    vector->cleaner = args.cleaner != NULL ? args.cleaner : NULL;

    vector->growth_factor = args.growth_factor != 0.0 ? args.growth_factor : 2.0;
    vector->element_size = args.element_size > 0 ? args.element_size : sizeof(void*);

    if (args.capacity == 0)
        args.capacity = 1;

    vector->capacity = args.capacity;
    vector->max_capacity = args.max_capacity != 0 ? args.max_capacity : SIZE_MAX / vector->element_size;

    if (vector->capacity > vector->max_capacity)
    {
        fl_vector_delete(vector);
        return NULL;
    }

    if (fl_std_umult_wrap(vector->max_capacity, vector->element_size, SIZE_MAX))
    {
        fl_vector_delete(vector);
        return NULL;
    }

    vector->data = fl_calloc(vector->capacity, vector->element_size);

	return vector;
}

size_t fl_vector_length(FlVector vector)
{
    return vector->length;
}

double fl_vector_growth_factor(FlVector vector)
{
    return vector->growth_factor;
}

size_t fl_vector_capacity(FlVector vector)
{
    return vector->capacity;
}

size_t fl_vector_max_capacity(FlVector vector)
{
    return vector->max_capacity;
}

size_t fl_vector_element_size(FlVector vector)
{
    return vector->element_size;
}

void* fl_vector_add(FlVector vector, const void *element) 
{
    size_t reqcapacity = vector->length + 1;

    if (reqcapacity > vector->capacity)
    {
        size_t newcapacity = calculate_growth(vector, reqcapacity);

        if (newcapacity == vector->max_capacity)
            return NULL;

        if (!resize_vector(vector,  newcapacity))
            return NULL;
    }

    void *retval = NULL;
    if (vector->writer == NULL)
    {
        retval = ((void**)vector->data)[vector->length] = (void*)element;
    }
    else
    {
        retval = vector->data + vector->length * vector->element_size;
        vector->writer(retval, element, vector->element_size);
    }
    
    vector->length++;
    
    return retval;
}

void* fl_vector_insert(FlVector vector, const void *element, size_t index)
{
    // Capacity required to store an element in the index-nth position
    size_t reqcapacity = index + 1;

    if (reqcapacity > vector->max_capacity)
        return NULL;

    // Check resize
    if (vector->length >= vector->capacity || reqcapacity > vector->capacity)
    {
        // Expected new capacity per growth factor
        size_t newcapacity = calculate_growth(vector, reqcapacity);

        if (!resize_vector(vector, newcapacity))
            return NULL;
    }

    // Zero out between the last element and the index if needed
    if (vector->length < index)
    {        
        size_t bytes = (index - vector->length) * vector->element_size;        
        memset(vector->data + vector->length * vector->element_size, 0, bytes);
    }

    // Calculate index' offset
    size_t offset = (index * vector->element_size);    

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

    void *retval = NULL;

    if (vector->writer == NULL)
    {
        retval = ((void**)vector->data)[index] = (void*)element;
    }
    else
    {
        retval = vector->data + offset;
        vector->writer(retval, element, vector->element_size);
    }

    return retval;
}

void* fl_vector_get(FlVector vector, size_t index)
{
    if (index >= vector->length)
        return NULL;

    if (vector->writer == NULL)
        return ((void**)vector->data)[index];

    return vector->data + index * vector->element_size;
}

void *fl_vector_last(FlVector vector)
{
    if (vector->length == 0)
        return NULL;
    
    if (vector->writer == NULL)
        return ((void**)vector->data)[vector->length-1];

    return vector->data + (vector->length-1) * vector->element_size;
}

void* fl_vector_shift(FlVector vector, void *dest)
{
    if (vector->length == 0)
        return NULL;

    if (vector->writer == NULL)
    {
        *(void**)dest = ((void**)vector->data)[0];
    }
    else
    {
        vector->writer(dest, vector->data, vector->element_size);
    }

    vector->length--;

    // Move the items one position to the left
    size_t offsetFirstElement = vector->element_size; // First element
    memmove(vector->data, vector->data + offsetFirstElement, vector->element_size * vector->length);

    // Clear old last element
    size_t offsetOldLastElement = vector->element_size * vector->length; // Old last element index
    memset(vector->data + offsetOldLastElement, 0, vector->element_size);

    return dest;
}

void* fl_vector_pop(FlVector vector, void *dest)
{
    if (vector->length == 0)
        return NULL;

    vector->length--;
    
    size_t offset = vector->element_size * vector->length;

    if (dest != NULL)
    {
        if (vector->writer == NULL)
        {
            *(void**)dest = ((void**)vector->data)[vector->length];
        }
        else
        {
            vector->writer(dest, vector->data + offset, vector->element_size);
        }

        return dest;
    }
    
    
    // memset(vector->data + offset, 0, vector->element_size);

    if (vector->cleaner)
    {
        if (vector->writer == NULL)
            vector->cleaner(((void**)vector->data)[vector->length]);
        else
            vector->cleaner(vector->data + offset);
    }

    return NULL;
}

bool fl_vector_contains(FlVector vector, const void *needle)
{
    size_t offset = vector->element_size * vector->length;
    for (size_t i=0; i < vector->length; i++)
    {
        // TODO
        if (memcmp(vector->data + offset, needle, vector->element_size) == 0)
            return true;
    }
    return false;
}

void fl_vector_concat(FlVector v, FlVector v2)
{
    size_t sizev = v->element_size*v->length;
    size_t sizev2 = v2->element_size*v2->length;
    if (v->length + v2->length >= v->capacity) {
        resize_vector(v, v->capacity + v2->length);
    }
    memcpy(v->data+sizev, v2->data, sizev2);
    v->length += v2->length;
}

FlVector fl_vector_merge(FlVector v, FlVector v2)
{
    size_t totallength = v->length + v2->length;
    FlVector newone = fl_vector_new(totallength, v->cleaner);
    size_t sizev = v->element_size*v->length;
    size_t sizev2 = v2->element_size*v2->length;
    memcpy(newone->data, v->data, sizev);
    memcpy(newone->data+sizev, v2->data, sizev2);
    newone->length = totallength;
    return newone;
}

bool fl_vector_remove(FlVector vector, size_t pos, void *dest)
{
    if (vector->length == 0 || pos >= vector->length)
        return false;

    size_t targetindex = vector->element_size * pos;
    if (dest != NULL)
    {
        if (vector->writer == NULL)
        {
            *(void**)dest = ((void**)vector->data)[targetindex];
        }
        else
        {
            vector->writer(dest, vector->data+targetindex, vector->element_size);
        }
    }
    else if (vector->cleaner)
    {
        vector->cleaner(vector->writer ? vector->data + targetindex : ((void**)vector->data + targetindex)[0]);
    }
    
    size_t targetnextindex = targetindex + vector->element_size;
    vector->length--;
    memmove(vector->data, vector->data + targetnextindex, vector->element_size * vector->length);

    return true;
}

/* Free the memory reserved for ar */
void fl_vector_delete(FlVector vector) 
{
    if (vector->cleaner)
    {
        if (vector->writer)
        {
            size_t total = vector->length * vector->element_size;
            for (size_t i = 0; i < total; i += vector->element_size)
            {
                FlByte *ptr = (FlByte*)vector->data + i;
                vector->cleaner(ptr);
            }
        }
        else
        {
            for (size_t i = 0; i < vector->length; i++)
            {
                vector->cleaner(((void**)vector->data)[i]);
            }
        }
    }
    fl_free(vector->data);
    fl_free(vector);
}

void* fl_vector_to_array(FlVector vector)
{
	void * array = fl_array_new(vector->element_size, vector->length);
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
    FlVector vector = container;
    return vit->base + vit->current == vector->data;
}

static bool it_end(void *it, void *container)
{
    FlVectorIterator *vit = it;
    FlVector vector = container;
    return vit->base + vit->current == vector->data + (vector->element_size * vector->length);
}

static void it_delete(void *it)
{
    fl_free(it);
}

FlIterator fl_vector_begin(const FlVector vector)
{
    FlVectorIterator *vector_it = fl_calloc(1, sizeof(struct FlVectorIterator));
    vector_it->base = (FlByte*)vector->data;
    vector_it->current = 0;
    vector_it->element_size = vector->element_size;
    return fl_iterator_new(IT_VECTOR, vector_it, &it_next, &it_prev, &it_value, &it_equals, &it_start, &it_end, &it_delete);
}

FlIterator fl_vector_end(const FlVector vector)
{
    FlVectorIterator *vector_it = fl_calloc(1, sizeof(struct FlVectorIterator));
    vector_it->base = (FlByte*)vector->data + (vector->element_size * vector->length);
    vector_it->current = 0;
    vector_it->element_size = vector->element_size;
    return fl_iterator_new(IT_VECTOR, vector_it, &it_next, &it_prev, &it_value, &it_equals, &it_start, &it_end, &it_delete);
}
