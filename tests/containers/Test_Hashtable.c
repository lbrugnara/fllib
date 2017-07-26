#include <time.h>
#include <fllib.h>
#include "../Test.h"
#include "Test_Hashtable.h"

void test_fl_hashtable_add()
{
    // Common use cases
    {
        FlHashtable ht = fl_hashtable_new(sizeof(char), sizeof(int));
        char k='a';
        int v=97;
        void* vp = fl_hashtable_add(ht, &k, &v);
        fl_expect("hashtable_add('a', 97) result must be equals to 97", v == *(int*)vp);

        char k2='a';
        int v2 = 97;
        vp = fl_hashtable_add(ht, &k2, &v2);
        fl_expect("Call to hashtable_add('a', 97) by second time must return NULL", NULL == vp);

        v=65;
        vp = fl_hashtable_add(ht, &k, &v);
        fl_expect("Call to hashtable_add('a', 65) by second time must return NULL", NULL == vp);


        //
        k='b';
        v=98;
        vp = fl_hashtable_add(ht, &k, &v);
        fl_expect("hashtable_add('b', 98) result must be equals to 98", v == *(int*)vp);

        k2='b';
        v2 = 98;
        vp = fl_hashtable_add(ht, &k2, &v2);
        fl_expect("Call to hashtable_add('b', 98) by second time must return NULL", NULL == vp);

        v=65;
        vp = fl_hashtable_add(ht, &k, &v);
        fl_expect("Call to hashtable_add('b', 66) by second time must return NULL", NULL == vp);

        fl_hashtable_delete(ht);
    }

    // Using NULL as key or value
    {
        FlHashtable ht2 = fl_hashtable_new(sizeof(char*), sizeof(int*));
        char *key = "one";
        int *val = NULL;
        int **vp = (int**)fl_hashtable_add(ht2, &key, &val);
        fl_expect("Call to hashtable_add(\"one\", NULL) must return NULL", NULL == *vp);
        
        key = NULL;
        int v = 95;
        val = &v;
        vp = (int**)fl_hashtable_add(ht2, &key, &val);
        fl_expect("Call to hashtable_add(NULL, 95) must return 95", v == **vp);

        double d = fl_hashtable_load_factor(ht2);
        fl_hashtable_resize(ht2, fl_hashtable_buckets_count(ht2) * 2);
        double d2 = fl_hashtable_load_factor(ht2);
        fl_expect("Hashtable load factor after resize should be lesser", d2 < d);

        fl_hashtable_delete(ht2);
    }
}

size_t hash_func(FlByte *key, size_t ksize)
{
    return 1;
}

void test_fl_hashtable_add_fhash()
{
    FlHashtable ht = fl_hashtable_new_args((struct FlHashtableArgs){
        .key_size = sizeof(char), 
        .value_size = sizeof(int), 
        .hash_function = &hash_func
    });
    
    char k='a';
    int v=97;
    void* vp = fl_hashtable_add(ht, &k, &v);
    fl_expect("hashtable_add('a', 97) result must be equals to 97", v == *(int*)vp);

    char k2='a';
    int v2 = 97;
    vp = fl_hashtable_add(ht, &k2, &v2);
    fl_expect("Call to hashtable_add('a', 97) by second time must return NULL", NULL == vp);
    double d = fl_hashtable_load_factor(ht);

    //
    k='b';
    v=98;
    vp = fl_hashtable_add(ht, &k, &v);
    fl_expect("hashtable_add('b', 98) result must be equals to 98", v == *(int*)vp);

    k2='b';
    v2 = 98;
    vp = fl_hashtable_add(ht, &k2, &v2);
    fl_expect("Call to hashtable_add('b', 98) by second time must return NULL", NULL == vp);
    double d2 = fl_hashtable_load_factor(ht);

    fl_expect("Hashtable load factor after 'b' should be greater than before 'b'", d2 > d);

    fl_hashtable_delete(ht);
}

void test_fl_hashtable_get()
{
    // Normal usage
    {
        FlHashtable ht = fl_hashtable_new(sizeof(char), sizeof(int));
        
        int i = 65;
        for (char c = 'A'; c <= 'Z'; c++, i++)
        {
            int *tmp = fl_hashtable_add(ht, &c, &i);
            fl_vexpect(tmp != NULL, "Value %d under key %c must be inserted into the hashtable (%d)", i, c, *tmp);
        }
        
        i = 97;
        for (char c = 'a'; c <= 'z'; c++, i++)
        {
            int *tmp = fl_hashtable_add(ht, &c, &i);
            fl_vexpect(tmp != NULL, "Value %d under key %c must be inserted into the hashtable (%d)", i, c, *tmp);
        }

        int *v;
        i = 65;
        for (char c = 'A'; c <= 'Z'; c++, i++)
        {
            int *tmp = fl_hashtable_get(ht, &c);
            if (tmp)
                fl_vexpect(*tmp == i, "Value with key %c must be equals to %d\n", c, i);
        }
        i = 97;
        for (char c = 'a'; c <= 'z'; c++, i++)
        {
            int *tmp = fl_hashtable_get(ht, &c);
            if (tmp)
                fl_vexpect(*tmp == i, "Value with key %c must be equals to %d\n", c, i);   
        }

        fl_hashtable_delete(ht);
    }
}

