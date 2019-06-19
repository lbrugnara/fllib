#include <stdlib.h>
#include <string.h>

#include "Std.h"
#include "Mem.h"
#include "Array.h"

/*
 * Type: FlArrayHeader
 * 	Tracks the datatype size and the length of the array
 * 
 * ===== C =====
 * typedef struct FlArrayHeader
 * {
 * 		size_t dtsize;
 * 		size_t length;
 * } FlArrayHeader;
 * 
 */
typedef struct FlArrayHeader
{
	size_t dtsize;
	size_t length;
} FlArrayHeader;

#define GetHeader(a) (((FlArrayHeader*)a)-1)
#define GetArray(h) (((FlArrayHeader*)h) + 1)

void* fl_array_new(size_t dtsize, size_t length)
{
	flm_assert(length > 0, "Number of elements must be greater than 0");

	FlArrayHeader *header = (FlArrayHeader*)fl_calloc(1, sizeof(FlArrayHeader) + dtsize * length);

	header->dtsize = dtsize;
	header->length = length;

	memset(GetArray(header), 0, dtsize * length);

	return GetArray(header);
}

FlArray fl_array_resize(FlArray array, size_t length)
{
	FlArrayHeader *header = GetHeader(array);

	void *tmp = fl_realloc((void*)header, sizeof(FlArrayHeader) + (length * header->dtsize));

	if (tmp == NULL)
		return NULL;

	header->length = length;
	header = tmp;

	return GetArray(header);
}

void fl_array_delete(FlArray array)
{
	FlArrayHeader *header = GetHeader(array);
	fl_free(header);
}

void fl_array_delete_h(FlArray array, void (*delete_handler)(FlByte*))
{
	FlArrayHeader *header = GetHeader(array);
	size_t bytes = header->length * header->dtsize;
	FlByte *data = array;
    for (size_t i=0; i < bytes; i += header->dtsize)
    {
        if (data+i)
            delete_handler(data+i);
    }
    fl_free(header);
}

size_t fl_array_length(const FlArray array)
{
	FlArrayHeader *header = GetHeader(array);
	return header->length;
}

bool fl_array_contains(const FlArray array, const void *needle) 
{
	FlArrayHeader *header = GetHeader(array);
	size_t nelems = header->length;
	size_t needlesize = header->dtsize;
	for (size_t i=0; i < nelems; i++)
	{
		if (memcmp(((FlByte*)array)+(i*needlesize), needle, needlesize) == 0)
			return true;
	}
	return false;
}

bool fl_array_contains_n(const void *array, size_t nelems, const void *needle, size_t needlesize) 
{
	const FlByte *bytes = (FlByte*)array;

	for (size_t i=0; i * needlesize < nelems; i++)
	{
		if (memcmp(&bytes[i * needlesize], needle, needlesize) == 0)
			return true;
	}
	return false;
}

FlVector fl_array_to_vector(const FlArray array)
{
	FlArrayHeader *header = GetHeader(array);
	FlVector vector = fl_vector_new(header->dtsize, header->length);

	const FlByte *data = array;
	size_t l = header->length * header->dtsize;
	for (size_t i=0; i < l; i += header->dtsize)
    {
        if (data+i)
            fl_vector_add(vector, data+i);
    }
    return vector;
}
