#ifndef FL_KEYVALUE_PAIR_H
#define FL_KEYVALUE_PAIR_H

#include "../Types.h"

typedef struct FlKeyValuePair FlKeyValuePair;

/**
 * Creates a new {FlKeyValuePair} using keys of {kdtsize} bytes and
 * values of {vdtsize} bytes
 */
FlKeyValuePair* fl_kvp_new(size_t kdtsize, size_t vdtsize);

/**
 * Creates a new {FlKeyValuePair} using keys of {kdtsize} bytes and
 * values of {vdtsize} bytesand initialize it with {key} and {val1}
 */
FlKeyValuePair* fl_kvp_new_set(const FlByte *key, size_t kdtsize, const FlByte *val, size_t vdtsize);

/**
 * Releases the memory used by {kvp}
 */
void fl_kvp_delete(FlKeyValuePair *kvp);

/**
 * Returns the size used to allocate each key in {kvp}
 */
size_t fl_kvp_key_dtsize(FlKeyValuePair *kvp);

/**
 * Returns the size used to allocate each value in {kvp}
 */
size_t fl_kvp_val_dtsize(FlKeyValuePair *kvp);

/**
 * Returns the key of {kvp}
 */
FlPointer fl_kvp_get_key(FlKeyValuePair *kvp);

/**
 * Returns the value of {kvp}
 */
FlPointer fl_kvp_get_val(FlKeyValuePair *kvp);

/**
 * Set the key and value of {kvp} using {key} and {val}
 */
void fl_kvp_set(FlKeyValuePair *kvp, const FlByte *key, const FlByte *val);

#endif /* FL_KEYVALUE_PAIR_H */