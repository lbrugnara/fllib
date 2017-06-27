#include <locale.h>
#include <fllib.h>
#include <string.h>

#include "../Test.h"

void test_fl_unicode_codepoint_at()
{
    // All the code points size in this string is two-bytes
    FlString str = "ĀāĂăĄąĆćĈĉĊċČčĎďĐđĒēĔĕĖėĘęĚěĜĝĞğĠġĢģĤĥĦħĨĩĪīĬĭĮįİıĲĳĴĵĶķĸĹĺĻļĽľĿŀŁłŃńŅņŇňŉŊŋŌōŎŏŐőŒœŔŕŖŗŘřŚśŜŝŞşŠšŢţŤťŦŧŨũŪūŬŭŮůŰűŲųŴŵŶŷŸŹźŻżŽž\x00";
    fl_expect("str is 127 code points long and it is composed of 256 code units", 
        fl_unicode_codepoint_sequence_length(FL_ENCODING_UTF8, (FlByte*)str, 0x00)
        && fl_unicode_codeunit_sequence_size(FL_ENCODING_UTF8, (FlByte*)str, 0x00)
    );

    // 
    FlByte dst[4] = {0,0,0,0};
    size_t nbytes = fl_unicode_codepoint_at(FL_ENCODING_UTF8, (FlByte*)str, 0x00, 0, dst);
    fl_expect("0 = Ā and it is 2 bytes long", memcmp(str, dst, nbytes) == 0 && nbytes == 2);

    // 
    memset(dst, 0, 4);
    nbytes = fl_unicode_codepoint_at(FL_ENCODING_UTF8, (FlByte*)str, 0x00, 28, dst);
    fl_expect("28 = Ĝ and it is 2 bytes long", memcmp(str+28*2, dst, nbytes) == 0 && nbytes == 2); // Char 28 multiplied by 2 bytes each one

    // 
    memset(dst, 0, 4);
    nbytes = fl_unicode_codepoint_at(FL_ENCODING_UTF8, (FlByte*)str, 0x00, 40, dst);
    fl_expect("40 = Ĩ and it is 2 bytes long", memcmp(str+40*2, dst, nbytes) == 0 && nbytes == 2); // Char 28 multiplied by 2 bytes each one

    // 
    memset(dst, 0, 4);
    nbytes = fl_unicode_codepoint_at(FL_ENCODING_UTF8, (FlByte*)str, 0x00, 60, dst);
    fl_expect("60 = ļ and it is 2 bytes long", memcmp(str+60*2, dst, nbytes) == 0 && nbytes == 2); // Char 28 multiplied by 2 bytes each one

    // 
    memset(dst, 0, 4);
    nbytes = fl_unicode_codepoint_at(FL_ENCODING_UTF8, (FlByte*)str, 0x00, 40, dst);
    fl_expect("40 = Ĩ and it is 2 bytes long", memcmp(str+40*2, dst, nbytes) == 0 && nbytes == 2); // Char 28 multiplied by 2 bytes each one

    // 
    memset(dst, 0, 4);
    nbytes = fl_unicode_codepoint_at(FL_ENCODING_UTF8, (FlByte*)str, 0x00, 92, dst);
    fl_expect("92 = Ŝ and it is 2 bytes long", memcmp(str+92*2, dst, nbytes) == 0 && nbytes == 2); // Char 28 multiplied by 2 bytes each one

    // 
    memset(dst, 0, 4);
    nbytes = fl_unicode_codepoint_at(FL_ENCODING_UTF8, (FlByte*)str, 0x00, 126, dst);
    fl_expect("126 = ž and it is 2 bytes long", memcmp(str+126*2, dst, nbytes) == 0 && nbytes == 2); // Char 28 multiplied by 2 bytes each one

    // 
    str = "\x00\x00\x00\x41\x00\x00\x00\x42";
    memset(dst, 0, 4);
    nbytes = fl_unicode_codepoint_at(FL_ENCODING_UTF32, (FlByte*)str, 0x00, 0, dst);
    fl_expect("AB in 0 should be A", memcmp(str, dst, nbytes) == 0 && nbytes == 4);

    // 
    memset(dst, 0, 4);
    nbytes = fl_unicode_codepoint_at(FL_ENCODING_UTF32, (FlByte*)str, 0x00, 1, dst);
    fl_expect("AB in 1 should be A", memcmp(str+1*4, dst, nbytes) == 0 && nbytes == 4);

    //
    fl_expect("Size of U+D800 is FL_UNICODE_INVALID_SIZE because it is a High Surrogate", fl_unicode_codepoint_size(FL_ENCODING_UTF32, (FlByte*)"\x00\xD8\x00\x00\x00", NULL) == FL_UNICODE_INVALID_SIZE);
    fl_expect("Size of U+DFFF is FL_UNICODE_INVALID_SIZE because it is a Low Surrogate", fl_unicode_codepoint_size(FL_ENCODING_UTF32, (FlByte*)"\x00\xDF\xFF\xFF\x00", NULL) == FL_UNICODE_INVALID_SIZE);
}

void test_fl_unicode_codeunit_sequence_size()
{
    fl_expect("abc123 has 6 bytes", fl_unicode_codeunit_sequence_size(FL_ENCODING_UTF8, (const FlByte*)"abc123", NULL) == 6);
    fl_expect("aƁc123 has 7 bytes", fl_unicode_codeunit_sequence_size(FL_ENCODING_UTF8, (const FlByte*)"aƁc123", NULL) == 7);
    fl_expect("a兔c123 has 9 bytes", fl_unicode_codeunit_sequence_size(FL_ENCODING_UTF8, (const FlByte*)"a兔c123", NULL) == 9);
    fl_expect("兔 has 4 bytes", fl_unicode_codeunit_sequence_size(FL_ENCODING_UTF8, (const FlByte*)"兔", NULL) == 4);
    fl_expect("ԱԲ has 4 bytes", fl_unicode_codeunit_sequence_size(FL_ENCODING_UTF8, (const FlByte*)"ԱԲ", NULL) == 4);
    fl_expect("Code Point size is the same as the Code Unit sequence size (of length 4) for 兔", fl_unicode_codeunit_sequence_size(FL_ENCODING_UTF8, (const FlByte*)"兔", NULL) == fl_unicode_codepoint_size(FL_ENCODING_UTF8, (const FlByte*)"兔", NULL));
    fl_expect("Code Point size is the same as the Code Unit sequence size (of length 2) for Ɓ", fl_unicode_codeunit_sequence_size(FL_ENCODING_UTF8, (const FlByte*)"Ɓ", NULL) == fl_unicode_codepoint_size(FL_ENCODING_UTF8, (const FlByte*)"Ɓ", NULL));
}

