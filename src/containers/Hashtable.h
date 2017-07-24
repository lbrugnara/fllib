#ifndef FL_HASHTABLE_H
#define FL_HASHTABLE_H

typedef struct FlHashtable* FlHashtable;

typedef size_t(*FlHashtableHashFunc)(FlByte *key, size_t kdtsize) ;
typedef void(*FlHashtableDeleteKvpFunc)(void*, size_t, void*, size_t) ;

/* -------------------------------------------------------------
* {function: fl_hashtable_new}
* -------------------------------------------------------------
* Creates a new hashtable that will handle entries with key
* of size {kdtsize} and values of size {vdtsize}. To hash
* the keys it accepts a pointer to a function that receives
* the key and its size and returns an unsigned integer. If the
* hash function is not provided, the hashtable will use a custom one
* -------------------------------------------------------------
* {param: size_t kdtsize} Size of the keys
* {param: size_t vdtsize} Size of the values
* {param: size_t (*)(FlByte*,size_t) hashfunc} Function to hash the key
* -------------------------------------------------------------
* {return: FlHashtable} Newly created hashtable
* -------------------------------------------------------------
*/
FlHashtable fl_hashtable_new(size_t kdtsize, size_t vdtsize, FlHashtableHashFunc hasher, FlHashtableDeleteKvpFunc cleanup);

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
void* fl_hashtable_add(FlHashtable ht, void *key, void *value);

void* fl_hashtable_get(FlHashtable ht, void *key);

void fl_hashtable_delete_kvp(void *key, size_t kdtsize, void *value, size_t vdtsize);

#endif /* FL_HASHTABLE_H */