void test_fl_hashtable_set()
{
    FlHashtable ht = fl_hashtable_new(sizeof(char), sizeof(int));
    char chr;

    // Add A-Z with its codes
    int i = 65;
    for (char c = 'A'; c <= 'Z'; c++, i++)
        fl_hashtable_add(ht, &c, &i);

    fl_expect("Current length after adding the alphabet letters A to Z with codes 65-90 must be 26", fl_hashtable_length(ht) == 26);

    int *ap = (chr = 'A', fl_hashtable_get(ht, &chr));
    int *mp = (chr = 'M', fl_hashtable_get(ht, &chr));
    int *zp = (chr = 'Z', fl_hashtable_get(ht, &chr));
    fl_expect("Expect letters {A, M, Z} to have values {65, 77, 90}", *ap == 65 && *mp == 77 && *zp == 90);

    // Replace A-Z by its lowercase codes
    i = 97;
    for (char c = 'A'; c <= 'Z'; c++, i++)        
        fl_hashtable_set(ht, &c, &i);

    fl_expect("Current length after replacing the alphabet letters A to Z with codes 97-122 must be 26", fl_hashtable_length(ht) == 26);

    // Add a-z with its codes
    i = 97;
    for (char c = 'a'; c <= 'z'; c++, i++)        
        fl_hashtable_set(ht, &c, &i);

    fl_expect("Current length after adding the alphabet letters a to z with codes 97-122 must be 52", fl_hashtable_length(ht) == 52);

    ap = (chr = 'a', fl_hashtable_get(ht, &chr));
    mp = (chr = 'm', fl_hashtable_get(ht, &chr));
    zp = (chr = 'z', fl_hashtable_get(ht, &chr));
    fl_expect("Expect letters {a, m, z} to have values {97, 109, 122}", *ap == 97 && *mp == 109 && *zp == 122);

    // Replace a-z by its uppercase codes
    i = 65;
    for (char c = 'a'; c <= 'z'; c++, i++)        
        fl_hashtable_set(ht, &c, &i);

    fl_expect("Current length after replacing the alphabet letters a to z with codes 65-90 must be 52", fl_hashtable_length(ht) == 52);

    // Check mappings
    ap = (chr = 'A', fl_hashtable_get(ht, &chr));
    mp = (chr = 'M', fl_hashtable_get(ht, &chr));
    zp = (chr = 'Z', fl_hashtable_get(ht, &chr));
    fl_expect("Expect letters {A, M, Z} to have values {97, 109, 122}", *ap == 97 && *mp == 109 && *zp == 122);

    ap = (chr = 'a', fl_hashtable_get(ht, &chr));
    mp = (chr = 'm', fl_hashtable_get(ht, &chr));
    zp = (chr = 'z', fl_hashtable_get(ht, &chr));
    fl_expect("Expect letters {a, m, z} to have values {65, 77, 90}", *ap == 65 && *mp == 77 && *zp == 90);
    
    fl_hashtable_delete(ht);
}

void test_fl_hashtable_clear()
{
    FlHashtable ht = fl_hashtable_new(sizeof(char), sizeof(int));

    // Add A-Z with its codes
    int i = 65;
    for (char c = 'A'; c <= 'Z'; c++, i++)
        fl_hashtable_add(ht, &c, &i);

    fl_expect("Current length after adding the alphabet letters A to Z with codes 65-90 must be 26", fl_hashtable_length(ht) == 26);

    fl_hashtable_clear(ht);
    fl_expect("Current length after clear the hashtable content must be 0", fl_hashtable_length(ht) == 0);

    fl_hashtable_delete(ht);
}

void test_fl_hashtable_keys_and_values()
{
    FlHashtable ht = fl_hashtable_new(sizeof(char), sizeof(int));
    // Add A-Z with its codes
    int i = 65;
    for (char c = 'A'; c <= 'Z'; c++, i++)
        fl_hashtable_add(ht, &c, &i);
    
    fl_expect("Current length after adding the alphabet letters A to Z with codes 65-90 must be 26", fl_hashtable_length(ht) == 26);
    
    char *keys = fl_hashtable_keys(ht);
    fl_expect("Keys array must contain 26 elements", fl_array_length(keys) == 26);

    char *values = fl_hashtable_values(ht);
    fl_expect("Values array must contain 26 elements", fl_array_length(values) == 26);

    fl_array_delete(keys);
    fl_array_delete(values);
    fl_hashtable_delete(ht);
}

void test_fl_hashtable_remove()
{
    FlHashtable ht = fl_hashtable_new(sizeof(char), sizeof(int));
    // Add A-Z with its codes
    int i = 65;
    for (char c = 'A'; c <= 'Z'; c++, i++)
        fl_hashtable_add(ht, &c, &i);
    
    fl_expect("Current length after adding the alphabet letters A to Z with codes 65-90 must be 26", fl_hashtable_length(ht) == 26);
    
    char *keys = fl_hashtable_keys(ht);
    size_t keys_length = fl_array_length(keys);

    for (size_t i=0; i < keys_length; i++)
    {
        fl_hashtable_remove(ht, keys+i);
    }

    fl_expect("Current length after removing all the elements by key must be 0", fl_hashtable_length(ht) == 0);

    fl_array_delete(keys);
    fl_hashtable_delete(ht);
}

void test_fl_hashtable_resize()
{
    FlHashtable ht = fl_hashtable_new(sizeof(size_t), sizeof(int));
    srand(time(NULL));
    for (size_t i=0; i < 52736; i++)
    {
        int r = rand();
        fl_hashtable_add(ht, &i, &r);
    }

    fl_hashtable_delete(ht);
}