void test_fl_unicode_unichar_validity()
{
    /*fl_expect("Replacement char U+FFFD is valid UTF-8", fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xefbfbd));
    
    // Overlon encodings
    fl_expect("Overlong encoding of U+0000 (0xC080) is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xC080));
    fl_expect("Overlong encoding of U+002F (0xc0af) is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xc0af));
    fl_expect("Overlong encoding of U+002F (0xe080af) is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xe080af));
    fl_expect("Overlong encoding of U+002F (0xf08080af) is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xf08080af));
    fl_expect("Overlong encoding of U+007F (0xc1bf) is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xc1bf));
    fl_expect("Overlong encoding of U+07FF (0xe09fbf) is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xe09fbf));
    fl_expect("Overlong encoding of U+FFFF (0xf08fbfbf) is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xf08fbfbf));
    fl_expect("Overlong encoding of U+0000 (0xc080) is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xc080));
    fl_expect("Overlong encoding of U+0000 (0xe08080) is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xe08080));
    fl_expect("Overlong encoding of U+0000 (0xf0808080) is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xf0808080));
    
    // UTF-16 Surrogates
    fl_expect("UTF-16 Surrogate U+D800 (0xeda080) is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xeda080));
    fl_expect("UTF-16 Surrogate U+DB7F (0xedadbf) is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xedadbf));
    fl_expect("UTF-16 Surrogate U+DB80 (0xedae80) is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xedae80));
    fl_expect("UTF-16 Surrogate U+DBFF (0xedafbf) is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xedafbf));
    fl_expect("UTF-16 Surrogate U+DC00 (0xedb080) is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xedb080));
    fl_expect("UTF-16 Surrogate U+DF80 (0xedbe80) is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xedbe80));
    fl_expect("UTF-16 Surrogate U+DFFF (0xedbfbf) is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xedbfbf));

    // Boundaries
    fl_expect("High surrogate U+D84C is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xEDA18C));
    fl_expect("High surrogate U+D84C is not valid UTF-32", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF32, 0xD84c));
    fl_expect("High surrogate boundary U+D7FF is valid UTF-8", fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xed9fbf));
    fl_expect("Max boundary U+10FFFF is valid UTF-8", fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xf48fbfbf));
    fl_expect("Max boundary+1 U+110000 is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xf4908080));

    fl_expect("U+0000 is valid UTF-8 (0x0000)", fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0x0000));
    fl_expect("U+0080 is valid UTF-8 (0xC280)", fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xC280));
    fl_expect("U+0800 is valid UTF-8 (0xe0a080)", fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xe0a080));
    fl_expect("U+10000 is valid UTF-8 (0xf0908080)", fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xf0908080));

    fl_expect("U+007F is valid UTF-8 (0x007F)", fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0x007F));
    fl_expect("U+07FF is valid UTF-8 (0xdfbf)", fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xdfbf));
    fl_expect("U+FFFF is valid UTF-8 (0xefbfbf)", fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xefbfbf));
    
    fl_expect("U+0E00 is valid UTF-8 (0xe0b880)", fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xe0b880));


    // Unexpected continuation bytes
    fl_expect("Continuation byte 0x80 is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0x80));
    fl_expect("Continuation byte 0xbf is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xbf));

    // 2-bytes sequence start bytes
    fl_expect("0xC0 is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xC0));
    fl_expect("0xC5 is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xC5));
    fl_expect("0xCA is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xCA));
    fl_expect("0xCF is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xCF));
    fl_expect("0xD0 is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xD0));
    fl_expect("0xD5 is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xD5));
    fl_expect("0xDA is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xDA));
    fl_expect("0xDF is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xDF));

    // 3-bytes sequence start bytes
    fl_expect("0xE0 is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xE0));
    fl_expect("0xE5 is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xE5));
    fl_expect("0xEA is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xEA));
    fl_expect("0xEF is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xEF));

    // 4-bytes sequence start bytes
    fl_expect("0xF0 is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xF0));
    fl_expect("0xF5 is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xF5));
    fl_expect("0xFA is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xFA));
    fl_expect("0xFE is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xFE));
    fl_expect("0xFF is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xFF));

    // Sequences
    
    // Last byte missing
    fl_expect("Sequence xC0 is not valid UTF-8", !fl_unicode_unichar_sequence_is_valid(FL_ENCODING_UTF8, (uint32_t[]){ 0xC0, 0x00 }, NULL)); // 0x00 == NULL terminated
    fl_expect("Sequence xDF is not valid UTF-8", !fl_unicode_unichar_sequence_is_valid(FL_ENCODING_UTF8, (uint32_t[]){ 0xDF, 0x00 }, NULL)); // 0x00 == NULL terminated
    fl_expect("Sequence xE0x80 is not valid UTF-8", !fl_unicode_unichar_sequence_is_valid(FL_ENCODING_UTF8, (uint32_t[]){ 0xE080, 0x00 }, NULL)); // 0x00 == NULL terminated
    fl_expect("Sequence xEFxBF is not valid UTF-8", !fl_unicode_unichar_sequence_is_valid(FL_ENCODING_UTF8, (uint32_t[]){ 0xEFBF, 0x00 }, NULL)); // 0x00 == NULL terminated
    fl_expect("Sequence xF0x80x80 is not valid UTF-8", !fl_unicode_unichar_sequence_is_valid(FL_ENCODING_UTF8, (uint32_t[]){ 0xF08080, 0x00 }, NULL)); // 0x00 == NULL terminated
    fl_expect("Sequence xF7xBFxBF is not valid UTF-8", !fl_unicode_unichar_sequence_is_valid(FL_ENCODING_UTF8, (uint32_t[]){ 0xF7BFBF, 0x00 }, NULL)); // 0x00 == NULL terminated

    // Surrogates
    fl_expect("Surrogates sequence U+D800 U+DC00 is not valid UTF-8", !fl_unicode_unichar_sequence_is_valid(FL_ENCODING_UTF8, (uint32_t[]) { 0xeda080, 0xedb080, 0x00 }, NULL)); // 0x00 == NULL terminated
    fl_expect("Surrogates sequence U+D800 U+DFFF is not valid UTF-8", !fl_unicode_unichar_sequence_is_valid(FL_ENCODING_UTF8, (uint32_t[]) { 0xeda080, 0xedbfbf, 0x00 }, NULL)); // 0x00 == NULL terminated
    fl_expect("Surrogates sequence U+DB7F U+DC00 is not valid UTF-8", !fl_unicode_unichar_sequence_is_valid(FL_ENCODING_UTF8, (uint32_t[]) { 0xedadbf, 0xedb080, 0x00 }, NULL)); // 0x00 == NULL terminated
    fl_expect("Surrogates sequence U+DB7F U+DFFF is not valid UTF-8", !fl_unicode_unichar_sequence_is_valid(FL_ENCODING_UTF8, (uint32_t[]) { 0xedadbf, 0xedbfbf, 0x00 }, NULL)); // 0x00 == NULL terminated
    fl_expect("Surrogates sequence U+DB80 U+DC00 is not valid UTF-8", !fl_unicode_unichar_sequence_is_valid(FL_ENCODING_UTF8, (uint32_t[]) { 0xedae80, 0xedb080, 0x00 }, NULL)); // 0x00 == NULL terminated
    fl_expect("Surrogates sequence U+DB80 U+DFFF is not valid UTF-8", !fl_unicode_unichar_sequence_is_valid(FL_ENCODING_UTF8, (uint32_t[]) { 0xedae80, 0xedbfbf, 0x00 }, NULL)); // 0x00 == NULL terminated
    fl_expect("Surrogates sequence U+DBFF U+DC00 is not valid UTF-8", !fl_unicode_unichar_sequence_is_valid(FL_ENCODING_UTF8, (uint32_t[]) { 0xedafbf, 0xedb080, 0x00 }, NULL)); // 0x00 == NULL terminated
    fl_expect("Surrogates sequence U+DBFF U+DFFF is not valid UTF-8", !fl_unicode_unichar_sequence_is_valid(FL_ENCODING_UTF8, (uint32_t[]) { 0xedafbf, 0xedbfbf, 0x00 }, NULL)); // 0x00 == NULL terminated*/
}

