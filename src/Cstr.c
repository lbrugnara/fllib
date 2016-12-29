#include <stdlib.h>

#include "Cstr.h"
#include "Mem.h"
#include "Array.h"
#include "containers/Vector.h"
#include "containers/Dictionary.h"

FlCstr
fl_cstr_new (size_t length) {
	FlCstr str = fl_calloc(length+1, sizeof(char));
	str[length] = FL_EOS;
	return str;
}

void
fl_cstr_delete (FlCstr str)
{
    flm_assert(str != NULL, "FlCstr argument to free cannot be NULL");
	fl_free(str);
}

void
fl_cstr_resize (FlCstr *dst, size_t length) 
{
    flm_assert(*dst != NULL, "FlCstr argument to resize cannot be NULL");
	*dst = fl_realloc(*dst, length + 1);
}

FlVector 
fl_cstr_split (const FlCstr str) 
{
	flm_assert(str != NULL, "FlCstr argument to split cannot be NULL");

	size_t l = strlen(str);
	FlVector ovector = fl_vector_new(sizeof(char), l);
	if (l == 0)
		return ovector;

	for (size_t i=0; i < l; i++)
		fl_vector_add(ovector, (void*)&str[i]);
	return ovector;
}

char* 
fl_cstr_to_array (const FlCstr str) 
{
    flm_assert(str != NULL, "FlCstr argument to split cannot be NULL");

    size_t l = strlen(str);
    FlPointer array = fl_array_new(sizeof(char), l);
    if (l == 0)
        return array;

    memcpy(array, str, l);
    return array;
}

FlCstr 
fl_cstr_dup (const FlCstr s) 
{
    flm_assert(s != NULL, "FlCstr argument to duplicate cannot be NULL");
	FlCstr ss = s;
    size_t l = strlen(ss);
    FlCstr sd = fl_cstr_new(l);
    FlCstr sdc = sd;
    while(*ss)
    {
    	*sdc = *ss;
    	ss++;
    	sdc++;
    }
    return sd;
}

FlCstr
fl_cstr_vdup(const FlCstr s, ...)
{
    flm_assert(s != NULL, "FlCstr argument to duplicate cannot be NULL");
    va_list args;
    va_start(args, s);
    FlCstr str = fl_cstr_vadup(s, args);
    va_end(args);
    return str;
}

size_t integer_length(long long i)
{
    if (i == 0)
        return 1;
    size_t l = i > 0 ? 1 : 2;
    while(i /= 10)l++;
    return l;
}

size_t uinteger_length(unsigned long long i)
{
    flm_assert(i > 0, "uinteger_length does not accept signed values");
    if (i == 0)
        return 1;
    size_t l = 1;
    while(i /= 10)l++;
    return l;
}

FlCstr
fl_cstr_vadup (const FlCstr s, va_list args)
{
    flm_assert(s != NULL, "FlCstr argument to duplicate cannot be NULL");
    FlCstr sc = s;
    size_t length = strlen(sc);
    FlVector parts = fl_vector_new(sizeof(char), length);
    while(*sc)
    {
        if (*sc != '%')
        {
            char c = *sc;
            fl_vector_add(parts, &c);
        } 
        else
        {
            switch(*++sc)
            {
                case 'c':
                {
                    char c = (char)va_arg(args, int);
                    fl_vector_add(parts, &c);
                    break;
                }
                case 'd':
                {
                    int i = va_arg(args, int);
                    size_t t = integer_length(i);
                    char dst[t];
                    sprintf(dst, "%d", i);
                    for (int j=0; j < t; j++)
                        fl_vector_add(parts, dst+j);
                    break;
                }
                case 's':
                {
                    FlCstr str = va_arg(args, FlCstr);
                    size_t strlength = strlen(str);
                    for (int j=0; j < strlength; j++)
                        fl_vector_add(parts, str+j);
                    break;
                }
                case '%':
                {
                    fl_vector_add(parts, sc);
                    break;
                }
            }
        }
        sc++;
    }
    FlCstr sd = fl_char_join(parts, "");
    fl_vector_delete(parts);
    return sd;
}

FlCstr 
fl_cstr_copy (FlCstr dest, const FlCstr source) 
{
    flm_assert(dest != NULL, "Destination cannot be NULL");
    flm_assert(source != NULL, "Source cannot be NULL");

    FlCstr d = dest;
    FlCstr ss = source;
    while(*ss)
    {
    	*d = *ss;
    	ss++;
    	d++;
    }
    return dest;
}

