#include <locale.h>
#include <fllib.h>
#include <string.h>

#include <flut/flut.h>

void test_fl_string_length()
{
    FlString str = "ASCII";
    flut_expect_compat("String 'ASCII' has 5 characters", fl_string_length(str, NULL) == 5);
  
    FlString str2 = "\x41\x00\x42"; // "A\0B"
    flut_expect_compat("String 'A\\0B' has 3 characters", fl_string_length(str2, (FlByte*)str2+3) == 3);
    
    FlString eos = "\0";
    flut_expect_compat("String '\\0' if formed by 1 characters", fl_string_length(eos, (FlByte*)eos+1) == 1);

    FlString eos2 = "\0";
    flut_expect_compat("'\\0' is an empty string", fl_string_length(eos, NULL) == 0);

    FlString unistr = "兔¡¢£¤¥¦§¨©ª«¬­®";
    flut_expect_compat("Length of string '兔¡¢£¤¥¦§¨©ª«¬­®' is 15 characters", fl_string_length(unistr, NULL) == 15);    
}

void test_fl_string_size()
{
    FlString str = "ASCII";
    flut_expect_compat("String 'ASCII' has 5 bytes", fl_string_size(str, NULL) == 5);
  
    FlString str2 = "\x41\x00\x42"; // "A\0B"
    flut_expect_compat("String 'A\\0B' has 3 bytes", fl_string_size(str2, (FlByte*)str2+3) == 3);
    
    FlString eos = "\0";
    flut_expect_compat("String '\\0' has 1 byte", fl_string_size(eos, (FlByte*)eos+1) == 1);

    FlString eos2 = "\0";
    flut_expect_compat("'\\0' is an empty string (0 bytes)", fl_string_size(eos, NULL) == 0);

    FlString unistr = "兔¡¢£¤¥¦§¨©ª«¬­®";
    flut_expect_compat("size of string '兔¡¢£¤¥¦§¨©ª«¬­®' uses 32 bytes", fl_string_size(unistr, NULL) == 32);

    FlString onebyte = "Z";
    flut_expect_compat("Character 'Z' occupies 1 byte", fl_string_size(onebyte, (FlByte*)onebyte+1) == 1);

    FlString nullchar = "\0";
    flut_expect_compat("Character '\\0' occupies 1 byte", fl_string_size(nullchar, (FlByte*)nullchar+1) == 1);
    
    FlString twobytes = "Π";
    flut_expect_compat("Character 'Π' occupies 2 bytes", fl_string_size(twobytes, (FlByte*)twobytes+2) == 2);
    
    FlString threebytes = "Ａ";
    flut_expect_compat("Character 'Ａ' occupies 3 bytes", fl_string_size(threebytes, (FlByte*)threebytes+3) == 3);
    
    FlString fourbytes = "兔";
    flut_expect_compat("Character '兔' occupies 4 bytes", fl_string_size(fourbytes, (FlByte*)fourbytes+4) == 4);
}

void test_fl_string_char_at()
{
    FlString str = "ĀāĂăĄąĆćĈĉĊċČčĎďĐđĒēĔĕĖėĘęĚěĜĝĞğĠġĢģĤĥĦħĨĩĪīĬĭĮįİıĲĳĴĵĶķĸĹĺĻļĽľĿŀŁłŃńŅņŇňŉŊŋŌōŎŏŐőŒœŔŕŖŗŘřŚśŜŝŞşŠšŢţŤťŦŧŨũŪūŬŭŮůŰűŲųŴŵŶŷŸŹźŻżŽž";
    /*flut_expect_compat("0 = Ā", fl_string_char_at(str, 0) == fl_char("Ā"));
    flut_expect_compat("28 = Ĝ", fl_string_char_at(str, 28) == fl_char("Ĝ"));
    flut_expect_compat("40 = Ĩ", fl_string_char_at(str, 40) == fl_char("Ĩ"));
    flut_expect_compat("60 = ļ", fl_string_char_at(str, 60) == fl_char("ļ"));
    flut_expect_compat("60 = ļ", fl_string_char_at(str, 60) == fl_char("ļ"));
    flut_expect_compat("92 = Ŝ", fl_string_char_at(str, 92) == fl_char("Ŝ"));
    flut_expect_compat("126 = ž", fl_string_char_at(str, 126) == fl_char("ž"));*/
}
