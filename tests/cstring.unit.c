#include <limits.h>
#include <fllib.h>
#include <flut/flut.h>

flut_suite(cstring) {
    flut_suite_description("C string functions");
    flut_suite_register_test(cstring_new, "Cstring new");
    flut_suite_register_test(cstring_dup, "Cstring dup");
    flut_suite_register_test(cstring_split, "Cstring split");
    flut_suite_register_test(cstring_replace_char, "Cstring replace char");
    flut_suite_register_test(cstring_replace, "Cstring replace");
    flut_suite_register_test(cstring_append, "Cstring append");
    flut_suite_register_test(cstring_join, "Cstring join");
    flut_suite_register_test(cstring_misc, "Cstring misc");
    flut_suite_register_test(cstring_find, "Cstring find");
}

static inline size_t integer_length(long long i) {
    size_t l = i >= 0 ? 1 : 2;
    while (i /= 10)
        l++;
    return l;
}

static inline size_t uinteger_length(unsigned long long i) {
    size_t l = 1;
    while (i /= 10)
        l++;
    return l;
}

flut_test(cstring_new) {
    flut_describe("fl_cstring_new should return valid pointers") {
        char *str = fl_cstring_new(5);
        
        flut_assert_is_not_null(str);

        flut_assert_string_has_length(0, str, false);

        str[0] = 'H';
        str[1] = 'e';
        str[2] = 'l';
        str[3] = 'l';
        str[4] = 'o';

        flut_assert_string_has_length(5, str, false);
        flut_assert_is_true(str[5] == '\0');

        fl_cstring_free(str);
    }
}

flut_test(cstring_dup) {
    char *str = fl_cstring_dup("Hello");
    flut_assert_is_not_null(str);
    flut_assert_string_is_equals("Hello", str, true);
}

flut_test(cstring_split) {
    FlVector *v = fl_cstring_split("Hello");

    flut_assert_is_true(fl_vector_length(v) == 5);

    size_t length = fl_vector_length(v);
    flut_assert_char_is_equals('H', flm_vector_get(v, char, 0));
    flut_assert_char_is_equals('e', flm_vector_get(v, char, 1));
    flut_assert_char_is_equals('l', flm_vector_get(v, char, 2));
    flut_assert_char_is_equals('l', flm_vector_get(v, char, 3));
    flut_assert_char_is_equals('o', flm_vector_get(v, char, 4));
    fl_vector_free(v);
}

flut_test(cstring_replace_char) {
    char *world = "World";
    char *worl = fl_cstring_replace_char(world, 'd', "");
    flut_assert_string_is_equals("Worl", worl, false);

    char *word = fl_cstring_replace_char(worl, 'l', "d");
    flut_assert_string_is_equals("Word", word, false);
    fl_cstring_free(word);
    fl_cstring_free(worl);

    char *dot = "object.property";
    char *no_dot = fl_cstring_replace_char(dot, '.', "");
    flut_assert_string_is_equals("objectproperty", no_dot, true);

    char *multiple_a = "abcabcabca";
    char *no_a = fl_cstring_replace_char(multiple_a, 'a', "");
    flut_assert_string_is_equals("bcbcbc", no_a, true);

    char *with_zz = fl_cstring_replace_char(multiple_a, 'a', "zz");
    flut_assert_string_is_equals("zzbczzbczzbczz", with_zz, true);
}

