#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "Vector.h"
#include "../Mem.h"
#include "../Cstring.h"
#include "../Array.h"

/**
 * {@type} Container type
 * {@dtisze} Data type size of each element to be almacenated
 * {@capacity} Max number of elements to be almacenated
 * {@length} Number of current saved elements
 * {@data} Memory used by {FlVector} to save elements
 */
struct FlVector {
    size_t dtsize;
    size_t capacity;
    size_t length;
    FlByte* data;
};

FlVector fl_vector_new(size_t dtsize, size_t nelem) 
{
    FlVector vector = fl_calloc(1, sizeof(struct FlVector));
    if (nelem == 0)
        nelem = 1;
    vector->data = fl_calloc(nelem, dtsize);
    vector->dtsize = dtsize;
    vector->length = 0;
    vector->capacity = nelem;
	return vector;
}

void* fl_vector_add(FlVector vector, const void *elem) 
{
    if (vector->length >= vector->capacity) {
        fl_vector_resize(vector, vector->capacity * 2);
    }
    size_t sizet = (vector->length * vector->dtsize);
    memcpy(vector->data + sizet, elem, vector->dtsize);
    vector->length++;
    return vector->data + sizet;
}

size_t fl_vector_length(FlVector vector)
{
    return vector->length;
}

size_t fl_vector_dtsize(FlVector vector)
{
    return vector->dtsize;
}

size_t fl_vector_capacity(FlVector vector)
{
    return vector->capacity;
}

void* fl_vector_insert(FlVector vector, const void *elem, size_t pos)
{
    if (elem == NULL)
    {
        flm_exit(ERR_FATAL, "elem is NULL");
    }

    if (vector->length >= vector->capacity) {
        fl_vector_resize(vector, vector->capacity * 2);
    }
    else if (pos >= vector->capacity) {
        fl_vector_resize(vector, vector->capacity + pos);
    }

    if (vector->length < pos)
    {
        memset(vector->data + (vector->length*vector->dtsize), 0, (pos - vector->length) * vector->dtsize);
    }

    size_t destpos = (pos * vector->dtsize);
    if (pos < vector->length)
    {
        size_t bytestomove = (vector->length - pos) * vector->dtsize;
        memmove((vector->data + destpos + vector->dtsize), (vector->data + destpos), bytestomove);
        vector->length++;
    } 
    else 
    {
        vector->length = pos+1 ;
    }
    memcpy(vector->data + destpos, elem, vector->dtsize);
    return vector->data+destpos;
}

void* fl_vector_unshift(FlVector vector, const void *elem)
{
    return fl_vector_insert(vector, elem, 0);
}

void fl_vector_resize(FlVector vector, size_t nelem)
{
    vector->capacity = nelem;
    size_t new_size = vector->dtsize * vector->capacity;
    vector->data = fl_realloc(vector->data, new_size);
}

/* Returns a pointer to the element in the index {index} in {vector->data} */
void* fl_vector_get(FlVector vector, size_t index) 
{
    if (index >= vector->length)
        return NULL;
    size_t sizet = (index * vector->dtsize);
    return (vector->data + sizet);
}

void *fl_vector_first(FlVector vector)
{
    if (vector->length == 0)
        return NULL;
    return vector->data;
}

void *fl_vector_last(FlVector vector)
{
    if (vector->length == 0)
        return NULL;
    return (vector->data + (vector->length-1) * vector->dtsize);
}

bool fl_vector_shift(FlVector vector, void *dest)
{
    if (vector->length == 0)
        return false;
    if (dest != NULL)
        memcpy(dest, vector->data, vector->dtsize);
    vector->length--;
    size_t o1 = vector->dtsize; // First element
    memmove(&vector->data[0], &vector->data[o1], vector->dtsize * vector->length); // Move the items one position to the left
    size_t offset = vector->dtsize * vector->length; // Old last element index
    memset(vector->data+offset, 0, vector->dtsize); // Clear old last element
    return true;
}

bool fl_vector_pop(FlVector vector, void *dest)
{
    if (vector->length == 0)
        return false;
    vector->length--;
    size_t offset = vector->dtsize * vector->length;
    if (dest != NULL)
        memcpy(dest, vector->data+offset, vector->dtsize);
    memset(vector->data+offset, 0, vector->dtsize);
    return true;
}

