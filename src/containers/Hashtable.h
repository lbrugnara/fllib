#ifndef FL_HASHTABLE_H
#define FL_HASHTABLE_H

#include "../Array.h"

typedef struct FlHashtable* FlHashtable;

#define FlHashtableOf(keyType, valueType) FlHashtable

typedef unsigned long(*FlHashtableHashFunc)(const FlByte *key);

typedef bool(*FlHashtableKeyComparerFunc)(const FlByte *key1, const FlByte *key2);

typedef void(*FlHashtableWriter)(FlByte **dest, const FlByte *src);

typedef void(*FlHashtableCleanupFunc)(void *key);

struct FlHashtableArgs {
    FlHashtableHashFunc hash_function;
    FlHashtableKeyComparerFunc key_comparer;
    FlHashtableCleanupFunc key_cleaner;
    FlHashtableCleanupFunc value_cleaner;
    FlHashtableWriter key_writer;
    FlHashtableWriter value_writer;
    double load_factor;
    size_t buckets_count;
};

FlHashtable fl_hashtable_new(FlHashtableHashFunc hash_func, FlHashtableKeyComparerFunc key_comparer);

/* -------------------------------------------------------------
* {function: fl_hashtable_new_args}
* -------------------------------------------------------------
* Creates a new hashtable as in {fl_hashtable_new}, but the user 
* can pass its own {hash_function} and {cleanup_function} to the
* constructor.
* -------------------------------------------------------------
* {param: struct FlHashtableArgs args} Arguments to build a new hashtable
* -------------------------------------------------------------
* {return: FlHashtable} Newly created hashtable
* -------------------------------------------------------------
*/
FlHashtable fl_hashtable_new_args(struct FlHashtableArgs args);

/* -------------------------------------------------------------
* {function: fl_hashtable_delete}
* -------------------------------------------------------------
* Releases the memory used by the hashtable and also frees the
* stored key and values. Use it when key and value are not mallocing
* memory for members (in the case of structs or unions).
* -------------------------------------------------------------
* {param: FlHashtable ht} Hashtable to release its memory
* -------------------------------------------------------------
* {return: void}
* -------------------------------------------------------------
*/
void fl_hashtable_delete(FlHashtable ht);

/* -------------------------------------------------------------
* {function: fl_hashtable_load_factor}
* -------------------------------------------------------------
* Returns the current load factor of the Hashtable
* -------------------------------------------------------------
* {param: FlHashtable ht} Target hashtable to retrieve its load factor
* -------------------------------------------------------------
* {return: double} Load factor
* -------------------------------------------------------------
*/
double fl_hashtable_load_factor(FlHashtable ht);

/* -------------------------------------------------------------
* {function: fl_hashtable_add}
* -------------------------------------------------------------
* Inserts a new element with value {value} under key {key}. If
* the key already exists it returns NULL without adding the value,
* else it inserts the new value and returns a pointer to it.
* -------------------------------------------------------------
* {param: FlHashtable ht} Table Hashtable to add the key-value pair
* {param: void* key} Pointer to a key element
* {param: void* value} Pointer to the value element
* -------------------------------------------------------------
* {return: void*} On success pointer to the new inserted element,
* NULL instead.
* -------------------------------------------------------------
*/
void* fl_hashtable_add(FlHashtable ht, const void *key, const void *value);

/* -------------------------------------------------------------
* {function: fl_hashtable_get}
* -------------------------------------------------------------
* If the hashtable contains the key, it returns a pointer to the
* mapped value. If the key does not exist in the container,
* it returns NULL
* -------------------------------------------------------------
* {param: FlHashtable ht} Container
* {param: void* key} Key value of the element whose mapped value is accessed
* -------------------------------------------------------------
* {return: void*} Pointer to the element mapped from the key value {key}
* -------------------------------------------------------------
*/
void* fl_hashtable_get(FlHashtable ht, const void *key);

/* -------------------------------------------------------------
* {function: fl_hashtable_set}
* -------------------------------------------------------------
* Inserts an element with value {value} under key {key}. If
* the key already exists, its mapped value will be replaced by the new one,
* and the old value will be cleaned up using the {cleanup_function}.
* If the key does not exist, this function inserts the new value.
* In both cases it returns a pointer to the inserted value
* -------------------------------------------------------------
* {param: FlHashtable ht} Table Hashtable to add the key-value pair
* {param: void* key} Pointer to a key element
* {param: void* value} Pointer to the value element
* -------------------------------------------------------------
* {return: void*} Pointer to the inserted value
* -------------------------------------------------------------
*/
void* fl_hashtable_set(FlHashtable ht, const void *key, const void *value);

