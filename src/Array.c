#include <stdlib.h>
#include <string.h>

#include "Std.h"
#include "Mem.h"
#include "Array.h"

/**
 * FlArrayHeader contains the
 * size ({s}) of each element in the array and the current
 * number of element that this array can contain ({n})
 */
typedef struct FlArrayHeader
{
	size_t s;
	size_t n;
} FlArrayHeader;

#define GetHeader(a) (((FlArrayHeader*)a)-1)

FlGenericArray fl_array_new(size_t size, size_t n)
{
	flm_assert(size >= 0, "Element size cannot be negative");
	FlGenericArray array = fl_calloc(1, sizeof(FlArrayHeader) + size * n);
	FlArrayHeader *h = ((FlArrayHeader*)array)+0;
	h->s = size;
	h->n = n;
	memset(h+1, 0, size * n);
	return h+1;
}

FlGenericArray fl_array_resize(FlGenericArray array, size_t n)
{
	FlArrayHeader *h = GetHeader(array);
	h->n = n;
	FlPointer tmp = fl_realloc((FlPointer)h, sizeof(FlArrayHeader) + (h->n * h->s));
	h = tmp;
	return h+1;
}

void fl_array_delete(FlGenericArray array)
{
	FlArrayHeader *h = GetHeader(array);
	fl_free(h);
}

void fl_array_delete_h(FlGenericArray array, void (*delete_handler)(FlByte*))
{
	FlArrayHeader *h = GetHeader(array);
	size_t l = h->n * h->s;
	FlByte *data = array;
    for (size_t i=0; i < l; i += h->s)
    {
        if (data+i)
        {
            delete_handler(data+i);
        }
    }
    fl_free(h);
}

size_t fl_array_length(const FlGenericArray array)
{
	FlArrayHeader *h = GetHeader(array);
	return h->n;
}

bool fl_array_contains_n(const FlGenericArray array, size_t nelems, const FlPointer needle, size_t needlesize) 
{
	for (size_t i=0; i < nelems; i++)
	{
		if (memcmp(((FlByte*)array)+(i*needlesize), needle, needlesize) == 0)
			return true;
	}
	return false;
}

bool fl_array_contains(const FlGenericArray array, const FlPointer needle) 
{
	FlArrayHeader *h = GetHeader(array);
	size_t nelems = h->n;
	size_t needlesize = h->s;
	for (size_t i=0; i < nelems; i++)
	{
		if (memcmp(((FlByte*)array)+(i*needlesize), needle, needlesize) == 0)
			return true;
	}
	return false;
}

FlVector fl_array_to_vector(const FlGenericArray array)
{
	FlArrayHeader *h = GetHeader(array);
	FlVector v = fl_vector_new(h->s, h->n);

	FlByte *data = array;
	size_t l = h->n * h->s;
	for (size_t i=0; i < l; i += h->s)
    {
        if (data+i)
        {
            fl_vector_add(v, data+i);
        }
    }
    return v;
}
