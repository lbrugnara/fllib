#include <stddef.h>
#include <string.h>

#include "../Std.h"
#include "../Mem.h"
#include "../Array.h"
#include "Hashtable.h"


// TODO: 
//    - Prime numbers for NBUCKETS. On resize it currently multiplies by 2, it is not efficient
//    - Iterator

// It represents a bucket's entry, it can be free or used
struct FlBucketEntry {
    void *key;
    void *value;
    unsigned short free : 1;
};

// Number of buckets to be allocated
#define NBUCKETS 83
// Number of entries to allocate inside each bucket
#define NBUCKETENTRIES 5

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
    size_t (*hashf)(const FlByte *key, size_t kdtsize);
    void (*cleanup)(void *key, size_t kdtsize, void *value, size_t vdtsize);
    struct FlBucketEntry **buckets; // fl_array
    double load_factor;
    size_t kdtsize;
    size_t vdtsize;
    size_t length;
};

// djb2 hash function. It is used by default if the user
// does not provide its own hash function
size_t fl_hashtable_hash(const FlByte *key, size_t kdtsize)
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

    struct FlHashtable *ht = fl_calloc(1, sizeof(struct FlHashtable));
    ht->kdtsize = args.key_size;
    ht->vdtsize = args.value_size;
    ht->load_factor = args.load_factor != 0 ? args.load_factor : 0.75;
    ht->buckets = fl_array_new(sizeof(struct FlBucketEntry*), args.buckets_count != 0 ? args.buckets_count : NBUCKETS);
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
struct FlBucketEntry* lookup_bucket(FlHashtable ht, const void *key, enum BucketLookupOperation lookup_op)
{
    // Get the key hash and the bucket (hash_bucket is an element in ht->buckets, that points to a dynamic array of
    // points to struct FlBucketEntry)
    size_t hash = ht->hashf(key, ht->kdtsize);
    struct FlBucketEntry **hash_bucket = ht->buckets + (hash % fl_array_length(ht->buckets));
    
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
    struct FlBucketEntry *target_bucket = NULL;
    if (*hash_bucket == NULL)
    {
        // If the bucket is NULL (not used) and we are looking for an EXISTENT 
        // bucket, we must return NULL, because there is not entry for our hash 
        // (key does not exist in the Hashtable)
        if (lookup_op == BUCKET_LOOKUP_EXISTENT)
            return NULL;

        // If the bucket is empty, and we are looking an UNUSED or ANY type
        // of bucket, it is secure to create a new entry space for this bucket
        *hash_bucket = fl_array_new(sizeof(struct FlBucketEntry), NBUCKETENTRIES);
        for (int i=0; i < NBUCKETENTRIES; i++)
        {
            target_bucket = *hash_bucket + i;
            target_bucket->key = NULL;
            target_bucket->value = NULL;
            target_bucket->free = 1; // New bucket's entry is free
        }
        target_bucket = *hash_bucket + 0;
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
                struct FlBucketEntry *b = (*hash_bucket+i);
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
            memset(target_bucket, 0, sizeof(struct FlBucketEntry));
            target_bucket->free = 1; // New bucket's entry is free
        }
    }
    return target_bucket;
}

void* ht_internal_add(FlHashtable ht, const void *key, const void *value, enum BucketLookupOperation lookup_type, bool allowResize)
{
    flm_assert(ht != NULL, "Hashtable must not be null");

    // Find the target bucket. Because of LOOKUP_ANY, the target backet can already
    // exist, in that case
    struct FlBucketEntry *target_bucket = lookup_bucket(ht, key, lookup_type);
    
    if (target_bucket == NULL && lookup_type == BUCKET_LOOKUP_UNUSED)
    {
        return NULL;
    }

    // Set key (if the bucket is free) and value
    if (lookup_type == BUCKET_LOOKUP_UNUSED || target_bucket->free)
    {
        target_bucket->key = fl_calloc(1, ht->kdtsize);
        memcpy(target_bucket->key, key, ht->kdtsize);
        // Increment the number of entries just if it is a new bucket in use
        ht->length++;
    }
    target_bucket->value = fl_calloc(1, ht->vdtsize);
    memcpy(target_bucket->value, value, ht->vdtsize);

    // Mark bucket as in use
    target_bucket->free = 0;

    if (allowResize)
    {
        size_t nbuckets = fl_array_length(ht->buckets);
        double load_factor = ht->length / (double)nbuckets;
        if (load_factor > ht->load_factor)
        {
            fl_hashtable_resize(ht, nbuckets * 2);
            target_bucket = lookup_bucket(ht, key, BUCKET_LOOKUP_EXISTENT);
            flm_assert(target_bucket != NULL, "Retrieving the key after rehashing MUST return a target_bucket");
        }   
    }
    // Return a reference to the inserted value
    return target_bucket->value;
}

void* fl_hashtable_add(FlHashtable ht, const void *key, const void *value)
{
    return ht_internal_add(ht, key, value, BUCKET_LOOKUP_UNUSED, true);
}

void* fl_hashtable_set(FlHashtable ht, const void *key, const void *value)
{
    return ht_internal_add(ht, key, value, BUCKET_LOOKUP_ANY, true);
}

void* fl_hashtable_get(FlHashtable ht, const void *key)
{
    flm_assert(ht != NULL, "Hashtable must not be null");
    
    // Try to get an EXISTENT bucket
    struct FlBucketEntry *target_bucket = lookup_bucket(ht, key, BUCKET_LOOKUP_EXISTENT);
    return target_bucket != NULL ? target_bucket->value : NULL;
}

bool fl_hashtable_remove(FlHashtable ht, const void *key)
{
    flm_assert(ht != NULL, "Hashtable must not be null");
    struct FlBucketEntry *target_bucket = lookup_bucket(ht, key, BUCKET_LOOKUP_EXISTENT);
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
                    struct FlBucketEntry *b = ht->buckets[i]+j;
                    if (b->free)
                        continue;
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

FlArray ht_get_content(FlHashtable ht, enum HashtableContent content_type)
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
                    struct FlBucketEntry *b = ht->buckets[i]+j;
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
    return ht_get_content(ht, HT_KEYS);
}

FlArray fl_hashtable_values(FlHashtable ht)
{
    return ht_get_content(ht, HT_VALUES);
}

bool fl_hashtable_has_key(FlHashtable ht, const void *key)
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

size_t fl_hashtable_buckets_count(FlHashtable ht)
{
    return fl_array_length(ht->buckets);
}

void fl_hashtable_resize(FlHashtable ht, size_t nbuckets)
{
    flm_assert(nbuckets > 0, "Number of buckets must be greater than 0");

    struct FlHashtable newht = {
        .kdtsize = ht->kdtsize,
        .vdtsize = ht->vdtsize,
        .buckets = fl_array_new(sizeof(struct FlBucketEntry*), nbuckets),
        .hashf = ht->hashf,
        .cleanup = ht->cleanup
    };

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
                    struct FlBucketEntry *b = ht->buckets[i]+j;
                    if (b->free)
                        continue;
                    ht_internal_add(&newht, b->key, b->value, BUCKET_LOOKUP_UNUSED, false);
                }
            }
        }
        fl_hashtable_clear(ht);
        fl_array_delete(ht->buckets);
        ht->buckets = newht.buckets;
        ht->length = newht.length;
    }
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
                    struct FlBucketEntry *b = ht->buckets[i]+j;
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
