#ifndef FL_HASHTABLE_H
#define FL_HASHTABLE_H

#include "../Array.h"
#include "Container.h"

typedef struct FlHashtable* FlHashtable;

/*
 * Type: unsigned long(*FlHashtableHashFunction)(const FlByte *key)
 *
 * Hash function that receives a key and returns an unsigned long.
 * By default if FlHashtableArgs.hash_function is not present, the
 * hashtable will use the fl_hashtable_hash_pointer function.
 * Another hash functions the hashtable module provides through the 
 * fl_hashtable_hash_* family are (all use the djb2 hash):
 *   string
 *   int
 *   char
 *   sizet
 *
 */
typedef unsigned long(*FlHashtableHashFunction)(const FlByte *key);

/* -------------------------------------------------------------
* {datatype: struct FlHashtableArgs}
* -------------------------------------------------------------
* This struct let's the caller configure the behavior of the
* hashtable when provided to the fl_hashtable_new_args function.
* Members:
*   - hash_function: Function to hash keys. If not present, fl_hashtable_hash_pointer will be used
*   - key_comparer: Function that compares keys. If not present, fl_hashtable_compare_pointer will be used
*   - key_cleaner: Function that frees a key when it is discarded. If not present, key remains untouched
*   - value_cleaner: Function that frees a value when it is discarded. If not present, value remains untouched
*   - key_allocator: Function that allocates and writes the key into the hashtable. If not present, the hashtable works with pointers
*   - value_allocator: Function that allocates and writes the value into the hashtable. If not present, the hashtable works with pointers
*   - load_factor: Hashtable's load factor. If not present, it takes a value of 0.75
*   - buckets_count: Number of buckets of the hashtable. If not present, a default value of NBUCKETS=83 is used
* -------------------------------------------------------------
*/
struct FlHashtableArgs {
    FlHashtableHashFunction hash_function;
    FlContainerEqualsFunction key_comparer;
    FlContainerCleanupFunction key_cleaner;
    FlContainerCleanupFunction value_cleaner;
    FlContainerAllocatorFunction key_allocator;
    FlContainerAllocatorFunction value_allocator;
    double load_factor;
    size_t buckets_count;
};

/* -------------------------------------------------------------
* {function: fl_hashtable_new}
* -------------------------------------------------------------
* Creates a hashtable
* -------------------------------------------------------------
* {param: FlHashtableHashFunction hash_func} Function to hash keys. 
*    The hashtable uses fl_hashtable_hash_pointer if not present
* {param: FlContainerEqualsFunction key_comparer} Function to 
*    compare keys. Hashtable uses fl_hashtable_compare_pointer if 
*    not present
* {param: FlContainerCleanupFunc key_cleaner} Function to clean 
*    the memory used by key. If not present, the key remains 
*    untouched
* {param: FlContainerCleanupFunc value_cleaner} Function to clean 
*    the memory used by value. If not present, the value remains 
*    untouched
* {param: FlContainerAllocatorFunction key_allocator} Allocates memory and 
*    writes the key to the hashtable's entry. If not present, the 
*    hashtable uses pointers
* {param: FlContainerAllocatorFunction value_allocator} Allocates memory and 
*    writes the value to the hashtable's entry. If not present, 
*    the hastable uses pointers
* -------------------------------------------------------------
* {return: FlHashtable} The new hashtable
* -------------------------------------------------------------
*/
FlHashtable fl_hashtable_new(
    FlHashtableHashFunction hash_func, 
    FlContainerEqualsFunction key_comparer, 
    FlContainerCleanupFunction key_cleaner, 
    FlContainerCleanupFunction value_cleaner, 
    FlContainerAllocatorFunction key_allocator, 
    FlContainerAllocatorFunction value_allocator
);

/*
 * Function: fl_hashtable_new_args
 *
 * Creates a new hashtable as in {fl_hashtable_new}, but the user 
 * can pass its own {hash_function} and {cleanup_function} to the
 * constructor.
 *
 * struct FlHashtableArgs args - Arguments to build a new hashtable
 *
 * {return: FlHashtable} Newly created hashtable
 *
 */
FlHashtable fl_hashtable_new_args(struct FlHashtableArgs args);

/*
 * Function: fl_hashtable_delete
 *
 * Releases the memory used by the hashtable and also frees the
 * stored key and values. Use it when key and value are not mallocing
 * memory for members (in the case of structs or unions).
 *
 * FlHashtable ht - Hashtable to release its memory
 *
 * {return: void}
 *
 */
void fl_hashtable_delete(FlHashtable ht);

/*
 * Function: fl_hashtable_load_factor
 *
 * Returns the current load factor of the Hashtable
 *
 * FlHashtable ht - Target hashtable to retrieve its load factor
 *
 * {return: double} Load factor
 *
 */
double fl_hashtable_load_factor(FlHashtable ht);

/*
 * Function: fl_hashtable_add
 *
 * Inserts a new element with value {value} under key {key}. If
 * the key already exists it returns NULL without adding the value,
 * else it inserts the new value and returns a pointer to it.
 *
 * FlHashtable ht - Table Hashtable to add the key-value pair
 * void* key - Pointer to a key element
 * void* value - Pointer to the value element
 *
 * {return: void*} On success pointer to the new inserted element,
 * NULL instead.
 *
 */
void* fl_hashtable_add(FlHashtable ht, const void *key, const void *value);

/*
 * Function: fl_hashtable_get
 *
 * If the hashtable contains the key, it returns a pointer to the
 * mapped value. If the key does not exist in the container,
 * it returns NULL
 *
 * FlHashtable ht - Container
 * void* key - Key value of the element whose mapped value is accessed
 *
 * {return: void*} Pointer to the element mapped from the key value {key}
 *
 */