flut_test(cstring_replace) {
    char *replaced = fl_cstring_replace("", "abc", "123");
    flut_assert_string_is_equals("", replaced, true);

    replaced = fl_cstring_replace("abc", "", "123");
    flut_assert_string_is_equals("123abc", replaced, true);

    replaced = fl_cstring_replace("", "", "123");
    flut_assert_string_is_equals("123", replaced, true);

    replaced = fl_cstring_replace("Remove my Es", "e", "");
    flut_assert_string_is_equals("Rmov my Es", replaced, true);

    replaced = fl_cstring_replace("hello", "k", "z");
    flut_assert_string_is_equals("hello", replaced, true);

    replaced = fl_cstring_replace("hello", "nhelloz", "z");
    flut_assert_string_is_equals("hello", replaced, true);

    replaced = fl_cstring_replace("hello", "e", "a");
    flut_assert_string_is_equals("hallo", replaced, true);

    replaced = fl_cstring_replace("hello", "ll", "l");
    flut_assert_string_is_equals("helo", replaced, true);

    replaced = fl_cstring_replace("hello", "ll", "lll");
    flut_assert_string_is_equals("helllo", replaced, true);

    replaced = fl_cstring_replace("hello", "l", "ll");
    flut_assert_string_is_equals("hellllo", replaced, true);

    replaced = fl_cstring_replace("hello", "l", "zz");
    flut_assert_string_is_equals("hezzzzo", replaced, true);

    replaced = fl_cstring_replace("hello", "hello", "");
    flut_assert_string_is_equals("", replaced, true);

    replaced = fl_cstring_replace("hellobye", "hellox", "");
    flut_assert_string_is_equals("hellobye", replaced, true);

    replaced = fl_cstring_replace("Hello world!", "o w", "O W");
    flut_assert_string_is_equals("HellO World!", replaced, true);

    replaced = fl_cstring_replace("hellohello", "jello", "zello");
    flut_assert_string_is_equals("hellohello", replaced, true);

    const char *msg = "Actually, we are changing all the text by a larger one";
    replaced = fl_cstring_replace("none", "none", msg);
    flut_assert_string_is_equals(msg, replaced, true);

    const char *shortermsg = "Now, a shorter one";
    replaced = fl_cstring_replace(msg, msg, shortermsg);
    flut_assert_string_is_equals(shortermsg, replaced, true);
}

flut_test(cstring_append) {
    char *helloWorld = fl_cstring_dup("Hello ");
    fl_cstring_append(&helloWorld, "world!");

    flut_assert_string_is_equals("Hello world!", helloWorld, false);
    flut_assert_string_has_length(12, helloWorld, false);

    fl_cstring_append(fl_cstring_append(fl_cstring_append(fl_cstring_append(&helloWorld, "\n"), "Bye"), "."), ".");
    flut_assert_string_is_equals("Hello world!\nBye..", helloWorld, false);

    fl_cstring_append_char(&helloWorld, '.');
    fl_cstring_append_char(&helloWorld, '\n');
    flut_assert_string_is_equals("Hello world!\nBye...\n", helloWorld, true);
}

flut_test(cstring_join) {
    FlVector *str_vector = flm_vector_new_with(.capacity = 3, .cleaner = fl_container_cleaner_pointer);
    char *str1 = fl_cstring_dup("one");
    char *str2 = fl_cstring_dup("two");
    char *str3 = fl_cstring_dup("three");

    fl_vector_add(str_vector, &str1);
    fl_vector_add(str_vector, &str2);
    fl_vector_add(str_vector, &str3);
    char *str = fl_cstring_join(str_vector, ", ");

    flut_assert_string_is_equals("one, two, three", str, false);
    flut_assert_string_has_length(15, str, true);

    fl_vector_free(str_vector);
}

flut_test(cstring_misc) {
    flut_assert_size_t_is_equals(1, integer_length(1));
    flut_assert_size_t_is_equals(2, integer_length(-1));
    flut_assert_size_t_is_greater_than_or_equals(19, integer_length(LLONG_MAX));
    flut_assert_size_t_is_greater_than_or_equals(20, integer_length(LLONG_MIN));
    flut_assert_size_t_is_equals(1, uinteger_length(1));
    flut_assert_size_t_is_greater_than_or_equals(20, uinteger_length(ULLONG_MAX));
    flut_assert_size_t_is_equals(1, uinteger_length(0));
}

flut_test(cstring_find) {
    flut_describe("Find should find ASCII chars within UTF-8 strings")
    {
        char *a = fl_cstring_find("兔¡¢£¤¥a¦§¨©ª«¬­®", "a");
        flut_assert_is_not_null(a);
        flut_assert_char_is_equals('a', *a);
    }

    flut_describe("Find should find literal UTF-8 chars within UTF-8 strings")
    {
        char *rabbit = fl_cstring_find("兔¡¢£¤¥兔¦§¨©ª«¬­®", "兔");
        flut_assert_is_not_null(rabbit);
        flut_assert_string_is_equals_n("兔", rabbit, strlen("兔"), false);
    }
}