FlCstr 
fl_cstr_copy_n (FlCstr dest, const FlCstr source, size_t n)
{
    flm_assert(dest != NULL, "Destination cannot be NULL");
    flm_assert(source != NULL, "Source cannot be NULL");

    FlCstr d = dest;
    FlCstr ss = source;
    while(n--)
    {
    	*d = *ss;
    	ss++;
    	d++;
    }
    return dest;
}

FlCstr
fl_cstr_replace_char(const FlCstr src, const char chr, const FlCstr rplc)
{
    flm_assert(src != NULL, "Source cannot be NULL");
    flm_assert(rplc != NULL, "Replacement cannot be NULL");

	size_t src_size = strlen(src);
	size_t rplc_size = strlen(rplc);
	size_t dst_size = src_size;
	FlCstr dst = fl_cstr_new(src_size);
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
			fl_cstr_resize(&dst, (dst_size += rplc_size-1));
	        memcpy(dst+j, rplc, rplc_size);
        }
        else
        {
            fl_cstr_resize(&dst, (dst_size -= 1));
        }

		j+=rplc_size;
	}
	dst[dst_size] = FL_EOS;
	return dst;
}

bool
cstr_match_backw(const FlCstr str1, const FlCstr str2, int n)
{
    flm_assert(str1 != NULL, "Input str1 cannot be NULL");
    flm_assert(str2 != NULL, "Input str2 cannot be NULL");
   
    for (int i = n-1; i >= 0; i--)
    {
        if (str1[i] != str2[i])
            return false;
    }
    return true;
}

/**
 * Uses the Boyer-Moore-Horspool algorithm to search for occurrences of {needle} in {str}.
 * When a match is found the starting index of the match in src is used to set the same 
 * index in {map} to 1. Finally, iterating over {src} and {map}, the new FlCstr {dst} is built
 * replacing all the {needle} occurrences by {rplc}
 */
FlCstr
fl_cstr_replace(const FlCstr src, const FlCstr needle, const FlCstr rplc)
{
    flm_assert(src != NULL, "Source cannot be NULL");
    flm_assert(rplc != NULL, "Replacement cannot be NULL");

    size_t src_size = strlen(src);
    size_t needle_size = (size_t)-1;
    // If needle is NULL or it is longer than src, no need to replace anything
    if (needle == NULL || src_size < (needle_size = strlen(needle)))
        return fl_cstr_dup(src);

    FlCstr dst = NULL;
    if (needle_size == 0)
    {
        dst = fl_cstr_dup(rplc);
        fl_cstr_append(&dst, src);
        return dst;
    }

    FlDictionary table = fl_dictionary_new(sizeof(char), sizeof(size_t));
    for (int i=0; i < needle_size-1; i++)
        flm_dictionary_set(table, char, needle[i], size_t, needle_size - i - 1);
    if (!fl_dictionary_contains_key(table, needle + (needle_size-1)))
        flm_dictionary_set(table, char, needle[needle_size-1], size_t, needle_size);

    FlCstr strptr = src;
    FlCstr result = NULL;
    size_t rplc_size = strlen(rplc);
    short map[src_size];
    memset(map, 0, src_size * sizeof(short));
    size_t newlength = src_size;
    int i = 0;
    for(i = needle_size-1; i < src_size;)
    {
        // i delta
        int d = needle_size;
        int k = i - (needle_size-1);
        if (strptr[i] != needle[needle_size-1] || (needle_size > 1 && !cstr_match_backw(strptr+k, needle, needle_size-1)))
        {
            int *pi = (int*)fl_dictionary_get_val(table, strptr+i);
            if (pi != NULL)
                d = *pi;
        }
        else
        {
            map[k] = 1;
            newlength += abs(rplc_size - needle_size);           
        }
        i += d;
    }
    fl_dictionary_delete(table);

    dst = fl_cstr_new(newlength);
    // spi = source pointer index
    // mpi = map pointer index
    // dpi = dest pointer index
    for (int spi=0, mpi=0, dpi=0; mpi < src_size;)
    {
        if (map[mpi])
        {
            for (int j=mpi; j < mpi+rplc_size; j++)
                dst[dpi++] = rplc[j-mpi];
            spi += needle_size;
            mpi += needle_size;
        }
        else
        {
            dst[dpi++] = src[spi++];
            mpi++;
        }
    }
    return dst;
}

