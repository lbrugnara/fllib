#ifndef FL_C_STR_H
#define FL_C_STR_H

#include <stdarg.h>
#include <string.h>

#include "Types.h"
#include "containers/Vector.h"

/**
 * {length} Length of the string to be created (no need to take in mind the \0, this function adds it)
 * Alloc memory for a new char* of size {length} plus null terminated character.
 */
char* fl_cstr_new(size_t length);

/**
 * {str} String to free
 * Free the memory used by {str}
 */
void fl_cstr_delete(char *str);

/**
 * {dst} String to resize
 * Resizes cstr {dst} to allow up to {length} elements (plus null terminated character)
 */
void fl_cstr_resize(char **dst, size_t length);

/**
 * {s} Source string
 * Copy {source} into {dest}. {dest} should has enough
 * size to save {source}
 */
char* fl_cstr_copy(char *dest, const char *source);

/**
 * {s} Source string
 * Copy {n} bytes of {source} into {dest}. {dest} should has enough
 * size to save {source}
 */
char* fl_cstr_copy_n(char *dest, const char *source, size_t n);

/**
 * {s} Source string
 * Alloc memory to create a duplicate of the {s} string
 */
char* fl_cstr_dup(const char *s);

/**
 * Create a duplicate of {n} characters of {s}
 */
char* fl_cstr_dup_n(const char *s, size_t n);

/**
 * Duplicates {s} replacing modifiers %c,%d and %s
 * using stdarg. Handles all the memory allocation
 */
char* fl_cstr_vdup (const char *s, ...);

/**
 * Duplicates {s} replacing modifiers %c,%d and %s
 * using va_args. Handles all the memory allocation
 */
char* fl_cstr_vadup (const char *s, va_list args);

/**
 * {input} Source string
 * Returns a {FlVector} containing all the chars of the input at each index
 */
FlVector fl_cstr_split(const char *input);

FlVector fl_cstr_split_by(const char *input, const char *separator);

/**
 * {input} Source string
 * Returns an array of chars containing all the chars of the input at each index.
 * The array is allocated by an fl_array_new call
 */
char* fl_cstr_to_array (const char *input);

/**
 * {src} Source string
 * {chr} Char to search for
 * {rplc} String to replace when {chr} is found
 * For each {chr} occurrences in {src} this function replace them with {rplc}. Alloc all
 * the necessary memory to return a new char*
 */
char* fl_cstr_replace_char(const char *src, const char chr, const char *rplc);

/**
 * {src} Source string
 * {needle} String to find and replace
 * {rplc} String to replace when {chr} is found
 * For each {needle} occurrences in {src} this function replace them with {rplc}. Alloc all
 * the necessary memory to return a new char*
 */
char* fl_cstr_replace(const char *src, const char *needle, const char *rplc);

size_t fl_cstr_replace_n(const char *src, size_t src_size, const char *needle, size_t needle_size, const char *rplc, size_t rplc_size, char **dst);

/**
 * {str} Target string
 * {needle} String to find in {str}
 * Returns true if an occurrence of {needle} exists in {str}
 */
bool fl_cstr_contains(const char *str, const char *needle);

/**
 * {str} Target string
 * {needle} String to find in {str}
 * Returns a pointer to the first occurrence of {needle} in {str}
 */
char* fl_cstr_find(const char *str, const char *needle);

/**
 * {dst} Target string
 * {str} Source string
 * Appends {str} into {dst}. It handles all the memory management to resize {dst}. 
 * Return {dest}
 */
char** fl_cstr_append(char **dst, const char *str);

/**
 * {dst} Target string
 * {c} Source char
 * Appends {c} into {dst}. It handles all the memory management to resize {dst}.
 * Return {dest}
 */
char** fl_cstr_append_char(char **dst, char c);

/**
 * {vector} Source vector of {char*}s
 * {glue} String used to link all the elements in {vector}
 * Joins all the char* elements of {vector} with char* {glue}
 * Alloc the memory to return a new char*
 */
char* fl_cstr_join(FlVector vector, char* glue);

/**
 * {vector} Source vector of chars
 * {glue} String used to link all the chars in {vector}
 * Joins all the char elements of {vector} with char* {glue}
 * Alloc the memory to return a new char*
 */
char* fl_char_join(FlVector vector, char* glue);

char* fl_cstr_concat(char **strings);

#define flm_cstr_equals(cstr1, cstr2) (strcmp((cstr1), (cstr2)) == 0)
#define flm_cstr_nequals(cstr1, cstr2, n) (strncmp((cstr1), (cstr2), (n)) == 0)

#endif /* FL_C_STR_H */
