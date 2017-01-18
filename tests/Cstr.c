#include <fllib.h>

#include "Cstr.h"
#include "Test.h"

void test_cstr_new()
{
    FlCstr str = fl_cstr_new(5);
    fl_expect("fl_cstr_new(5) returns a valid pointer", str != NULL);
    str[0] = 'H';
    str[1] = 'e';
    str[2] = 'l';
    str[3] = 'l';
    str[4] = 'o';
    fl_expect("Assign each character into str to form \"Hello\"", flm_cstr_equals(str, "Hello"));
    fl_expect("str[5] must be the null character", str[5] == '\0');
    fl_cstr_delete(str);
}

void test_cstr_dup()
{
    FlCstr str = fl_cstr_dup("Hello");
    fl_expect("str not null", str != NULL);
    fl_expect("str == \"Hello\"", flm_cstr_equals(str, "Hello"));
    fl_cstr_delete(str);
}

void test_cstr_split()
{
    FlVector v = fl_cstr_split("Hello");
    size_t length = fl_vector_length(v);
    fl_expect("Split resulted in a vector with length 5", length == 5);
    for(int i=0; i < length; i++)
    {
        char c = flm_vector_get(v, char, i);
        switch(i)
        {
            case 0:
                fl_expect("Char in position 0 must be 'H'", c == 'H');
                break;
            case 1:
                fl_expect("Char in position 1 must be 'e'", c == 'e');
                break;
            case 2:
                fl_expect("Char in position 2 must be 'l'", c == 'l');
                break;
            case 3:
                fl_expect("Char in position 3 must be 'l'", c == 'l');
                break;
            case 4:
                fl_expect("Char in position 4 must be 'o'", c == 'o');
                break;
        }
    }
    fl_vector_delete(v);
}

void test_cstr_replace_char()
{
    FlCstr world = "World";
    FlCstr worl = fl_cstr_replace_char(world, 'd', "");
    fl_expect("Replace char 'd' with empty string in 'World' results in 'Worl'", flm_cstr_equals(worl, "Worl"));
    FlCstr word = fl_cstr_replace_char(worl, 'l', "d");
    fl_expect("Replace char 'l' with 'd' in 'Worl' results in 'Word'", flm_cstr_equals(word, "Word"));
    fl_cstr_delete(word);
    fl_cstr_delete(worl);

    FlCstr dot = "object.property";
    FlCstr noDot = fl_cstr_replace_char(dot, '.', "");
    fl_expect("Replace char '.' with empty string in 'object.property' results in 'objectproperty'", flm_cstr_equals(noDot, "objectproperty"));
    fl_cstr_delete(noDot);

    FlCstr multipleA = "abcabcabca";
    FlCstr noA = fl_cstr_replace_char(multipleA, 'a', "");
    fl_expect("Replace char 'a' with empty string in 'abcabcabca' results in 'bcbcbc'", flm_cstr_equals(noA, "bcbcbc"));
    fl_cstr_delete(noA);

    FlCstr withZz = fl_cstr_replace_char(multipleA, 'a', "zz");
    fl_expect("Replace char 'a' with string 'zz' in 'abcabcabca' results in 'zzbczzbczzbczz'", flm_cstr_equals(withZz, "zzbczzbczzbczz"));
    fl_cstr_delete(withZz);
}

void test_cstr_append()
{
    FlCstr helloWorld = fl_cstr_dup("Hello ");
    fl_cstr_append(&helloWorld, "world!");
    fl_expect("Append 'world!' to string 'Hello ' results in 'Hello world!'", flm_cstr_equals(helloWorld, "Hello world!"));
    fl_expect("Combined string  'Hello world!' has 12 characters", strlen(helloWorld) == 12);
    fl_cstr_append(fl_cstr_append(fl_cstr_append(fl_cstr_append(&helloWorld, "\n"), "Bye"), "."), ".");
    fl_expect("4 calls to append with '\\n', 'Bye', '.' and '.' results in string 'Hello world!\\nBye..'", flm_cstr_equals(helloWorld, "Hello world!\nBye.."));

    fl_cstr_append_char(&helloWorld, '.');
    fl_cstr_append_char(&helloWorld, '\n');
    fl_expect("2 calls to append_char with '.' and '\\n' results in 'Hello world!\\nBye...\\n'", flm_cstr_equals(helloWorld, "Hello world!\nBye...\n"));
    fl_cstr_delete(helloWorld);
}

void test_cstr_join()
{
    FlVector str_vector = flm_vector_new(FlCstr, 3);
    FlCstr str1 = fl_cstr_dup("one");
    FlCstr str2 = fl_cstr_dup("two");
    FlCstr str3 = fl_cstr_dup("three");

    fl_vector_add(str_vector, &str1);
    fl_vector_add(str_vector, &str2);
    fl_vector_add(str_vector, &str3);
    FlCstr str = fl_cstr_join(str_vector, ", ");
    fl_expect("Join vector with three items 'one', 'two' and 'three' using ', ' as glue, results in 'one, two, three'", flm_cstr_equals(str, "one, two, three"));
    fl_expect("Length of previous joined string is 15 characters", strlen(str) == 15);
    fl_cstr_delete(str);
    fl_vector_delete_ptrs(str_vector);
}