void test_fl_unicode_codepoint_validity()
{
    fl_expect("Replacement char U+FFFD is valid UTF-8", fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xef\xbf\xbd\x00", 0x00)); // NULL 0x00 breaks
    
    // Overlon encodings
    fl_expect("Overlong encoding of U+0000 (0xC080) is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xC0\x80\x00", 0x00)); // NULL 0x00 breaks
    fl_expect("Overlong encoding of U+002F (0xc0af) is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xc0\xaf\x00", 0x00)); // NULL 0x00 breaks
    fl_expect("Overlong encoding of U+002F (0xe080af) is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xe0\x80\xaf\x00", 0x00)); // NULL 0x00 breaks
    fl_expect("Overlong encoding of U+002F (0xf08080af) is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xf0\x80\x80\xaf\x00", 0x00)); // NULL 0x00 breaks
    fl_expect("Overlong encoding of U+007F (0xc1bf) is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xc1\xbf\x00", 0x00)); // NULL 0x00 breaks
    fl_expect("Overlong encoding of U+07FF (0xe09fbf) is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xe0\x9f\xbf\x00", 0x00)); // NULL 0x00 breaks
    fl_expect("Overlong encoding of U+FFFF (0xf08fbfbf) is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xf0\x8f\xbf\xbf\x00", 0x00)); // NULL 0x00 breaks
    fl_expect("Overlong encoding of U+0000 (0xc080) is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xc0\x80\x00", 0x00)); // NULL 0x00 breaks
    fl_expect("Overlong encoding of U+0000 (0xe08080) is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xe0\x80\x80\x00", 0x00)); // NULL 0x00 breaks
    fl_expect("Overlong encoding of U+0000 (0xf0808080) is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xf0\x80\x80\x80\x00", 0x00)); // NULL 0x00 breaks
    
    // UTF-16 Surrogates
    fl_expect("UTF-16 Surrogate U+D800 (0xeda080) is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xed\xa0\x80\x00", 0x00)); // NULL 0x00 breaks
    fl_expect("UTF-16 Surrogate U+DB7F (0xedadbf) is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xed\xad\xbf\x00", 0x00)); // NULL 0x00 breaks
    fl_expect("UTF-16 Surrogate U+DB80 (0xedae80) is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xed\xae\x80\x00", 0x00)); // NULL 0x00 breaks
    fl_expect("UTF-16 Surrogate U+DBFF (0xedafbf) is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xed\xaf\xbf\x00", 0x00)); // NULL 0x00 breaks
    fl_expect("UTF-16 Surrogate U+DC00 (0xedb080) is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xed\xb0\x80\x00", 0x00)); // NULL 0x00 breaks
    fl_expect("UTF-16 Surrogate U+DF80 (0xedbe80) is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xed\xbe\x80\x00", 0x00)); // NULL 0x00 breaks
    fl_expect("UTF-16 Surrogate U+DFFF (0xedbfbf) is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xed\xbf\xbf\x00", 0x00)); // NULL 0x00 breaks

    // Boundaries
    fl_expect("High surrogate U+D84C is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xED\xA1\x8C\x00", 0x00)); // NULL 0x00 breaks
    uint32_t d84c = 0xD84C;
    fl_expect("High surrogate U+D84C is not valid UTF-32", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF32, (FlByte*)&d84c, 0x00)); // NULL 0x00 breaks
    fl_expect("High surrogate boundary U+D7FF is valid UTF-8", fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xed\x9f\xbf\x00", 0x00)); // NULL 0x00 breaks
    fl_expect("Max boundary U+10FFFF is valid UTF-8", fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xf4\x8f\xbf\xbf\x00", 0x00)); // NULL 0x00 breaks
    fl_expect("Max boundary+1 U+110000 is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xf4\x90\x80\x80\x00", 0x00)); // NULL 0x00 breaks

    fl_expect("U+0000 is valid UTF-8 (0x0000)", fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\x00\x00\x00", 0x00)); // NULL 0x00 breaks
    fl_expect("U+0080 is valid UTF-8 (0xC280)", fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xC2\x80\x00", 0x00)); // NULL 0x00 breaks
    fl_expect("U+0800 is valid UTF-8 (0xe0a080)", fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xe0\xa0\x80\x00", 0x00)); // NULL 0x00 breaks
    fl_expect("U+10000 is valid UTF-8 (0xf0908080)", fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xf0\x90\x80\x80\x00", 0x00)); // NULL 0x00 breaks

    fl_expect("U+007F is valid UTF-8 (0x007F)", fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\x00\x7F\x00", 0x00)); // NULL 0x00 breaks
    fl_expect("U+07FF is valid UTF-8 (0xdfbf)", fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xdf\xbf\x00", 0x00)); // NULL 0x00 breaks
    fl_expect("U+FFFF is valid UTF-8 (0xefbfbf)", fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xef\xbf\xbf\x00", 0x00)); // NULL 0x00 breaks
    
    fl_expect("U+0E00 is valid UTF-8 (0xe0b880)", fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xe0\xb8\x80\x00", 0x00)); // NULL 0x00 breaks


    // Unexpected continuation bytes
    fl_expect("Continuation byte 0x80 is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\x80\x00", 0x00)); // NULL 0x00 breaks
    fl_expect("Continuation byte 0xbf is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xbf\x00", 0x00)); // NULL 0x00 breaks

    // 2-bytes sequence start bytes
    fl_expect("0xC0 is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xC0\x00", 0x00)); // NULL 0x00 breaks
    fl_expect("0xC5 is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xC5\x00", 0x00)); // NULL 0x00 breaks
    fl_expect("0xCA is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xCA\x00", 0x00)); // NULL 0x00 breaks
    fl_expect("0xCF is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xCF\x00", 0x00)); // NULL 0x00 breaks
    fl_expect("0xD0 is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xD0\x00", 0x00)); // NULL 0x00 breaks
    fl_expect("0xD5 is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xD5\x00", 0x00)); // NULL 0x00 breaks
    fl_expect("0xDA is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xDA\x00", 0x00)); // NULL 0x00 breaks
    fl_expect("0xDF is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xDF\x00", 0x00)); // NULL 0x00 breaks

    // 3-bytes sequence start bytes
    fl_expect("0xE0 is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xE0\x00", 0x00)); // NULL 0x00 breaks
    fl_expect("0xE5 is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xE5\x00", 0x00)); // NULL 0x00 breaks
    fl_expect("0xEA is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xEA\x00", 0x00)); // NULL 0x00 breaks
    fl_expect("0xEF is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xEF\x00", 0x00)); // NULL 0x00 breaks

    // 4-bytes sequence start bytes
    fl_expect("0xF0 is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xF0\x00", 0x00)); // NULL 0x00 breaks
    fl_expect("0xF5 is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xF5\x00", 0x00)); // NULL 0x00 breaks
    fl_expect("0xFA is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xFA\x00", 0x00)); // NULL 0x00 breaks
    fl_expect("0xFE is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xFE\x00", 0x00)); // NULL 0x00 breaks
    fl_expect("0xFF is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xFF\x00", 0x00)); // NULL 0x00 breaks

    // Sequences
    
    // Last byte missing
    fl_expect("Sequence xC0 is not valid UTF-8", !fl_unicode_codeunit_sequence_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xC0\x00", NULL)); // x00 == NULL terminated
    fl_expect("Sequence xDF is not valid UTF-8", !fl_unicode_codeunit_sequence_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xDF\x00", NULL)); // x00 == NULL terminated
    fl_expect("Sequence xE0x80 is not valid UTF-8", !fl_unicode_codeunit_sequence_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xE0\x80\x00", NULL)); // x00 == NULL terminated
    fl_expect("Sequence xEFxBF is not valid UTF-8", !fl_unicode_codeunit_sequence_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xEF\xBF\x00", NULL)); // x00 == NULL terminated
    fl_expect("Sequence xF0x80x80 is not valid UTF-8", !fl_unicode_codeunit_sequence_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xF0\x80\x80\x00", NULL)); // x00 == NULL terminated
    fl_expect("Sequence xF7xBFxBF is not valid UTF-8", !fl_unicode_codeunit_sequence_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xF7\xBF\xBF\x00", NULL)); // x00 == NULL terminated

    // Surrogates
    fl_expect("Surrogates sequence U+D800 U+DC00 is not valid UTF-8", !fl_unicode_codeunit_sequence_is_valid(FL_ENCODING_UTF8, (FlByte*) "\xed\xa0\x80\xed\xb0\x80\x00", NULL)); // x00 == NULL terminated
    fl_expect("Surrogates sequence U+D800 U+DFFF is not valid UTF-8", !fl_unicode_codeunit_sequence_is_valid(FL_ENCODING_UTF8, (FlByte*) "\xed\xa0\x80\xed\xbf\xbf\x00", NULL)); // x00 == NULL terminated
    fl_expect("Surrogates sequence U+DB7F U+DC00 is not valid UTF-8", !fl_unicode_codeunit_sequence_is_valid(FL_ENCODING_UTF8, (FlByte*) "\xed\xad\xbf\xed\xb0\x80\x00", NULL)); // x00 == NULL terminated
    fl_expect("Surrogates sequence U+DB7F U+DFFF is not valid UTF-8", !fl_unicode_codeunit_sequence_is_valid(FL_ENCODING_UTF8, (FlByte*) "\xed\xad\xbf\xed\xbf\xbf\x00", NULL)); // x00 == NULL terminated
    fl_expect("Surrogates sequence U+DB80 U+DC00 is not valid UTF-8", !fl_unicode_codeunit_sequence_is_valid(FL_ENCODING_UTF8, (FlByte*) "\xed\xae\x80\xed\xb0\x80\x00", NULL)); // x00 == NULL terminated
    fl_expect("Surrogates sequence U+DB80 U+DFFF is not valid UTF-8", !fl_unicode_codeunit_sequence_is_valid(FL_ENCODING_UTF8, (FlByte*) "\xed\xae\x80\xed\xbf\xbf\x00", NULL)); // x00 == NULL terminated
    fl_expect("Surrogates sequence U+DBFF U+DC00 is not valid UTF-8", !fl_unicode_codeunit_sequence_is_valid(FL_ENCODING_UTF8, (FlByte*) "\xed\xaf\xbf\xed\xb0\x80\x00", NULL)); // x00 == NULL terminated
    fl_expect("Surrogates sequence U+DBFF U+DFFF is not valid UTF-8", !fl_unicode_codeunit_sequence_is_valid(FL_ENCODING_UTF8, (FlByte*) "\xed\xaf\xbf\xed\xbf\xbf\x00", NULL)); // x00 == NULL terminated
}

void test_fl_unicode_unichar_sequence_validate()
{
    /*#define seq_are_eq(a, b, l) (memcmp((a), (b), sizeof(uint32_t)*(l)) == 0)
    #define REPLCHAR 0xEFBFBD
    
    //
    uint32_t *validated = fl_unicode_unichar_sequence_validate(FL_ENCODING_UTF8, UCHARSEQ(0x31, 0x32, 0xC0, 0x34, 0x00), NULL); // 12�4
    fl_expect("UTF8 sequence 0x31 0x32 0xC0 0x34 validates as 0x31 0x32 0xFFFD 0x34", seq_are_eq(validated, UCHARSEQ(0x31, 0x32, REPLCHAR, 0x34), 4));
    fl_free(validated);

    //
    validated = fl_unicode_unichar_sequence_validate(FL_ENCODING_UTF8, UCHARSEQ(0xC2, 0x41, 0x42, 0x00), NULL);
    fl_expect("UTF8 sequence 0xC2 0x41 0x42 validates as 0xFFFD 0x41 0x42", seq_are_eq(validated, UCHARSEQ(REPLCHAR, 0x41, 0x42), 3));
    fl_free(validated);

    //
    validated = fl_unicode_unichar_sequence_validate(FL_ENCODING_UTF32, UCHARSEQ(0x31, 0x32, 0xD800, 0x34, 0x00), NULL); // 12�4
    fl_expect("UTF32 sequence 0x31 0x32 0xD800 0x34 validates as 0x31 0x32 0xFFFD 0x34", seq_are_eq(validated, UCHARSEQ(0x31, 0x32, FL_UNICODE_REPLACEMENT_CHARACTER, 0x34), 4));
    fl_free(validated);*/
}

void test_fl_unicode_codepoint_convert()
{
    FlByte dst[4] = {0,0,0,0};
    size_t bytes = 0;
    
    //
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x00\x00\x41\x00", NULL, FL_ENCODING_UTF8, dst);
    fl_expect("UTF-32 U+0041 encoded to UTF-8 is equals to 0x41 and occupies 1 byte", bytes == 1 && fl_equals(dst, "\x41", bytes));

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"\x41\x00", NULL, FL_ENCODING_UTF32, dst);
    fl_expect("UTF-8 0x41 encoded to UTF-32 is equals to 0x00000041 and occupies 4 bytes", bytes == 4 && fl_equals(dst, "\x00\x00\x00\x41", bytes));

    //
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x00\x01\x00", NULL, FL_ENCODING_UTF8, dst);
    fl_expect("UTF-32 U+0100 encoded to UTF-8 is equals to 0xC480 and occupies 2 bytes", bytes == 2 && fl_equals(dst, "Ā\x00", bytes));

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"Ā\x00", NULL, FL_ENCODING_UTF32, dst);
    fl_expect("UTF-8 Ā encoded to UTF-32 is equals to 0x00000100 and occupies 4 bytes", bytes == 4 && fl_equals(dst, "\x00\x00\x01\x00", bytes));

    //
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x00\x00\x5A", NULL, FL_ENCODING_UTF8, dst);
    fl_expect("UTF-32 U+005A encoded to UTF-8 is equals to 0x005A and occupies 1 byte", bytes == 1 && fl_equals(dst, "\x5A\x00", bytes));

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"Z\x00", NULL, FL_ENCODING_UTF32, dst);
    fl_expect("UTF-8 Z encoded to UTF-32 is equals to 0x0000005A and occupies 4 bytes", bytes == 4 && fl_equals(dst, "\x00\x00\x00\x5A", bytes));

    //
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x00\x03\xA0", NULL, FL_ENCODING_UTF8, dst);
    fl_expect("UTF-32 U+03A0 encoded to UTF-8 is equals to 0xCEA0 and occupies 2 bytes", bytes == 2 && fl_equals(dst, "\xCE\xA0", bytes));

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"\xCE\xA0\x00", NULL, FL_ENCODING_UTF32, dst);
    fl_expect("UTF-8 0xCEA000 encoded to UTF-32 is equals to 0x000003A0 and occupies 4 bytes", bytes == 4 && fl_equals(dst, "\x00\x00\x03\xA0", bytes));

    //
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x00\xFF\x21", NULL, FL_ENCODING_UTF8, dst);
    fl_expect("UTF-32 U+FF21 encoded to UTF-8 is equals to 0xEFBCA1 and occupies 3 bytes", bytes == 3 && fl_equals(dst, "\xEF\xBC\xA1", bytes));

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"\xEF\xBC\xA1\x00", NULL, FL_ENCODING_UTF32, dst);
    fl_expect("UTF-8 0xEFBCA1 encoded to UTF-32 is equals to 0x0000FF21 and occupies 4 bytes", bytes == 4 && fl_equals(dst, "\x00\x00\xFF\x21", bytes));

    //
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x02\xF8\x0F\x00", NULL, FL_ENCODING_UTF8, dst);
    fl_expect("UTF-32 U+2F80F encoded to UTF-8 is equals to 0xF0AFA08F and occupies 3 bytes", bytes == 4 && fl_equals(dst, "\xF0\xAF\xA0\x8F", bytes));

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"\xF0\xAF\xA0\x8F\x00", NULL, FL_ENCODING_UTF32, dst);
    fl_expect("UTF-8 0xF0AFA08F encoded to UTF-32 is equals to 0x00002F80F and occupies 4 bytes", bytes == 4 && fl_equals(dst, "\x00\x02\xF8\x0F", bytes));

    //
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x00\xD8\x00\x00", NULL, FL_ENCODING_UTF8, dst);
    fl_expect("UTF-32 U+D800 encoded to UTF-8 is invalid because it is a High Surrogate", bytes == FL_UNICODE_INVALID_SIZE);

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"\xED\xA0\x80", NULL, FL_ENCODING_UTF32, dst);
    fl_expect("UTF-8 0xEDA080 encoded to UTF-32 is invalid because it is a High Surrogate", bytes == FL_UNICODE_INVALID_SIZE);

    //
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x00\xD8\x00\x00", NULL, FL_ENCODING_UTF8, dst);
    fl_expect("UTF-32 U+D800 encoded to UTF-8 is invalid because it is the first High Surrogate", bytes == FL_UNICODE_INVALID_SIZE);

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"\xED\xA0\x80", NULL, FL_ENCODING_UTF32, dst);
    fl_expect("UTF-8 0xEDA080 encoded to UTF-32 is invalid because it is the first High Surrogate U+D800", bytes == FL_UNICODE_INVALID_SIZE);

    //
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x00\xDF\xFF\x00", NULL, FL_ENCODING_UTF8, dst);
    fl_expect("UTF-32 U+DFFF encoded to UTF-8 is invalid because it is the last Low Surrogate", bytes == FL_UNICODE_INVALID_SIZE);

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"\xED\xBF\xBF", NULL, FL_ENCODING_UTF32, dst);
    fl_expect("UTF-8 0xEDBFBF encoded to UTF-32 is invalid because it is the last Low Surrogate U+DFFF", bytes == FL_UNICODE_INVALID_SIZE);

    // UTF8 1-byte
    memset(dst, 0, 4);
    const FlByte *str = (FlByte*)"\x00\x00\x00\x00";
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)str, str+4, FL_ENCODING_UTF8, dst);
    fl_expect("UTF-32 U+0000 encoded to UTF-8 is equals to 0x00 and occupies 1 byte", bytes == 1 && fl_equals(dst, "\x00", bytes));

    memset(dst, 0, 4);
    str = (FlByte*)"\x00";
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)str, str+1, FL_ENCODING_UTF32, dst);
    fl_expect("UTF-8 0x00 encoded to UTF-32 is equals to 0x00000000 and occupies 4 bytes", bytes == 4 && fl_equals(dst, "\x00\x00\x00\x00", bytes));

    //
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x00\x00\x48\x00", NULL, FL_ENCODING_UTF8, dst);
    fl_expect("UTF-32 U+0048 encoded to UTF-8 is equals to 0x48 and occupies 1 byte", bytes == 1 && fl_equals(dst, "H\x00", bytes));

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"H\x00", NULL, FL_ENCODING_UTF32, dst);
    fl_expect("UTF-8 H encoded to UTF-32 is equals to 0x00000048 and occupies 4 bytes", bytes == 4 && fl_equals(dst, "\x00\x00\x00\x48", bytes));

    //
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x00\x00\x7F\x00", NULL, FL_ENCODING_UTF8, dst);
    fl_expect("UTF-32 U+007F encoded to UTF-8 is equals to 0x7F and occupies 1 byte", bytes == 1 && fl_equals(dst, "\x7F\x00", bytes));

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"\x7F\x00", NULL, FL_ENCODING_UTF32, dst);
    fl_expect("UTF-8 0x7F encoded to UTF-32 is equals to 0x0000007F and occupies 4 bytes", bytes == 4 && fl_equals(dst, "\x00\x00\x00\x7F", bytes));

    // UTF8 2-bytes
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x00\x00\x80\x00", NULL, FL_ENCODING_UTF8, dst);
    fl_expect("UTF-32 U+0080 encoded to UTF-8 is equals to 0xC280 and occupies 2 bytes", bytes == 2 && fl_equals(dst, "\xC2\x80", bytes));

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"\xC2\x80\x00", NULL, FL_ENCODING_UTF32, dst);
    fl_expect("UTF-8 0xC280 encoded to UTF-32 is equals to 0x00000080 and occupies 4 bytes", bytes == 4 && fl_equals(dst, "\x00\x00\x00\x80", bytes));

    //
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x00\x01\x81\x00", NULL, FL_ENCODING_UTF8, dst);
    fl_expect("UTF-32 U+0181 encoded to UTF-8 is equals to 0xC681 and occupies 2 bytes", bytes == 2 && fl_equals(dst, "\xC6\x81", bytes));

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"\xC6\x81\x00", NULL, FL_ENCODING_UTF32, dst);
    fl_expect("UTF-8 0xC681 encoded to UTF-32 is equals to 0x00000181 and occupies 4 bytes", bytes == 4 && fl_equals(dst, "\x00\x00\x01\x81", bytes));

    //
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x00\x07\xFF\x00", NULL, FL_ENCODING_UTF8, dst);
    fl_expect("UTF-32 U+07FF encoded to UTF-8 is equals to 0xDFBF and occupies 2 bytes", bytes == 2 && fl_equals(dst, "\xDF\xBF", bytes));

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"\xDF\xBF\x00", NULL, FL_ENCODING_UTF32, dst);
    fl_expect("UTF-8 0xDFBF encoded to UTF-32 is equals to 0x000007FF and occupies 4 bytes", bytes == 4 && fl_equals(dst, "\x00\x00\x07\xFF", bytes));

    // UTF8 3-bytes
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x00\x08\x00\x00", NULL, FL_ENCODING_UTF8, dst);
    fl_expect("UTF-32 U+0800 encoded to UTF-8 is equals to 0xE0A080 and occupies 3 bytes", bytes == 3 && fl_equals(dst, "\xE0\xA0\x80", bytes));

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"\xE0\xA0\x80\x00", NULL, FL_ENCODING_UTF32, dst);
    fl_expect("UTF-8 0xE0A080 encoded to UTF-32 is equals to 0x00000800 and occupies 4 bytes", bytes == 4 && fl_equals(dst, "\x00\x00\x08\x00", bytes));

    //
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x00\xA8\xF9\x00", NULL, FL_ENCODING_UTF8, dst);
    fl_expect("UTF-32 U+A8F9 encoded to UTF-8 is equals to 0xEAA3B9 and occupies 3 bytes", bytes == 3 && fl_equals(dst, "\xEA\xA3\xB9", bytes));

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"\xEA\xA3\xB9\x00", NULL, FL_ENCODING_UTF32, dst);
    fl_expect("UTF-8 0xEAA3B9 encoded to UTF-32 is equals to 0x0000A8F9 and occupies 4 bytes", bytes == 4 && fl_equals(dst, "\x00\x00\xA8\xF9", bytes));

    //
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x00\xFF\xFF\x00", NULL, FL_ENCODING_UTF8, dst);
    fl_expect("UTF-32 U+FFFF encoded to UTF-8 is equals to 0xEFBFBF and occupies 3 bytes", bytes == 3 && fl_equals(dst, "\xEF\xBF\xBF", bytes));

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"\xEF\xBF\xBF\x00", NULL, FL_ENCODING_UTF32, dst);
    fl_expect("UTF-8 0xEFBFBF encoded to UTF-32 is equals to 0x0000FFFF and occupies 4 bytes", bytes == 4 && fl_equals(dst, "\x00\x00\xFF\xFF", bytes));

    // UTF8 4-bytes
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x01\x00\x00\x00", NULL, FL_ENCODING_UTF8, dst);
    fl_expect("UTF-32 U+10000 encoded to UTF-8 is equals to 0xF0908080 and occupies 4 bytes", bytes == 4 && fl_equals(dst, "\xF0\x90\x80\x80", bytes));

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"\xF0\x90\x80\x80\x00", NULL, FL_ENCODING_UTF32, dst);
    fl_expect("UTF-8 0xF0908080 encoded to UTF-32 is equals to 0x00010000 and occupies 4 bytes", bytes == 4 && fl_equals(dst, "\x00\x01\x00\x00", bytes));

    //
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x01\xF6\x15\x00", NULL, FL_ENCODING_UTF8, dst);
    fl_expect("UTF-32 U+1F615 encoded to UTF-8 is equals to 0xF09F9895 and occupies 4 bytes", bytes == 4 && fl_equals(dst, "\xF0\x9F\x98\x95", bytes));

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"\xF0\x9F\x98\x95\x00", NULL, FL_ENCODING_UTF32, dst);
    fl_expect("UTF-8 0xF09F9895 encoded to UTF-32 is equals to 0x0001F615 and occupies 4 bytes", bytes == 4 && fl_equals(dst, "\x00\x01\xF6\x15", bytes));

    //
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x10\xFF\xFF\x00", NULL, FL_ENCODING_UTF8, dst);
    fl_expect("UTF-32 U+10FFFF encoded to UTF-8 is equals to 0xF48FBFBF and occupies 4 bytes", bytes == 4 && fl_equals(dst, "\xF4\x8F\xBF\xBF", bytes));

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"\xF4\x8F\xBF\xBF\x00", NULL, FL_ENCODING_UTF32, dst);
    fl_expect("UTF-8 0xF48FBFBF encoded to UTF-32 is equals to 0x0010FFFF and occupies 4 bytes", bytes == 4 && fl_equals(dst, "\x00\x10\xFF\xFF", bytes));

    //
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x11\x00\x00\x00", NULL, FL_ENCODING_UTF8, dst);
    fl_expect("UTF-32 U+110000 encoded to UTF-8 is invalid because it is beyond the upper bound", bytes == FL_UNICODE_INVALID_SIZE);

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"\xF4\x8F\xBF\xC0\x00", NULL, FL_ENCODING_UTF32, dst);
    fl_expect("UTF-8 0xF48FBFC0 encoded to UTF-32 is invalid because it is beyond the upper bound", bytes == FL_UNICODE_INVALID_SIZE);

    //
    FlByte *chr = (FlByte*)"你\x00";
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, chr, NULL, FL_ENCODING_UTF32, dst);
    fl_expect("Char 你 is 4 bytes in UTF-32", bytes == 4);
    fl_expect("Char 你 is encoded as 0x2F804 in UTF-32", fl_equals(dst, "\x00\x02\xF8\x04", bytes));
    
    FlByte dst2[4] = {0,0,0,0};
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, dst, NULL, FL_ENCODING_UTF8, dst2);
    fl_expect("Char 你 is 4 bytes in UTF-8", bytes == 4);
    fl_expect("Char 你 is encoded as 0xF0AFA084 in UTF-8", fl_equals(dst2, "\xF0\xAF\xA0\x84", bytes));
}

