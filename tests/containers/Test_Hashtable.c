#include <fllib.h>
#include "../Test.h"
#include "Test_Hashtable.h"

void test_fl_hashtable_add()
{
    // Common use cases
    {
        FlHashtable ht = fl_hashtable_new(sizeof(char), sizeof(int), NULL, NULL);
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
        FlHashtable ht2 = fl_hashtable_new(sizeof(char*), sizeof(int*), NULL, NULL);
        char *key = "one";
        int *val = NULL;
        int **vp = (int**)fl_hashtable_add(ht2, &key, &val);
        fl_expect("Call to hashtable_add(\"one\", NULL) must return NULL", NULL == *vp);
        
        key = NULL;
        int v = 95;
        val = &v;
        vp = (int**)fl_hashtable_add(ht2, &key, &val);
        fl_expect("Call to hashtable_add(NULL, 95) must return 95", v == **vp);

        fl_hashtable_delete(ht2);
    }

    // Normal usage
    {
        FlHashtable ht = fl_hashtable_new(sizeof(char), sizeof(int), NULL, NULL);
        for (char c = 'A', i = 65; c <= 'Z'; c++, i++)        
            fl_hashtable_add(ht, &c, &i);
        for (char c = 'a', i = 97; c <= 'z'; c++, i++)        
            fl_hashtable_add(ht, &c, &i);
        
        fl_hashtable_delete(ht);
    }
}

size_t hash_func(FlByte *key, size_t ksize)
{
    return 1;
}

void test_fl_hashtable_add_fhash()
{
    FlHashtable ht = fl_hashtable_new(sizeof(char), sizeof(int), &hash_func, NULL);
    
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
        FlHashtable ht = fl_hashtable_new(sizeof(char), sizeof(int), NULL, NULL);
        
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
