#include <stdlib.h>
#include <string.h>

#include "Container.h"
#include "../Mem.h"

bool fl_container_equals_pointer(const FlByte *val1, const FlByte *val2)
{
    return val1 == val2;
}

bool fl_container_equals_string(const FlByte *val1, const FlByte *val2)
{
    return strcmp((const char*)val1, (const char*)val2) == 0;
}

bool fl_container_equals_int(const FlByte *val1, const FlByte *val2)
{
    return *(const int*)val1 == *(const int*)val2;
}

bool fl_container_equals_char(const FlByte *val1, const FlByte *val2)
{
    return *(const char*)val1 == *(const char*)val2;
}

bool fl_container_equals_sizet(const FlByte *val1, const FlByte *val2)
{
    return *(size_t*)val1 == *(size_t*)val2;
}

void fl_container_allocator_string(FlByte **dest, const FlByte *src)
{
    if (!src)
        return;

    const char *string = *(const char**)src;

    if (!string)
        return;

    size_t size = strlen(string) + 1;
    *dest = fl_malloc(size);
    memcpy(*dest, src, size-1);
    (*dest)[size-1] = '\0';
}

void fl_container_allocator_int(FlByte **dest, const FlByte *src)
{
    *dest = fl_malloc(sizeof(int));
    memcpy(*dest, src, sizeof(int));
}

void fl_container_allocator_char(FlByte **dest, const FlByte *src)
{
    *dest = fl_malloc(sizeof(char));
    memcpy(*dest, src, sizeof(char));
}

void fl_container_allocator_sizet(FlByte **dest, const FlByte *src)
{
    *dest = fl_malloc(sizeof(size_t));
    memcpy(*dest, src, sizeof(size_t));
}

void fl_container_writer_int(FlByte *dest, const FlByte *src)
{
    memcpy(dest, src, sizeof(int));
}

void fl_container_writer_char(FlByte *dest, const FlByte *src)
{
    memcpy(dest, src, sizeof(char));
}

void fl_container_writer_sizet(FlByte *dest, const FlByte *src)
{
    memcpy(dest, src, sizeof(size_t));
}

void fl_container_cleaner_pointer(void *obj)
{
    if (obj)
        fl_free(obj);
}
