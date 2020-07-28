#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "Cstring.h"
#include "Mem.h"
#include "Array.h"
#include "containers/Vector.h"
#include "containers/Hashtable.h"

char *fl_cstring_new(size_t length)
{
    char *str = fl_calloc(length + 1, sizeof(char));

    if (!str)
        return NULL;

    str[length] = '\0';

    return str;
}

void fl_cstring_free(const char *str)
{
    fl_free(str);
}

void fl_cstring_resize(char **dst, size_t length)
{
    flm_assert(*dst != NULL, "char* argument to resize cannot be NULL");
    *dst = fl_realloc(*dst, length + 1);
}

FlVector* fl_cstring_split(const char *str)
{
    flm_assert(str != NULL, "char* argument to split cannot be NULL");

    size_t length = strlen(str);
    FlVector *vector = flm_vector_new_with(.element_size = sizeof(char), .capacity = length);
    
    if (length == 0)
        return vector;

    for (size_t i = 0; i < length; i++)
        fl_vector_add(vector, str + i);

    return vector;
}

FlVector* fl_cstring_split_by(const char *string, const char *separator)
{
    flm_assert(string != NULL, "char* argument to split cannot be NULL");

    FlVector *parts = flm_vector_new_with(.capacity = 1, .cleaner = fl_container_cleaner_pointer);

    size_t separatorLength = strlen(separator);
    size_t index = 0;
    char *temp = NULL;
    
    while ((temp = strstr(string + index, separator)))
    {
        size_t length = temp - string - index;

        char *part = fl_cstring_dup_n(string + index, length);
        fl_vector_add(parts, &part);
        
        index += length + separatorLength;
    }

    size_t stringLength = strlen(string);

    if (stringLength - index != 0)
    {
        char *part = fl_cstring_dup(string + index);
        fl_vector_add(parts, &part);
    }

    return parts;
}

char *fl_cstring_to_array(const char *str)
{
    flm_assert(str != NULL, "char* argument to split cannot be NULL");

    size_t l = strlen(str);
    void *array = fl_array_new(sizeof(char), l);
    if (l == 0)
        return array;

    memcpy(array, str, l);
    return array;
}

char *fl_cstring_dup(const char *s)
{
    flm_assert(s != NULL, "char* argument to duplicate cannot be NULL");
    size_t l = strlen(s);
    return fl_cstring_dup_n(s, l);
}

char *fl_cstring_dup_n(const char *s, size_t n)
{
    flm_assert(s != NULL, "char* argument to duplicate cannot be NULL");
    char *sd = fl_cstring_new(n + 1);
    memcpy(sd, s, n);
    sd[n] = '\0';
    return sd;
}

char *fl_cstring_vdup(const char *s, ...)
{
    flm_assert(s != NULL, "char* argument to duplicate cannot be NULL");
    va_list args;
    va_start(args, s);
    char *str = fl_cstring_vadup(s, args);
    va_end(args);
    return str;
}

static inline size_t integer_length(long long i)
{
    size_t l = i >= 0 ? 1 : 2;
    while (i /= 10)
        l++;
    return l;
}

static inline size_t uinteger_length(unsigned long long i)
{
    size_t l = 1;
    while (i /= 10)
        l++;
    return l;
}

static inline size_t size_t_length(size_t i)
{
    size_t l = 1;
    while (i /= 10)
        l++;
    return l;
}

char *fl_cstring_vadup(const char *s, va_list args)
{
    flm_assert(s != NULL, "char* argument to duplicate cannot be NULL");

    va_list args_length;
    va_copy(args_length, args);
    size_t length = vsnprintf(NULL, 0, s, args_length);
    va_end(args_length);

    if (length == 0)
        return fl_cstring_new(0);

    char *string = fl_cstring_new(length);

    va_list args_format;
    va_copy(args_format, args);
    vsnprintf(string, length + 1, s, args_format);
    va_end(args_format);
    string[length] = '\0';

    return string;
}

char *fl_cstring_copy(char *dest, const char *source)
{
    flm_assert(dest != NULL, "Destination cannot be NULL");
    flm_assert(source != NULL, "Source cannot be NULL");

    char *d = dest;
    const char *ss = source;
    while (*ss)
    {
        *d = *ss;
        ss++;
        d++;
    }
    return dest;
}