/* -------------------------------------------------------------
* {function: fl_hashtable_remove}
* -------------------------------------------------------------
* This function removes the value mapped by key. If key exists
* both key and value will be cleaned up using the {cleanup_function}.
* Returns true if the key exists and both key and value are deleted,
* false if the key does not exist.
* -------------------------------------------------------------
* {param: FlHashtable ht} Container to remove the key-value pair
* {param: void* key} Pointer to a key element
* -------------------------------------------------------------
* {return: bool} true if key exists and both key and value are deleted.
* false if key does not exist or there is an error removing the element.
* -------------------------------------------------------------
*/
bool fl_hashtable_remove(FlHashtable ht, const void *key);

/* -------------------------------------------------------------
* {function: fl_hashtable_clear}
* -------------------------------------------------------------
* Removes all the key-value pairs and release the memory used
* by the element through the {cleanup_function} and reduces the memory
* used by the container.
* -------------------------------------------------------------
* {param: FlHashtable ht} Hashtable to be cleared
* -------------------------------------------------------------
* {return: void}
* -------------------------------------------------------------
*/
void fl_hashtable_clear(FlHashtable ht);

/* -------------------------------------------------------------
* {function: fl_hashtable_length}
* -------------------------------------------------------------
* Returns the current number of key-value pairs in the hashtable
* -------------------------------------------------------------
* {param: FlHashtable ht} Container to retrieve its current number of elements
* -------------------------------------------------------------
* {return: size_t} Number of elements stored in {ht}
* -------------------------------------------------------------
*/
size_t fl_hashtable_length(FlHashtable ht);

/* -------------------------------------------------------------
* {function: fl_hashtable_keys}
* -------------------------------------------------------------
* Returns an FlArray (dynamic array) containing all the keys
* present in the hashtable.
* -------------------------------------------------------------
* {param: FlHashtable ht} Hashtable to retrieve its keys
* -------------------------------------------------------------
* {return: void*} Pointer to a dynamic array containing all the keys in {ht}
* -------------------------------------------------------------
*/
FlArray fl_hashtable_keys(FlHashtable ht);

/* -------------------------------------------------------------
* {function: fl_hashtable_values}
* -------------------------------------------------------------
* Returns an FlArray (dynamic array) containing all the values
* present in the hashtable.
* -------------------------------------------------------------
* {param: FlHashtable ht} Hashtable to retrieve its values
* -------------------------------------------------------------
* {return: void*} Pointer to a dynamic array containing all the values in {ht}
* -------------------------------------------------------------
*/
FlArray fl_hashtable_values(FlHashtable ht);

/* -------------------------------------------------------------
* {function: fl_hashtable_has_key}
* -------------------------------------------------------------
* Returns true if key is present in the hashtable
* -------------------------------------------------------------
* {param: FlHashtable ht} {key} will be searched on this hashtable
* {param: void* key} Key to be searched in {ht}
* -------------------------------------------------------------
* {return: bool} true if {key} is present in hashtable, otherwise false.
* -------------------------------------------------------------
*/
bool fl_hashtable_has_key(FlHashtable ht, const void *key);

/* -------------------------------------------------------------
* {function: fl_hashtable_resize}
* -------------------------------------------------------------
* Resizes hashtable to contain {nbuckets} buckets
* -------------------------------------------------------------
* {param: FlHashtable ht} Hashtable to be resized
* {param: size_t nbuckets} Number of buckets to use in {ht}
* -------------------------------------------------------------
* {return: void}
* -------------------------------------------------------------
*/
void fl_hashtable_resize(FlHashtable ht, size_t nbuckets);

/* -------------------------------------------------------------
* {function: fl_hashtable_buckets_count}
* -------------------------------------------------------------
* Returns the number of buckets allocated by {ht}
* -------------------------------------------------------------
* {param: FlHashtable ht} Hashtable to retrieve its buckets count 
* -------------------------------------------------------------
* {return: size_t} Number of buckets allocated into {ht}
* -------------------------------------------------------------
*/
size_t fl_hashtable_buckets_count(FlHashtable ht);

void fl_hashtable_cleaner_pointer(void *obj);

void fl_hashtable_writer_string(FlByte **dest, const FlByte *src);
void fl_hashtable_writer_int(FlByte **dest, const FlByte *src);
void fl_hashtable_writer_char(FlByte **dest, const FlByte *src);
void fl_hashtable_writer_sizet(FlByte **dest, const FlByte *src);

unsigned long fl_hashtable_hash_pointer(const FlByte *key);
unsigned long fl_hashtable_hash_string(const FlByte *key);
unsigned long fl_hashtable_hash_int(const FlByte *key);
unsigned long fl_hashtable_hash_char(const FlByte *key);
unsigned long fl_hashtable_hash_sizet(const FlByte *key);

bool fl_hashtable_compare_pointer(const FlByte *key1, const FlByte *key2);
bool fl_hashtable_compare_string(const FlByte *key1, const FlByte *key2);
bool fl_hashtable_compare_int(const FlByte *key1, const FlByte *key2);
bool fl_hashtable_compare_char(const FlByte *key1, const FlByte *key2);
bool fl_hashtable_compare_sizet(const FlByte *key1, const FlByte *key2);

#endif /* FL_HASHTABLE_H */
