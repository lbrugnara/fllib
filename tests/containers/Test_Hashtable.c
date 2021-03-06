#include <time.h>
#include <stdbool.h>
#include <fllib.h>

#include <flut/flut.h>
#include "Test_Hashtable.h"

void test_fl_hashtable_add()
{
    // Common use cases
    {
        FlHashtable *ht = fl_hashtable_new_args((struct FlHashtableArgs) {
            .hash_function = fl_hashtable_hash_char,
            .key_comparer = fl_container_equals_char,
            .key_cleaner = fl_container_cleaner_pointer,
            .value_cleaner = fl_container_cleaner_pointer,
            .key_allocator = fl_container_allocator_char,
            .value_allocator = fl_container_allocator_int
        });

        char k='a';
        int v=97;
        void* vp = fl_hashtable_add(ht, &k, &v);
        flut_expect_compat("hashtable_add('a', 97) result must be equals to 97", v == *(int*)vp);

        char k2='a';
        int v2 = 97;
        vp = fl_hashtable_add(ht, &k2, &v2);
        flut_expect_compat("Call to hashtable_add('a', 97) by second time must return NULL", NULL == vp);

        v=65;
        vp = fl_hashtable_add(ht, &k, &v);
        flut_expect_compat("Call to hashtable_add('a', 65) by second time must return NULL", NULL == vp);


        //
        k='b';
        v=98;
        vp = fl_hashtable_add(ht, &k, &v);
        flut_expect_compat("hashtable_add('b', 98) result must be equals to 98", v == *(int*)vp);

        k2='b';
        v2 = 98;
        vp = fl_hashtable_add(ht, &k2, &v2);
        flut_expect_compat("Call to hashtable_add('b', 98) by second time must return NULL", NULL == vp);

        v=65;
        vp = fl_hashtable_add(ht, &k, &v);
        flut_expect_compat("Call to hashtable_add('b', 66) by second time must return NULL", NULL == vp);

        fl_hashtable_free(ht);
    }

    // Using NULL as key or value
    {
        /*FlHashtable *ht2 = fl_hashtable_new(fl_hashtable_hash_string, fl_container_equals_string, NULL, NULL, NULL, NULL);
        char *key = "one";
        int *val = NULL;
        int **vp = (int**)fl_hashtable_add(ht2, &key, &val);
        flut_expect_compat("Call to hashtable_add(\"one\", NULL) must return NULL", NULL == *vp);
        
        key = NULL;
        int v = 95;
        val = &v;
        vp = (int**)fl_hashtable_add(ht2, &key, &val);
        flut_expect_compat("Call to hashtable_add(NULL, 95) must return 95", v == **vp);

        double d = fl_hashtable_load_factor(ht2);
        fl_hashtable_resize(ht2, fl_hashtable_buckets_count(ht2) * 2);
        double d2 = fl_hashtable_load_factor(ht2);
        flut_expect_compat("Hashtable load factor after resize should be lesser", d2 < d);

        fl_hashtable_free(ht2);*/
    }
}

unsigned long hash_func(const FlByte *key)
{
    return 1;
}

void test_fl_hashtable_add_fhash()
{
    FlHashtable *ht = fl_hashtable_new(hash_func, fl_container_equals_char, NULL, NULL, NULL, NULL);
    
    char k='a';
    int v=97;
    void* vp = fl_hashtable_add(ht, &k, &v);
    flut_expect_compat("hashtable_add('a', 97) result must be equals to 97", v == *(int*)vp);

    char k2='a';
    int v2 = 97;
    vp = fl_hashtable_add(ht, &k2, &v2);
    flut_expect_compat("Call to hashtable_add('a', 97) by second time must return NULL", NULL == vp);
    double d = fl_hashtable_load_factor(ht);

    //
    char k3='b';
    v=98;
    vp = fl_hashtable_add(ht, &k3, &v);
    flut_expect_compat("hashtable_add('b', 98) result must be equals to 98", v == *(int*)vp);

    char k4='b';
    v2 = 98;
    vp = fl_hashtable_add(ht, &k4, &v2);
    flut_expect_compat("Call to hashtable_add('b', 98) by second time must return NULL", NULL == vp);
    double d2 = fl_hashtable_load_factor(ht);

    flut_expect_compat("Hashtable load factor after 'b' should be greater than before 'b'", d2 > d);

    fl_hashtable_free(ht);
}

