#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

#define FL_ARRAY_HEADER(a) (((FlArrayHeader*)a)-1)
#define FL_ARRAY_DATA(h) (((FlArrayHeader*)h) + 1)

void* fl_array_new(size_t element_size, size_t length)
{
	FlArrayHeader *header = (FlArrayHeader*) fl_calloc(1, sizeof(FlArrayHeader) + element_size * length);

	header->element_size = element_size;
	header->length = length;

	memset(FL_ARRAY_DATA(header), 0, element_size * length);

	return FL_ARRAY_DATA(header);
}

FlArray* fl_array_resize(FlArray *array, size_t length)
{
	FlArrayHeader *header = FL_ARRAY_HEADER(array);

	void *tmp = fl_realloc((void*)header, sizeof(FlArrayHeader) + (length * header->element_size));

	if (tmp == NULL)
		return NULL;

	header = tmp;
	header->length = length;

	return FL_ARRAY_DATA(header);
}

void fl_array_free(FlArray *array)
{
	FlArrayHeader *header = FL_ARRAY_HEADER(array);
	fl_free(header);
}

void fl_array_free_each(FlArray *array, FlArrayFreeElementFunc item_free_func)
{
	FlArrayHeader *header = FL_ARRAY_HEADER(array);
	size_t bytes = header->length * header->element_size;
	FlByte *data = array;
    for (size_t i=0; i < bytes; i += header->element_size)
    {
        if (data+i)
            item_free_func(data+i);
    }
    fl_free(header);
}

void fl_array_free_each_pointer(FlArray *array, FlArrayFreeElementFunc item_free_func)
{
	FlArrayHeader *header = FL_ARRAY_HEADER(array);
	void **data = array;
    for (size_t i=0; i < header->length; i++)
    {
        if (data[i])
            item_free_func(data[i]);
    }
    fl_free(header);
}

FlArray* fl_array_combine(FlArray *dest_array, const FlArray * const src_array)
{
	FlArrayHeader *dest_header = FL_ARRAY_HEADER(dest_array);
	FlArrayHeader *src_header = FL_ARRAY_HEADER(src_array);
	
	if (dest_header->element_size != src_header->element_size)
		return NULL;

	size_t orig_length = dest_header->length;
	size_t new_length = dest_header->length + src_header->length;
	void *tmp = fl_array_resize(dest_array, new_length);

	if (tmp == NULL)
		return NULL;

	dest_header = FL_ARRAY_HEADER(tmp);
	dest_header->length = new_length;
	dest_array = tmp;


	for (size_t i=0; orig_length < new_length; orig_length++, i++)
		memcpy((FlByte*)dest_array + (orig_length * dest_header->element_size), (FlByte*)src_array + (i * src_header->element_size), dest_header->element_size);

	return dest_array;
}

FlArray* fl_array_append(FlArray *array, const void *element)
{
	FlArrayHeader *header = FL_ARRAY_HEADER(array);
	FlArray *tmp = fl_array_resize(array, header->length + 1);

	if (!tmp)
		return NULL;

	array = tmp;
	header = FL_ARRAY_HEADER(array);

	memcpy((FlByte*)array + ((header->length - 1) * header->element_size), element, header->element_size);

	return array;
}

size_t fl_array_length(const FlArray * const array)
{
	FlArrayHeader *header = FL_ARRAY_HEADER(array);
	return header->length;
}

size_t fl_array_element_size(const FlArray * const array)
{
	FlArrayHeader *header = FL_ARRAY_HEADER(array);
	return header->element_size;
}

bool fl_array_contains(const FlArray * const array, const void *needle) 
{
	FlArrayHeader *header = FL_ARRAY_HEADER(array);
	size_t nelems = header->length;
	size_t needlesize = header->element_size;
	for (size_t i=0; i < nelems; i++)
	{
		if (memcmp(((FlByte*)array)+(i*needlesize), needle, needlesize) == 0)
			return true;
	}
	return false;
}

bool fl_array_contains_n(const void *array, size_t array_length, const void *needle, size_t needle_size) 
{
	const FlByte *bytes = (FlByte*) array;
	size_t total_length = array_length * needle_size;

	for (size_t i=0; i * needle_size < total_length; i++)
	{
		if (memcmp(&bytes[i * needle_size], needle, needle_size) == 0)
			return true;
	}
	return false;
}

FlVector* fl_array_to_vector(const FlArray * const array)
{
	FlArrayHeader *header = FL_ARRAY_HEADER(array);
	FlVector *vector = flm_vector_new_with(.capacity = header->length);

	const FlByte *data = array;
	size_t l = header->length * header->element_size;
	for (size_t i=0; i < l; i += header->element_size)
    {
        if (data+i)
            fl_vector_add(vector, data+i);
    }
    return vector;
}
