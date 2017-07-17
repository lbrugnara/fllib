#ifndef FL_DICTIONARY_H
#define FL_DICTIONARY_H

#include "KeyValuePair.h"
#include "List.h"
#include "../Array.h"

typedef struct FlDictionary* FlDictionary;

#define flm_dictionary_set(d, kt, k, vt, v) \
do {										\
	kt key = k;								\
	vt val = v;								\
	fl_dictionary_set(d, &key, &val);		\
} while (0)

/** 
 * Creates an empty dictionary with keys of {keydtsize} bytes
 * and values of {valdtsize} bytes
 */
FlDictionary fl_dictionary_new(size_t keydtsize, size_t valdtsize);

/**
 * Returns the current count of elements in {dictionary}
 */
size_t fl_dictionary_length(const FlDictionary dictionary);

/**
 * Returns the size of the type used as key for {dictionary}
 */
size_t fl_dictionary_key_dtsize(const FlDictionary dictionary);

/**
 * Returns the size of the type used as value for {dictionary}
 */
size_t fl_dictionary_val_dtsize(const FlDictionary dictionary);

/**
 * {@dictionary} Target dictionary to add a new value
 * {@key} key index
 * {@value} value to save in {key}
 * Adds the {key}-{value} pair into {dictionary}
 * Return a pointer to the inserted element
 */
FlKeyValuePair fl_dictionary_add(FlDictionary dictionary, const void *key, const void *value);

/**
 * {@dictionary} Target dictionary to add a new value
 * {@key} key index
 * {@value} value to save in {key}
 * If {key} exists, this function overrides its value with {val}. If not, works as {fl_dictionary_add}.
 * Return a pointer to the inserted element.
 */
FlKeyValuePair fl_dictionary_set(FlDictionary dict, const void *key, const void *val);

/**
 * Returns a pointer to the element with key {key}
 */
void* fl_dictionary_get_val(const FlDictionary dictionary, const void *key);

/**
 * If {val} exists, returns a pointer to its key 
 */
void* fl_dictionary_get_key(const FlDictionary dictionary, const void *val);

void* fl_dictionary_keys(const FlDictionary dictionary);
void* fl_dictionary_values(const FlDictionary dictionary);

/**
 * Returns a pointer to a list element (the internal representation of FlDictionary is an FlList of FlKeyValuePair elements)
 */
FlList fl_dictionary_to_list(const FlDictionary dictionary);

/**
 * Returns a pointer to an FlKeyValuePair element where the key is {key}
 */
FlKeyValuePair fl_dictionary_get_kvp(const FlDictionary dictionary, const void *key);

/**
 * Return true if {dictionary} contains a key with value {key}
 */
bool fl_dictionary_contains_key(const FlDictionary dictionary, const void *key);

/**
 * Return true if {dictionary} contains a value with value {value}
 */
bool fl_dictionary_contains_val(const FlDictionary dictionary, const void *value);

/**
 * Append {dictionary2} elements at the end of {dictionary}. Repeated keys are overrided
 * in {dictionary}
 */
void fl_dictionary_concat(FlDictionary dictionary, FlDictionary dictionary2);

/**
 * Creates a new FlDictionary that will contain all the elements of
 * {dictionary} and {dictionary2}. Repeated keys are overrided from {dictionary2}
 * in {dictionary}
 */
FlDictionary fl_dictionary_merge(const FlDictionary dictionary, const FlDictionary dictionary2);

/**
 * Remove from {dictionary}, the element with key {key}.
 * The value for the removed element will be copied in {dest} if it is provided
 */
bool fl_dictionary_remove(FlDictionary dictionary, const void *key, void *dest);

/**
 * Remove all elements from the dictionary
 */
void fl_dictionary_clear(FlDictionary dictionary);

/**
 * Free the memory reserved for dictionary 
 */
void fl_dictionary_delete(FlDictionary dictionary);

/**
 * Free the memory reserved for dictionary using {delete_handler} for each element.
 * The handler MUST free the memory used by each element
 */
void fl_dictionary_delete_h(FlDictionary dictionary, void (*delete_handler)(FlByte*));

/* -------------------------------------------------------------
 * {function: fl_dictionary_start}
 * -------------------------------------------------------------
 * Returns an {FlIterator} that points to the first element in {param: dict}
 * The element pointed by the {FlIterator} is an {FlKeyValuePair}
 * -------------------------------------------------------------
 * {param: const FlList dict} Target dictionary to be pointed by the iterator
 * -------------------------------------------------------------
 * {return: FlIterator} Iterator pointing to the first element in {param: dict}
 * -------------------------------------------------------------
 */
FlIterator fl_dictionary_start(const FlDictionary dict);

/* -------------------------------------------------------------
 * {function: fl_dictionary_end}
 * -------------------------------------------------------------
 * Returns an {FlIterator} that points to the past-last-nth element in {param: dict}
 * The FlIterator returned by this function MUST NOT be dereferenced by fl_iterator_value.
 * The element pointed by the {FlIterator} is an {FlKeyValuePair}
 * -------------------------------------------------------------
 * {param: const FlList dict} Target dictionary to be pointed by the iterator
 * -------------------------------------------------------------
 * {return: FlIterator} Iterator pointing beyond the last element in {param: dict}
 * -------------------------------------------------------------
 */
FlIterator fl_dictionary_end(const FlDictionary dict);

#endif /* FL_DICTIONARY_H */