void test_fl_hashtable_get()
{
    // Normal usage
    {
        FlHashtable *ht = fl_hashtable_new_args((struct FlHashtableArgs) {
            .hash_function = fl_hashtable_hash_char,
            .key_comparer = fl_container_equals_char,
            .key_cleaner = fl_container_cleaner_pointer,
            .value_cleaner = fl_container_cleaner_pointer,
            .key_allocator = fl_container_allocator_char,
            .value_allocator = fl_container_allocator_int
        });
        
        int i = 65;
        for (char c = 'A'; c <= 'Z'; c++, i++)
        {
            int *tmp = fl_hashtable_add(ht, &c, &i);
            flut_vexpect_compat(tmp != NULL, "Value %d under key %c must be inserted into the hashtable (%d)", i, c, *tmp);
        }
        
        i = 97;
        for (char c = 'a'; c <= 'z'; c++, i++)
        {
            int *tmp = fl_hashtable_add(ht, &c, &i);
            flut_vexpect_compat(tmp != NULL, "Value %d under key %c must be inserted into the hashtable (%d)", i, c, *tmp);
        }

        int *v;
        i = 65;
        for (char c = 'A'; c <= 'Z'; c++, i++)
        {
            int *tmp = fl_hashtable_get(ht, &c);
            if (tmp)
                flut_vexpect_compat(*tmp == i, "Value with key %c must be equals to %d\n", c, i);
        }
        i = 97;
        for (char c = 'a'; c <= 'z'; c++, i++)
        {
            int *tmp = fl_hashtable_get(ht, &c);
            if (tmp)
                flut_vexpect_compat(*tmp == i, "Value with key %c must be equals to %d\n", c, i);   
        }

        fl_hashtable_free(ht);
    }
}

void test_fl_hashtable_set()
{
    FlHashtable *ht = fl_hashtable_new_args((struct FlHashtableArgs) {
        .hash_function = fl_hashtable_hash_char,
        .key_comparer = fl_container_equals_char,
        .key_cleaner = fl_container_cleaner_pointer,
        .value_cleaner = fl_container_cleaner_pointer,
        .key_allocator = fl_container_allocator_char,
        .value_allocator = fl_container_allocator_int
    });

    char chr;

    // Add A-Z with its codes
    int i = 65;
    for (char c = 'A'; c <= 'Z'; c++, i++)
    {
        fl_hashtable_add(ht, &c, &i);
    }

    flut_expect_compat("Current length after adding the alphabet letters A to Z with codes 65-90 must be 26", fl_hashtable_length(ht) == 26);

    int *ap = (chr = 'A', fl_hashtable_get(ht, &chr));
    int *mp = (chr = 'M', fl_hashtable_get(ht, &chr));
    int *zp = (chr = 'Z', fl_hashtable_get(ht, &chr));
    flut_expect_compat("Expect letters {A, M, Z} to have values {65, 77, 90}", *ap == 65 && *mp == 77 && *zp == 90);

    // Replace A-Z by its lowercase codes
    i = 97;
    for (char c = 'A'; c <= 'Z'; c++, i++)        
    {
        fl_hashtable_set(ht, &c, &i);
    }

    flut_expect_compat("Current length after replacing the alphabet letters A to Z with codes 97-122 must be 26", fl_hashtable_length(ht) == 26);

    // Add a-z with its codes
    i = 97;
    for (char c = 'a'; c <= 'z'; c++, i++)
    {
        fl_hashtable_set(ht, &c, &i);
    }

    flut_expect_compat("Current length after adding the alphabet letters a to z with codes 97-122 must be 52", fl_hashtable_length(ht) == 52);

    ap = (chr = 'a', fl_hashtable_get(ht, &chr));
    mp = (chr = 'm', fl_hashtable_get(ht, &chr));
    zp = (chr = 'z', fl_hashtable_get(ht, &chr));
    flut_expect_compat("Expect letters {a, m, z} to have values {97, 109, 122}", *ap == 97 && *mp == 109 && *zp == 122);

    // Replace a-z by its uppercase codes
    i = 65;
    for (char c = 'a'; c <= 'z'; c++, i++)
    {
        fl_hashtable_set(ht, &c, &i);
    }

    flut_expect_compat("Current length after replacing the alphabet letters a to z with codes 65-90 must be 52", fl_hashtable_length(ht) == 52);

    // Check mappings
    ap = (chr = 'A', fl_hashtable_get(ht, &chr));
    mp = (chr = 'M', fl_hashtable_get(ht, &chr));
    zp = (chr = 'Z', fl_hashtable_get(ht, &chr));
    flut_expect_compat("Expect letters {A, M, Z} to have values {97, 109, 122}", *ap == 97 && *mp == 109 && *zp == 122);

    ap = (chr = 'a', fl_hashtable_get(ht, &chr));
    mp = (chr = 'm', fl_hashtable_get(ht, &chr));
    zp = (chr = 'z', fl_hashtable_get(ht, &chr));
    flut_expect_compat("Expect letters {a, m, z} to have values {65, 77, 90}", *ap == 65 && *mp == 77 && *zp == 90);
    
    fl_hashtable_free(ht);
}

