#include <limits.h>
#include <fllib.h>

#include "Test.h"
#include "Test_Cstring.h"

static inline size_t integer_length(long long i)
{
    size_t l = i >= 0 ? 1 : 2;
    while (i /= 10)
        l++;
    return l;
}

static inline size_t uinteger_length(unsigned long long i)
{
    size_t l = 1;
    while (i /= 10)
        l++;
    return l;
}

void test_cstring_new()
{
    char *str = fl_cstring_new(5);
    fl_expect("fl_cstring_new(5) returns a valid pointer", str != NULL);
    str[0] = 'H';
    str[1] = 'e';
    str[2] = 'l';
    str[3] = 'l';
    str[4] = 'o';
    fl_expect("Assign each character into str to form \"Hello\"", flm_cstring_equals(str, "Hello"));
    fl_expect("Because fl_cstring_new adds 1 byte for the NULL character, str[5] must be the NULL character", str[5] == '\0');
    fl_cstring_free(str);
}

void test_cstring_dup()
{
    char *str = fl_cstring_dup("Hello");
    fl_expect("str not null", str != NULL);
    fl_expect("str == \"Hello\"", flm_cstring_equals(str, "Hello"));
    fl_cstring_free(str);
}

void test_cstring_split()
{
    FlVector v = fl_cstring_split("Hello");
    size_t length = fl_vector_length(v);
    fl_expect("Split resulted in a vector with length 5", length == 5);
    for(size_t i=0; i < length; i++)
    {
        char c = *(char*)fl_vector_get(v, i);
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
    fl_vector_free(v);
}

void test_cstring_replace_char()
{
    char *world = "World";
    char *worl = fl_cstring_replace_char(world, 'd', "");
    fl_expect("Replace char 'd' with empty string in 'World' results in 'Worl'", flm_cstring_equals(worl, "Worl"));
    char *word = fl_cstring_replace_char(worl, 'l', "d");
    fl_expect("Replace char 'l' with 'd' in 'Worl' results in 'Word'", flm_cstring_equals(word, "Word"));
    fl_cstring_free(word);
    fl_cstring_free(worl);

    char *dot = "object.property";
    char *noDot = fl_cstring_replace_char(dot, '.', "");
    fl_expect("Replace char '.' with empty string in 'object.property' results in 'objectproperty'", flm_cstring_equals(noDot, "objectproperty"));
    fl_cstring_free(noDot);

    char *multipleA = "abcabcabca";
    char *noA = fl_cstring_replace_char(multipleA, 'a', "");
    fl_expect("Replace char 'a' with empty string in 'abcabcabca' results in 'bcbcbc'", flm_cstring_equals(noA, "bcbcbc"));
    fl_cstring_free(noA);

    char *withZz = fl_cstring_replace_char(multipleA, 'a', "zz");
    fl_expect("Replace char 'a' with string 'zz' in 'abcabcabca' results in 'zzbczzbczzbczz'", flm_cstring_equals(withZz, "zzbczzbczzbczz"));
    fl_cstring_free(withZz);
}

void test_cstring_replace()
{
    char *replaced = fl_cstring_replace("", "abc", "123");
    fl_expect("Replace 'abc' with '123' on empty string returns empty string", flm_cstring_equals(replaced, ""));
    fl_cstring_free(replaced);

    replaced = fl_cstring_replace("abc", "", "123");
    fl_expect("Replace empty string with '123' on 'abc' returns '123abc'", flm_cstring_equals(replaced, "123abc"));
    fl_cstring_free(replaced);

    replaced = fl_cstring_replace("", "", "123");
    fl_expect("Replace empty string with '123' on empty string returns '123", flm_cstring_equals(replaced, "123"));
    fl_cstring_free(replaced);

    replaced = fl_cstring_replace("Remove my Es", "e", "");
    fl_expect("Replace 'e' with empty string in 'Remove my Es' returns 'Remov my Es'", flm_cstring_equals(replaced, "Rmov my Es"));
    fl_cstring_free(replaced);

    replaced = fl_cstring_replace("hello", "k", "z");
    fl_expect("Replace 'k' with 'z' on 'hello' returns 'hello'", flm_cstring_equals(replaced, "hello"));
    fl_cstring_free(replaced);

    replaced = fl_cstring_replace("hello", "nhelloz", "z");
    fl_expect("Replace 'nhelloz' with 'z' on 'hello' returns 'hello'", flm_cstring_equals(replaced, "hello"));
    fl_cstring_free(replaced);

    replaced = fl_cstring_replace("hello", "e", "a");
    fl_expect("Replace 'e' with 'a' in 'hello' returns 'hallo'", flm_cstring_equals(replaced, "hallo"));
    fl_cstring_free(replaced);

    replaced = fl_cstring_replace("hello", "ll", "l");
    fl_expect("Replace 'll' with 'l' on 'hello' returns 'helo'", flm_cstring_equals(replaced, "helo"));
    fl_cstring_free(replaced);

    replaced = fl_cstring_replace("hello", "ll", "lll");
    fl_expect("Replace 'll' with 'lll' on 'hello' returns 'helllo'", flm_cstring_equals(replaced, "helllo"));
    fl_cstring_free(replaced);

    replaced = fl_cstring_replace("hello", "l", "ll");
    fl_expect("Replace 'l' with 'll' on 'hello' returns 'hellllo'", flm_cstring_equals(replaced, "hellllo"));
    fl_cstring_free(replaced);

    replaced = fl_cstring_replace("hello", "l", "zz");
    fl_expect("Replace 'l' with 'zz' on 'hello' returns 'hezzzzo'", flm_cstring_equals(replaced, "hezzzzo"));
    fl_cstring_free(replaced);

    replaced = fl_cstring_replace("hello", "hello", "");
    fl_expect("Replace 'hello' with empty string on 'hello' returns empty string", flm_cstring_equals(replaced, ""));
    fl_cstring_free(replaced);

    replaced = fl_cstring_replace("hellobye", "hellox", "");
    fl_expect("Replace 'hellox' with empty string in 'hellobye' returns 'hellobye'", flm_cstring_equals(replaced, "hellobye"));
    fl_cstring_free(replaced);

    replaced = fl_cstring_replace("Hello world!", "o w", "O W");
    fl_expect("Replace 'o w' with 'O W' on 'Hello world!' returns 'HellO World!'", flm_cstring_equals(replaced, "HellO World!"));
    fl_cstring_free(replaced);

    replaced = fl_cstring_replace("hellohello", "jello", "zello");
    fl_expect("Replace 'jello' with 'zello' on 'hellohello' returns 'hellohello'", flm_cstring_equals(replaced, "hellohello"));
    fl_cstring_free(replaced);

    const char *msg = "Actually, we are changing all the text by a larger one";
    replaced = fl_cstring_replace("none", "none", msg);
    fl_expect("Replace 'none' with 'Actually, we are changing all the text by a larger one' on 'none' returns 'Actually, we are changing all the text by a larger one'", flm_cstring_equals(replaced, msg));
    fl_cstring_free(replaced);

    const char *shortermsg = "Now, a shorter one";
    replaced = fl_cstring_replace(msg, msg, shortermsg);
    fl_expect("Replace 'Actually, we are changing all the text by a larger one' with 'Now, a shorter one' on 'Actually, we are changing all the text by a larger one' returns 'Now, a shorter one'", flm_cstring_equals(replaced, shortermsg));
    fl_cstring_free(replaced);
}

void test_cstring_append()
{
    char *helloWorld = fl_cstring_dup("Hello ");
    fl_cstring_append(&helloWorld, "world!");
    fl_expect("Append 'world!' to string 'Hello ' results in 'Hello world!'", flm_cstring_equals(helloWorld, "Hello world!"));
    fl_expect("Combined string  'Hello world!' has 12 characters", strlen(helloWorld) == 12);
    fl_cstring_append(fl_cstring_append(fl_cstring_append(fl_cstring_append(&helloWorld, "\n"), "Bye"), "."), ".");
    fl_expect("4 calls to append with '\\n', 'Bye', '.' and '.' results in string 'Hello world!\\nBye..'", flm_cstring_equals(helloWorld, "Hello world!\nBye.."));

    fl_cstring_append_char(&helloWorld, '.');
    fl_cstring_append_char(&helloWorld, '\n');
    fl_expect("2 calls to append_char with '.' and '\\n' results in 'Hello world!\\nBye...\\n'", flm_cstring_equals(helloWorld, "Hello world!\nBye...\n"));
    fl_cstring_free(helloWorld);
}

void test_cstring_join()
{
    FlVector str_vector = fl_vector_new(3, fl_container_cleaner_pointer);
    char *str1 = fl_cstring_dup("one");
    char *str2 = fl_cstring_dup("two");
    char *str3 = fl_cstring_dup("three");

    fl_vector_add(str_vector, str1);
    fl_vector_add(str_vector, str2);
    fl_vector_add(str_vector, str3);
    char *str = fl_cstring_join(str_vector, ", ");
    fl_expect("Join vector with three items 'one', 'two' and 'three' using ', ' as glue, results in 'one, two, three'", flm_cstring_equals(str, "one, two, three"));
    fl_expect("Length of previous joined string is 15 characters", strlen(str) == 15);
    fl_cstring_free(str);
    fl_vector_free(str_vector);
}

void test_cstring_misc()
{
    size_t length;
    long long number;

    number = 1;
    length = integer_length(number);
    fl_vexpect(1 == length, "Integer %lld to string contains %d chars (length = %d)", number, 1, length);

    number = -1;
    length = integer_length(number);
    fl_vexpect(2 == length, "Integer %lld to string contains %d chars (length = %d)", number, 2, length);

    number = LLONG_MAX;
    length = integer_length(number);
    fl_vexpect(19 >= length, "Integer %lld (LLONG_MAX) to string is at least %d chars (Min 64 bits) (length = %d)", number, 19, length);

    number = LLONG_MIN;
    length = integer_length(number);
    fl_vexpect(20 >= length, "Integer %lld (LLONG_MAX) to string is at least %d chars (Min 64 bits) (length = %d)", number, 20, length);

    //
    unsigned long long unumber;
    unumber = 1;
    length = uinteger_length(unumber);
    fl_vexpect(1 == length, "Integer %llu to string contains %d chars (length = %d)", unumber, 1, length);

    unumber = ULLONG_MAX;
    length = uinteger_length(unumber);
    fl_vexpect(20 >= length, "Integer %llu (ULLONG_MAX) to string is at least %d chars (Min 64 bits) (length = %d)", unumber, 20, length);

    unumber = 0;
    length = uinteger_length(unumber);
    fl_vexpect(1 == length, "Integer %llu (ULLONG_MIN) to string is %d chars (length = %d)", unumber, 1, length);
}
