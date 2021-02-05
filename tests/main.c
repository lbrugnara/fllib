#include "text/Test_Unicode.h"
#include "text/Test_String.h"
#include "text/Test_Regex.h"
#include "containers/Test_Vector.h"
#include "containers/Test_List.h"

#include <fllib.h>
#include <flut/suite.h>
#include <stdio.h>
#include <locale.h>
#include <time.h>

int main(int argc, char **argv) 
{
    flut_run_tests(
        argc,
        argv,
        flut_register_suite(std),
        flut_register_suite(array),
        flut_register_suite(slice),
        flut_register_suite(cstr),
        flut_register_suite(io),
        flut_suite("Unicode",
            { "fl_unicode_codepoint_at", &test_fl_unicode_codepoint_at },
            { "fl_unicode_codeunit_sequence_size", &test_fl_unicode_codeunit_sequence_size},
            { "fl_unicode_unichar_validity", &test_fl_unicode_unichar_validity },
            { "fl_unicode_codepoint_validity", &test_fl_unicode_codepoint_validity },
            { "fl_unicode_unichar_sequence_validate", &test_fl_unicode_unichar_sequence_validate },
            { "fl_unicode_codepoint_convert", &test_fl_unicode_codepoint_convert },
            { "fl_unicode_codepoint_sequence_validate", &test_fl_unicode_codepoint_sequence_validate },
            { "fl_unicode_data", &test_fl_unicode_data }
        ),
        flut_suite("String", 
            { "fl_string_length", &test_fl_string_length },
            { "fl_string_size", &test_fl_string_size },
            { "fl_string_char_at", &test_fl_string_char_at }
        ),
        flut_suite("Regex",
            { "fl_regex_tokenize", &test_fl_regex_tokenize },
            { "fl_regex_match", &test_fl_regex_match }
        ),
        flut_suite("Vector",
            { "fl_vector_new", &test_fl_vector_new },
            { "fl_vector_add", &test_fl_vector_add },
            { "fl_vector_insert", &test_fl_vector_insert },
            { "fl_vector_shift", &test_fl_vector_shift },
            { "fl_vector_pop", &test_fl_vector_pop },
            { "fl_vector_ref_get", &test_fl_vector_get },
        ),
        flut_register_suite(hashtable),
        flut_suite("List",
            { "fl_list_head", &test_fl_list_head },
            { "fl_list_tail", &test_fl_list_tail },
            { "fl_list_append", &test_fl_list_append },
            { "fl_list_prepend", &test_fl_list_prepend },
            { "fl_list_insert_after", &test_fl_list_insert_after },
            { "fl_list_insert_before", &test_fl_list_insert_before }
        ),
        flut_register_suite(binary_heap)
    );
}
