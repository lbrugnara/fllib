#include <stddef.h>
#include <string.h>

#include "../Std.h"
#include "../Mem.h"
#include "../Array.h"
#include "Hashtable.h"

#define NBUCKETS 20

// {todo: 
//    - Resize and rehash, prime numbers for NBUCKETS
//    - Iterator
// }

// free is used to mark removed bucket's entries as vacant
// entry to save new values
struct FlBucket {
    void *key;
    void *value;
    unsigned short free : 1;
};

/* -------------------------------------------------------------
* {datatype: struct FlHashtable}
* -------------------------------------------------------------
* Each hashtable uses its own hash (hashf) and clean up (cleanup) functions
* to generate the key hash and to free the memory used by a key-value pair.
* {buckets} will contain NBUCKETS that points to dynamic arrays that will 
* keep the entries of the hashtable.
* {kdtsize} and {vdtsize} are the sizes of the key and value data types.
* {length} keeps track of the current number of elements in the hashtable.
* -------------------------------------------------------------
*/
struct FlHashtable {
    size_t (*hashf)(FlByte *key, size_t kdtsize);
    void (*cleanup)(void *key, size_t kdtsize, void *value, size_t vdtsize);
    struct FlBucket **buckets; // fl_array
    size_t kdtsize;
    size_t vdtsize;
    size_t length;
};

// djb2 hash function. It is used by default if the user
// does not provide its own hash function
size_t fl_hashtable_hash(FlByte *key, size_t kdtsize)
{
    size_t hash = 5381;
    FlByte c;

    for (size_t i=0; i < kdtsize; i++)
        hash = ((hash << 5) + hash) + key[i];
    return hash;
}

// This function is used by default when the user does not provide its own clean up function
void fl_hashtable_delete_kvp(void *key, size_t kdtsize, void *value, size_t vdtsize)
{
    if (key) fl_free(key);
    if (value) fl_free(value);
}

FlHashtable fl_hashtable_new(size_t kdtsize, size_t vdtsize)
{
    return fl_hashtable_new_args((struct FlHashtableArgs){ .key_size = kdtsize, .value_size = vdtsize });
}

FlHashtable fl_hashtable_new_args(struct FlHashtableArgs args)
{
    flm_assert(args.key_size > 0, "Key data type size must be greater than 0");
    flm_assert(args.value_size > 0, "Key data type size must be greater than 0");

    struct FlHashtable *ht = fl_malloc(sizeof(struct FlHashtable));
    ht->kdtsize = args.key_size;
    ht->vdtsize = args.value_size;
    ht->buckets = fl_array_new(sizeof(struct FlBucket*), NBUCKETS);
    ht->hashf = args.hash_function != NULL ? args.hash_function : fl_hashtable_hash;
    ht->cleanup = args.cleanup_function != NULL ? args.cleanup_function : fl_hashtable_delete_kvp;
    return ht;
}

// To make buckets lookup, we can ask for:
//  EXISTENT: The bucket must exists and not be {free}
//  UNUSED: The bucket must NOT exist
//  ANY: Just give me a bucket for this hash, no matter if it is being used or not :)
enum BucketLookupOperation {
    BUCKET_LOOKUP_EXISTENT,
    BUCKET_LOOKUP_UNUSED,
    BUCKET_LOOKUP_ANY
};

// This functions returns a bucket for the hashed key based on the lookup operation
struct FlBucket* lookup_bucket(FlHashtable ht, void *key, enum BucketLookupOperation lookup_op)
{
    // Get the key hash and the bucket (hash_bucket is an element in ht->buckets, that points to a dynamic array of
    // points to struct FlBucket)
    size_t hash = ht->hashf(key, ht->kdtsize);
    struct FlBucket **hash_bucket = ht->buckets + (hash % fl_array_length(ht->buckets));
    