void* fl_hashtable_get(FlHashtable ht, const void *key);

/*
 * Function: fl_hashtable_set
 *
 * Inserts an element with value {value} under key {key}. If
 * the key already exists, its mapped value will be replaced by the new one,
 * and the old value will be cleaned up using the {cleanup_function}.
 * If the key does not exist, this function inserts the new value.
 * In both cases it returns a pointer to the inserted value
 *
 * FlHashtable ht - Table Hashtable to add the key-value pair
 * void* key - Pointer to a key element
 * void* value - Pointer to the value element
 *
 * {return: void*} Pointer to the inserted value
 *
 */
void* fl_hashtable_set(FlHashtable ht, const void *key, const void *value);

/*
 * Function: fl_hashtable_remove
 *
 *  This function removes the value mapped by key. If key exists and
 *  *clean* is true, both key and value will be cleaned up using the {cleanup_function}.
 *
 * Parameters:
 *  ht - Container to remove the key-value pair
 *  key - Pointer to a key element
 *  clean - Boolean to indicate if both key and value should be cleaned up
 *
 * Returns:
 *  bool - *true* if key exists and in case *clean* is true, also when both key and value are deleted.
 *   *false* otherwise.
 *
 */
bool fl_hashtable_remove(FlHashtable ht, const void *key, bool clean);

/*
 * Function: fl_hashtable_clear
 *
 * Removes all the key-value pairs and release the memory used
 * by the element through the {cleanup_function} and reduces the memory
 * used by the container.
 *
 * FlHashtable ht - Hashtable to be cleared
 *
 * {return: void}
 *
 */
void fl_hashtable_clear(FlHashtable ht);

/*
 * Function: fl_hashtable_length
 *
 * Returns the current number of key-value pairs in the hashtable
 *
 * FlHashtable ht - Container to retrieve its current number of elements
 *
 * {return: size_t} Number of elements stored in {ht}
 *
 */
size_t fl_hashtable_length(FlHashtable ht);

/*
 * Function: fl_hashtable_keys
 *
 * Returns an FlArray (dynamic array) containing all the keys
 * present in the hashtable.
 *
 * FlHashtable ht - Hashtable to retrieve its keys
 *
 * {return: void*} Pointer to a dynamic array containing all the keys in {ht}
 *
 */
FlArray fl_hashtable_keys(FlHashtable ht);

/*
 * Function: fl_hashtable_values
 *
 * Returns an FlArray (dynamic array) containing all the values
 * present in the hashtable.
 *
 * FlHashtable ht - Hashtable to retrieve its values
 *
 * {return: void*} Pointer to a dynamic array containing all the values in {ht}
 *
 */
FlArray fl_hashtable_values(FlHashtable ht);

/*
 * Function: fl_hashtable_has_key
 *
 * Returns true if key is present in the hashtable
 *
 * FlHashtable ht} {key - will be searched on this hashtable
 * void* key} Key to be searched in {ht -
 *
 * {return: bool} true if {key} is present in hashtable, otherwise false.
 *
 */
bool fl_hashtable_has_key(FlHashtable ht, const void *key);

/*
 * Function: fl_hashtable_resize
 *
 * Resizes hashtable to contain {nbuckets} buckets
 *
 * FlHashtable ht - Hashtable to be resized
 * size_t nbuckets} Number of buckets to use in {ht -
 *
 * {return: void}
 *
 */
void fl_hashtable_resize(FlHashtable ht, size_t nbuckets);

/*
 * Function: fl_hashtable_buckets_count
 *
 * Returns the number of buckets allocated by {ht}
 *
 * FlHashtable ht - Hashtable to retrieve its buckets count 
 *
 * {return: size_t} Number of buckets allocated into {ht}
 *
 */
size_t fl_hashtable_buckets_count(FlHashtable ht);

/*
 * Function: fl_hashtable_hash_pointer
 *
 * Creates a hash from the pointer {key}. It uses djb2 hash function
 *
 * const FlByte* key - Key to be hashed
 *
 * {return: unsigned long} Hash of {key}
 *
 */
unsigned long fl_hashtable_hash_pointer(const FlByte *key);

/*
 * Function: fl_hashtable_hash_string
 *
 * Creates a hash from the null terminated string pointed by {key}. 
 * It uses djb2 hash function
 *
 * const FlByte* key - string to be hashed
 *
 * {return: unsigned long} Hash of the string pointed by {key}
 *
 */
unsigned long fl_hashtable_hash_string(const FlByte *key);

/*
 * Function: fl_hashtable_hash_int
 *
 * Creates a hash from the integer pointed by {key}. It uses djb2 hash function
 *
 * const FlByte* key - integer to be hashed
 *
 * {return: unsigned long} Hash of the integer pointed by {key}
 *
 */
unsigned long fl_hashtable_hash_int(const FlByte *key);

/*
 * Function: fl_hashtable_hash_char
 *
 * Creates a hash from the char pointed by {key}. It uses djb2 hash function
 *
 * const FlByte* key - char to be hashed
 *
 * {return: unsigned long} Hash of the char pointed by {key}
 *
 */
unsigned long fl_hashtable_hash_char(const FlByte *key);

/*
 * Function: fl_hashtable_hash_sizet
 *
 * Creates a hash from the size_t object pointed by {key}. 
 * It uses djb2 hash function
 *
 * const FlByte* key - size_t object to be hashed
 *
 * {return: unsigned long} Hash of the size_t object pointed by {key}
 *
 */
unsigned long fl_hashtable_hash_sizet(const FlByte *key);

#endif /* FL_HASHTABLE_H */
