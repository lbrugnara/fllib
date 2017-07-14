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
            { "fl_cstr_replace",        &test_cstr_replace      },
            { "fl_cstr_append{_char}",  &test_cstr_append       },
            { "fl_cstr_join",           &test_cstr_join         }        
        ),
        fl_test_suite("Module File", 
            { "fl_file_write_all_bytes and fl_file_read_all_bytes", &test_file_rw_all_bytes }
        ),
        fl_test_suite("Module Unicode",
            { "fl_unicode_codepoint_at", &test_fl_unicode_codepoint_at },
            { "fl_unicode_codeunit_sequence_size", &test_fl_unicode_codeunit_sequence_size},
            { "test_fl_unicode_unichar_validity", &test_fl_unicode_unichar_validity },
            { "test_fl_unicode_codepoint_validity", &test_fl_unicode_codepoint_validity },
            { "test_fl_unicode_unichar_sequence_validate", &test_fl_unicode_unichar_sequence_validate },
            { "test_fl_unicode_codepoint_convert", &test_fl_unicode_codepoint_convert },
            { "test_fl_unicode_codepoint_sequence_validate", &test_fl_unicode_codepoint_sequence_validate },
            { "test_fl_unicode_data", &test_fl_unicode_data }
        ),
        fl_test_suite("Module String", 
            { "fl_string_length", &test_fl_string_length },
            { "fl_string_size", &test_fl_string_size },
            { "fl_string_char_at", &test_fl_string_char_at }
        )
    );
}
