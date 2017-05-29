#include "Test.h"

// Tests
#include "Std.h"
#include "Cstr.h"
#include "File.h"
#include "text/Test_Unicode.h"
#include "text/Test_String.h"

#include <fllib.h>
#include <stdio.h>
#include <locale.h>
#include <time.h>

int main(void)
{
    fl_test_run_all_suites(
        fl_test_suite("Module Std", 
            { "Exception handling", &test_std_exception },
            { "Global error handling thread safety", &test_errors }
        ),
        fl_test_suite("Module Cstr", 
            { "fl_cstr_new",            &test_cstr_new          }, 
            { "fl_cstr_dup",            &test_cstr_dup          },
            { "fl_cstr_split",          &test_cstr_split        },
            { "fl_cstr_replace_char",   &test_cstr_replace_char },
            { "fl_cstr_append{_char}",  &test_cstr_append       },
            { "fl_cstr_join",           &test_cstr_join         }        
        ),
        fl_test_suite("Module File", 
            { "fl_file_write_all_bytes and fl_file_read_all_bytes", &test_file_rw_all_bytes }
        ),
        fl_test_suite("Module Unicode",
            { "fl_unicode_unichar_at", &test_fl_unicode_unichar_at },
            { "fl_unicode_char", &test_fl_unicode_char },
            { "fl_unicode_unichar_encode_to", &test_fl_unicode_unichar_encode_to },
            { "fl_unicode_codepoint_to_unichar", &test_fl_unicode_codepoint_to_unichar },
            { "fl_unicode_codeunit_sequence_size", &test_fl_unicode_codeunit_sequence_size},
            { "test_fl_unicode_unichar_validity", &test_fl_unicode_unichar_validity },
            { "test_fl_unicode_codepoint_validity", &test_fl_unicode_codepoint_validity }
        ),
        fl_test_suite("Module String", 
            { "fl_string_length", &test_fl_string_length },
            { "fl_string_size", &test_fl_string_size },
            { "fl_string_char_at", &test_fl_string_char_at }
        )
    );
}
