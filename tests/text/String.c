#include <locale.h>
#include <fllib.h>
#include <String.h>

#include "../Test.h"

void test_fl_string_length()
{
    FlString str = "ASCII";
    fl_expect("String 'ASCII' has 5 characters", fl_string_length(str, NULL) == 5);
  
    FlString str2 = "\x41\x00\x42"; // "A\0B"
    fl_expect("String 'A\\0B' has 3 characters", fl_string_length(str2, (FlByte*)str2+4) == 3);
    
    FlString eos = "\0";
    fl_expect("String '\\0' if formed by 1 characters", fl_string_length(eos, (FlByte*)eos+1) == 1);

    FlString eos2 = "\0";
    fl_expect("'\\0' is an empty string", fl_string_length(eos, NULL) == 0);

    setlocale(LC_ALL, "");
    FlString unistr = "兔¡¢£¤¥¦§¨©ª«¬­®";
    fl_expect("Length of string '兔¡¢£¤¥¦§¨©ª«¬­®' is 15 characters", fl_string_length(unistr, NULL) == 15);    
}

void test_fl_string_size()
{
    FlString str = "ASCII";
    fl_expect("String 'ASCII' has 5 bytes", fl_string_size(str, NULL) == 5);
  
    FlString str2 = "\x41\x00\x42"; // "A\0B"
    fl_expect("String 'A\\0B' has 3 bytes", fl_string_size(str2, (FlByte*)str2+3) == 3);
    
    FlString eos = "\0";
    fl_expect("String '\\0' has 1 byte", fl_string_size(eos, (FlByte*)eos+1) == 1);

    FlString eos2 = "\0";
    fl_expect("'\\0' is an empty string (0 bytes)", fl_string_size(eos, NULL) == 0);

    setlocale(LC_ALL, "");
    FlString unistr = "兔¡¢£¤¥¦§¨©ª«¬­®";
    fl_expect("size of string '兔¡¢£¤¥¦§¨©ª«¬­®' uses 32 bytes", fl_string_size(unistr, NULL) == 32);

    FlString onebyte = "Z";
    fl_expect("Character 'Z' occupies 1 byte", fl_string_size(onebyte, (FlByte*)onebyte+1) == 1);

    FlString nullchar = "\0";
    fl_expect("Character '\\0' occupies 1 byte", fl_string_size(nullchar, (FlByte*)nullchar+1) == 1);
    
    FlString twobytes = "Π";
    fl_expect("Character 'Π' occupies 2 bytes", fl_string_size(twobytes, (FlByte*)twobytes+1) == 2);
    
    FlString threebytes = "Ａ";
    fl_expect("Character 'Ａ' occupies 3 bytes", fl_string_size(threebytes, (FlByte*)threebytes+1) == 3);
    
    FlString fourbytes = "兔";
    fl_expect("Character '兔' occupies 4 bytes", fl_string_size(fourbytes, (FlByte*)fourbytes+1) == 4);
}

void test_fl_string_char_at()
{
    FlString str = "ĀāĂăĄąĆćĈĉĊċČčĎďĐđĒēĔĕĖėĘęĚěĜĝĞğĠġĢģĤĥĦħĨĩĪīĬĭĮįİıĲĳĴĵĶķĸĹĺĻļĽľĿŀŁłŃńŅņŇňŉŊŋŌōŎŏŐőŒœŔŕŖŗŘřŚśŜŝŞşŠšŢţŤťŦŧŨũŪūŬŭŮůŰűŲųŴŵŶŷŸŹźŻżŽž";
    fl_expect("0 = Ā", fl_string_char_at(str, 0) == fl_char("Ā"));
    fl_expect("28 = Ĝ", fl_string_char_at(str, 28) == fl_char("Ĝ"));
    fl_expect("40 = Ĩ", fl_string_char_at(str, 40) == fl_char("Ĩ"));
    fl_expect("60 = ļ", fl_string_char_at(str, 60) == fl_char("ļ"));
    fl_expect("60 = ļ", fl_string_char_at(str, 60) == fl_char("ļ"));
    fl_expect("92 = Ŝ", fl_string_char_at(str, 92) == fl_char("Ŝ"));
    fl_expect("126 = ž", fl_string_char_at(str, 126) == fl_char("ž"));
}
