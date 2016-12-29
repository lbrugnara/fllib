#include "Test.h"

// Tests
#include "Std.h"
#include "Cstr.h"
#include "File.h"

#include <fllib.h>
#include <stdio.h>

int main(void)
{
    FlTestSuite suite_cstr = fl_test_suite_new("Module Cstr", 
        { "fl_cstr_new",            &test_cstr_new          }, 
        { "fl_cstr_dup",            &test_cstr_dup          },
        { "fl_cstr_split",          &test_cstr_split        },
        { "fl_cstr_replace_char",   &test_cstr_replace_char },
        { "fl_cstr_append{_char}",  &test_cstr_append       },
        { "fl_cstr_join",           &test_cstr_join         }        
    );
    fl_test_suite_run(suite_cstr);
    fl_test_suite_delete(suite_cstr);

    FlTestSuite suite_std = fl_test_suite_new("Module Std", 
        { "Exception handling", &test_std_exception }       
    );
    fl_test_suite_run(suite_std);
    fl_test_suite_delete(suite_std);

    FlTestSuite suite_file = fl_test_suite_new("Module File", 
        { "fl_file_write_all_bytes and fl_file_read_all_bytes", &test_file_rw_all_bytes }
    );
    fl_test_suite_run(suite_file);
    fl_test_suite_delete(suite_file);

    return 0;
}