    // Find the target_bucket (or target entry). If hash_bucket points to nothing, we need
    // to allocate a dynamic array for our bucket.
    // If it is already pointing to an array, start looking in that array for our target_bucket
    //  
    // ht->buckets => [0][1][...][hash_bucket][...][NBUCKETS]
    //                                |
    //                                |
    //                                v
    //                               [ 0 ]
    //                               [ 1 ]
    //                               [...]
    //                               [target_bucket] --> This is the entry we are looking for
    //                               [...]
    //                               [ M ] -> dynamic, it grows for each new entry (if there is not an empty (free) bucket present)
    struct FlBucket *target_bucket = NULL;
    if (*hash_bucket == NULL)
    {
        // If the bucket is NULL (not used) and we are looking for an EXISTENT 
        // bucket, we must return NULL, because there is not entry for our hash 
        // (key does not exist in the Hashtable)
        if (lookup_op == BUCKET_LOOKUP_EXISTENT)
            return NULL;

        // If the bucket is empty, and we are looking an UNUSED or ANY type
        // of bucket, it is secure to create a new entry space for this bucket
        *hash_bucket = fl_array_new(sizeof(struct FlBucket), 1);
        target_bucket = *hash_bucket + 0;
        memset(target_bucket, 0, sizeof(struct FlBucket));
        target_bucket->free = 1; // New bucket's entry is free
    }
    else
    {
        // If the bucket has already been created we must check entries on it:
        //  Bucket is free: 
        //          UNUSED or ANY: If a bucket with this hash is empty, set it as a possible target bucket
        //                          but keep iterating, because we might find an entry with the same key
        //                          and it will take precedence over the free bucket
        //          EXISTENT: Do nothing, just keep iterating searching for an entry
        //
        //  Key exists:
        //      EXISTENT: return the bucket that matches key as is
        //      ANY: clean the current key and value and return the bucket that matches key
        //      UNUSED: return NULL, the current key is in use
        //
        // If the bucket does not contain entries, use its first element as target_bucket
        size_t l = fl_array_length(*hash_bucket);
        for (size_t i=0; i < l; i++)
        {
            if ((*hash_bucket+i))
            {
                struct FlBucket *b = (*hash_bucket+i);
                if (b->free)
                {
                    // If the bucket is free, mark it as a possible target on UNUSED or ANY.
                    // Continue searching for a (better) match.
                    if (lookup_op == BUCKET_LOOKUP_UNUSED ||lookup_op == BUCKET_LOOKUP_ANY)
                        target_bucket = (*hash_bucket+i);
                    continue;
                }

                // Bucket is not free and the entry is not equal to our key, keep iterating.
                if (!fl_equals(b->key, key, ht->kdtsize))
                    continue;

                // Equals but we want UNUSED, return NULL
                if (lookup_op == BUCKET_LOOKUP_UNUSED)
                    return NULL;

                // Equals and we want ANY, that means we need to clean up the pair
                if (lookup_op == BUCKET_LOOKUP_ANY)
                {
                    // Keep the key
                    ht->cleanup(NULL, 0, b->value, ht->vdtsize);
                }

                // If we are here, there is an entry with our same key,
                // but we need to return it as is (EXISTENT) or we cleaned up
                // the key-value pair (ANY)
                target_bucket = (*hash_bucket+i);
                break;
            }
            else
            {
                // NOTE: This conditions should not happen.
                // If we land here, the bucket is not being used, set it
                // as target bucket (NULL), if the lookup opeartion is ANY or UNUSED,
                // we will create a new bucket entry.
                target_bucket = (*hash_bucket+i);
                break;
            }
        }

        // Our target_bucket has not being allocated, check for lookup operation
        // EXISTENT to return NULL, for ANY or UNUSED, expand the hash_bucket to 
        // contain one more element (our target_bucket)
        if (!target_bucket)
        {
            if (lookup_op == BUCKET_LOOKUP_EXISTENT)
                return NULL;
            *hash_bucket = fl_array_resize((void*)*hash_bucket, l+1);
            target_bucket = *hash_bucket + l;
            memset(target_bucket, 0, sizeof(struct FlBucket));
            target_bucket->free = 1; // New bucket's entry is free
        }
    }
    return target_bucket;
}

void* fl_hashtable_add(FlHashtable ht, void *key, void *value)
{
    flm_assert(ht != NULL, "Hashtable must not be null");

    // Find the target bucket
    struct FlBucket *target_bucket = lookup_bucket(ht, key, BUCKET_LOOKUP_UNUSED);

    // If target_bucket is NULL, it means the key already exists (UNUSED returns NULL if the key is in the hash table)
    if (target_bucket == NULL)
        return NULL;
    
    // Increment the number of entries
    ht->length++;

    // Set key and value
    target_bucket->key = fl_malloc(ht->kdtsize);
    memcpy(target_bucket->key, key, ht->kdtsize);
    target_bucket->value = fl_malloc(ht->vdtsize);
    memcpy(target_bucket->value, value, ht->vdtsize);
    
    // Mark the bucket as used
    target_bucket->free = 0;

    // Return a reference to the newly created value
    return target_bucket->value;
}

void* fl_hashtable_get(FlHashtable ht, void *key)
{
    flm_assert(ht != NULL, "Hashtable must not be null");
    
    // Try to get an EXISTENT bucket
    struct FlBucket *target_bucket = lookup_bucket(ht, key, BUCKET_LOOKUP_EXISTENT);
    return target_bucket != NULL ? target_bucket->value : NULL;
}


