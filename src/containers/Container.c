#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../Std.h"
#include "../Mem.h"
#include "Container.h"

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
    const char *string = (const char*)src;

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

void fl_container_writer(FlByte *dest, const FlByte *src, size_t struct_size)
{
    memcpy(dest, src, struct_size);
}

void fl_container_cleaner_pointer(void *obj)
{
    if (obj)
        fl_free(obj);
}

int fl_container_compare_pointer(const FlByte *obj1, const FlByte *obj2)
{
    uintptr_t ptr1 = *(const uintptr_t*) obj1;
    uintptr_t ptr2 = *(const uintptr_t*) obj2;

    // Prevent overflow
    if (ptr2 > ptr1)
        return -1;

    return ptr1 - ptr2;
}

int fl_container_compare_string(const FlByte *obj1, const FlByte *obj2)
{
    return strcmp((const char*) obj1, (const char*) obj2);
}

int fl_container_compare_int(const FlByte *obj1, const FlByte *obj2)
{
    int int1 = *(const int*) obj1;
    int int2 = *(const int*) obj2;

    if (fl_std_int_sub_overflow(int1, int2, INT_MIN, INT_MAX))
        return (int1 > int2) - (int1 < int2);

    return int1 - int2;
}

int fl_container_compare_char(const FlByte *obj1, const FlByte *obj2)
{
    char char1 = *(const char*) obj1;
    char char2 = *(const char*) obj2;

    if (fl_std_int_sub_overflow(char1, char2, CHAR_MIN, CHAR_MAX))
        return (char1 > char2) - (char1 < char2);

    return char1 - char2;
}

int fl_container_compare_sizet(const FlByte *obj1, const FlByte *obj2)
{
    size_t size1 = *(const size_t*) obj1;
    size_t size2 = *(const size_t*) obj2;

    // Prevent overflow
    if (size2 > size1)
        return -1;

    return size1 - size2;
}
