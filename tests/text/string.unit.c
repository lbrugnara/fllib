#include <locale.h>
#include <fllib.h>
#include <string.h>

#include <flut/flut.h>

flut_define_suite(string) {
    flut_suite_description("UTF-8 string manipulation functions");
    flut_suite_register_test(string_length, "fl_string_length function");
    flut_suite_register_test(string_size, "fl_string_size function");
    flut_suite_register_test(string_char_at, "fl_string_char_at function");
}

flut_define_test(string_length) {
    flut_assert_size_t_is_equals(5, fl_string_length("ASCII", NULL));
  
    FlString str = "\x41\x00\x42"; // "A\0B"
    flut_explain(flut_assert_size_t_is_equals(3, fl_string_length(str, str + 3)), "String contains a null byte in the middle: \"\\x41\\x00\\x42\" -> \"A\\0B\"");

    FlString zero = "\0";
    flut_explain(flut_assert_size_t_is_equals(1, fl_string_length("\0", zero + 1)), "String contains only the '\\0' character");

    flut_assert_size_t_is_equals(0, fl_string_length("\0", NULL));

    flut_assert_size_t_is_equals(15, fl_string_length("兔¡¢£¤¥¦§¨©ª«¬­®", NULL));
}

flut_define_test(string_size) {
    flut_assert_size_t_is_equals(5, fl_string_size("ASCII", NULL));
  
    FlString str = "\x41\x00\x42"; // "A\0B"
    flut_explain(flut_assert_size_t_is_equals(3, fl_string_size(str, str + 3)), "String contains a null byte in the middle: \"\\x41\\x00\\x42\" -> \"A\\0B\"");
    
    FlString null_char = "\0";
    flut_explain(flut_assert_size_t_is_equals(1, fl_string_size(null_char, null_char + 1)), "String contains only the '\\0' character");

    flut_assert_size_t_is_equals(0, fl_string_size("\0", NULL));

    flut_assert_size_t_is_equals(32, fl_string_size("兔¡¢£¤¥¦§¨©ª«¬­®", NULL));

    flut_assert_size_t_is_equals(1, fl_string_size("Z", NULL));
    
    flut_assert_size_t_is_equals(2, fl_string_size("Π", NULL));
    
    flut_assert_size_t_is_equals(3, fl_string_size("Ａ", NULL));
    
    flut_assert_size_t_is_equals(4, fl_string_size("兔", NULL));
}

flut_define_test(string_char_at) {
    FlString str = "ĀāĂăĄąĆćĈĉĊċČčĎďĐđĒēĔĕĖėĘęĚěĜĝĞğĠġĢģĤĥĦħĨĩĪīĬĭĮįİıĲĳĴĵĶķĸĹĺĻļĽľĿŀŁłŃńŅņŇňŉŊŋŌōŎŏŐőŒœŔŕŖŗŘřŚśŜŝŞşŠšŢţŤťŦŧŨũŪūŬŭŮůŰűŲųŴŵŶŷŸŹźŻżŽž";
    flut_assert_is_true(fl_string_char_at(str, 0) == fl_char("Ā"));
    flut_assert_is_true(fl_string_char_at(str, 28) == fl_char("Ĝ"));
    flut_assert_is_true(fl_string_char_at(str, 40) == fl_char("Ĩ"));
    flut_assert_is_true(fl_string_char_at(str, 60) == fl_char("ļ"));
    flut_assert_is_true(fl_string_char_at(str, 75) == fl_char("ŋ"));
    flut_assert_is_true(fl_string_char_at(str, 92) == fl_char("Ŝ"));
    flut_assert_is_true(fl_string_char_at(str, 126) == fl_char("ž"));
}
