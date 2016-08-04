#include "Test.h"

// Tests
#include "Std.h"
#include "Cstr.h"

int main(void)
{
    FlTestSuite *suite_cstr = fl_test_suite_new("Module Cstr", 
        { "fl_cstr_new", &test_cstr_new }, 
        { "fl_cstr_dup", &test_cstr_dup }
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