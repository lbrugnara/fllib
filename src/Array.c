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
 * 		size_t element_size;
 * 		size_t length;
 * } FlArrayHeader;
 * 
 */
typedef struct FlArrayHeader
{
	size_t element_size;
	size_t length;
} FlArrayHeader;

#define GetHeader(a) (((FlArrayHeader*)a)-1)
#define GetArray(h) (((FlArrayHeader*)h) + 1)

void* fl_array_new(size_t element_size, size_t length)
{
	flm_assert(length >= 0, "Number of elements must be greater than 0");

	FlArrayHeader *header = (FlArrayHeader*)fl_calloc(1, sizeof(FlArrayHeader) + element_size * length);

	header->element_size = element_size;
	header->length = length;

	memset(GetArray(header), 0, element_size * length);

	return GetArray(header);
}

FlArray fl_array_resize(FlArray array, size_t length)
{
	FlArrayHeader *header = GetHeader(array);

	void *tmp = fl_realloc((void*)header, sizeof(FlArrayHeader) + (length * header->element_size));

	if (tmp == NULL)
		return NULL;

	header = tmp;
	header->length = length;

	return GetArray(header);
}

void fl_array_delete(const FlArray array)
{
	FlArrayHeader *header = GetHeader(array);
	fl_free(header);
}

void fl_array_delete_each(const FlArray array, void (*delete_handler)(FlByte*))
{
	FlArrayHeader *header = GetHeader(array);
	size_t bytes = header->length * header->element_size;
	FlByte *data = array;
    for (size_t i=0; i < bytes; i += header->element_size)
    {
        if (data+i)
            delete_handler(data+i);
    }
    fl_free(header);
}

FlArray fl_array_combine(FlArray dest_array, FlArray src_array)
{
	FlArrayHeader *dest_header = GetHeader(dest_array);
	FlArrayHeader *src_header = GetHeader(src_array);
	
	if (dest_header->element_size != src_header->element_size)
		return NULL;

	size_t orig_length = dest_header->length;
	size_t new_length = dest_header->length + src_header->length;
	void *tmp = fl_array_resize(dest_array, new_length);

	if (tmp == NULL)
		return NULL;

	dest_header = GetHeader(tmp);
	dest_header->length = new_length;
	dest_array = tmp;


	for (size_t i=0; orig_length < new_length; orig_length++, i++)
		memcpy((FlByte*)dest_array + (orig_length * dest_header->element_size), (FlByte*)src_array + (i * src_header->element_size), dest_header->element_size);

	return dest_array;
}

size_t fl_array_length(const FlArray array)
{
	FlArrayHeader *header = GetHeader(array);
	return header->length;
}

size_t fl_array_element_size(const FlArray array)
{
	FlArrayHeader *header = GetHeader(array);
	return header->element_size;
}

bool fl_array_contains(const FlArray array, const void *needle) 
{
	FlArrayHeader *header = GetHeader(array);
	size_t nelems = header->length;
	size_t needlesize = header->element_size;
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
	FlVector vector = fl_vector_new(header->length, NULL);

	const FlByte *data = array;
	size_t l = header->length * header->element_size;
	for (size_t i=0; i < l; i += header->element_size)
    {
        if (data+i)
            fl_vector_add(vector, data+i);
    }
    return vector;
}