void* fl_hashtable_set(FlHashtable ht, void *key, void *value)
{
    flm_assert(ht != NULL, "Hashtable must not be null");

    // Find the target bucket. Because of LOOKUP_ANY, the target backet can already
    // exist, in that case
    struct FlBucket *target_bucket = lookup_bucket(ht, key, BUCKET_LOOKUP_ANY);
    
    // Set key (if the bucket is free) and value
    if (target_bucket->free)
    {
        target_bucket->key = fl_malloc(ht->kdtsize);
        memcpy(target_bucket->key, key, ht->kdtsize);
        // Increment the number of entries just if it is a new bucket in use
        ht->length++;
    }
    target_bucket->value = fl_malloc(ht->vdtsize);
    memcpy(target_bucket->value, value, ht->vdtsize);

    // Mark bucket as in use
    target_bucket->free = 0;
    
    // Return a reference to the inserted value
    return target_bucket->value;
}

bool fl_hashtable_remove(FlHashtable ht, void *key)
{
    flm_assert(ht != NULL, "Hashtable must not be null");
    struct FlBucket *target_bucket = lookup_bucket(ht, key, BUCKET_LOOKUP_EXISTENT);
    if (!target_bucket)
        return false;
    ht->cleanup(target_bucket->key, ht->kdtsize, target_bucket->value, ht->vdtsize);
    // Mark this bucket as free
    target_bucket->free = 1;

    // Decrement the number of entries
    ht->length--;
    return true;
}

void fl_hashtable_clear(FlHashtable ht)
{
    flm_assert(ht != NULL, "Hashtable must not be null");
    if (ht->buckets)
    {
        size_t l = fl_array_length(ht->buckets);
        for (size_t i=0; i < l ; i++)
        {
            if (!ht->buckets[i])
                continue;
            size_t l = fl_array_length(ht->buckets[i]);
            for (size_t j=0; j < l; j++)
            {
                if ((ht->buckets[i]+j))
                {
                    struct FlBucket *b = ht->buckets[i]+j;
                    ht->cleanup(b->key, ht->kdtsize, b->value, ht->vdtsize);
                    ht->length--;
                }
            }
            fl_array_delete(ht->buckets[i]);
            ht->buckets[i] = NULL;
        }
    }
    flm_assert(ht->length == 0, "Length of Hashtable after clear must be 0");
}

enum HashtableContent {
    HT_KEYS,
    HT_VALUES
};

FlArray get_content(FlHashtable ht, enum HashtableContent content_type)
{
    FlByte *content;
    if (content_type == HT_KEYS)
        content = fl_array_new(ht->kdtsize, ht->length);
    else
        content = fl_array_new(ht->vdtsize, ht->length);

    size_t k = 0;
    if (ht->buckets)
    {
        size_t l = fl_array_length(ht->buckets);
        for (size_t i=0; i < l ; i++)
        {
            if (!ht->buckets[i])
                continue;
            size_t l = fl_array_length(ht->buckets[i]);
            for (size_t j=0; j < l; j++)
            {
                if ((ht->buckets[i]+j))
                {
                    struct FlBucket *b = ht->buckets[i]+j;
                    if (b->free)
                        continue;
                    if (content_type == HT_KEYS)
                        memcpy(content+(k * ht->kdtsize), b->key, ht->kdtsize);
                    else
                        memcpy(content+(k * ht->vdtsize), b->value, ht->vdtsize);
                    k++;
                }
            }
        }
    }
    return content;
}

FlArray fl_hashtable_keys(FlHashtable ht)
{
    return get_content(ht, HT_KEYS);
}

FlArray fl_hashtable_values(FlHashtable ht)
{
    return get_content(ht, HT_VALUES);
}

bool fl_hashtable_has_key(FlHashtable ht, void *key)
{
    return fl_hashtable_get(ht, key) != NULL;
}

size_t fl_hashtable_length(FlHashtable ht)
{
    flm_assert(ht != NULL, "Hashtable must not be null");
    return ht->length;
}

double fl_hashtable_load_factor(FlHashtable ht)
{
    flm_assert(ht != NULL, "Hashtable must not be null");
    return ht->length / (double)fl_array_length(ht->buckets);
}

void fl_hashtable_delete(FlHashtable ht)
{
    flm_assert(ht != NULL, "Hashtable must not be null");
    if (ht->buckets)
    {
        size_t l = fl_array_length(ht->buckets);
        for (size_t i=0; i < l ; i++)
        {
            if (!ht->buckets[i])
                continue;
            size_t l = fl_array_length(ht->buckets[i]);
            for (size_t j=0; j < l; j++)
            {
                if ((ht->buckets[i]+j))
                {
                    struct FlBucket *b = ht->buckets[i]+j;
                    if (b->free)
                        continue;
                    ht->cleanup(b->key, ht->kdtsize, b->value, ht->vdtsize);
                }
            }
            fl_array_delete(ht->buckets[i]);
        }
    }
    fl_array_delete(ht->buckets);
    fl_free(ht);
}
