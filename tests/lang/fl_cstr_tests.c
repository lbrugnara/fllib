#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>

#include "../fl_test_utils.h"
#include "../../src/Cstr.h"

void
fl_cstr_tests()
{
	printf("\n==============\n");
	printf("Test: fl_cstr_split\n");
	printf("Input: 'Hello world'\n");

	FlCstr str = "Hello world";
	FlCstr str2 = fl_cstr_dup(str);

	FlVector *v = fl_cstr_split(str2);
	for(int i=0; i < fl_vector_length(v); i++)
		printf("Value in strarray[%d]=%c\n", i, flm_vector_get(v, char, i));

	free(str2);
	fl_vector_delete(v);

	FlCstr rstr = fl_cstr_replace_char("testing some code with string replace functionality", 'e', "zxc");
	printf("%s\n", rstr);
	fl_cstr_delete(rstr);

	FlCstr append = fl_cstr_dup("Hello ");
	flm_test(strlen(append) == 6);
	fl_cstr_append(&append, "world!");
	flm_test(strlen(append) == 12);
	flm_test(strcmp(append, "Hello world!") == 0);

	printf("Append <-->\\r\\n to %s\n", append);
	fl_cstr_append(fl_cstr_append(fl_cstr_append(fl_cstr_append(&append, "<"), "--"), ">"), "\r\n");
	flm_test(strcmp(append, "Hello world!<-->\r\n") == 0);
	flm_test(strlen(append) == 18);
	fl_cstr_append_char(&append, 'x');
	fl_cstr_append_char(&append, 'y');
	fl_cstr_append_char(&append, 'z');
	printf("%s\n", append);
	fl_cstr_delete(append);
	printf("FlCstr -> Ok\n");

	{
		FlVector* str_vector = flm_vector_new(FlCstr, 3);
		
		/* Creates 3 string */
	    FlCstr str1 = fl_cstr_dup("one");
	    FlCstr str2 = fl_cstr_dup("two");
	    FlCstr str3 = fl_cstr_dup("three");

	    fl_vector_add(str_vector, &str1);
	    fl_vector_add(str_vector, &str2);
	    fl_vector_add(str_vector, &str3);

	    for (int i=0; i < fl_vector_length(str_vector); i++)
	    	printf("%s\n", flm_vector_get(str_vector, FlCstr, i));

	    FlCstr str4 = flm_vector_get(str_vector, FlCstr, 1);
	    printf("str4 = %s\n", str4);

		FlCstr str = fl_cstr_join(str_vector, ", ");
		printf("%s\n",str);
		flm_test(strcmp(str, "one, two, three") == 0);
		flm_test(strlen(str) == 15);

		free(str);
	    fl_vector_delete_ptrs(str_vector);
	}

	{
		FlError *error = NULL;
		fl_error_set(&error, 99, "%sa", "str");
		flm_test(fl_error_get_id(error) == 99);
		FlCstr msg = fl_error_get_message(error);
		printf("%s - %zu\n", msg, strlen(msg));
		flm_test(strlen(msg) == 4);
		flm_test(flm_cstr_equals(msg, "stra"));
		fl_error_delete(error);
	}

	{
		FlError *error = NULL;
		fl_error_set(&error, 99, "Test [%c][%d][%s]", 'a',1,"str");
		flm_test(fl_error_get_id(error) == 99);
		FlCstr msg = fl_error_get_message(error);
		printf("%s\n", msg);
		flm_test(strlen(msg) == 16);
		flm_test(flm_cstr_equals(msg, "Test [a][1][str]"));
		fl_error_delete(error);

		fl_error_set(&error, 98, "My %s message", "error");
		flm_test(fl_error_get_id(error) == 98);
		msg = fl_error_get_message(error);
		printf("%s\n", msg);
		flm_test(flm_cstr_equals(msg, "My error message"));
		fl_error_delete(error);

	}

	{
		FlCstr msg = NULL;

		msg = fl_cstr_vdup("%d, %d!", 1, 2);
		flm_test(flm_cstr_equals(msg, "1, 2!"));
		fl_cstr_delete(msg);

		msg = fl_cstr_vdup("%c, %c!", 'a', 'b');
		flm_test(flm_cstr_equals(msg, "a, b!"));
		fl_cstr_delete(msg);

		msg = fl_cstr_vdup("%s, %s!", "Hello", "World");
		flm_test(flm_cstr_equals(msg, "Hello, World!"));
		fl_cstr_delete(msg);
	}

	{
		flm_test(fl_cstr_contains("Hello world!", "o w"));
		flm_test(fl_cstr_contains("Hello world!", ""));
		flm_test(fl_cstr_contains("Hello world!", "!"));
		flm_test(fl_cstr_contains("Hello world!", "wor"));
	}

	{
		FlCstr replaced = fl_cstr_replace("", "abc", "123");
		flm_test(flm_cstr_equals(replaced, ""));
		fl_cstr_delete(replaced);

		replaced = fl_cstr_replace("abc", "", "123");
		flm_test(flm_cstr_equals(replaced, "123abc"));
		fl_cstr_delete(replaced);

		replaced = fl_cstr_replace("", "", "123");
		flm_test(flm_cstr_equals(replaced, "123"));
		fl_cstr_delete(replaced);

		replaced = fl_cstr_replace("Remove my Es", "e", "");
		flm_test(flm_cstr_equals(replaced, "Rmov my Es"));
		fl_cstr_delete(replaced);

		replaced = fl_cstr_replace("hello", "k", "z");
		flm_test(flm_cstr_equals(replaced, "hello"));
		fl_cstr_delete(replaced);

		replaced = fl_cstr_replace("hello", "nhelloz", "z");
		flm_test(flm_cstr_equals(replaced, "hello"));
		fl_cstr_delete(replaced);

		replaced = fl_cstr_replace("hello", "e", "a");
		flm_test(flm_cstr_equals(replaced, "hallo"));
		fl_cstr_delete(replaced);

		replaced = fl_cstr_replace("hello", "ll", "l");
		flm_test(flm_cstr_equals(replaced, "helo"));
		fl_cstr_delete(replaced);

		replaced = fl_cstr_replace("hello", "ll", "lll");
		flm_test(flm_cstr_equals(replaced, "helllo"));
		fl_cstr_delete(replaced);

		replaced = fl_cstr_replace("hello", "hello", "");
		flm_test(flm_cstr_equals(replaced, ""));
		fl_cstr_delete(replaced);

		replaced = fl_cstr_replace("hellobye", "hellox", "");
		flm_test(flm_cstr_equals(replaced, "hellobye"));
		fl_cstr_delete(replaced);

		replaced = fl_cstr_replace("Hello world!", "o w", "O W");
		flm_test(flm_cstr_equals(replaced, "HellO World!"));
		fl_cstr_delete(replaced);

		replaced = fl_cstr_replace("hellohello", "jello", "zello");
		flm_test(flm_cstr_equals(replaced, "hellohello"));
		fl_cstr_delete(replaced);

		const FlCstr msg = "Actually, we are changing all the text by a larger one";
		replaced = fl_cstr_replace("none", "none", msg);
		flm_test(flm_cstr_equals(replaced, msg));
		fl_cstr_delete(replaced);

		const FlCstr shortermsg = "Now, a shorter one";
		replaced = fl_cstr_replace(msg, msg, shortermsg);
		flm_test(flm_cstr_equals(replaced, shortermsg));
		fl_cstr_delete(replaced);
	}

	printf("Max used mem: %zu kbytes\n", fl_memtrack_get()/1024);
}