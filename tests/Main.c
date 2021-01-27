#include "Test_Slice.h"
#include "Test_Cstring.h"
#include "IO.h"
#include "text/Test_Unicode.h"
#include "text/Test_String.h"
#include "text/Test_Regex.h"
#include "containers/Test_BinaryHeap.h"
#include "containers/Test_Hashtable.h"
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
        flut_suite("Slice", 
            { "Slice allocation", &test_slice },
            { "Slice iterator", &test_slice_iterator }
        ),
        flut_suite("Cstr", 
            { "fl_cstring_new", &test_cstring_new }, 
            { "fl_cstring_dup", &test_cstring_dup },
            { "fl_cstring_split", &test_cstring_split },
            { "fl_cstring_replace_char", &test_cstring_replace_char },
            { "fl_cstring_replace", &test_cstring_replace },
            { "fl_cstring_append{_char}", &test_cstring_append },
            { "fl_cstring_join", &test_cstring_join },
            { "cstr_misc_functions", &test_cstring_misc },
            { "fl_cstring_find", &test_cstring_find }
        ),
        flut_suite("IO", 
            { "fl_io_file_write_all_bytes and fl_io_file_read_all_bytes", &test_file_rw_all_bytes },
            { "IO realpath", &test_io_realpath },
            { "IO Paths", &test_io_path },
        ),
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
        flut_suite("Hashtable",
            { "fl_hashtable_add", &test_fl_hashtable_add },
            { "fl_hashtable_add_fhash", &test_fl_hashtable_add_fhash },
            { "fl_hashtable_get", &test_fl_hashtable_get },
            { "fl_hashtable_set", &test_fl_hashtable_set },
            { "fl_hashtable_clear", &test_fl_hashtable_clear },
            { "fl_hashtable_keys_and_values", &test_fl_hashtable_keys_and_values },
            { "fl_hashtable_remove", &test_fl_hashtable_remove },
            { "fl_hashtable_resize", &test_fl_hashtable_resize },
            { "fl_hashtable_values", &test_fl_hashtable_values },
        ),
        flut_suite("List",
            { "fl_list_head", &test_fl_list_head },
            { "fl_list_tail", &test_fl_list_tail },
            { "fl_list_append", &test_fl_list_append },
            { "fl_list_prepend", &test_fl_list_prepend },
            { "fl_list_insert_after", &test_fl_list_insert_after },
            { "fl_list_insert_before", &test_fl_list_insert_before }
        ),
        flut_suite("Binary Heap",
            { "fl_binheap_new", &test_fl_binheap_new },
            { "fl_binheap_insert", &test_fl_binheap_insert },
        )
    );
}
