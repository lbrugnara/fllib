#include "Test.h"
#include "../src/Cstr.h"

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
    int i=1, j=0;
    int d = i/j;
    //while(i++){ if (i == 50000)   {} };
    //int *p = NULL; *p = 2;

    fl_expect("Fail misserably", false);
    fl_expect("str not null", str != NULL);
    fl_expect("str == \"Hello\"", flm_cstr_equals(str, "Hello"));
    fl_cstr_delete(str);
}

void test_cstr_sig()
{
    fl_expect("Success", true);
}

extern void test_std_exception(void);

int main(void)
{
    FlTestSuite *suite_cstr = fl_test_suite_new("Module Cstr", 
        { "fl_cstr_new", &test_cstr_new }, 
        { "fl_cstr_dup", &test_cstr_dup },
        { "Signals", &test_cstr_sig }
    );
    fl_test_suite_run(suite_cstr);
    fl_test_suite_delete(suite_cstr);

    FlTestSuite *suite_std = fl_test_suite_new("Module Std", 
        { "Exception handling", &test_std_exception }       
    );
    fl_test_suite_run(suite_std);
    fl_test_suite_delete(suite_std);
    return 0;
}