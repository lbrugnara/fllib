#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../fl_test_utils.h"
#include "../../src/containers/Vector.h"
#include "../../src/containers/List.h"
#include "../../src/containers/Dictionary.h"
#include "../../src/Cstring.h"
#include "../../src/Array.h"

void
fl_vector_tests() 
{
	if (1)
	{
		int *zeroelem = fl_array_new(sizeof(int), 0);
		flm_test(fl_array_length(zeroelem) == 0);
		fl_array_delete(zeroelem);
		int *integers = fl_array_new(sizeof(int), 5);
		integers[0] = 0;
		integers[1] = 1;
		integers[2] = 2;
		integers[3] = 3;
		integers[4] = 4;
		flm_test(fl_array_length(integers) == 5);
		size_t e = fl_array_length(integers);
		for (int i=0; i < e; i++)
			printf("Integers[%d]=%d\n", i, integers[i]);

		int testn = 0;
		flm_test(fl_array_contains(integers, &testn));
		testn = 1;
		flm_test(fl_array_contains(integers, &testn));
		testn = 2;
		flm_test(fl_array_contains(integers, &testn));
		testn = 5;
		flm_test(!fl_array_contains(integers, &testn));

		integers = fl_array_resize(integers, 10);
		flm_test(fl_array_length(integers) == 10);

	 	size_t ne = fl_array_length(integers);
		for (int i=e; i < ne; i++)
		{
			printf("Adding[%d]=%d\n", i, i);
			integers[i] = i;
		}

		for (int i=0; i < ne; i++)
			printf("Integers[%d]=%d\n", i, integers[i]);

		fl_array_delete(integers);
	}
	
	if (1) {
		FlVector* fv = fl_vector_new(sizeof(int), 5);
	    
	    int integer = 12356;
	    fl_vector_add(fv, &integer);
	    flm_vector_addl(fv, int, 1234);
	    flm_vector_addl(fv, int, 2345);
	    flm_vector_addl(fv, int, 3456);
	    flm_vector_addl(fv, int, 999);

	    for (int i=0; i < fl_vector_length(fv); i++)
	    {
	    	printf("i in foreach = %d\n", flm_vector_get(fv, int, i));
	    }

		int myInt;
		do {
			fl_vector_shift(fv, &myInt);
			printf("shift=%d\n", myInt);
		} while (fl_vector_length(fv));

		flm_vector_addl(fv, int, 987);
	    flm_vector_addl(fv, int, 654);
	    flm_vector_addl(fv, int, 321);
	    myInt = 10;
	    fl_vector_insert(fv, &myInt, 10);

	    FlIterator 	*it = fl_vector_begin(fv);
	    for (; !fl_iterator_is_end(it, fv); fl_iterator_next(it))
	    {
	    	printf("FlVectorIterator value=%d\n", flm_iterator_value(it, int));
	    }
	    fl_iterator_delete(it);

	    do {
			fl_vector_pop(fv, &myInt);
			printf("pop=%d\n", myInt);
		} while (fl_vector_length(fv));

		printf("/* ------------------------------------------------------------------------*/\n");
	    fl_vector_delete(fv);
	}

	if (1) {
		printf("\n==============\n");
		printf("Test: FlVector\n");

		FlVector* fv = fl_vector_new(sizeof(int), 5);
		
		/* Initial capacity 5 but 0 elements in vector */
		flm_test(fl_vector_capacity(fv) == 5);
		flm_test(fl_vector_length(fv) == 0);
	    
	    /* Add 5 integers */
		int integer = 16;
		fl_vector_add(fv, &integer);
		integer += 12;
	    fl_vector_add(fv, &integer);
		integer = 11;
	    fl_vector_add(fv, &integer);
		integer = 98;
	    fl_vector_add(fv, &integer);
	    integer = 1234;
	    fl_vector_add(fv, &integer);
		
		/* Capacity should be 5 and length 5 */
		flm_test(fl_vector_capacity(fv) == 5);
		flm_test(fl_vector_length(fv) == 5);
		
		int thirdElement = *(int*)fl_vector_get(fv, 2);
		int fourthElement = *(int*)fl_vector_get(fv, 3);
		int fifthElement = *(int*)fl_vector_get(fv, 4);
		flm_test(thirdElement == 11);
		flm_test(fourthElement == 98);
		flm_test(fifthElement == 1234);

		flm_vector_addl(fv, int, 1);
		flm_vector_addl(fv, int, 987);
		flm_test(flm_vector_get(fv, int, 6) == 987);

		flm_test(fl_vector_capacity(fv) == 10);
		flm_test(fl_vector_length(fv) == 7);
		
		/*FlIterator *sit = fl_iterator_start(fv);
		FlIterator *eit = fl_iterator_end(fv);
		while(!fl_iterator_equals(fl_iterator_next(sit), eit))
		{
			printf("it.value = %d\n", flm_iterator_value(sit, int));
		}
		fl_free(sit);
		fl_free(eit);
		*/
		int myInt = 5;
		do {
			fl_vector_shift(fv, &myInt);
			printf("myInt=%d\n", myInt);
		} while (fl_vector_length(fv));

		printf("/* ------------------------------------------------------------------------*/\n");
	    fl_vector_delete(fv);
	}

/* ------------------------------------------------------------------------*/

	if (1) {
		FlVector* fv2 = fl_vector_new(sizeof(int*), 5);
		
		/* Initial capacity 5 but 0 elements in vector */
		flm_test(fl_vector_capacity(fv2) == 5);
		flm_test(fl_vector_length(fv2) == 0);
	    
	    /* Add 3 pointers to int */

		// 1- Creates a pointer to an integer
		// 2- Add the pointer to the vector of int* pointers
	    // 3- Change the value in our pointer
		int *pos1 = fl_malloc(sizeof(int));
	    fl_vector_add(fv2, &pos1);
	    *pos1 = 1;

		int *pos2 = fl_malloc(sizeof(int));
	    fl_vector_add(fv2, &pos2);
	    *pos2 = 2;

	    int *pos3 = fl_malloc(sizeof(int));
	    fl_vector_add(fv2, &pos3);
	    *pos3 = 3;

		/* Capacity should be 5 and length 5 */
		flm_test(fl_vector_capacity(fv2) == 5);
		flm_test(fl_vector_length(fv2) == 3);
		
		/* Get the last inserted */
		int* thirdPointer = *(int**)fl_vector_get(fv2, 2);
		flm_test(thirdPointer == pos3);
		flm_test(*thirdPointer == 3);
		
		fl_vector_clear(fv2);
		flm_test(fl_vector_length(fv2) == 0);

	    fl_vector_delete(fv2);
	    free(pos1);
	    free(pos2);
	    free(pos3);
	}

/* ------------------------------------------------------------------------*/
    if (1) {
		FlVector* fv3 = fl_vector_new(sizeof(char*), 15);
		
		/* Initial capacity 5 but 0 elements in vector */
		flm_test(fl_vector_capacity(fv3) == 15);
		flm_test(fl_vector_length(fv3) == 0);
	    
	    /* Creates 3 string */
	    const FlCstr str1 = "Testing vector";
	    const FlCstr str2 = " with pointers ";
	    const FlCstr str3 = " to chars.";

	    FlVector *strv = fl_cstring_split(str1);
	    for (int i=0; i < fl_vector_length(strv); i++)
	    {
	    	printf("Char c = %c\n", flm_vector_get(strv, char, i));
	    }
	    fl_vector_delete(strv);

	    fl_vector_add_cstr(fv3, str1);
	    fl_vector_add_cstr(fv3, str2);
	    fl_vector_add_cstr(fv3, str3);

		/* Capacity should be 5 and length 5 */
		flm_test(fl_vector_capacity(fv3) == 15);
		flm_test(fl_vector_length(fv3) == 3);
		
		/* Get the last inserted */
		char* thirdStrPointer = *(FlCstr*)fl_vector_get(fv3, 2);
		flm_test(strcmp(thirdStrPointer, " to chars.") == 0);
		flm_test(thirdStrPointer[1] == str3[1] && str3[1] == 't');
		
	    fl_vector_delete_ptrs(fv3);
	}
	
	if (1) {
		FlVector* fv3 = fl_vector_new(sizeof(FlCstr), 15);
	
		/* Initial capacity 5 but 0 elements in vector */
		flm_test(fl_vector_capacity(fv3) == 15);
		flm_test(fl_vector_length(fv3) == 0);
	    
	    /* Creates 3 string */
	    const FlCstr str1 = "Testing vector";
	    const FlCstr str2 = " with FlCstr.";
		int l2 = strlen(str2);

		// 1- Creates a pointer to a char pointer
		// 2- Add the pointer to the vector of FlCstr pointers
	    // 3- Copy one of the strings in our new FlCstr
	    fl_vector_add_cstr(fv3, str1);

		FlCstr strpos2 = fl_cstring_dup(str2);
	    fl_vector_add(fv3, &strpos2);

		/* Capacity should be 5 and length 5 */
		flm_test(fl_vector_capacity(fv3) == 15);
		flm_test(fl_vector_length(fv3) == 2);
		
		/* Get the last inserted. get return void* (pointer to the saved element). Cast and dereference */
		FlCstr secondStrPointer = *(FlCstr*)fl_vector_get(fv3, 1);
		flm_test(secondStrPointer == strpos2);
		flm_test(*secondStrPointer == *str2);
		flm_test(secondStrPointer[1] == str2[1] && str2[1] == 'w');
		
	    fl_vector_delete_ptrs(fv3);
	}

	if (1) {
		FlVector* fv = fl_vector_new(sizeof(int), 5);

	    flm_vector_addl(fv, int, 1);
	    flm_vector_addl(fv, int, 3);
	    flm_vector_addl(fv, int, 4);
	    flm_test(fl_vector_length(fv) == 3);
	    flm_test(fl_vector_capacity(fv) == 5);
	    int two = 2;
	    int *twop = fl_vector_insert(fv, &two, 1);
	    flm_test(*twop == two);
	    flm_test(flm_vector_get(fv, int, 1) == 2);
	    flm_test(fl_vector_length(fv) == 4);
	    flm_test(fl_vector_capacity(fv) == 5);
	    int zero = 0;
	    fl_vector_unshift(fv, &zero);
	    flm_test(flm_vector_get(fv, int, 0) == 0);
	    flm_test(flm_vector_get(fv, int, 2) == 2);
	    flm_test(fl_vector_length(fv) == 5);
	    flm_test(fl_vector_capacity(fv) == 5);

	    for (int i=0; i < fl_vector_length(fv); i++)
	    {
	    	printf("i in foreach = %d\n", flm_vector_get(fv, int, i));
	    	// fl_vector_remove(fv, 3, NULL); // Uncomment to receive an ERR_FATAL
	    }

	    fl_vector_delete(fv);
	}

	{
		FlList *list = fl_list_new(sizeof(int));
		int zero = 0, one = 1, three = 3;
		fl_list_insert(list, &zero, 0);
		fl_list_add(list, &one);
		fl_list_add(list, &three);
		
		flm_test(fl_list_length(list) == 3);

		int two = 2, four = 4;
		fl_list_insert(list, &two, 2);
		fl_list_insert(list, &four, 4);
		// fl_list_insert(list, &two, 10); // this will fail

		int* twoptr = fl_list_get(list, 2);
		flm_test(*twoptr == 2);

		int* fourptr = fl_list_get(list, 4);
		flm_test(*fourptr == 4);

		for (int i=0; i < fl_list_length(list); i++)
		{
			printf("i in FlList foreach = %d\n", *(int*)fl_list_get(list, i));
		}
		//int* error = fl_list_get(list, 100);
		//int* error = fl_list_get(list, -1);

		zero = 99;
		fl_list_shift(list, &zero);
		flm_test(zero == 0);
		flm_test(fl_list_length(list) == 4);
		fl_list_unshift(list, &zero);
		flm_test(fl_list_length(list) == 5);
		zero = *(int*)fl_list_get(list, 0);
		flm_test(zero == 0);

		int num = 0;
		flm_test(num == 0 && fl_list_contains(list, &num));
		num = 1;
		flm_test(num == 1 && fl_list_contains(list, &num));
		num = 2;
		flm_test(num == 2 && fl_list_contains(list, &num));
		num = 3;
		flm_test(num == 3 && fl_list_contains(list, &num));
		num = 4;
		flm_test(num == 4 && fl_list_contains(list, &num));
		num = 5;
		flm_test(num == 5 && !fl_list_contains(list, &num));
		num = 123;
		flm_test(num == 123 && !fl_list_contains(list, &num));

		FlIterator 	*it = fl_list_begin(list);
	    for (; !fl_iterator_is_end(it, list); fl_iterator_next(it))
	    {
	    	printf("FlListIterator value=%d\n", flm_iterator_value(it, int));
	    }
	    fl_iterator_delete(it);

		int *numptr = fl_malloc(sizeof(int));
		while(fl_list_length(list))
		{
			fl_list_pop(list, numptr);
			printf("Num = %d\n", *numptr);
		}
		fl_free(numptr);
		flm_test(fl_list_length(list) == 0);
		fl_list_delete(list);
	}

	{
		FlList *list1 = fl_list_new(sizeof(int));
		FlList *list2 = fl_list_new(sizeof(int));
		int zero = 0, one = 1, two = 2, three = 3;
		fl_list_add(list1, &zero);
		fl_list_add(list1, &one);
		fl_list_add(list2, &two);
		fl_list_add(list2, &three);

		flm_test(fl_list_length(list1) == 2);
		flm_test(fl_list_length(list2) == 2);

		fl_list_concat(list1, list2);

		flm_test(fl_list_length(list1) == 4);
		flm_test(fl_list_length(list2) == 2);

		for (int i=0; i < fl_list_length(list2); i++)
		{
			printf("i in FlList2 foreach = %d\n", *(int*)fl_list_get(list2, i));
		}

		fl_list_pop(list2, NULL);
		fl_list_pop(list2, NULL);

		flm_test(fl_list_length(list2) == 0);

		for (int i=0; i < fl_list_length(list1); i++)
		{
			printf("i in FlList1 foreach = %d\n", *(int*)fl_list_get(list1, i));
		}


		fl_list_delete(list1);
		fl_list_delete(list2);
	}

	{
		FlList *list1 = fl_list_new(sizeof(int));
		FlList *list2 = fl_list_new(sizeof(int));
		int zero = 0, one = 1, two = 2, three = 3;
		fl_list_add(list1, &zero);
		fl_list_add(list1, &one);
		fl_list_add(list2, &two);
		fl_list_add(list2, &three);

		flm_test(fl_list_length(list1) == 2);
		flm_test(fl_list_length(list2) == 2);

		FlList *mergedlist = fl_list_merge(list1, list2);

		flm_test(fl_list_length(list1) == 2);
		flm_test(fl_list_length(list2) == 2);
		flm_test(fl_list_length(mergedlist) == 4);

		for (int i=0; i < fl_list_length(list2); i++)
		{
			printf("i in FlList2 foreach = %d\n", *(int*)fl_list_get(list2, i));
		}

		fl_list_pop(list2, NULL);
		fl_list_pop(list2, NULL);
		flm_test(fl_list_length(list2) == 0);

		for (int i=0; i < fl_list_length(list1); i++)
		{
			printf("i in FlList1 foreach = %d\n", *(int*)fl_list_get(list1, i));
		}

		fl_list_pop(list1, NULL);
		fl_list_pop(list1, NULL);
		flm_test(fl_list_length(list1) == 0);

		for (int i=0; i < fl_list_length(mergedlist); i++)
		{
			printf("i in mergedlist foreach = %d\n", *(int*)fl_list_get(mergedlist, i));
		}

		int num;
		fl_list_remove(mergedlist, 3, &num);
		flm_test(num == 3);
		flm_test(fl_list_length(mergedlist) == 3);

		fl_list_clear(mergedlist);
		flm_test(fl_list_length(mergedlist) == 0);

		fl_list_delete(list1);
		fl_list_delete(list2);
		fl_list_delete(mergedlist);
	}

	{
		FlDictionary *dict = fl_dictionary_new(sizeof(FlCstr), sizeof(int));
		flm_test(fl_dictionary_length(dict) == 0);
		flm_test(fl_dictionary_key_dtsize(dict) == sizeof(FlCstr));
		flm_test(fl_dictionary_val_dtsize(dict) == sizeof(int));
		FlCstr k = "one";
		int n = 1;
		fl_dictionary_add(dict, &k, &n);
		flm_test(fl_dictionary_length(dict) == 1);
		flm_test(fl_dictionary_get_val(dict, &k) != NULL);
		flm_test(*(int*)fl_dictionary_get_val(dict, &k) == 1);
		flm_test(flm_cstring_equals(*(FlCstr*)fl_dictionary_get_key(dict, &n), "one"));

		k = "two";
		n = 2;
		fl_dictionary_add(dict, &k, &n);
		//fl_dictionary_add(dict, &k, &n); // ERROR!
		fl_dictionary_set(dict, &k, &n);
		
		k = "three";
		n = 3;
		fl_dictionary_add(dict, &k, &n);
		k = "four";
		n = 4;
		fl_dictionary_set(dict, &k, &n);

		flm_test(fl_dictionary_length(dict) == 4);
		flm_test(fl_dictionary_get_val(dict, &k) != NULL);
		flm_test(*(int*)fl_dictionary_get_val(dict, &k) == 4);
		flm_test(flm_cstring_equals(*(FlCstr*)fl_dictionary_get_key(dict, &n), "four"));

		flm_test(fl_dictionary_contains_key(dict, &k) == true);
		flm_test(fl_dictionary_contains_val(dict, &n) == true);

		FlKeyValuePair *kvp = fl_dictionary_get_kvp(dict, &k);
		flm_test(flm_cstring_equals(*(FlCstr*)fl_kvp_get_key(kvp), "four"));
		flm_test(*(int*)fl_kvp_get_val(kvp) == 4);

		FlCstr *keys = fl_dictionary_keys(dict);
		for (int i=0; i < fl_array_length(keys); i++)
		{
			printf("Dictionary[%s] => %d\n", keys[i], *(int*)fl_dictionary_get_val(dict, keys+i));
		}
		fl_array_delete(keys);

		k = "five";
		n = 5;
		flm_test(fl_dictionary_contains_key(dict, &k) == false);
		flm_test(fl_dictionary_contains_val(dict, &n) == false);

		FlDictionary *dict2 = fl_dictionary_new(sizeof(FlCstr), sizeof(int));
		fl_dictionary_add(dict2, &k, &n);
		k = "six";
		n = 6;
		fl_dictionary_add(dict2, &k, &n);

		flm_test(fl_dictionary_length(dict2) == 2);

		fl_dictionary_concat(dict, dict2);
		flm_test(fl_dictionary_length(dict) == 6);

		FlIterator 	*it = fl_dictionary_begin(dict);
	    for (; !fl_iterator_is_end(it, dict); fl_iterator_next(it))
	    {
	    	FlKeyValuePair *kvp = flm_iterator_value(it, FlKeyValuePair*);
	    	printf("Dictionary Iterator key=%s|value=%d\n", *(FlCstr*)fl_kvp_get_key(kvp), *(int*)fl_kvp_get_val(kvp));
	    }
	    fl_iterator_delete(it);

		int *values = fl_dictionary_values(dict);
		for (int i=0; i < fl_array_length(values); i++)
		{
			printf("Dictionary[%s] => %d\n", *(FlCstr*)fl_dictionary_get_key(dict, values+i), values[i]);
		}
		fl_array_delete(values);

		fl_dictionary_clear(dict2);
		flm_test(fl_dictionary_length(dict2) == 0);

		k = "seven";
		n = 7;
		fl_dictionary_add(dict2, &k, &n);

		FlDictionary *dict3 = fl_dictionary_merge(dict, dict2);

		flm_test(fl_dictionary_length(dict) == 6);
		flm_test(fl_dictionary_length(dict2) == 1);
		flm_test(fl_dictionary_length(dict3) == 7);

		int out;
		k = "one";
		fl_dictionary_remove(dict, &k, &out);
		flm_test(out == 1);
		k = "two";
		fl_dictionary_remove(dict, &k, &out);
		flm_test(out == 2);
		k = "three";
		fl_dictionary_remove(dict, &k, &out);
		flm_test(out == 3);

		flm_test(fl_dictionary_length(dict) == 3);

		fl_dictionary_delete(dict);
		fl_dictionary_delete(dict2);
		fl_dictionary_delete(dict3);
	}
	printf("FlVector -> Ok\n");
	printf("Max used mem: %zu kbytes\n", fl_memtrack_get()/1024);
}
