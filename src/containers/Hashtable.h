#ifndef FL_HASHTABLE_H
#define FL_HASHTABLE_H

#include "../Array.h"

typedef struct FlHashtable* FlHashtable;

/* -------------------------------------------------------------
* {datatype: unsigned long(*FlHashtableHashFunc)(const FlByte *key)}
* -------------------------------------------------------------
* Hash function that receives a key and returns an unsigned long.
* By default if FlHashtableArgs.hash_function is not present, the
* hashtable will use the fl_hashtable_hash_pointer function.
* Another hash functions the hashtable module provides through the 
* fl_hashtable_hash_* family are (all use the djb2 hash):
*   string
*   int
*   char
*   sizet
* -------------------------------------------------------------
*/
typedef unsigned long(*FlHashtableHashFunc)(const FlByte *key);

/* -------------------------------------------------------------
* {datatype: bool(*FlHashtableKeyComparerFunc)(const FlByte *key1, const FlByte *key2)}
* -------------------------------------------------------------
* Function that compares two sequence of bytes representing keys
* in the hashtable.
* If FlHashtableArgs.key_comparer is not provided in the hashtable's
* new function, it will use the fl_hashtable_compare_pointer function.
* Another key comparers the hashtable module provides through the 
* fl_hashtable_compare_* family are:
*   string
*   int
*   char
*   sizet
* -------------------------------------------------------------
*/
typedef bool(*FlHashtableKeyComparerFunc)(const FlByte *key1, const FlByte *key2);

/* -------------------------------------------------------------
* {datatype: void(*FlHashtableWriter)(FlByte **dest, const FlByte *src)}
* -------------------------------------------------------------
* Functions that writes the sequence of bytes in {src} to the 
* destination object {dest}. This function MUST allocate the 
* memory before writing to the object.
* By default hashtables work with pointers for both key and value.
* In order to use the actual value behind the pointers, the caller
* must provide a writer function to know how to save/restore the
* key or value from within the hashtable.
* Writers this module provide for the FlHashtableArgs.key_writer and
* FlHashtableArgs.value_writer objects in the family of the 
* fl_hashtable_writer_* functions family are:
*   string
*   int
*   char
*   sizet
* -------------------------------------------------------------
*/
typedef void(*FlHashtableWriter)(FlByte **dest, const FlByte *src);

/* -------------------------------------------------------------
* {datatype: void(*FlHashtableCleanupFunc)(void *key)}
* -------------------------------------------------------------
* Function can be use with FlHashtableArgs.key_cleaner and 
* FlHashtableArgs.value_cleaner to handle key/value freeing.
* This module provides the fl_hashtable_cleaner_pointer function
* that simply frees the value pointer by {obj}.
* -------------------------------------------------------------
*/
typedef void(*FlHashtableCleanupFunc)(void *obj);

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
*   - key_writer: Function that writes the key into the hashtable. If not present, the hashtable works with pointers
*   - value_writer: Function that writes the value into the hashtable. If not present, the hashtable works with pointers
*   - load_factor: Hashtable's load factor. If not present, it takes a value of 0.75
*   - buckets_count: Number of buckets of the hashtable. If not present, a default value of NBUCKETS=83 is used
* -------------------------------------------------------------
*/
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

/* -------------------------------------------------------------
* {function: fl_hashtable_new}
* -------------------------------------------------------------
* Creates a hashtable
* -------------------------------------------------------------
* {param: FlHashtableHashFunc hash_func} Function to hash keys. 
*    The hashtable uses fl_hashtable_hash_pointer if not present
* {param: FlHashtableKeyComparerFunc key_comparer} Function to 
*    compare keys. Hashtable uses fl_hashtable_compare_pointer if 
*    not present
* {param: FlHashtableCleanupFunc key_cleaner} Function to clean 
*    the memory used by key. If not present, the key remains 
*    untouched
* {param: FlHashtableCleanupFunc value_cleaner} Function to clean 
*    the memory used by value. If not present, the value remains 
*    untouched
* {param: FlHashtableWriter key_writer} Allocates memory and 
*    writes the key to the hashtable's entry. If not present, the 
*    hashtable uses pointers
* {param: FlHashtableWriter value_writer} Allocates memory and 
*    writes the value to the hashtable's entry. If not present, 
*    the hastable uses pointers
* -------------------------------------------------------------
* {return: FlHashtable} The new hashtable
* -------------------------------------------------------------
*/
FlHashtable fl_hashtable_new(
    FlHashtableHashFunc hash_func, 
    FlHashtableKeyComparerFunc key_comparer, 
    FlHashtableCleanupFunc key_cleaner, 
    FlHashtableCleanupFunc value_cleaner, 
    FlHashtableWriter key_writer, 
    FlHashtableWriter value_writer
);

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