char *fl_cstring_copy_n(char *dest, const char *source, size_t n)
{
    flm_assert(dest != NULL, "Destination cannot be NULL");
    flm_assert(source != NULL, "Source cannot be NULL");

    char *d = dest;
    const char *ss = source;
    while (n--)
    {
        *d = *ss;
        ss++;
        d++;
    }
    return dest;
}

char *fl_cstring_replace_char(const char *src, const char chr, const char *rplc)
{
    flm_assert(src != NULL, "Source cannot be NULL");
    flm_assert(rplc != NULL, "Replacement cannot be NULL");

    size_t src_size = strlen(src);
    size_t rplc_size = strlen(rplc);
    size_t dst_size = src_size;
    char *dst = fl_cstring_new(src_size);
    char cchr;
    size_t i = 0, j = 0;

    while ((cchr = src[i++]) != FL_EOS)
    {
        if (cchr != chr)
        {
            dst[j++] = cchr;
            continue;
        }

        if (rplc_size == 1)
        {
            dst[j++] = rplc[0];
            continue;
        }

        if (rplc_size > 1)
        {
            fl_cstring_resize(&dst, (dst_size += rplc_size - 1));
            memcpy(dst + j, rplc, rplc_size);
        }
        else
        {
            fl_cstring_resize(&dst, (dst_size -= 1));
        }

        j += rplc_size;
    }
    dst[dst_size] = FL_EOS;
    return dst;
}

bool cstr_match_backw(const char *str1, const char *str2, size_t n)
{
    flm_assert(str1 != NULL, "Input str1 cannot be NULL");
    flm_assert(str2 != NULL, "Input str2 cannot be NULL");

    for (size_t i = n - 1;; i--)
    {
        if (str1[i] != str2[i])
            return false;
        if (i == 0)
            break;
    }
    return true;
}

/**
 * Uses the Boyer-Moore-Horspool algorithm to search for occurrences of {needle} in {str}.
 * When a match is found the starting index of the match in src is used to set the same 
 * index in {map} to 1. Finally, iterating over {src} and {map}, the new char* {dst} is built
 * replacing all the {needle} occurrences by {rplc}
 */
char *fl_cstring_replace(const char *src, const char *needle, const char *rplc)
{
    flm_assert(src != NULL, "Source cannot be NULL");
    flm_assert(rplc != NULL, "Replacement cannot be NULL");

    size_t src_size = strlen(src);
    size_t needle_size = needle == NULL ? 0 : strlen(needle);
    size_t rplc_size = strlen(rplc);

    char *dst;
    fl_cstring_replace_n(src, src_size, needle, needle_size, rplc, rplc_size, &dst);
    return dst;
}

char* fl_cstring_replace_realloc(char *source, const char *needle, const char *replacement)
{
    flm_assert(source != NULL, "Source cannot be NULL");
    flm_assert(replacement != NULL, "Replacement cannot be NULL");

    size_t src_size = strlen(source);
    size_t needle_size = needle == NULL ? 0 : strlen(needle);
    size_t rplc_size = strlen(replacement);

    char *dst = NULL;
    fl_cstring_replace_n(source, src_size, needle, needle_size, replacement, rplc_size, &dst);

    if (!dst)
        return NULL;

    fl_cstring_free(source);

    return dst;
}