bool
fl_cstr_contains(const FlCstr str, const FlCstr needle)
{
    return fl_cstr_find(str, needle) != NULL;
}

/**
 * Uses the Boyer-Moore-Horspool algorithm to search for occurrences of {needle} in {str}
 */
FlCstr
fl_cstr_find(const FlCstr str, const FlCstr needle)
{
    flm_assert(str != NULL, "Source cannot be NULL");

    size_t str_size = strlen(str);
    size_t needle_size = (size_t)-1;
    // If needle is NULL or it is longer than str, no need to replace anything
    if (needle == NULL || str_size < (needle_size = strlen(needle)))
        return NULL;

    if (needle_size == 0)
        return str;

    FlDictionary table = fl_dictionary_new(sizeof(char), sizeof(size_t));
    for (int i=0; i < needle_size; i++)
        flm_dictionary_set(table, char, needle[i], size_t, needle_size - i - 1);
    if (!fl_dictionary_contains_key(table, needle + (needle_size-1)))
        flm_dictionary_set(table, char, needle[needle_size-1], size_t, needle_size);

    FlCstr strptr = str;
    FlCstr result = NULL;
    for(int i = needle_size-1; i < str_size;)
    {
        int k = i - (needle_size-1);
        if (strptr[i] != needle[needle_size-1] || (needle_size > 1 && !cstr_match_backw(strptr+k, needle, needle_size-1)))
        {
            int *pi = (int*)fl_dictionary_get_val(table, strptr+i);
            i += (pi == NULL ? needle_size : *pi);
            continue;
        }
        result = strptr+k;
        break;
    }
    fl_dictionary_delete(table);
    return result;
}

FlCstr*
fl_cstr_append(FlCstr *dst, const FlCstr str)
{
	size_t length_dst = strlen(*dst);
	size_t length_str = strlen(str);
	fl_cstr_resize(dst, length_dst + length_str + 1);
	fl_cstr_copy_n((*dst)+length_dst, str, length_str);
	(*dst)[length_dst+length_str] = FL_EOS;
	return dst;
}

FlCstr*
fl_cstr_append_char(FlCstr *dst, char c)
{
	size_t length = strlen((*dst));
	fl_cstr_resize(dst, length + 1);
	(*dst)[length] = c;
	(*dst)[length+1] = FL_EOS;
	return dst;	
}

FlCstr
fl_cstr_join(FlVector vector, FlCstr glue)
{
    size_t glue_length = strlen(glue);
    FlCstr str = fl_cstr_new(0);
    size_t i=0, end = fl_vector_length(vector);
    while (i < end)
    {
    	// Current element and its length
    	FlCstr el = flm_vector_get(vector, FlCstr, i);
    	size_t el_length = strlen(el);
    	// Str current length
    	size_t str_length = strlen(str);
    	// Glue length depends on end variable, when el is the final element, glue won't be appended
    	glue_length = end > 0 ? glue_length : 0;
    	
    	// Resize str to allow new element (and glue if it is necessary)
    	size_t newSize = str_length + el_length + glue_length;

    	fl_cstr_resize(&str, newSize);
    	fl_cstr_append(&str, el);
    	// If is not the end, append the glue
    	if (i < end-1)
    		fl_cstr_append(&str, glue);
        i++;
    }
    return str;
}

FlCstr
fl_char_join(FlVector vector, FlCstr glue)
{
    size_t glue_length = strlen(glue);
    FlCstr str = fl_cstr_new(0);
    size_t i=0, end = fl_vector_length(vector);
    while (i < end)
    {
        // Current element and its length
        char el = flm_vector_get(vector, char, i);
        // Str current length
        size_t str_length = strlen(str);
        // Glue length depends on end variable, when el is the final element, glue won't be appended
        glue_length = end > 0 ? glue_length : 0;
        
        // Resize str to allow new element (and glue if it is necessary)
        size_t newSize = str_length + 1 + glue_length;

        fl_cstr_resize(&str, newSize);
        fl_cstr_append_char(&str, el);
        // If is not the end, append the glue
        if (i < end-1)
            fl_cstr_append(&str, glue);
        i++;
    }
    return str;
}