/* -------------------------------------------------------------
* {function: fl_hashtable_cleaner_pointer}
* -------------------------------------------------------------
* Frees the memory allocated by {obj} by calling {fl_free}
* -------------------------------------------------------------
* {param: void* obj} Object to free
* -------------------------------------------------------------
* {return: void}
* -------------------------------------------------------------
*/
void fl_hashtable_cleaner_pointer(void *obj);

/* -------------------------------------------------------------
* {function: fl_hashtable_writer_string}
* -------------------------------------------------------------
* Copies the char* pointed by {src} into {dest} by allocating
* space to make {src} and the NULL terminator fit in {dest}
* -------------------------------------------------------------
* {param: FlByte** dest} Destination object. It is allocated by this function
* {param: const FlByte* src} Sequence of bytes containing a NULL terminated string
* -------------------------------------------------------------
* {return: void}
* -------------------------------------------------------------
*/
void fl_hashtable_writer_string(FlByte **dest, const FlByte *src);

/* -------------------------------------------------------------
* {function: fl_hashtable_writer_int}
* -------------------------------------------------------------
* Copies the integer pointed by {src} into {dest} by allocating
* space to fit an int.
* -------------------------------------------------------------
* {param: FlByte** dest} Destination object. It is allocated by this function
* {param: const FlByte* src} Pointer to an integer
* -------------------------------------------------------------
* {return: void}
* -------------------------------------------------------------
*/
void fl_hashtable_writer_int(FlByte **dest, const FlByte *src);

/* -------------------------------------------------------------
* {function: fl_hashtable_writer_char}
* -------------------------------------------------------------
* Copies the char pointed by {src} into {dest} by allocating
* space to fit a char.
* -------------------------------------------------------------
* {param: FlByte** dest} Destination object. It is allocated by this function
* {param: const FlByte* src} Pointer to a char
* -------------------------------------------------------------
* {return: void}
* -------------------------------------------------------------
*/
void fl_hashtable_writer_char(FlByte **dest, const FlByte *src);

/* -------------------------------------------------------------
* {function: fl_hashtable_writer_sizet}
* -------------------------------------------------------------
* Copies the size_t pointed by {src} into {dest} by allocating
* space to fit a size_t object.
* -------------------------------------------------------------
* {param: FlByte** dest} Destination object. It is allocated by this function
* {param: const FlByte* src} Pointer to a size_t object
* -------------------------------------------------------------
* {return: void}
* -------------------------------------------------------------
*/
void fl_hashtable_writer_sizet(FlByte **dest, const FlByte *src);

/* -------------------------------------------------------------
* {function: fl_hashtable_hash_pointer}
* -------------------------------------------------------------
* Creates a hash from the pointer {key}. It uses djb2 hash function
* -------------------------------------------------------------
* {param: const FlByte* key} Key to be hashed
* -------------------------------------------------------------
* {return: unsigned long} Hash of {key}
* -------------------------------------------------------------
*/
unsigned long fl_hashtable_hash_pointer(const FlByte *key);

/* -------------------------------------------------------------
* {function: fl_hashtable_hash_string}
* -------------------------------------------------------------
* Creates a hash from the null terminated string pointed by {key}. 
* It uses djb2 hash function
* -------------------------------------------------------------
* {param: const FlByte* key} string to be hashed
* -------------------------------------------------------------
* {return: unsigned long} Hash of the string pointed by {key}
* -------------------------------------------------------------
*/
unsigned long fl_hashtable_hash_string(const FlByte *key);