void test_fl_unicode_codepoint_sequence_validate()
{
    /// UTF-32
    // 1-Byte length sequence
    FlByte *str = (FlByte*)"\x00\x00\x00\x01";
    FlByte *validstr = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF32, str, str+4);
    fl_expect("UTF-32: Validated Code Point Sequence {U+0001} is equals to {U+0001}", fl_equals(str, validstr, 4));
    free(validstr);

    // Sequence larger than 1 byte
    str = (FlByte*)"\x00\x00\x00\x01\x00\x00\x00\x02\x00\x00\x00\x03\x00\x00\x00\x04";
    validstr = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF32, str, str+4*4);
    fl_expect("UTF-32: Validated Code Point Sequence {U+0001,U+0002,U+0003,U+0004} is equals to {U+0001,U+0002,U+0003,U+0004}", fl_equals(str, validstr, 4*4));
    free(validstr);

    // One ill-formed code point in the middle of the sequence
    str = (FlByte*)"\x00\x00\x00\x01\xFF\xFF\xFF\xFF\x00\x00\x00\x03\x00\x00\x00\x04";
    validstr = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF32, str, str+4*4);
    fl_expect("UTF-32: Validated Code Point Sequence {U+0001,U+FFFFFFFF,U+0003,U+0004} is equals to {U+0001,U+FFFD,U+0003,U+0004}", fl_equals((const FlByte*)"\x00\x00\x00\x01\x00\x00\xFF\xFD\x00\x00\x00\x03\x00\x00\x00\x04", validstr, 4*4));
    free(validstr);

    // Two ill-formed code point in the middle of the sequence
    str = (FlByte*)"\x00\x00\x00\x01\xFF\xFF\xFF\xFF\x00\x00\x00\x03\xFF\x10\xFF\xEE\x00\x00\x00\x05";
    validstr = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF32, str, str+4*5);
    fl_expect("UTF-32: Validated Code Point Sequence {U+0001,U+FFFFFFFF,U+0003,U+FF10FFEE,U+0005} is equals to {U+0001,U+FFFD,U+0003,U+FFFD,U+0005}", fl_equals((const FlByte*)"\x00\x00\x00\x01\x00\x00\xFF\xFD\x00\x00\x00\x03\x00\x00\xFF\xFD\x00\x00\x00\x05", validstr, 4*5));
    free(validstr);

    // Two ill-formed code point, one in the middle of the sequence another in the upper bound
    str = (FlByte*)"\x00\x00\x00\x01\xFF\xFF\xFF\xFF\x00\x00\x00\x03\xFF\x10\xFF\xEE";
    validstr = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF32, str, str+4*4);
    fl_expect("UTF-32: Validated Code Point Sequence {U+0001,U+FFFFFFFF,U+0003,U+FF10FFEE} is equals to {U+0001,U+FFFD,U+0003,U+FFFD}", fl_equals((const FlByte*)"\x00\x00\x00\x01\x00\x00\xFF\xFD\x00\x00\x00\x03\x00\x00\xFF\xFD", validstr, 4*4));
    free(validstr);

    // Two ill-formed code point, one in the lower bound of the sequence another in the middle
    str = (FlByte*)"\xFF\x10\xFF\xEE\x00\x00\x00\x02\x00\x00\x00\x03\xFF\xFF\xFF\xFF\x00\x00\x00\x05";
    validstr = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF32, str, str+4*5);
    fl_expect("UTF-32: Validated Code Point Sequence {U+FF10FFEE,U+0002,U+0003,U+FFFFFFFF,U+0005} is equals to {U+FFFD,U+0002,U+0003,U+FFFD,U+0005}", fl_equals((const FlByte*)"\x00\x00\xFF\xFD\x00\x00\x00\x02\x00\x00\x00\x03\x00\x00\xFF\xFD\x00\x00\x00\x05", validstr, 4*5));
    free(validstr);

    // Two ill-formed code point at both bounds
    str = (FlByte*)"\xFF\x10\xFF\xEE\x00\x00\x00\x02\x00\x00\x00\x03\xFF\xFF\xFF\xFF";
    validstr = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF32, str, str+4*4);
    fl_expect("UTF-32: Validated Code Point Sequence {U+FF10FFEE,U+0002,U+0003,U+FFFFFFFF} is equals to {U+FFFD,U+0002,U+0003,U+FFFD}", fl_equals((const FlByte*)"\x00\x00\xFF\xFD\x00\x00\x00\x02\x00\x00\x00\x03\x00\x00\xFF\xFD", validstr, 4*4));
    free(validstr);

    // Interleaved ill-formed and well-formed code points
    str = (FlByte*)"\xFF\x10\xFF\xEE\x00\x00\x00\x02\xFF\xFF\xFF\xFF\x00\x00\x00\x04\xFF\xFF\xFF\xDD\x00\x00\x00\x06";
    validstr = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF32, str, str+4*6);
    fl_expect("UTF-32: Validated Code Point Sequence {U+FF10FFEE,U+0002,U+FFFFFFFF,U+0004,U+FFFFFFDD,U+0006} is equals to {U+FFFD,U+0002,U+FFFD,U+0004,U+FFFD,U+0006}", fl_equals((const FlByte*)"\x00\x00\xFF\xFD\x00\x00\x00\x02\x00\x00\xFF\xFD\x00\x00\x00\x04\x00\x00\xFF\xFD\x00\x00\x00\x06", validstr, 4*6));
    free(validstr);

    // Full ill-formed sequence
    str = (FlByte*)"\xFF\x10\xFF\xEE\xFF\xEF\xBC\xED\xFF\xEA\xAF\xA3\xFF\xFF\xFF\xFF\xF9\xFF\xEF\xFF";
    validstr = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF32, str, str+4*5);
    fl_expect("UTF-32: Validated Code Point Sequence {U+FF10FFEE,U+FFEFBCED,U+FFEAAFA3,U+FFFFFFFF,U+F9FFEFFF} is equals to {U+FFFD,U+FFFD,U+FFFD,U+FFFD,U+FFFD}", fl_equals((const FlByte*)"\x00\x00\xFF\xFD\x00\x00\xFF\xFD\x00\x00\xFF\xFD\x00\x00\xFF\xFD\x00\x00\xFF\xFD", validstr, 4*5));
    free(validstr);
}