void test_fl_hashtable_clear()
{
    FlHashtable *ht = fl_hashtable_new_args((struct FlHashtableArgs) {
            .hash_function = fl_hashtable_hash_char,
            .key_comparer = fl_container_equals_char,
            .key_cleaner = fl_container_cleaner_pointer,
            .value_cleaner = fl_container_cleaner_pointer,
            .key_allocator = fl_container_allocator_char,
            .value_allocator = fl_container_allocator_int
        });

    // Add A-Z with its codes
    int i = 65;
    for (char c = 'A'; c <= 'Z'; c++, i++)
        fl_hashtable_add(ht, &c, &i);

    flut_expect_compat("Current length after adding the alphabet letters A to Z with codes 65-90 must be 26", fl_hashtable_length(ht) == 26);

    fl_hashtable_clear(ht);
    flut_expect_compat("Current length after clear the hashtable content must be 0", fl_hashtable_length(ht) == 0);

    fl_hashtable_free(ht);
}

void test_fl_hashtable_keys_and_values()
{
    FlHashtable *ht = fl_hashtable_new_args((struct FlHashtableArgs) {
        .hash_function = fl_hashtable_hash_char,
        .key_comparer = fl_container_equals_char,
        .key_cleaner = fl_container_cleaner_pointer,
        .value_cleaner = fl_container_cleaner_pointer,
        .key_allocator = fl_container_allocator_char,
        .value_allocator = fl_container_allocator_int
    });

    // Add A-Z with its codes
    int i = 65;
    for (char c = 'A'; c <= 'Z'; c++, i++)
        fl_hashtable_add(ht, &c, &i);
    
    flut_expect_compat("Current length after adding the alphabet letters A to Z with codes 65-90 must be 26", fl_hashtable_length(ht) == 26);
    
    char **keys = fl_hashtable_keys(ht);
    flut_expect_compat("Keys array must contain 26 elements", fl_array_length(keys) == 26);

    char **values = fl_hashtable_values(ht);
    flut_expect_compat("Values array must contain 26 elements", fl_array_length(values) == 26);

    fl_array_free(keys);
    fl_array_free(values);
    fl_hashtable_free(ht);
}

