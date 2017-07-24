#include <stddef.h>
#include <string.h>

#include "../Std.h"
#include "../Mem.h"
#include "../Array.h"
#include "Hashtable.h"

#define NBUCKETS 20

// {todo: 
//    - Resize and rehash, prime numbers for NBUCKETS
//    - Insert
//    - Delete/Clear
//    - Length
//    - Iterator
// }

struct FlBucket {
    void *key;
    void *value;
};

struct FlHashtable {
    size_t (*hashf)(FlByte *key, size_t kdtsize);
    void (*cleanup)(void *key, size_t kdtsize, void *value, size_t vdtsize);
    struct FlBucket **buckets; // fl_array
    size_t kdtsize;
    size_t vdtsize;
    size_t length;
};

// djb2 hash function
size_t fl_hashtable_hash(FlByte *key, size_t kdtsize)
{
    size_t hash = 5381;
    FlByte c;

    for (size_t i=0; i < kdtsize; i++)
        hash = ((hash << 5) + hash) + key[i];
    return hash;
}

void fl_hashtable_delete_kvp(void *key, size_t kdtsize, void *value, size_t vdtsize)
{
    if (key) fl_free(key);
    if (value) fl_free(value);
}


FlHashtable fl_hashtable_new(size_t kdtsize, size_t vdtsize, FlHashtableHashFunc hashfunc, FlHashtableDeleteKvpFunc cleanup)
{
    struct FlHashtable *ht = fl_malloc(sizeof(struct FlHashtable));
    ht->kdtsize = kdtsize;
    ht->vdtsize = vdtsize;
    ht->buckets = fl_array_new(sizeof(struct FlBucket*), NBUCKETS);
    ht->hashf = hashfunc != NULL ? hashfunc : fl_hashtable_hash;
    ht->cleanup = cleanup != NULL ? cleanup : fl_hashtable_delete_kvp;
    return ht;
}

struct FlBucket* find_bucket(FlHashtable ht, void *key, bool lookup_only, bool override)
{
    size_t hash = ht->hashf(key, ht->kdtsize);
    struct FlBucket **hash_bucket = ht->buckets + (hash % fl_array_length(ht->buckets));
    struct FlBucket *target_bucket = NULL;

    // If the bucket is empty, create it
    if (*hash_bucket == NULL)
    {
        if (lookup_only)
            return NULL;
        *hash_bucket = fl_array_new(sizeof(struct FlBucket), 1);
        target_bucket = *hash_bucket + 0;
    }
    else
    {
        // If the bucket has already been created and override is false, check if the current key already exists
        // and if it does, return NULL. If not expand the array to allow one more entry in 
        // the bucket, and return a reference to it
        size_t l = fl_array_length(*hash_bucket);
        for (size_t i=0; i < l; i++)
        {
            if ((*hash_bucket+i))
            {
                struct FlBucket *b = (*hash_bucket+i);
                if (!fl_equals(b->key, key, ht->kdtsize))
                    continue;

                if (!lookup_only && !override)
                    return NULL;

                if (!lookup_only && ht->cleanup)
                    ht->cleanup(b->key, ht->kdtsize, b->value, ht->vdtsize);
            }
            target_bucket = (*hash_bucket+i);
            break;
        }
        if (!target_bucket)
        {
            if (lookup_only)
                return NULL;
            *hash_bucket = fl_array_resize((void*)*hash_bucket, l+1);
            target_bucket = *hash_bucket + l;
        }
    }
    return target_bucket;
}

void* fl_hashtable_add(FlHashtable ht, void *key, void *value)
{
    flm_assert(ht != NULL, "Hashtable must not be null");

    // Find the target bucket
    struct FlBucket *target_bucket = find_bucket(ht, key, false, false);
    // Key already exists
    if (target_bucket == NULL)
        return NULL;
    
    // Set key and value
    target_bucket->key = fl_malloc(ht->kdtsize);
    memcpy(target_bucket->key, key, ht->kdtsize);
    target_bucket->value = fl_malloc(ht->vdtsize);
    memcpy(target_bucket->value, value, ht->vdtsize);
    
    // Increment the number of entries counter
    ht->length++;

    // Return a reference to the newly created value
    return target_bucket->value;
}

void* fl_hashtable_get(FlHashtable ht, void *key)
{
    flm_assert(ht != NULL, "Hashtable must not be null");

    struct FlBucket *target_bucket = find_bucket(ht, key, true, false);

    return target_bucket != NULL ? target_bucket->value : NULL;
}

void fl_hashtable_delete(FlHashtable ht)
{
    flm_assert(ht != NULL, "Hashtable to delete cannot be NULL");
    if (ht->buckets)
    {
        size_t l = fl_array_length(ht->buckets);
        for (size_t i=0; i < l ; i++)
        {
            if (!*(ht->buckets+i))
                continue;
            size_t l = fl_array_length(*(ht->buckets+i));
            for (size_t j=0; j < l; j++)
            {
                if ((*(ht->buckets+i)+j) && ht->cleanup)
                {
                    struct FlBucket *b = *(ht->buckets+i)+j;
                    ht->cleanup(b->key, ht->kdtsize, b->value, ht->vdtsize);
                }
            }
            fl_array_delete(*(ht->buckets+i));
        }
    }
    fl_array_delete(ht->buckets);
    fl_free(ht);
}

double fl_hashtable_load_factor(FlHashtable ht)
{
    flm_assert(ht != NULL, "Hashtable must not be NULL");
    return ht->length / (double)fl_array_length(ht->buckets);
}