bool fl_vector_contains(FlVector vector, const void *needle)
{
    size_t offset = vector->dtsize * vector->length;
    for (size_t i=0; i < vector->length; i++)
    {
        // TODO
        if (memcmp(vector->data+offset, needle, vector->dtsize) == 0)
            return true;
    }
    return false;
}

void fl_vector_concat(FlVector v, FlVector v2)
{
    size_t sizev = v->dtsize*v->length;
    size_t sizev2 = v2->dtsize*v2->length;
    if (v->length + v2->length >= v->capacity) {
        fl_vector_resize(v, v->capacity + v2->length);
    }
    memcpy(v->data+sizev, v2->data, sizev2);
    v->length += v2->length;
}

FlVector fl_vector_merge(FlVector v, FlVector v2)
{
    size_t totallength = v->length + v2->length;
    FlVector newone = fl_vector_new(v->dtsize, totallength);
    size_t sizev = v->dtsize*v->length;
    size_t sizev2 = v2->dtsize*v2->length;
    memcpy(newone->data, v->data, sizev);
    memcpy(newone->data+sizev, v2->data, sizev2);
    newone->length = totallength;
    return newone;
}

bool fl_vector_remove(FlVector vector, size_t pos, void *dest)
{
    if (vector->length == 0 || pos >= vector->length)
        return false;

    size_t targetindex = vector->dtsize * pos;
    if (dest != NULL)
        memcpy(dest, vector->data+targetindex, vector->dtsize);
    size_t targetnextindex = targetindex + vector->dtsize;
    vector->length--;
    memmove(&vector->data[targetindex], &vector->data[targetnextindex], vector->dtsize * vector->length);
    return true;
}

void fl_vector_clear(FlVector vector)
{
    if (vector->length == 0)
        return;
    size_t nbytes = vector->dtsize * vector->length;
    memset(vector->data, 0, nbytes);
    vector->length = 0;
}

/* Free the memory reserved for ar */
void fl_vector_delete(FlVector vector) 
{
    fl_free(vector->data);
    fl_free(vector);
}

void fl_vector_delete_ptrs(FlVector vector)
{
    FlByte** data = (FlByte**)vector->data;
    size_t offset;
    for (size_t i=0; i < vector->length; i++)
    {
        if (data[i])
            fl_free(data[i]);
    }
    fl_free(vector->data);
    fl_free(vector);
}

// TODO: data[i] is not valid when typeof is not a pointer
void fl_vector_delete_h(FlVector vector, void (*delete_handler)(FlByte*))
{
    FlByte** data = (FlByte**)vector->data;
    size_t offset;
    for (size_t i=0; i < vector->length; i++)
    {
        if (data[i])
        {
            delete_handler(data[i]);
        }
    }
    fl_free(vector->data);
    fl_free(vector);
}

void* fl_vector_to_array(FlVector vector)
{
	void * array = fl_array_new(vector->dtsize, vector->length);
    memcpy(array, vector->data, vector->dtsize * vector->length);
    return array;
}

/*
 * FlIterator support
 *
 */
typedef struct FlVectorIterator {
    FlByte* base;
    unsigned int current;
    size_t dtsize;  
} FlVectorIterator;

static void it_next(void *it)
{
    FlVectorIterator *vit = (FlVectorIterator*)it;
    vit->current += vit->dtsize;
}

static void it_prev(void *it)
{
    FlVectorIterator *vit = (FlVectorIterator*)it;
    vit->current -= vit->dtsize;
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
    return vit->base + vit->current == vector->data + (vector->dtsize * vector->length);
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
    vector_it->dtsize = vector->dtsize;
    return fl_iterator_new(IT_VECTOR, vector_it, &it_next, &it_prev, &it_value, &it_equals, &it_start, &it_end, &it_delete);
}

FlIterator fl_vector_end(const FlVector vector)
{
    FlVectorIterator *vector_it = fl_calloc(1, sizeof(struct FlVectorIterator));
    vector_it->base = (FlByte*)vector->data + (vector->dtsize * vector->length);
    vector_it->current = 0;
    vector_it->dtsize = vector->dtsize;
    return fl_iterator_new(IT_VECTOR, vector_it, &it_next, &it_prev, &it_value, &it_equals, &it_start, &it_end, &it_delete);
}