void test_fl_hashtable_remove()
{
    FlHashtable *ht = fl_hashtable_new_args((struct FlHashtableArgs) {
        .hash_function = fl_hashtable_hash_char,
        .key_comparer = fl_container_equals_char,
        .key_cleaner = fl_container_cleaner_pointer,
        .value_cleaner = fl_container_cleaner_pointer,
        .key_allocator = fl_container_allocator_char,
        .value_allocator = fl_container_allocator_int
    });

    // Add A-Z with its codes
    int i = 65;
    for (char c = 'A'; c <= 'Z'; c++, i++)
        fl_hashtable_add(ht, &c, &i);
    
    flut_expect_compat("Current length after adding the alphabet letters A to Z with codes 65-90 must be 26", fl_hashtable_length(ht) == 26);
    
    char **keys = fl_hashtable_keys(ht);
    size_t keys_length = fl_array_length(keys);

    for (size_t j=0; j < keys_length; j++)
    {
        fl_hashtable_remove(ht, keys[j], true, true);
    }

    flut_expect_compat("Current length after removing all the elements by key must be 0", fl_hashtable_length(ht) == 0);

    fl_array_free(keys);
    fl_hashtable_free(ht);
}

unsigned long hash_func2(const FlByte *key)
{
    return *(size_t*)key;
}

bool key_comparer2(const FlByte *key1, const FlByte *key2)
{
    return *(size_t*)key1 == *(size_t*)key2;
}

void size_t_writer(FlByte **dest, const FlByte *src)
{
    *dest = fl_malloc(sizeof(size_t));
    memcpy(*dest, src, sizeof(size_t));
}

void test_fl_hashtable_resize()
{
    FlHashtable *ht = fl_hashtable_new_args((struct FlHashtableArgs){
        .hash_function = hash_func2,
        .key_comparer = key_comparer2,
        .key_cleaner = fl_container_cleaner_pointer,
        .value_cleaner = fl_container_cleaner_pointer,
        .key_allocator = size_t_writer,
        .value_allocator = size_t_writer,
        .buckets_count = 52735,
        .load_factor = 1.0
    });

    FlTimer *timer = fl_timer_create();
    fl_timer_start(timer);
    for (size_t i=0; i < 52736; i++)
    {
        fl_hashtable_add(ht, &i, &i);
    }
    fl_timer_end(timer);
    printf("Elapsed milliseconds: %ld\n", fl_timer_elapsed_ms(timer));
    fl_timer_free(timer);
    fl_hashtable_free(ht);
}

void allocate_struct(FlByte **dest, const FlByte *src)
{
    size_t size = sizeof( struct FlHashtableArgs);
    *dest = fl_malloc(size);
    memcpy(*dest, src, size);
}

void test_fl_hashtable_values()
{
    struct FlHashtableArgs args = { 0 };
    args.hash_function = fl_hashtable_hash_char;
    args.key_comparer = fl_container_equals_char;
    args.key_allocator = fl_container_allocator_char;
    args.key_cleaner = fl_container_cleaner_pointer;
    args.value_allocator = allocate_struct;
    args.value_cleaner = fl_container_cleaner_pointer;
    args.buckets_count = 10;

    FlHashtable *ht = fl_hashtable_new_args(args);

    // We use a pointer to char :grinning:
    fl_hashtable_add(ht, "a", &args);

    struct FlHashtableArgs **values = fl_hashtable_values(ht);

    flut_expect_compat("values variable must be a valid array", values != NULL);
    flut_expect_compat("values variable must contain 1 element", fl_array_length(values) == 1);
    flut_expect_compat("The only element in the values variable must be a pointer to a struct FlHashtableArgs equals to the struct FlHashtableArgs args variable", 
        memcmp(&args, values[0], sizeof(struct FlHashtableArgs)) == 0);

    values[0]->buckets_count = 20;
    flut_expect_compat("A modification to the element in the array must not impact on the args variable", memcmp(&args, values[0], sizeof(struct FlHashtableArgs)) != 0);

    struct FlHashtableArgs *valueptr = fl_hashtable_get(ht, "a");
    flut_expect_compat("The modification to the element in the array impacts on the element in the hashtable (both point to the same element)", 
        memcmp(valueptr, values[0], sizeof(struct FlHashtableArgs)) == 0);

    fl_array_free(values);
    fl_hashtable_free(ht);
}
