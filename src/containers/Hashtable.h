#ifndef FL_HASHTABLE_H
#define FL_HASHTABLE_H

#include "../Array.h"

typedef struct FlHashtable* FlHashtable;

/* -------------------------------------------------------------
* {datatype: size_t (*)(FlByte*,size_t) FlHashtableHashFunc}
* -------------------------------------------------------------
* Hash function used by FlHashtable to hash they keys.
* It receives a pointer to a key element and its size, and return
* an unsigned integer.
* -------------------------------------------------------------
*/
typedef size_t(*FlHashtableHashFunc)(const FlByte *key, size_t kdtsize) ;

/* -------------------------------------------------------------
* {datatype: void(*)(void*,size_t,void*,size_t) FlHashtableDeleteKvpFunc}
* -------------------------------------------------------------
* Function used by FlHashtable to clean up entries on remove, clear or delete.
* It receives the key and value to be cleaned up and its respectives sizes.
* -------------------------------------------------------------
*/
typedef void(*FlHashtableDeleteKvpFunc)(void *key, size_t kdtsize, void *value, size_t vdtsize) ;

/* -------------------------------------------------------------
* {datatype: struct FlHashtableArgs}
* -------------------------------------------------------------
* Used to build a new FlHashtable. {key_size} and {value_size} members
* are used to track the size of both of them, they are mandatory. {hash_function}
* and {cleanup_function} can be NULL, and in that case, both will use a default version
* of a hash and clean up function.
* {buckets_count} is used to set the starting number of buckets to allocate. By default
* it is 103
* -------------------------------------------------------------
*/
struct FlHashtableArgs {
    size_t key_size;
    size_t value_size;
    size_t buckets_count;
    FlHashtableHashFunc hash_function;
    FlHashtableDeleteKvpFunc cleanup_function;
};

/* -------------------------------------------------------------
* {function: fl_hashtable_new}
* -------------------------------------------------------------
* Creates a new hashtable that will handle entries with key
* of size {kdtsize} and values of size {vdtsize}. The hashtable will use
* a default {hash_function} (djb2 hash function) and a default {cleanup_function}
* (check {fl_hashtable_delete_kvp}).
* -------------------------------------------------------------
* {param: size_t kdtsize} Size of the keys
* {param: size_t vdtsize} Size of the values
* {param: size_t (*)(FlByte*,size_t) hashfunc} Function to hash the key
* -------------------------------------------------------------
* {return: FlHashtable} Newly created hashtable
* -------------------------------------------------------------
*/
FlHashtable fl_hashtable_new(size_t kdtsize, size_t vdtsize);

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
* {function: fl_hashtable_delete_kvp}
* -------------------------------------------------------------
* Default function used by the Hashtable to release the memory
* used by a key-value pair when no {cleanup_function} is provided.
* If the user provides its own function to clean up elements,
* the function MUST check for NULL values on both key and value,
* sometimes hashtable will call it passing just the key or the value
* pointer. When key or value are not passed to the {cleanup_function},
* its size will be 0, it can be used as a flag.
* -------------------------------------------------------------
* {param: void* key} Pointer to a key element to be cleaned up
* {param: size_t kdtsize} Size of the key element
* {param: void* value} Pointer to a value element to be cleaned up
* {param: size_t vdtsize} Size of the value element
* -------------------------------------------------------------
* {return: void}
* -------------------------------------------------------------
*/
void fl_hashtable_delete_kvp(void *key, size_t kdtsize, void *value, size_t vdtsize);

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

#endif /* FL_HASHTABLE_H */
