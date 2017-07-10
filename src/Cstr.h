#ifndef FL_C_STR_H
#define FL_C_STR_H

#include <stdarg.h>
#include <string.h>

#include "Types.h"
#include "containers/Vector.h"

/**
 * {length} Length of the string to be created (no need to take in mind the \0, this function adds it)
 * Alloc memory for a new FlCstr of size {length} plus null terminated character.
 */
FlCstr fl_cstr_new(size_t length);

/**
 * {str} String to free
 * Free the memory used by {str}
 */
void fl_cstr_delete(FlCstr str);

/**
 * {dst} String to resize
 * Resizes cstr {dst} to allow up to {length} elements (plus null terminated character)
 */
void fl_cstr_resize(FlCstr *dst, size_t length);

/**
 * {s} Source string
 * Copy {source} into {dest}. {dest} should has enough
 * size to save {source}
 */
FlCstr fl_cstr_copy(FlCstr dest, const FlCstr source);

/**
 * {s} Source string
 * Copy {n} bytes of {source} into {dest}. {dest} should has enough
 * size to save {source}
 */
FlCstr fl_cstr_copy_n(FlCstr dest, const char* source, size_t n);

/**
 * {s} Source string
 * Alloc memory to create a duplicate of the {s} string
 */
FlCstr fl_cstr_dup(const FlCstr s);

/**
 * Duplicates {s} replacing modifiers %c,%d and %s
 * using stdarg. Handles all the memory allocation
 */
FlCstr fl_cstr_vdup (const FlCstr s, ...);

/**
 * Duplicates {s} replacing modifiers %c,%d and %s
 * using va_args. Handles all the memory allocation
 */
FlCstr fl_cstr_vadup (const FlCstr s, va_list args);

/**
 * {input} Source string
 * Returns a {FlVector} containing all the chars of the input at each index
 */
FlVector fl_cstr_split(const FlCstr input);

/**
 * {input} Source string
 * Returns an array of chars containing all the chars of the input at each index.
 * The array is allocated by an fl_array_new call
 */
char* fl_cstr_to_array (const FlCstr input);

/**
 * {src} Source string
 * {chr} Char to search for
 * {rplc} String to replace when {chr} is found
 * For each {chr} occurrences in {src} this function replace them with {rplc}. Alloc all
 * the necessary memory to return a new FlCstr
 */
FlCstr fl_cstr_replace_char(const FlCstr src, const char chr, const FlCstr rplc);

/**
 * {src} Source string
 * {needle} String to find and replace
 * {rplc} String to replace when {chr} is found
 * For each {needle} occurrences in {src} this function replace them with {rplc}. Alloc all
 * the necessary memory to return a new FlCstr
 */
FlCstr fl_cstr_replace(const FlCstr src, const FlCstr needle, const FlCstr rplc);

size_t fl_cstr_replace_n(const FlCstr src, size_t src_size, const FlCstr needle, size_t needle_size, const FlCstr rplc, size_t rplc_size, FlCstr *dst);

/**
 * {str} Target string
 * {needle} String to find in {str}
 * Returns true if an occurrence of {needle} exists in {str}
 */
bool fl_cstr_contains(const FlCstr str, const FlCstr needle);

/**
 * {str} Target string
 * {needle} String to find in {str}
 * Returns a pointer to the first occurrence of {needle} in {str}
 */
FlCstr fl_cstr_find(const FlCstr str, const FlCstr needle);

/**
 * {dst} Target string
 * {str} Source string
 * Appends {str} into {dst}. It handles all the memory management to resize {dst}. 
 * Return {dest}
 */
FlCstr* fl_cstr_append(FlCstr *dst, const FlCstr str);

/**
 * {dst} Target string
 * {c} Source char
 * Appends {c} into {dst}. It handles all the memory management to resize {dst}.
 * Return {dest}
 */
FlCstr* fl_cstr_append_char(FlCstr *dst, char c);

/**
 * {vector} Source vector of {FlCstr}s
 * {glue} String used to link all the elements in {vector}
 * Joins all the FlCstr elements of {vector} with FlCstr {glue}
 * Alloc the memory to return a new FlCstr
 */
FlCstr fl_cstr_join(FlVector vector, FlCstr glue);

/**
 * {vector} Source vector of chars
 * {glue} String used to link all the chars in {vector}
 * Joins all the char elements of {vector} with FlCstr {glue}
 * Alloc the memory to return a new FlCstr
 */
FlCstr fl_char_join(FlVector vector, FlCstr glue);

FlCstr fl_cstr_concat(FlCstr *strings);

#define flm_cstr_equals(cstr1, cstr2) (strcmp((cstr1), (cstr2)) == 0)
#define flm_cstr_nequals(cstr1, cstr2, n) (strncmp((cstr1), (cstr2), (n)) == 0)

#endif /* FL_C_STR_H */