/* -------------------------------------------------------------
* {function: fl_hashtable_hash_int}
* -------------------------------------------------------------
* Creates a hash from the integer pointed by {key}. It uses djb2 hash function
* -------------------------------------------------------------
* {param: const FlByte* key} integer to be hashed
* -------------------------------------------------------------
* {return: unsigned long} Hash of the integer pointed by {key}
* -------------------------------------------------------------
*/
unsigned long fl_hashtable_hash_int(const FlByte *key);

/* -------------------------------------------------------------
* {function: fl_hashtable_hash_char}
* -------------------------------------------------------------
* Creates a hash from the char pointed by {key}. It uses djb2 hash function
* -------------------------------------------------------------
* {param: const FlByte* key} char to be hashed
* -------------------------------------------------------------
* {return: unsigned long} Hash of the char pointed by {key}
* -------------------------------------------------------------
*/
unsigned long fl_hashtable_hash_char(const FlByte *key);

/* -------------------------------------------------------------
* {function: fl_hashtable_hash_sizet}
* -------------------------------------------------------------
* Creates a hash from the size_t object pointed by {key}. 
* It uses djb2 hash function
* -------------------------------------------------------------
* {param: const FlByte* key} size_t object to be hashed
* -------------------------------------------------------------
* {return: unsigned long} Hash of the size_t object pointed by {key}
* -------------------------------------------------------------
*/
unsigned long fl_hashtable_hash_sizet(const FlByte *key);

/* -------------------------------------------------------------
* {function: fl_hashtable_compare_pointer}
* -------------------------------------------------------------
* Compares the two keys by just applying the equality operator
* -------------------------------------------------------------
* {param: const FlByte* key1} One of the keys to be compared
* {param: const FlByte* key2} Another key to be compared
* -------------------------------------------------------------
* {return: bool} True if the keys are equals
* -------------------------------------------------------------
*/
bool fl_hashtable_compare_pointer(const FlByte *key1, const FlByte *key2);

/* -------------------------------------------------------------
* {function: fl_hashtable_compare_pointer}
* -------------------------------------------------------------
* Compares the two keys by comparing the string they are pointing
* to using the strcmp function. The strings must be null terminated.
* -------------------------------------------------------------
* {param: const FlByte* key1} One of the keys to be compared
* {param: const FlByte* key2} Another key to be compared
* -------------------------------------------------------------
* {return: bool} True if the keys are equals
* -------------------------------------------------------------
*/
bool fl_hashtable_compare_string(const FlByte *key1, const FlByte *key2);

/* -------------------------------------------------------------
* {function: fl_hashtable_compare_int}
* -------------------------------------------------------------
* Compares the two keys by just applying the equality operator
* on the dereferenced integers
* -------------------------------------------------------------
* {param: const FlByte* key1} One of the keys to be compared
* {param: const FlByte* key2} Another key to be compared
* -------------------------------------------------------------
* {return: bool} True if the keys are equals
* -------------------------------------------------------------
*/
bool fl_hashtable_compare_int(const FlByte *key1, const FlByte *key2);

/* -------------------------------------------------------------
* {function: fl_hashtable_compare_char}
* -------------------------------------------------------------
* Compares the two keys by just applying the equality operator
* on the dereferenced chars
* -------------------------------------------------------------
* {param: const FlByte* key1} One of the keys to be compared
* {param: const FlByte* key2} Another key to be compared
* -------------------------------------------------------------
* {return: bool} True if the keys are equals
* -------------------------------------------------------------
*/
bool fl_hashtable_compare_char(const FlByte *key1, const FlByte *key2);

/* -------------------------------------------------------------
* {function: fl_hashtable_compare_size_t}
* -------------------------------------------------------------
* Compares the two keys by just applying the equality operator
* on the dereferenced size_t objects
* -------------------------------------------------------------
* {param: const FlByte* key1} One of the keys to be compared
* {param: const FlByte* key2} Another key to be compared
* -------------------------------------------------------------
* {return: bool} True if the keys are equals
* -------------------------------------------------------------
*/
bool fl_hashtable_compare_sizet(const FlByte *key1, const FlByte *key2);

#endif /* FL_HASHTABLE_H */
