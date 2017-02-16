#include <locale.h>
#include <fllib.h>
#include <string.h>

#include "../Test.h"

void test_fl_unicode_char_at()
{
    FlString str = "ĀāĂăĄąĆćĈĉĊċČčĎďĐđĒēĔĕĖėĘęĚěĜĝĞğĠġĢģĤĥĦħĨĩĪīĬĭĮįİıĲĳĴĵĶķĸĹĺĻļĽľĿŀŁłŃńŅņŇňŉŊŋŌōŎŏŐőŒœŔŕŖŗŘřŚśŜŝŞşŠšŢţŤťŦŧŨũŪūŬŭŮůŰűŲųŴŵŶŷŸŹźŻżŽž";
    fl_expect("0 = Ā", fl_unicode_char_at((FlByte*)str, FL_ENCODING_UTF8, 0) == fl_unicode_char_from_str((FlByte*)"Ā", FL_ENCODING_UTF8));
    fl_expect("28 = Ĝ", fl_unicode_char_at((FlByte*)str, FL_ENCODING_UTF8, 28) == fl_unicode_char_from_str((FlByte*)"Ĝ", FL_ENCODING_UTF8));
    fl_expect("40 = Ĩ", fl_unicode_char_at((FlByte*)str, FL_ENCODING_UTF8, 40) == fl_unicode_char_from_str((FlByte*)"Ĩ", FL_ENCODING_UTF8));
    fl_expect("60 = ļ", fl_unicode_char_at((FlByte*)str, FL_ENCODING_UTF8, 60) == fl_unicode_char_from_str((FlByte*)"ļ", FL_ENCODING_UTF8));
    fl_expect("60 = ļ", fl_unicode_char_at((FlByte*)str, FL_ENCODING_UTF8, 60) == fl_unicode_char_from_str((FlByte*)"ļ", FL_ENCODING_UTF8));
    fl_expect("92 = Ŝ", fl_unicode_char_at((FlByte*)str, FL_ENCODING_UTF8, 92) == fl_unicode_char_from_str((FlByte*)"Ŝ", FL_ENCODING_UTF8));
    fl_expect("126 = ž", fl_unicode_char_at((FlByte*)str, FL_ENCODING_UTF8, 126) == fl_unicode_char_from_str((FlByte*)"ž", FL_ENCODING_UTF8));

    size_t strl = fl_string_length(str, NULL);
    size_t strs = fl_string_size(str, NULL);
    size_t chrl = 0;
    size_t chrs = 0;
    for (size_t i=0; i < strl; i++)
    {
        FlUnicodeChar chr = fl_unicode_char_at((FlByte*)str, FL_ENCODING_UTF8, i);
        chrl++;
        chrs += fl_unicode_char_size(chr, FL_ENCODING_UTF8);
    }
    fl_expect("Length and size of 'str' is equals to length and size of the characters iteration", strl == chrl && strs == chrs);
}

void test_fl_unicode_char()
{
    FlUnicodeChar chr1 = fl_unicode_char_from_str((FlByte*)"Ā", FL_ENCODING_UTF8);
    FlUnicodeChar chr2 = fl_unicode_char_from_hex(0xC480); // UTF-8
    FlUnicodeChar chr3 = fl_unicode_char_from_hex(0x0100); // UTF-32
    fl_expect("Char 'Ā', Hexadecimal 0xC480 and U+0100 result in the same FlUnicodeChar", chr1 == chr2 && chr2 == chr3);

    FlUnicodeChar z1 = fl_unicode_char_from_str((FlByte*)"Z", FL_ENCODING_UTF8);
    FlUnicodeChar z2 = fl_unicode_char_from_hex(0x005A); // UTF-8
    FlUnicodeChar z3 = fl_unicode_char_from_hex(0x005A); // UTF-32
    fl_expect("Char 'Z', Hexadecimal 0x005A and U+005A result in the same FlUnicodeChar", z1 == z2 && z2 == z3);

    FlUnicodeChar pi1 = fl_unicode_char_from_str((FlByte*)"Π", FL_ENCODING_UTF8);
    FlUnicodeChar pi2 = fl_unicode_char_from_hex(0xCEA0); // UTF-8
    FlUnicodeChar pi3 = fl_unicode_char_from_hex(0x03A0); // UTF-32
    fl_expect("Char 'Π', Hexadecimal 0xCEA0 and U+03A0 result in the same FlUnicodeChar", pi1 == pi2 && pi2 == pi3);

    FlUnicodeChar latinA1 = fl_unicode_char_from_str((FlByte*)"Ａ", FL_ENCODING_UTF8);
    FlUnicodeChar latinA2 = fl_unicode_char_from_hex(0xEFBCA1); // UTF-8
    FlUnicodeChar latinA3 = fl_unicode_char_from_hex(0xFF21); // UTF-32
    fl_expect("Char 'Ａ', Hexadecimal 0xEFBCA1 and U+FF21 result in the same FlUnicodeChar", latinA1 == latinA2 && latinA2 == latinA3);

    FlUnicodeChar chr4b1 = fl_unicode_char_from_str((FlByte*)"兔", FL_ENCODING_UTF8);
    FlUnicodeChar chr4b2 = fl_unicode_char_from_hex(0xF0AFA08F); // UTF-8
    FlUnicodeChar chr4b3 = fl_unicode_char_from_hex(0x2F80F); // UTF-32
    fl_expect("Char '兔', Hexadecimal 0xF0AFA08F and U+2F80F result in the same FlUnicodeChar", chr4b1 == chr4b2 && chr4b2 == chr4b3);
}