size_t fl_cstring_replace_n(const char *src, size_t src_size, const char *needle, size_t needle_size, const char *rplc, size_t rplc_size, char **dst)
{
    flm_assert(src != NULL, "Source cannot be NULL");
    flm_assert(rplc != NULL, "Replacement cannot be NULL");

    // If needle is NULL or it is longer than src, no need to replace anything
    if (needle == NULL || src_size < needle_size)
    {
        *dst = fl_cstring_dup(src);
        return src_size;
    }

    if (needle_size == 0)
    {
        *dst = fl_cstring_dup(rplc);
        fl_cstring_append(dst, src);
        return rplc_size + src_size;
    }

    // We need room to work with the 255 signed values of char because of 
    // that we place the "middle" of the array at index 128:
    //  Array:       [  0 , ..., 128, ..., 255 ]
    //  Char range:  [-128, ...,  0 , ..., 127 ]
    //
    //  FL_INDEX_AT(arr, CHAR_MIN)  => arr[-128 + 127 + 1] => arr[ 0 ]
    //  FL_INDEX_AT(arr, 0)         => arr[   0 + 127 + 1] => arr[128]
    //  FL_INDEX_AT(arr, CHAR_MAX)  => arr[ 127 + 127 + 1] => arr[255]
    //
    #define FL_INDEX_AT(arr, i) ((arr)[ (size_t) ((i) + CHAR_MAX + 1 )])

    size_t move_distance[255] = { 0 };
    char exists[255] = { 0 };

    for (size_t i = 0; i < needle_size - 1; i++)
    {
        FL_INDEX_AT(move_distance, needle[i]) = needle_size - i - 1;
        FL_INDEX_AT(exists, needle[i]) = 1;
    }

    if (FL_INDEX_AT(exists, needle[needle_size - 1]) == 0)
    {
        FL_INDEX_AT(move_distance, needle[needle_size - 1]) = needle_size;
        FL_INDEX_AT(exists, needle[needle_size - 1]) = 1;
    }

    const char *strptr = src;
    char *result = NULL;
    size_t *map = fl_array_new(sizeof(size_t), src_size);
    memset(map, 0, src_size * sizeof(size_t));
    size_t newlength = src_size;
    size_t i = 0;
    for (i = needle_size - 1; i < src_size;)
    {
        // i delta
        size_t d = needle_size;
        size_t k = i - (needle_size - 1);
        if (strptr[i] != needle[needle_size - 1] || (needle_size > 1 && !cstr_match_backw(strptr + k, needle, needle_size - 1)))
        {
            if (FL_INDEX_AT(exists, strptr[i]))
                d = FL_INDEX_AT(move_distance, strptr[i]);
        }
        else
        {
            map[k] = 1;
            newlength += (int)rplc_size - (int)needle_size;
        }
        i += d;
    }

    *dst = fl_cstring_new(newlength);
    // spi = source pointer index
    // mpi = map pointer index
    // dpi = dest pointer index
    for (size_t spi = 0, mpi = 0, dpi = 0; mpi < src_size;)
    {
        if (map[mpi])
        {
            for (size_t j = mpi; j < mpi + rplc_size; j++, dpi++)
                (*dst)[dpi] = rplc[j - mpi];
            spi += needle_size;
            mpi += needle_size;
        }
        else
        {
            (*dst)[dpi++] = src[spi++];
            mpi++;
        }
    }
    fl_array_free(map);

    #undef FL_INDEX_AT

    return newlength;
}

bool fl_cstring_contains(const char *str, const char *needle)
{
    return fl_cstring_find(str, needle) != NULL;
}

/**
 * Uses the Boyer-Moore-Horspool algorithm to search for occurrences of {needle} in {str}
 */
char *fl_cstring_find(const char *str, const char *needle)
{
    flm_assert(str != NULL, "Source cannot be NULL");

    size_t str_size = strlen(str);
    size_t needle_size = 0;
    // If needle is NULL or it is longer than str, no need to replace anything
    if (needle == NULL || str_size < (needle_size = strlen(needle)))
        return NULL;

    if (needle_size == 0)
        return (char*) str;

    // We need room to work with the 255 signed values of char because of 
    // that we place the "middle" of the array at index 128:
    //  Array:       [  0 , ..., 128, ..., 255 ]
    //  Char range:  [-128, ...,  0 , ..., 127 ]
    //
    //  FL_INDEX_AT(arr, CHAR_MIN)  => arr[-128 + 127 + 1] => arr[ 0 ]
    //  FL_INDEX_AT(arr, 0)         => arr[   0 + 127 + 1] => arr[128]
    //  FL_INDEX_AT(arr, CHAR_MAX)  => arr[ 127 + 127 + 1] => arr[255]
    //
    #define FL_INDEX_AT(arr, i) ((arr)[ (size_t) ((i) + CHAR_MAX + 1 )])

    size_t move_distance[256] = { 0 };
    char exists[256] = { 0 };

    for (size_t i = 0; i < needle_size - 1; i++)
    {
        FL_INDEX_AT(move_distance, needle[i]) = needle_size - i - 1;
        FL_INDEX_AT(exists, needle[i]) = 1;
    }

    if (FL_INDEX_AT(exists, needle[needle_size - 1]) == 0)
    {
        FL_INDEX_AT(move_distance, needle[needle_size - 1]) = needle_size;
        FL_INDEX_AT(exists, needle[needle_size - 1]) = 1;
    }

    const char *strptr = str;
    const char *result = NULL;
    for (size_t i = needle_size - 1; i < str_size;)
    {
        size_t k = i - (needle_size - 1);
        if (strptr[i] != needle[needle_size - 1] || (needle_size > 1 && !cstr_match_backw(strptr + k, needle, needle_size - 1)))
        {
            if (FL_INDEX_AT(exists, strptr[i]) != 0)
            {
                size_t dist = FL_INDEX_AT(move_distance, strptr[i]);
                if (dist == 0)
                {
                    result = strptr + i;
                    break;   
                }
                i += dist;
            }
            else
            {
                i += needle_size;
            }
            continue;
        }
        result = strptr + k;
        break;
    }

    #undef FL_INDEX_AT

    return (char *)result;
}

