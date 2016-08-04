#include "../src/Cstr.h"

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