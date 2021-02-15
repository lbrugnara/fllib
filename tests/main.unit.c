#include "text/Test_Unicode.h"
#include "text/Test_Regex.h"

#include <fllib.h>
#include <flut/flut.h>
#include <stdio.h>
#include <locale.h>
#include <time.h>

int main(int argc, char **argv) 
{
    bool success;
    flut_run_tests(
        argc,
        argv,
        &success,
        flut_register_suite(std),
        flut_register_suite(array),
        flut_register_suite(slice),
        flut_register_suite(cstring),
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
        flut_register_suite(string),
        flut_suite("Regex",
            { "fl_regex_tokenize", &test_fl_regex_tokenize },
            { "fl_regex_match", &test_fl_regex_match }
        ),
        flut_register_suite(vector),
        flut_register_suite(hashtable),
        flut_register_suite(list),
        flut_register_suite(binheap)
    );
    return success ? 0 : -1;
}