void test_fl_unicode_encode_char_to()
{
    // UTF8 1-byte
    FlUnicodeChar u32 = 0x0000;
    FlUnicodeChar u8 = fl_unicode_char_from_hex(0x0);
    fl_expect("UTF32 -> UTF8: U+0000 equals to 0x0", fl_unicode_encode_char_to(u32, FL_ENCODING_UTF32, FL_ENCODING_UTF8) == u8);
    fl_expect("UTF8 -> UTF32: 0x0 equals to U+0000", fl_unicode_encode_char_to(u8, FL_ENCODING_UTF8, FL_ENCODING_UTF32) == u32);

    u32 = 0x0048;
    u8 = fl_unicode_char_from_hex(0x48);
    fl_expect("UTF32 -> UTF8: U+0048 equals to 0x48", fl_unicode_encode_char_to(u32, FL_ENCODING_UTF32, FL_ENCODING_UTF8) == u8);
    fl_expect("UTF8 -> UTF32: 0x48 equals to U+0048", fl_unicode_encode_char_to(u8, FL_ENCODING_UTF8, FL_ENCODING_UTF32) == u32);

    u32 = 0x007F;
    u8 = fl_unicode_char_from_hex(0x7f);
    fl_expect("UTF32 -> UTF8: U+007F equals to 0x7f", fl_unicode_encode_char_to(u32, FL_ENCODING_UTF32, FL_ENCODING_UTF8) == u8);
    fl_expect("UTF8 -> UTF32: 0x7f equals to U+007F", fl_unicode_encode_char_to(u8, FL_ENCODING_UTF8, FL_ENCODING_UTF32) == u32);

    // UTF8 2-bytes
    u32 = 0x0080;
    u8 = fl_unicode_char_from_hex(0xC280);
    fl_expect("UTF32 -> UTF8: U+0080 equals to 0xC280", fl_unicode_encode_char_to(u32, FL_ENCODING_UTF32, FL_ENCODING_UTF8) == u8);
    fl_expect("UTF8 -> UTF32: 0xC280 equals to U+0080", fl_unicode_encode_char_to(u8, FL_ENCODING_UTF8, FL_ENCODING_UTF32) == u32);

    u32 = 0x0100;
    u8 = fl_unicode_char_from_hex(0xC480);
    fl_expect("UTF32 -> UTF8: U+0100 equals to 0xC480", fl_unicode_encode_char_to(u32, FL_ENCODING_UTF32, FL_ENCODING_UTF8) == u8);
    fl_expect("UTF8 -> UTF32: 0xC480 equals to U+0100", fl_unicode_encode_char_to(u8, FL_ENCODING_UTF8, FL_ENCODING_UTF32) == u32);

    u32 = 0x07FF;
    u8 = fl_unicode_char_from_hex(0xdfbf);
    fl_expect("UTF32 -> UTF8: U+07FF equals to 0xdfbf", fl_unicode_encode_char_to(u32, FL_ENCODING_UTF32, FL_ENCODING_UTF8) == u8);
    fl_expect("UTF8 -> UTF32: 0xdfbf equals to U+07FF", fl_unicode_encode_char_to(u8, FL_ENCODING_UTF8, FL_ENCODING_UTF32) == u32);

    // UTF8 3-bytes
    u32 = 0x0800;
    u8 = fl_unicode_char_from_hex(0xe0a080);
    fl_expect("UTF32 -> UTF8: U+0800 equals to 0xe0a080", fl_unicode_encode_char_to(u32, FL_ENCODING_UTF32, FL_ENCODING_UTF8) == u8);
    fl_expect("UTF8 -> UTF32: 0xe0a080 equals to U+0800", fl_unicode_encode_char_to(u8, FL_ENCODING_UTF8, FL_ENCODING_UTF32) == u32);

    u32 = 0xA8F9;
    u8 = fl_unicode_char_from_hex(0xeaa3b9);
    fl_expect("UTF32 -> UTF8: U+A8F9 equals to 0xeaa3b9", fl_unicode_encode_char_to(u32, FL_ENCODING_UTF32, FL_ENCODING_UTF8) == u8);
    fl_expect("UTF8 -> UTF32: 0xeaa3b9 equals to U+A8F9", fl_unicode_encode_char_to(u8, FL_ENCODING_UTF8, FL_ENCODING_UTF32) == u32);

    u32 = 0xFFFF;
    u8 = fl_unicode_char_from_hex(0xefbfbf);
    fl_expect("UTF32 -> UTF8: U+FFFF equals to 0xefbfbf", fl_unicode_encode_char_to(u32, FL_ENCODING_UTF32, FL_ENCODING_UTF8) == u8);
    fl_expect("UTF8 -> UTF32: 0xefbfbf equals to U+FFFF", fl_unicode_encode_char_to(u8, FL_ENCODING_UTF8, FL_ENCODING_UTF32) == u32);

    // UTF8 4-bytes
    u32 = 0x10000;
    u8 = fl_unicode_char_from_hex(0xf0908080);
    fl_expect("UTF32 -> UTF8: U+10000 equals to 0xf0908080", fl_unicode_encode_char_to(u32, FL_ENCODING_UTF32, FL_ENCODING_UTF8) == u8);
    fl_expect("UTF8 -> UTF32: 0xf0908080 equals to U+10000", fl_unicode_encode_char_to(u8, FL_ENCODING_UTF8, FL_ENCODING_UTF32) == u32);

    u32 = 0x1F615;
    u8 = fl_unicode_char_from_hex(0xf09f9895);
    fl_expect("UTF32 -> UTF8: U+1F615 equals to 0xf09f9895", fl_unicode_encode_char_to(u32, FL_ENCODING_UTF32, FL_ENCODING_UTF8) == u8);
    fl_expect("UTF8 -> UTF32: 0xf09f9895 equals to U+1F615", fl_unicode_encode_char_to(u8, FL_ENCODING_UTF8, FL_ENCODING_UTF32) == u32);

    u32 = 0x10FFFF;
    u8 = fl_unicode_char_from_hex(0xf48fbfbf);
    fl_expect("UTF32 -> UTF8: U+FFFF equals to 0xf48fbfbf", fl_unicode_encode_char_to(u32, FL_ENCODING_UTF32, FL_ENCODING_UTF8) == u8);
    fl_expect("UTF8 -> UTF32: 0xf48fbfbf equals to U+FFFF", fl_unicode_encode_char_to(u8, FL_ENCODING_UTF8, FL_ENCODING_UTF32) == u32);
    
    // Invalid
    u32 = 0x110000;
    fl_expect("UTF8 -> UTF32: 0xf48fbfc0 is an invalid Unicode char", fl_unicode_encode_char_to(u32, FL_ENCODING_UTF32, FL_ENCODING_UTF8) == FL_UNICODE_INVALID_CHAR);

    // From string
    u8 = fl_unicode_char_from_str((FlByte*)"Ɓ", FL_ENCODING_UTF8);
    u32 = 0x0181;
    fl_expect("UTF32 -> UTF8: U+0181 equals to 0xC681", fl_unicode_encode_char_to(u32, FL_ENCODING_UTF32, FL_ENCODING_UTF8) == u8);
    fl_expect("UTF8 -> UTF32: 0xC681 equals to U+0181", fl_unicode_encode_char_to(u8, FL_ENCODING_UTF8, FL_ENCODING_UTF32) == u32);
}