char **
fl_cstring_append(char **dst, const char *str)
{
    size_t length_dst = strlen(*dst);
    size_t length_str = strlen(str);
    fl_cstring_resize(dst, length_dst + length_str);
    fl_cstring_copy_n((*dst) + length_dst, str, length_str);
    (*dst)[length_dst + length_str] = FL_EOS;
    return dst;
}

char **
fl_cstring_append_char(char **dst, char c)
{
    size_t length = strlen((*dst));
    fl_cstring_resize(dst, length + 1);
    (*dst)[length] = c;
    (*dst)[length + 1] = FL_EOS;
    return dst;
}

char** fl_cstring_vappend(char **dst, const char *format, ...)
{
    va_list args;
    va_start(args, format);    
    size_t length = vsnprintf(NULL, 0, format, args);
    va_end(args);

    if (length == 0)
    {
        return dst;
    }

    size_t original_length = strlen(*dst);
    fl_cstring_resize(dst, original_length + length);

    va_start(args, format);
    vsnprintf((*dst + original_length), length + 1, format, args);
    va_end(args);

    return dst;
}

char *fl_cstring_join(FlVector *vector, char *glue)
{
    size_t glue_length = strlen(glue);
    char *str = fl_cstring_new(0);
    size_t i = 0, end = fl_vector_length(vector);
    while (i < end)
    {
        // Current element and its length
        char *el = *(char**) fl_vector_ref_get(vector, i);
        size_t el_length = strlen(el);
        // Str current length
        size_t str_length = strlen(str);
        // Glue length depends on end variable, when el is the final element, glue won't be appended
        glue_length = end > 0 ? glue_length : 0;

        // Resize str to allow new element (and glue if it is necessary)
        size_t newSize = str_length + el_length + glue_length;

        fl_cstring_resize(&str, newSize);
        fl_cstring_append(&str, el);
        // If is not the end, append the glue
        if (i < end - 1)
            fl_cstring_append(&str, glue);
        i++;
    }
    return str;
}

char *fl_char_join(FlVector *vector, char *glue)
{
    size_t glue_length = strlen(glue);
    char *str = fl_cstring_new(0);
    size_t i = 0, end = fl_vector_length(vector);
    while (i < end)
    {
        // Current element and its length
        char el = *(char*)fl_vector_ref_get(vector, i);
        // Str current length
        size_t str_length = strlen(str);
        // Glue length depends on end variable, when el is the final element, glue won't be appended
        glue_length = end > 0 ? glue_length : 0;

        // Resize str to allow new element (and glue if it is necessary)
        size_t newSize = str_length + 1 + glue_length;

        fl_cstring_resize(&str, newSize);
        fl_cstring_append_char(&str, el);
        // If is not the end, append the glue
        if (i < end - 1)
            fl_cstring_append(&str, glue);
        i++;
    }
    return str;
}

char *fl_cstring_concat(char **strings)
{
    char *result = fl_cstring_new(0);
    int i = 0;
    while (strings[i])
    {
        char *string = strings[i++];
        fl_cstring_append(&result, string);
    }
    return result;
}
