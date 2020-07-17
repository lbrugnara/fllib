#include <locale.h>
#include <fllib.h>
#include <string.h>

#include <flut/flut.h>

void test_fl_unicode_codepoint_at()
{
    // All the code points size in this string is two-bytes
    char *str = "ĀāĂăĄąĆćĈĉĊċČčĎďĐđĒēĔĕĖėĘęĚěĜĝĞğĠġĢģĤĥĦħĨĩĪīĬĭĮįİıĲĳĴĵĶķĸĹĺĻļĽľĿŀŁłŃńŅņŇňŉŊŋŌōŎŏŐőŒœŔŕŖŗŘřŚśŜŝŞşŠšŢţŤťŦŧŨũŪūŬŭŮůŰűŲųŴŵŶŷŸŹźŻżŽž\x00";
    flut_expect_compat("str is 127 code points long and it is composed of 256 code units", 
        fl_unicode_codepoint_sequence_length(FL_ENCODING_UTF8, (FlByte*)str, 0x00)
        && fl_unicode_codeunit_sequence_size(FL_ENCODING_UTF8, (FlByte*)str, 0x00)
    );

    // 
    FlByte dst[4] = {0,0,0,0};
    size_t nbytes = fl_unicode_codepoint_at(FL_ENCODING_UTF8, (FlByte*)str, 0x00, 0, dst);
    flut_expect_compat("0 = Ā and it is 2 bytes long", memcmp(str, dst, nbytes) == 0 && nbytes == 2);

    // 
    memset(dst, 0, 4);
    nbytes = fl_unicode_codepoint_at(FL_ENCODING_UTF8, (FlByte*)str, 0x00, 28, dst);
    flut_expect_compat("28 = Ĝ and it is 2 bytes long", memcmp(str+28*2, dst, nbytes) == 0 && nbytes == 2); // Char 28 multiplied by 2 bytes each one

    // 
    memset(dst, 0, 4);
    nbytes = fl_unicode_codepoint_at(FL_ENCODING_UTF8, (FlByte*)str, 0x00, 40, dst);
    flut_expect_compat("40 = Ĩ and it is 2 bytes long", memcmp(str+40*2, dst, nbytes) == 0 && nbytes == 2); // Char 40 multiplied by 2 bytes each one

    // 
    memset(dst, 0, 4);
    nbytes = fl_unicode_codepoint_at(FL_ENCODING_UTF8, (FlByte*)str, 0x00, 60, dst);
    flut_expect_compat("60 = ļ and it is 2 bytes long", memcmp(str+60*2, dst, nbytes) == 0 && nbytes == 2); // Char 60 multiplied by 2 bytes each one

    // 
    memset(dst, 0, 4);
    nbytes = fl_unicode_codepoint_at(FL_ENCODING_UTF8, (FlByte*)str, 0x00, 40, dst);
    flut_expect_compat("40 = Ĩ and it is 2 bytes long", memcmp(str+40*2, dst, nbytes) == 0 && nbytes == 2); // Char 40 multiplied by 2 bytes each one

    // 
    memset(dst, 0, 4);
    nbytes = fl_unicode_codepoint_at(FL_ENCODING_UTF8, (FlByte*)str, 0x00, 92, dst);
    flut_expect_compat("92 = Ŝ and it is 2 bytes long", memcmp(str+92*2, dst, nbytes) == 0 && nbytes == 2); // Char 92 multiplied by 2 bytes each one

    // 
    memset(dst, 0, 4);
    nbytes = fl_unicode_codepoint_at(FL_ENCODING_UTF8, (FlByte*)str, 0x00, 126, dst);
    flut_expect_compat("126 = ž and it is 2 bytes long", memcmp(str+126*2, dst, nbytes) == 0 && nbytes == 2); // Char 126 multiplied by 2 bytes each one

    // 
    str = "\x00\x00\x00\x41\x00\x00\x00\x42";
    memset(dst, 0, 4);
    nbytes = fl_unicode_codepoint_at(FL_ENCODING_UTF32, (FlByte*)str, 0x00, 0, dst);
    flut_expect_compat("AB in 0 should be A", memcmp(str, dst, nbytes) == 0 && nbytes == 4);

    // 
    memset(dst, 0, 4);
    nbytes = fl_unicode_codepoint_at(FL_ENCODING_UTF32, (FlByte*)str, 0x00, 1, dst);
    flut_expect_compat("AB in 1 should be B", memcmp(str+1*4, dst, nbytes) == 0 && nbytes == 4);

    //
    flut_expect_compat("Size of U+D800 is FL_UNICODE_INVALID_SIZE because it is a High Surrogate", fl_unicode_codepoint_size(FL_ENCODING_UTF32, (FlByte*)"\x00\xD8\x00\x00\x00", 0x00) == FL_UNICODE_INVALID_SIZE);
    flut_expect_compat("Size of U+DFFF is FL_UNICODE_INVALID_SIZE because it is a Low Surrogate", fl_unicode_codepoint_size(FL_ENCODING_UTF32, (FlByte*)"\x00\xDF\xFF\xFF\x00", 0x00) == FL_UNICODE_INVALID_SIZE);
}

void test_fl_unicode_codeunit_sequence_size()
{
    flut_expect_compat("'abc123' has 6 bytes", fl_unicode_codeunit_sequence_size(FL_ENCODING_UTF8, (const FlByte*)"abc123", 0x00) == 6);
    flut_expect_compat("'aƁc123' has 7 bytes", fl_unicode_codeunit_sequence_size(FL_ENCODING_UTF8, (const FlByte*)"aƁc123", 0x00) == 7);
    flut_expect_compat("'a兔c123' has 9 bytes", fl_unicode_codeunit_sequence_size(FL_ENCODING_UTF8, (const FlByte*)"a兔c123", 0x00) == 9);
    flut_expect_compat("'兔' has 4 bytes", fl_unicode_codeunit_sequence_size(FL_ENCODING_UTF8, (const FlByte*)"兔", 0x00) == 4);
    flut_expect_compat("'ԱԲ' has 4 bytes", fl_unicode_codeunit_sequence_size(FL_ENCODING_UTF8, (const FlByte*)"ԱԲ", 0x00) == 4);
    flut_expect_compat("Code Point size is the same as the Code Unit sequence size (of length 4) for '兔'", fl_unicode_codeunit_sequence_size(FL_ENCODING_UTF8, (const FlByte*)"兔", 0x00) == fl_unicode_codepoint_size(FL_ENCODING_UTF8, (const FlByte*)"兔", 0x00));
    flut_expect_compat("Code Point size is the same as the Code Unit sequence size (of length 2) for 'Ɓ'", fl_unicode_codeunit_sequence_size(FL_ENCODING_UTF8, (const FlByte*)"Ɓ", 0x00) == fl_unicode_codepoint_size(FL_ENCODING_UTF8, (const FlByte*)"Ɓ", 0x00));
}

void test_fl_unicode_unichar_validity()
{
    /*flut_expect_compat("Replacement char U+FFFD is valid UTF-8", fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xefbfbd));
    
    // Overlon encodings
    flut_expect_compat("Overlong encoding of U+0000 (0xC080) is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xC080));
    flut_expect_compat("Overlong encoding of U+002F (0xc0af) is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xc0af));
    flut_expect_compat("Overlong encoding of U+002F (0xe080af) is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xe080af));
    flut_expect_compat("Overlong encoding of U+002F (0xf08080af) is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xf08080af));
    flut_expect_compat("Overlong encoding of U+007F (0xc1bf) is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xc1bf));
    flut_expect_compat("Overlong encoding of U+07FF (0xe09fbf) is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xe09fbf));
    flut_expect_compat("Overlong encoding of U+FFFF (0xf08fbfbf) is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xf08fbfbf));
    flut_expect_compat("Overlong encoding of U+0000 (0xc080) is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xc080));
    flut_expect_compat("Overlong encoding of U+0000 (0xe08080) is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xe08080));
    flut_expect_compat("Overlong encoding of U+0000 (0xf0808080) is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xf0808080));
    
    // UTF-16 Surrogates
    flut_expect_compat("UTF-16 Surrogate U+D800 (0xeda080) is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xeda080));
    flut_expect_compat("UTF-16 Surrogate U+DB7F (0xedadbf) is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xedadbf));
    flut_expect_compat("UTF-16 Surrogate U+DB80 (0xedae80) is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xedae80));
    flut_expect_compat("UTF-16 Surrogate U+DBFF (0xedafbf) is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xedafbf));
    flut_expect_compat("UTF-16 Surrogate U+DC00 (0xedb080) is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xedb080));
    flut_expect_compat("UTF-16 Surrogate U+DF80 (0xedbe80) is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xedbe80));
    flut_expect_compat("UTF-16 Surrogate U+DFFF (0xedbfbf) is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xedbfbf));

    // Boundaries
    flut_expect_compat("High surrogate U+D84C is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xEDA18C));
    flut_expect_compat("High surrogate U+D84C is not valid UTF-32", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF32, 0xD84c));
    flut_expect_compat("High surrogate boundary U+D7FF is valid UTF-8", fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xed9fbf));
    flut_expect_compat("Max boundary U+10FFFF is valid UTF-8", fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xf48fbfbf));
    flut_expect_compat("Max boundary+1 U+110000 is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xf4908080));

    flut_expect_compat("U+0000 is valid UTF-8 (0x0000)", fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0x0000));
    flut_expect_compat("U+0080 is valid UTF-8 (0xC280)", fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xC280));
    flut_expect_compat("U+0800 is valid UTF-8 (0xe0a080)", fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xe0a080));
    flut_expect_compat("U+10000 is valid UTF-8 (0xf0908080)", fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xf0908080));

    flut_expect_compat("U+007F is valid UTF-8 (0x007F)", fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0x007F));
    flut_expect_compat("U+07FF is valid UTF-8 (0xdfbf)", fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xdfbf));
    flut_expect_compat("U+FFFF is valid UTF-8 (0xefbfbf)", fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xefbfbf));
    
    flut_expect_compat("U+0E00 is valid UTF-8 (0xe0b880)", fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xe0b880));


    // Unexpected continuation bytes
    flut_expect_compat("Continuation byte 0x80 is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0x80));
    flut_expect_compat("Continuation byte 0xbf is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xbf));

    // 2-bytes sequence start bytes
    flut_expect_compat("0xC0 is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xC0));
    flut_expect_compat("0xC5 is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xC5));
    flut_expect_compat("0xCA is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xCA));
    flut_expect_compat("0xCF is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xCF));
    flut_expect_compat("0xD0 is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xD0));
    flut_expect_compat("0xD5 is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xD5));
    flut_expect_compat("0xDA is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xDA));
    flut_expect_compat("0xDF is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xDF));

    // 3-bytes sequence start bytes
    flut_expect_compat("0xE0 is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xE0));
    flut_expect_compat("0xE5 is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xE5));
    flut_expect_compat("0xEA is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xEA));
    flut_expect_compat("0xEF is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xEF));

    // 4-bytes sequence start bytes
    flut_expect_compat("0xF0 is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xF0));
    flut_expect_compat("0xF5 is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xF5));
    flut_expect_compat("0xFA is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xFA));
    flut_expect_compat("0xFE is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xFE));
    flut_expect_compat("0xFF is not valid UTF-8", !fl_unicode_unichar_is_valid(FL_ENCODING_UTF8, 0xFF));

    // Sequences
    
    // Last byte missing
    flut_expect_compat("Sequence xC0 is not valid UTF-8", !fl_unicode_unichar_sequence_is_valid(FL_ENCODING_UTF8, (uint32_t[]){ 0xC0, 0x00 }, NULL)); // 0x00 == NULL terminated
    flut_expect_compat("Sequence xDF is not valid UTF-8", !fl_unicode_unichar_sequence_is_valid(FL_ENCODING_UTF8, (uint32_t[]){ 0xDF, 0x00 }, NULL)); // 0x00 == NULL terminated
    flut_expect_compat("Sequence xE0x80 is not valid UTF-8", !fl_unicode_unichar_sequence_is_valid(FL_ENCODING_UTF8, (uint32_t[]){ 0xE080, 0x00 }, NULL)); // 0x00 == NULL terminated
    flut_expect_compat("Sequence xEFxBF is not valid UTF-8", !fl_unicode_unichar_sequence_is_valid(FL_ENCODING_UTF8, (uint32_t[]){ 0xEFBF, 0x00 }, NULL)); // 0x00 == NULL terminated
    flut_expect_compat("Sequence xF0x80x80 is not valid UTF-8", !fl_unicode_unichar_sequence_is_valid(FL_ENCODING_UTF8, (uint32_t[]){ 0xF08080, 0x00 }, NULL)); // 0x00 == NULL terminated
    flut_expect_compat("Sequence xF7xBFxBF is not valid UTF-8", !fl_unicode_unichar_sequence_is_valid(FL_ENCODING_UTF8, (uint32_t[]){ 0xF7BFBF, 0x00 }, NULL)); // 0x00 == NULL terminated

    // Surrogates
    flut_expect_compat("Surrogates sequence U+D800 U+DC00 is not valid UTF-8", !fl_unicode_unichar_sequence_is_valid(FL_ENCODING_UTF8, (uint32_t[]) { 0xeda080, 0xedb080, 0x00 }, NULL)); // 0x00 == NULL terminated
    flut_expect_compat("Surrogates sequence U+D800 U+DFFF is not valid UTF-8", !fl_unicode_unichar_sequence_is_valid(FL_ENCODING_UTF8, (uint32_t[]) { 0xeda080, 0xedbfbf, 0x00 }, NULL)); // 0x00 == NULL terminated
    flut_expect_compat("Surrogates sequence U+DB7F U+DC00 is not valid UTF-8", !fl_unicode_unichar_sequence_is_valid(FL_ENCODING_UTF8, (uint32_t[]) { 0xedadbf, 0xedb080, 0x00 }, NULL)); // 0x00 == NULL terminated
    flut_expect_compat("Surrogates sequence U+DB7F U+DFFF is not valid UTF-8", !fl_unicode_unichar_sequence_is_valid(FL_ENCODING_UTF8, (uint32_t[]) { 0xedadbf, 0xedbfbf, 0x00 }, NULL)); // 0x00 == NULL terminated
    flut_expect_compat("Surrogates sequence U+DB80 U+DC00 is not valid UTF-8", !fl_unicode_unichar_sequence_is_valid(FL_ENCODING_UTF8, (uint32_t[]) { 0xedae80, 0xedb080, 0x00 }, NULL)); // 0x00 == NULL terminated
    flut_expect_compat("Surrogates sequence U+DB80 U+DFFF is not valid UTF-8", !fl_unicode_unichar_sequence_is_valid(FL_ENCODING_UTF8, (uint32_t[]) { 0xedae80, 0xedbfbf, 0x00 }, NULL)); // 0x00 == NULL terminated
    flut_expect_compat("Surrogates sequence U+DBFF U+DC00 is not valid UTF-8", !fl_unicode_unichar_sequence_is_valid(FL_ENCODING_UTF8, (uint32_t[]) { 0xedafbf, 0xedb080, 0x00 }, NULL)); // 0x00 == NULL terminated
    flut_expect_compat("Surrogates sequence U+DBFF U+DFFF is not valid UTF-8", !fl_unicode_unichar_sequence_is_valid(FL_ENCODING_UTF8, (uint32_t[]) { 0xedafbf, 0xedbfbf, 0x00 }, NULL)); // 0x00 == NULL terminated*/
}

void test_fl_unicode_codepoint_validity()
{
    flut_expect_compat("Replacement char U+FFFD is valid UTF-8", fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xef\xbf\xbd\x00", 0x00)); // NULL 0x00 breaks
    
    // Overlon encodings
    flut_expect_compat("Overlong encoding of U+0000 (0xC080) is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xC0\x80\x00", 0x00)); // NULL 0x00 breaks
    flut_expect_compat("Overlong encoding of U+002F (0xc0af) is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xc0\xaf\x00", 0x00)); // NULL 0x00 breaks
    flut_expect_compat("Overlong encoding of U+002F (0xe080af) is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xe0\x80\xaf\x00", 0x00)); // NULL 0x00 breaks
    flut_expect_compat("Overlong encoding of U+002F (0xf08080af) is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xf0\x80\x80\xaf\x00", 0x00)); // NULL 0x00 breaks
    flut_expect_compat("Overlong encoding of U+007F (0xc1bf) is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xc1\xbf\x00", 0x00)); // NULL 0x00 breaks
    flut_expect_compat("Overlong encoding of U+07FF (0xe09fbf) is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xe0\x9f\xbf\x00", 0x00)); // NULL 0x00 breaks
    flut_expect_compat("Overlong encoding of U+FFFF (0xf08fbfbf) is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xf0\x8f\xbf\xbf\x00", 0x00)); // NULL 0x00 breaks
    flut_expect_compat("Overlong encoding of U+0000 (0xc080) is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xc0\x80\x00", 0x00)); // NULL 0x00 breaks
    flut_expect_compat("Overlong encoding of U+0000 (0xe08080) is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xe0\x80\x80\x00", 0x00)); // NULL 0x00 breaks
    flut_expect_compat("Overlong encoding of U+0000 (0xf0808080) is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xf0\x80\x80\x80\x00", 0x00)); // NULL 0x00 breaks
    
    // UTF-16 Surrogates
    flut_expect_compat("UTF-16 Surrogate U+D800 (0xeda080) is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xed\xa0\x80\x00", 0x00)); // NULL 0x00 breaks
    flut_expect_compat("UTF-16 Surrogate U+DB7F (0xedadbf) is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xed\xad\xbf\x00", 0x00)); // NULL 0x00 breaks
    flut_expect_compat("UTF-16 Surrogate U+DB80 (0xedae80) is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xed\xae\x80\x00", 0x00)); // NULL 0x00 breaks
    flut_expect_compat("UTF-16 Surrogate U+DBFF (0xedafbf) is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xed\xaf\xbf\x00", 0x00)); // NULL 0x00 breaks
    flut_expect_compat("UTF-16 Surrogate U+DC00 (0xedb080) is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xed\xb0\x80\x00", 0x00)); // NULL 0x00 breaks
    flut_expect_compat("UTF-16 Surrogate U+DF80 (0xedbe80) is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xed\xbe\x80\x00", 0x00)); // NULL 0x00 breaks
    flut_expect_compat("UTF-16 Surrogate U+DFFF (0xedbfbf) is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xed\xbf\xbf\x00", 0x00)); // NULL 0x00 breaks

    // Boundaries
    flut_expect_compat("High surrogate U+D84C is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xED\xA1\x8C\x00", 0x00)); // NULL 0x00 breaks
    uint32_t d84c = 0xD84C;
    flut_expect_compat("High surrogate U+D84C is not valid UTF-32", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF32, (FlByte*)&d84c, 0x00)); // NULL 0x00 breaks
    flut_expect_compat("High surrogate boundary U+D7FF is valid UTF-8", fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xed\x9f\xbf\x00", 0x00)); // NULL 0x00 breaks
    flut_expect_compat("Max boundary U+10FFFF is valid UTF-8", fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xf4\x8f\xbf\xbf\x00", 0x00)); // NULL 0x00 breaks
    flut_expect_compat("Max boundary+1 U+110000 is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xf4\x90\x80\x80\x00", 0x00)); // NULL 0x00 breaks

    flut_expect_compat("U+0000 is valid UTF-8 (0x0000)", fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\x00\x00\x00", 0x00)); // NULL 0x00 breaks
    flut_expect_compat("U+0080 is valid UTF-8 (0xC280)", fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xC2\x80\x00", 0x00)); // NULL 0x00 breaks
    flut_expect_compat("U+0800 is valid UTF-8 (0xe0a080)", fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xe0\xa0\x80\x00", 0x00)); // NULL 0x00 breaks
    flut_expect_compat("U+10000 is valid UTF-8 (0xf0908080)", fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xf0\x90\x80\x80\x00", 0x00)); // NULL 0x00 breaks

    flut_expect_compat("U+007F is valid UTF-8 (0x007F)", fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\x00\x7F\x00", 0x00)); // NULL 0x00 breaks
    flut_expect_compat("U+07FF is valid UTF-8 (0xdfbf)", fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xdf\xbf\x00", 0x00)); // NULL 0x00 breaks
    flut_expect_compat("U+FFFF is valid UTF-8 (0xefbfbf)", fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xef\xbf\xbf\x00", 0x00)); // NULL 0x00 breaks
    
    flut_expect_compat("U+0E00 is valid UTF-8 (0xe0b880)", fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xe0\xb8\x80\x00", 0x00)); // NULL 0x00 breaks


    // Unexpected continuation bytes
    flut_expect_compat("Continuation byte 0x80 is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\x80\x00", 0x00)); // NULL 0x00 breaks
    flut_expect_compat("Continuation byte 0xbf is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xbf\x00", 0x00)); // NULL 0x00 breaks

    // 2-bytes sequence start bytes
    flut_expect_compat("0xC0 is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xC0\x00", 0x00)); // NULL 0x00 breaks
    flut_expect_compat("0xC5 is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xC5\x00", 0x00)); // NULL 0x00 breaks
    flut_expect_compat("0xCA is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xCA\x00", 0x00)); // NULL 0x00 breaks
    flut_expect_compat("0xCF is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xCF\x00", 0x00)); // NULL 0x00 breaks
    flut_expect_compat("0xD0 is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xD0\x00", 0x00)); // NULL 0x00 breaks
    flut_expect_compat("0xD5 is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xD5\x00", 0x00)); // NULL 0x00 breaks
    flut_expect_compat("0xDA is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xDA\x00", 0x00)); // NULL 0x00 breaks
    flut_expect_compat("0xDF is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xDF\x00", 0x00)); // NULL 0x00 breaks

    // 3-bytes sequence start bytes
    flut_expect_compat("0xE0 is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xE0\x00", 0x00)); // NULL 0x00 breaks
    flut_expect_compat("0xE5 is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xE5\x00", 0x00)); // NULL 0x00 breaks
    flut_expect_compat("0xEA is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xEA\x00", 0x00)); // NULL 0x00 breaks
    flut_expect_compat("0xEF is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xEF\x00", 0x00)); // NULL 0x00 breaks

    // 4-bytes sequence start bytes
    flut_expect_compat("0xF0 is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xF0\x00", 0x00)); // NULL 0x00 breaks
    flut_expect_compat("0xF5 is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xF5\x00", 0x00)); // NULL 0x00 breaks
    flut_expect_compat("0xFA is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xFA\x00", 0x00)); // NULL 0x00 breaks
    flut_expect_compat("0xFE is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xFE\x00", 0x00)); // NULL 0x00 breaks
    flut_expect_compat("0xFF is not valid UTF-8", !fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xFF\x00", 0x00)); // NULL 0x00 breaks

    // Sequences
    
    // Last byte missing
    flut_expect_compat("Sequence xC0 is not valid UTF-8", !fl_unicode_codepoint_sequence_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xC0\x00", NULL)); // x00 == NULL terminated
    flut_expect_compat("Sequence xDF is not valid UTF-8", !fl_unicode_codepoint_sequence_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xDF\x00", NULL)); // x00 == NULL terminated
    flut_expect_compat("Sequence xE0x80 is not valid UTF-8", !fl_unicode_codepoint_sequence_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xE0\x80\x00", NULL)); // x00 == NULL terminated
    flut_expect_compat("Sequence xEFxBF is not valid UTF-8", !fl_unicode_codepoint_sequence_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xEF\xBF\x00", NULL)); // x00 == NULL terminated
    flut_expect_compat("Sequence xF0x80x80 is not valid UTF-8", !fl_unicode_codepoint_sequence_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xF0\x80\x80\x00", NULL)); // x00 == NULL terminated
    flut_expect_compat("Sequence xF7xBFxBF is not valid UTF-8", !fl_unicode_codepoint_sequence_is_valid(FL_ENCODING_UTF8, (FlByte*)"\xF7\xBF\xBF\x00", NULL)); // x00 == NULL terminated

    // Surrogates
    flut_expect_compat("Surrogates sequence U+D800 U+DC00 is not valid UTF-8", !fl_unicode_codepoint_sequence_is_valid(FL_ENCODING_UTF8, (FlByte*) "\xed\xa0\x80\xed\xb0\x80\x00", NULL)); // x00 == NULL terminated
    flut_expect_compat("Surrogates sequence U+D800 U+DFFF is not valid UTF-8", !fl_unicode_codepoint_sequence_is_valid(FL_ENCODING_UTF8, (FlByte*) "\xed\xa0\x80\xed\xbf\xbf\x00", NULL)); // x00 == NULL terminated
    flut_expect_compat("Surrogates sequence U+DB7F U+DC00 is not valid UTF-8", !fl_unicode_codepoint_sequence_is_valid(FL_ENCODING_UTF8, (FlByte*) "\xed\xad\xbf\xed\xb0\x80\x00", NULL)); // x00 == NULL terminated
    flut_expect_compat("Surrogates sequence U+DB7F U+DFFF is not valid UTF-8", !fl_unicode_codepoint_sequence_is_valid(FL_ENCODING_UTF8, (FlByte*) "\xed\xad\xbf\xed\xbf\xbf\x00", NULL)); // x00 == NULL terminated
    flut_expect_compat("Surrogates sequence U+DB80 U+DC00 is not valid UTF-8", !fl_unicode_codepoint_sequence_is_valid(FL_ENCODING_UTF8, (FlByte*) "\xed\xae\x80\xed\xb0\x80\x00", NULL)); // x00 == NULL terminated
    flut_expect_compat("Surrogates sequence U+DB80 U+DFFF is not valid UTF-8", !fl_unicode_codepoint_sequence_is_valid(FL_ENCODING_UTF8, (FlByte*) "\xed\xae\x80\xed\xbf\xbf\x00", NULL)); // x00 == NULL terminated
    flut_expect_compat("Surrogates sequence U+DBFF U+DC00 is not valid UTF-8", !fl_unicode_codepoint_sequence_is_valid(FL_ENCODING_UTF8, (FlByte*) "\xed\xaf\xbf\xed\xb0\x80\x00", NULL)); // x00 == NULL terminated
    flut_expect_compat("Surrogates sequence U+DBFF U+DFFF is not valid UTF-8", !fl_unicode_codepoint_sequence_is_valid(FL_ENCODING_UTF8, (FlByte*) "\xed\xaf\xbf\xed\xbf\xbf\x00", NULL)); // x00 == NULL terminated
}

void test_fl_unicode_unichar_sequence_validate()
{
    /*#define seq_are_eq(a, b, l) (memcmp((a), (b), sizeof(uint32_t)*(l)) == 0)
    #define REPLCHAR 0xEFBFBD
    
    //
    uint32_t *validated = fl_unicode_unichar_sequence_validate(FL_ENCODING_UTF8, UCHARSEQ(0x31, 0x32, 0xC0, 0x34, 0x00), NULL); // 12�4
    flut_expect_compat("UTF8 sequence 0x31 0x32 0xC0 0x34 validates as 0x31 0x32 0xFFFD 0x34", seq_are_eq(validated, UCHARSEQ(0x31, 0x32, REPLCHAR, 0x34), 4));
    fl_free(validated);

    //
    validated = fl_unicode_unichar_sequence_validate(FL_ENCODING_UTF8, UCHARSEQ(0xC2, 0x41, 0x42, 0x00), NULL);
    flut_expect_compat("UTF8 sequence 0xC2 0x41 0x42 validates as 0xFFFD 0x41 0x42", seq_are_eq(validated, UCHARSEQ(REPLCHAR, 0x41, 0x42), 3));
    fl_free(validated);

    //
    validated = fl_unicode_unichar_sequence_validate(FL_ENCODING_UTF32, UCHARSEQ(0x31, 0x32, 0xD800, 0x34, 0x00), NULL); // 12�4
    flut_expect_compat("UTF32 sequence 0x31 0x32 0xD800 0x34 validates as 0x31 0x32 0xFFFD 0x34", seq_are_eq(validated, UCHARSEQ(0x31, 0x32, FL_UNICODE_REPLACEMENT_CHARACTER, 0x34), 4));
    fl_free(validated);*/
}

void test_fl_unicode_codepoint_convert()
{
    FlByte dst[4] = {0,0,0,0};
    size_t bytes = 0;

    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x00\x00\x41\x00", NULL, FL_ENCODING_UTF32, dst);
    flut_expect_compat("UTF-32 U+0041 encoded to UTF-32 should return the same sequence of bytes", bytes == 4 && fl_equals(dst, "\x00\x00\x00\x41\x00", bytes));
    
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"\x41\x00", NULL, FL_ENCODING_UTF8, dst);
    flut_expect_compat("UTF-8 0x41 encoded to UTF-8 should return the same sequence of bytes", bytes == 1 && fl_equals(dst, "\x41", bytes));

    //
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x00\x00\x41\x00", NULL, FL_ENCODING_UTF8, dst);
    flut_expect_compat("UTF-32 U+0041 encoded to UTF-8 is equals to 0x41 and occupies 1 byte", bytes == 1 && fl_equals(dst, "\x41", bytes));

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"\x41\x00", NULL, FL_ENCODING_UTF32, dst);
    flut_expect_compat("UTF-8 0x41 encoded to UTF-32 is equals to 0x00000041 and occupies 4 bytes", bytes == 4 && fl_equals(dst, "\x00\x00\x00\x41", bytes));

    //
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x00\x01\x00", NULL, FL_ENCODING_UTF8, dst);
    flut_expect_compat("UTF-32 U+0100 encoded to UTF-8 is equals to 0xC480 and occupies 2 bytes", bytes == 2 && fl_equals(dst, "Ā\x00", bytes));

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"Ā\x00", NULL, FL_ENCODING_UTF32, dst);
    flut_expect_compat("UTF-8 Ā encoded to UTF-32 is equals to 0x00000100 and occupies 4 bytes", bytes == 4 && fl_equals(dst, "\x00\x00\x01\x00", bytes));

    //
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x00\x00\x5A", NULL, FL_ENCODING_UTF8, dst);
    flut_expect_compat("UTF-32 U+005A encoded to UTF-8 is equals to 0x005A and occupies 1 byte", bytes == 1 && fl_equals(dst, "\x5A\x00", bytes));

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"Z\x00", NULL, FL_ENCODING_UTF32, dst);
    flut_expect_compat("UTF-8 Z encoded to UTF-32 is equals to 0x0000005A and occupies 4 bytes", bytes == 4 && fl_equals(dst, "\x00\x00\x00\x5A", bytes));

    //
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x00\x03\xA0", NULL, FL_ENCODING_UTF8, dst);
    flut_expect_compat("UTF-32 U+03A0 encoded to UTF-8 is equals to 0xCEA0 and occupies 2 bytes", bytes == 2 && fl_equals(dst, "\xCE\xA0", bytes));

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"\xCE\xA0\x00", NULL, FL_ENCODING_UTF32, dst);
    flut_expect_compat("UTF-8 0xCEA000 encoded to UTF-32 is equals to 0x000003A0 and occupies 4 bytes", bytes == 4 && fl_equals(dst, "\x00\x00\x03\xA0", bytes));

    //
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x00\xFF\x21", NULL, FL_ENCODING_UTF8, dst);
    flut_expect_compat("UTF-32 U+FF21 encoded to UTF-8 is equals to 0xEFBCA1 and occupies 3 bytes", bytes == 3 && fl_equals(dst, "\xEF\xBC\xA1", bytes));

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"\xEF\xBC\xA1\x00", NULL, FL_ENCODING_UTF32, dst);
    flut_expect_compat("UTF-8 0xEFBCA1 encoded to UTF-32 is equals to 0x0000FF21 and occupies 4 bytes", bytes == 4 && fl_equals(dst, "\x00\x00\xFF\x21", bytes));

    //
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x02\xF8\x0F\x00", NULL, FL_ENCODING_UTF8, dst);
    flut_expect_compat("UTF-32 U+2F80F encoded to UTF-8 is equals to 0xF0AFA08F and occupies 3 bytes", bytes == 4 && fl_equals(dst, "\xF0\xAF\xA0\x8F", bytes));

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"\xF0\xAF\xA0\x8F\x00", NULL, FL_ENCODING_UTF32, dst);
    flut_expect_compat("UTF-8 0xF0AFA08F encoded to UTF-32 is equals to 0x00002F80F and occupies 4 bytes", bytes == 4 && fl_equals(dst, "\x00\x02\xF8\x0F", bytes));

    //
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x00\xD8\x00\x00", NULL, FL_ENCODING_UTF8, dst);
    flut_expect_compat("UTF-32 U+D800 encoded to UTF-8 is invalid because it is a High Surrogate", bytes == FL_UNICODE_INVALID_SIZE);

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"\xED\xA0\x80", NULL, FL_ENCODING_UTF32, dst);
    flut_expect_compat("UTF-8 0xEDA080 encoded to UTF-32 is invalid because it is a High Surrogate", bytes == FL_UNICODE_INVALID_SIZE);

    //
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x00\xD8\x00\x00", NULL, FL_ENCODING_UTF8, dst);
    flut_expect_compat("UTF-32 U+D800 encoded to UTF-8 is invalid because it is the first High Surrogate", bytes == FL_UNICODE_INVALID_SIZE);

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"\xED\xA0\x80", NULL, FL_ENCODING_UTF32, dst);
    flut_expect_compat("UTF-8 0xEDA080 encoded to UTF-32 is invalid because it is the first High Surrogate U+D800", bytes == FL_UNICODE_INVALID_SIZE);

    //
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x00\xDF\xFF\x00", NULL, FL_ENCODING_UTF8, dst);
    flut_expect_compat("UTF-32 U+DFFF encoded to UTF-8 is invalid because it is the last Low Surrogate", bytes == FL_UNICODE_INVALID_SIZE);

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"\xED\xBF\xBF", NULL, FL_ENCODING_UTF32, dst);
    flut_expect_compat("UTF-8 0xEDBFBF encoded to UTF-32 is invalid because it is the last Low Surrogate U+DFFF", bytes == FL_UNICODE_INVALID_SIZE);

    // UTF8 1-byte
    memset(dst, 0, 4);
    const FlByte *str = (FlByte*)"\x00\x00\x00\x00";
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)str, str+4, FL_ENCODING_UTF8, dst);
    flut_expect_compat("UTF-32 U+0000 encoded to UTF-8 is equals to 0x00 and occupies 1 byte", bytes == 1 && fl_equals(dst, "\x00", bytes));

    memset(dst, 0, 4);
    str = (FlByte*)"\x00";
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)str, str+1, FL_ENCODING_UTF32, dst);
    flut_expect_compat("UTF-8 0x00 encoded to UTF-32 is equals to 0x00000000 and occupies 4 bytes", bytes == 4 && fl_equals(dst, "\x00\x00\x00\x00", bytes));

    //
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x00\x00\x48\x00", NULL, FL_ENCODING_UTF8, dst);
    flut_expect_compat("UTF-32 U+0048 encoded to UTF-8 is equals to 0x48 and occupies 1 byte", bytes == 1 && fl_equals(dst, "H\x00", bytes));

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"H\x00", NULL, FL_ENCODING_UTF32, dst);
    flut_expect_compat("UTF-8 H encoded to UTF-32 is equals to 0x00000048 and occupies 4 bytes", bytes == 4 && fl_equals(dst, "\x00\x00\x00\x48", bytes));

    //
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x00\x00\x7F\x00", NULL, FL_ENCODING_UTF8, dst);
    flut_expect_compat("UTF-32 U+007F encoded to UTF-8 is equals to 0x7F and occupies 1 byte", bytes == 1 && fl_equals(dst, "\x7F\x00", bytes));

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"\x7F\x00", NULL, FL_ENCODING_UTF32, dst);
    flut_expect_compat("UTF-8 0x7F encoded to UTF-32 is equals to 0x0000007F and occupies 4 bytes", bytes == 4 && fl_equals(dst, "\x00\x00\x00\x7F", bytes));

    // UTF8 2-bytes
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x00\x00\x80\x00", NULL, FL_ENCODING_UTF8, dst);
    flut_expect_compat("UTF-32 U+0080 encoded to UTF-8 is equals to 0xC280 and occupies 2 bytes", bytes == 2 && fl_equals(dst, "\xC2\x80", bytes));

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"\xC2\x80\x00", NULL, FL_ENCODING_UTF32, dst);
    flut_expect_compat("UTF-8 0xC280 encoded to UTF-32 is equals to 0x00000080 and occupies 4 bytes", bytes == 4 && fl_equals(dst, "\x00\x00\x00\x80", bytes));

    //
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x00\x01\x81\x00", NULL, FL_ENCODING_UTF8, dst);
    flut_expect_compat("UTF-32 U+0181 encoded to UTF-8 is equals to 0xC681 and occupies 2 bytes", bytes == 2 && fl_equals(dst, "\xC6\x81", bytes));

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"\xC6\x81\x00", NULL, FL_ENCODING_UTF32, dst);
    flut_expect_compat("UTF-8 0xC681 encoded to UTF-32 is equals to 0x00000181 and occupies 4 bytes", bytes == 4 && fl_equals(dst, "\x00\x00\x01\x81", bytes));

    //
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x00\x07\xFF\x00", NULL, FL_ENCODING_UTF8, dst);
    flut_expect_compat("UTF-32 U+07FF encoded to UTF-8 is equals to 0xDFBF and occupies 2 bytes", bytes == 2 && fl_equals(dst, "\xDF\xBF", bytes));

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"\xDF\xBF\x00", NULL, FL_ENCODING_UTF32, dst);
    flut_expect_compat("UTF-8 0xDFBF encoded to UTF-32 is equals to 0x000007FF and occupies 4 bytes", bytes == 4 && fl_equals(dst, "\x00\x00\x07\xFF", bytes));

    // UTF8 3-bytes
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x00\x08\x00\x00", NULL, FL_ENCODING_UTF8, dst);
    flut_expect_compat("UTF-32 U+0800 encoded to UTF-8 is equals to 0xE0A080 and occupies 3 bytes", bytes == 3 && fl_equals(dst, "\xE0\xA0\x80", bytes));

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"\xE0\xA0\x80\x00", NULL, FL_ENCODING_UTF32, dst);
    flut_expect_compat("UTF-8 0xE0A080 encoded to UTF-32 is equals to 0x00000800 and occupies 4 bytes", bytes == 4 && fl_equals(dst, "\x00\x00\x08\x00", bytes));

    //
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x00\xA8\xF9\x00", NULL, FL_ENCODING_UTF8, dst);
    flut_expect_compat("UTF-32 U+A8F9 encoded to UTF-8 is equals to 0xEAA3B9 and occupies 3 bytes", bytes == 3 && fl_equals(dst, "\xEA\xA3\xB9", bytes));

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"\xEA\xA3\xB9\x00", NULL, FL_ENCODING_UTF32, dst);
    flut_expect_compat("UTF-8 0xEAA3B9 encoded to UTF-32 is equals to 0x0000A8F9 and occupies 4 bytes", bytes == 4 && fl_equals(dst, "\x00\x00\xA8\xF9", bytes));

    //
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x00\xFF\xFF\x00", NULL, FL_ENCODING_UTF8, dst);
    flut_expect_compat("UTF-32 U+FFFF encoded to UTF-8 is equals to 0xEFBFBF and occupies 3 bytes", bytes == 3 && fl_equals(dst, "\xEF\xBF\xBF", bytes));

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"\xEF\xBF\xBF\x00", NULL, FL_ENCODING_UTF32, dst);
    flut_expect_compat("UTF-8 0xEFBFBF encoded to UTF-32 is equals to 0x0000FFFF and occupies 4 bytes", bytes == 4 && fl_equals(dst, "\x00\x00\xFF\xFF", bytes));

    // UTF8 4-bytes
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x01\x00\x00\x00", NULL, FL_ENCODING_UTF8, dst);
    flut_expect_compat("UTF-32 U+10000 encoded to UTF-8 is equals to 0xF0908080 and occupies 4 bytes", bytes == 4 && fl_equals(dst, "\xF0\x90\x80\x80", bytes));

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"\xF0\x90\x80\x80\x00", NULL, FL_ENCODING_UTF32, dst);
    flut_expect_compat("UTF-8 0xF0908080 encoded to UTF-32 is equals to 0x00010000 and occupies 4 bytes", bytes == 4 && fl_equals(dst, "\x00\x01\x00\x00", bytes));

    //
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x01\xF6\x15\x00", NULL, FL_ENCODING_UTF8, dst);
    flut_expect_compat("UTF-32 U+1F615 encoded to UTF-8 is equals to 0xF09F9895 and occupies 4 bytes", bytes == 4 && fl_equals(dst, "\xF0\x9F\x98\x95", bytes));

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"\xF0\x9F\x98\x95\x00", NULL, FL_ENCODING_UTF32, dst);
    flut_expect_compat("UTF-8 0xF09F9895 encoded to UTF-32 is equals to 0x0001F615 and occupies 4 bytes", bytes == 4 && fl_equals(dst, "\x00\x01\xF6\x15", bytes));

    //
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x10\xFF\xFF\x00", NULL, FL_ENCODING_UTF8, dst);
    flut_expect_compat("UTF-32 U+10FFFF encoded to UTF-8 is equals to 0xF48FBFBF and occupies 4 bytes", bytes == 4 && fl_equals(dst, "\xF4\x8F\xBF\xBF", bytes));

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"\xF4\x8F\xBF\xBF\x00", NULL, FL_ENCODING_UTF32, dst);
    flut_expect_compat("UTF-8 0xF48FBFBF encoded to UTF-32 is equals to 0x0010FFFF and occupies 4 bytes", bytes == 4 && fl_equals(dst, "\x00\x10\xFF\xFF", bytes));

    //
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x11\x00\x00\x00", NULL, FL_ENCODING_UTF8, dst);
    flut_expect_compat("UTF-32 U+110000 encoded to UTF-8 is invalid because it is beyond the upper bound", bytes == FL_UNICODE_INVALID_SIZE);

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"\xF4\x8F\xBF\xC0\x00", NULL, FL_ENCODING_UTF32, dst);
    flut_expect_compat("UTF-8 0xF48FBFC0 encoded to UTF-32 is invalid because it is beyond the upper bound", bytes == FL_UNICODE_INVALID_SIZE);

    //
    FlByte *chr = (FlByte*)"你\x00";
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, chr, NULL, FL_ENCODING_UTF32, dst);
    flut_expect_compat("Char 你 is 4 bytes in UTF-32", bytes == 4);
    flut_expect_compat("Char 你 is encoded as 0x2F804 in UTF-32", fl_equals(dst, "\x00\x02\xF8\x04", bytes));
    
    FlByte dst2[4] = {0,0,0,0};
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, dst, NULL, FL_ENCODING_UTF8, dst2);
    flut_expect_compat("Char 你 is 4 bytes in UTF-8", bytes == 4);
    flut_expect_compat("Char 你 is encoded as 0xF0AFA084 in UTF-8", fl_equals(dst2, "\xF0\xAF\xA0\x84", bytes));
}

void test_fl_unicode_codepoint_sequence_validate()
{
    FlByte *str = NULL;
    FlByte *validstr = NULL;
    size_t size = 0;


    /// UTF-32
    // Empty sequence
    str = (FlByte*)"";
    size = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF32, str, str, &validstr);
    flut_expect_compat("UTF-32: Empty sequence has size == 0", size == 0);
    free(validstr);

    // 1-Byte length sequence
    str = (FlByte*)"\x00\x00\x00\x01";
    size = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF32, str, str+4, &validstr);
    flut_expect_compat("UTF-32: Validated Code Point Sequence {U+0001} has size 4", size == 4);
    flut_expect_compat("UTF-32: Validated Code Point Sequence {U+0001} is equals to {U+0001}", fl_equals(str, validstr, 4));
    free(validstr);

    // Sequence larger than 1 byte
    str = (FlByte*)"\x00\x00\x00\x01\x00\x00\x00\x02\x00\x00\x00\x03\x00\x00\x00\x04";
    size = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF32, str, str+4*4, &validstr);
    flut_expect_compat("UTF-32: Validated Code Point Sequence {U+0001,U+0002,U+0003,U+0004} has size 16", size == 16);
    flut_expect_compat("UTF-32: Validated Code Point Sequence {U+0001,U+0002,U+0003,U+0004} is equals to {U+0001,U+0002,U+0003,U+0004}", fl_equals(str, validstr, 4*4));
    free(validstr);

    // One ill-formed code point in the middle of the sequence
    str = (FlByte*)"\x00\x00\x00\x01\xFF\xFF\xFF\xFF\x00\x00\x00\x03\x00\x00\x00\x04";
    size = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF32, str, str+4*4, &validstr);
    flut_expect_compat("UTF-32: Validated Code Point Sequence {U+0001,U+FFFFFFFF,U+0003,U+0004} has size 16", size == 16);
    flut_expect_compat("UTF-32: Validated Code Point Sequence {U+0001,U+FFFFFFFF,U+0003,U+0004} is equals to {U+0001,U+FFFD,U+0003,U+0004}", fl_equals((const FlByte*)"\x00\x00\x00\x01\x00\x00\xFF\xFD\x00\x00\x00\x03\x00\x00\x00\x04", validstr, 4*4));
    free(validstr);

    // Two ill-formed code point in the middle of the sequence
    str = (FlByte*)"\x00\x00\x00\x01\xFF\xFF\xFF\xFF\x00\x00\x00\x03\xFF\x10\xFF\xEE\x00\x00\x00\x05";
    size = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF32, str, str+4*5, &validstr);
    flut_expect_compat("UTF-32: Validated Code Point Sequence {U+0001,U+FFFFFFFF,U+0003,U+FF10FFEE,U+0005} has size 20}", size == 20);
    flut_expect_compat("UTF-32: Validated Code Point Sequence {U+0001,U+FFFFFFFF,U+0003,U+FF10FFEE,U+0005} is equals to {U+0001,U+FFFD,U+0003,U+FFFD,U+0005}", fl_equals((const FlByte*)"\x00\x00\x00\x01\x00\x00\xFF\xFD\x00\x00\x00\x03\x00\x00\xFF\xFD\x00\x00\x00\x05", validstr, 4*5));
    free(validstr);

    // Two ill-formed code point, one in the middle of the sequence another in the upper bound
    str = (FlByte*)"\x00\x00\x00\x01\xFF\xFF\xFF\xFF\x00\x00\x00\x03\xFF\x10\xFF\xEE";
    size = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF32, str, str+4*4, &validstr);
    flut_expect_compat("UTF-32: Validated Code Point Sequence {U+0001,U+FFFFFFFF,U+0003,U+FF10FFEE} has size 16", size == 16);
    flut_expect_compat("UTF-32: Validated Code Point Sequence {U+0001,U+FFFFFFFF,U+0003,U+FF10FFEE} is equals to {U+0001,U+FFFD,U+0003,U+FFFD}", fl_equals((const FlByte*)"\x00\x00\x00\x01\x00\x00\xFF\xFD\x00\x00\x00\x03\x00\x00\xFF\xFD", validstr, 4*4));
    free(validstr);

    // Two ill-formed code point, one in the lower bound of the sequence another in the middle
    str = (FlByte*)"\xFF\x10\xFF\xEE\x00\x00\x00\x02\x00\x00\x00\x03\xFF\xFF\xFF\xFF\x00\x00\x00\x05";
    size = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF32, str, str+4*5, &validstr);
    flut_expect_compat("UTF-32: Validated Code Point Sequence {U+FF10FFEE,U+0002,U+0003,U+FFFFFFFF,U+0005} has size 20", size == 20);
    flut_expect_compat("UTF-32: Validated Code Point Sequence {U+FF10FFEE,U+0002,U+0003,U+FFFFFFFF,U+0005} is equals to {U+FFFD,U+0002,U+0003,U+FFFD,U+0005}", fl_equals((const FlByte*)"\x00\x00\xFF\xFD\x00\x00\x00\x02\x00\x00\x00\x03\x00\x00\xFF\xFD\x00\x00\x00\x05", validstr, 4*5));
    free(validstr);

    // Two ill-formed code point at both bounds
    str = (FlByte*)"\xFF\x10\xFF\xEE\x00\x00\x00\x02\x00\x00\x00\x03\xFF\xFF\xFF\xFF";
    size = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF32, str, str+4*4, &validstr);
    flut_expect_compat("UTF-32: Validated Code Point Sequence {U+FF10FFEE,U+0002,U+0003,U+FFFFFFFF} has size 16", size == 16);
    flut_expect_compat("UTF-32: Validated Code Point Sequence {U+FF10FFEE,U+0002,U+0003,U+FFFFFFFF} is equals to {U+FFFD,U+0002,U+0003,U+FFFD}", fl_equals((const FlByte*)"\x00\x00\xFF\xFD\x00\x00\x00\x02\x00\x00\x00\x03\x00\x00\xFF\xFD", validstr, 4*4));
    free(validstr);

    // Interleaved ill-formed and well-formed code points
    str = (FlByte*)"\xFF\x10\xFF\xEE\x00\x00\x00\x02\xFF\xFF\xFF\xFF\x00\x00\x00\x04\xFF\xFF\xFF\xDD\x00\x00\x00\x06";
    size = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF32, str, str+4*6, &validstr);
    flut_expect_compat("UTF-32: Validated Code Point Sequence {U+FF10FFEE,U+0002,U+FFFFFFFF,U+0004,U+FFFFFFDD,U+0006} has size 24", size == 24);
    flut_expect_compat("UTF-32: Validated Code Point Sequence {U+FF10FFEE,U+0002,U+FFFFFFFF,U+0004,U+FFFFFFDD,U+0006} is equals to {U+FFFD,U+0002,U+FFFD,U+0004,U+FFFD,U+0006}", fl_equals((const FlByte*)"\x00\x00\xFF\xFD\x00\x00\x00\x02\x00\x00\xFF\xFD\x00\x00\x00\x04\x00\x00\xFF\xFD\x00\x00\x00\x06", validstr, 4*6));
    free(validstr);

    // Full ill-formed sequence
    str = (FlByte*)"\xFF\x10\xFF\xEE\xFF\xEF\xBC\xED\xFF\xEA\xAF\xA3\xFF\xFF\xFF\xFF\xF9\xFF\xEF\xFF";
    size = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF32, str, str+4*5, &validstr);
    flut_expect_compat("UTF-32: Validated Code Point Sequence {U+FF10FFEE,U+FFEFBCED,U+FFEAAFA3,U+FFFFFFFF,U+F9FFEFFF} has size 20", size == 20);
    flut_expect_compat("UTF-32: Validated Code Point Sequence {U+FF10FFEE,U+FFEFBCED,U+FFEAAFA3,U+FFFFFFFF,U+F9FFEFFF} is equals to {U+FFFD,U+FFFD,U+FFFD,U+FFFD,U+FFFD}", fl_equals((const FlByte*)"\x00\x00\xFF\xFD\x00\x00\xFF\xFD\x00\x00\xFF\xFD\x00\x00\xFF\xFD\x00\x00\xFF\xFD", validstr, 4*5));
    free(validstr);

    /// UTF-8
    // Empty sequence
    str = (FlByte*)"";
    size = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF8, str, str, &validstr);
    flut_expect_compat("UTF-8: Empty sequence has size == 0", size == 0);
    free(validstr);

    str = (FlByte*)"\x61\x62\x63\x64\x65\x66";
    size = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF8, str, str+6, &validstr);
    flut_expect_compat("UTF-8: Validated form of <61 62 63 64 65 66> has 6 bytes", size == 6);
    flut_expect_compat("UTF-8: Validated form of <61 62 63 64 65 66> has 6 code points", fl_unicode_codepoint_sequence_length(FL_ENCODING_UTF8, validstr, validstr+size) == 6);
    flut_expect_compat("UTF-8: Validated form of <61 62 63 64 65 66> is the same sequence", fl_equals((const FlByte*)str, validstr, 6));
    free(validstr);

    str = (FlByte*)"\x61\x62\x63\x80\x64\x65\x66";
    size = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF8, str, str+7, &validstr);
    flut_expect_compat("UTF-8: Validated form of <61 62 63 80 64 65 66> has 9 bytes", size == 9);
    flut_expect_compat("UTF-8: Validated form of <61 62 63 80 64 65 66> has 7 code points", fl_unicode_codepoint_sequence_length(FL_ENCODING_UTF8, validstr, validstr+size) == 7);
    flut_expect_compat("UTF-8: Validated form of <61 62 63 80 64 65 66> is <61 62 63 EF BF BD 64 65 66>", fl_equals((const FlByte*)"\x61\x62\x63\xEF\xBF\xBD\x64\x65\x66", validstr, 9));
    free(validstr);

    str = (FlByte*)"\x61\xF1\x80\x80\xE1\x80\xC2\x62\x80\x63\x80\xBF\x64";
    size = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF8, str, str+13, &validstr);
    flut_expect_compat("UTF-8: Validated form of <61 F1 80 80 E1 80 C2 62 80 63 80 BF 64> has 22 bytes", size == 22);
    flut_expect_compat("UTF-8: Validated form of <61 F1 80 80 E1 80 C2 62 80 63 80 BF 64> has 10 code points", fl_unicode_codepoint_sequence_length(FL_ENCODING_UTF8, validstr, validstr+size) == 10);
    flut_expect_compat("UTF-8: Validated form of <61 F1 80 80 E1 80 C2 62 80 63 80 BF 64> is <61 EF BF BD EF BF BD EF BF BD 62 EF BF BD 63 EF BF BD EF BF BD 64>", fl_equals((const FlByte*)"\x61\xEF\xBF\xBD\xEF\xBF\xBD\xEF\xBF\xBD\x62\xEF\xBF\xBD\x63\xEF\xBF\xBD\xEF\xBF\xBD\x64", validstr, 22));
    free(validstr);

    str = (FlByte*)"\x41\xC0\xAF\x41\xF4\x80\x80\x41";
    size = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF8, str, str+8, &validstr);
    flut_expect_compat("UTF-8: Validated form of <41 C0 AF 41 F4 80 80 41> has 12 bytes", size == 12);
    flut_expect_compat("UTF-8: Validated form of <41 C0 AF 41 F4 80 80 41> has 6 code points", fl_unicode_codepoint_sequence_length(FL_ENCODING_UTF8, validstr, validstr+size) == 6);
    flut_expect_compat("UTF-8: Validated form of <41 C0 AF 41 F4 80 80 41> is <41 EF BF BD EF BF BD 41 EF BF BD 41>", fl_equals((const FlByte*)"\x41\xEF\xBF\xBD\xEF\xBF\xBD\x41\xEF\xBF\xBD\x41", validstr, 12));
    free(validstr);

    str = (FlByte*)"\x41\xE0\x9F\x80\x41";
    size = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF8, str, str+5, &validstr);
    flut_expect_compat("UTF-8: Validated form of <41 E0 9F 80 41> has 11 bytes", size == 11);
    flut_expect_compat("UTF-8: Validated form of <41 E0 9F 80 41> has 5 code points", fl_unicode_codepoint_sequence_length(FL_ENCODING_UTF8, validstr, validstr+size) == 5);
    flut_expect_compat("UTF-8: Validated form of <41 E0 9F 80 41> is <41 EF BF BD EF BF BD EF BF BD 41>", fl_equals((const FlByte*)"\x41\xEF\xBF\xBD\xEF\xBF\xBD\xEF\xBF\xBD\x41", validstr, 11));
    free(validstr);   

    //TODO: Test edge cases like full ill-formed, starts with ill-formed code point or end with it

    // Starts with an ill-formed sequence
    str = (FlByte*)"\xF1\x80\x80\x41\x42\x43\x44\x00";
    size = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF8, str, str+8, &validstr);
    flut_expect_compat("UTF-8: Validated form of <F1 80 80 41 42 43 44 00> has 8 bytes", size == 8);
    flut_expect_compat("UTF-8: Validated form of <F1 80 80 41 42 43 44 00> has 6 code points", fl_unicode_codepoint_sequence_length(FL_ENCODING_UTF8, validstr, validstr+size) == 6);
    flut_expect_compat("UTF-8: Validated form of <F1 80 80 41 42 43 44 00> is <EF BF BD 41 42 43 44 00>", fl_equals((const FlByte*)"\xEF\xBF\xBD\x41\x42\x43\x44\x00", validstr, 8));
    free(validstr);

    // Ends with an ill-formed sequence
    str = (FlByte*)"\x41\x42\x43\x44\x00\xF1\x80\x80";
    size = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF8, str, str+8, &validstr);
    flut_expect_compat("UTF-8: Validated form of <41 42 43 44 00 F1 80 80> has 8 bytes", size == 8);
    flut_expect_compat("UTF-8: Validated form of <41 42 43 44 00 F1 80 80> has 6 code points", fl_unicode_codepoint_sequence_length(FL_ENCODING_UTF8, validstr, validstr+size) == 6);
    flut_expect_compat("UTF-8: Validated form of <41 42 43 44 00 F1 80 80> is <41 42 43 44 00 EF BF BD>", fl_equals((const FlByte*)"\x41\x42\x43\x44\x00\xEF\xBF\xBD", validstr, 8));
    free(validstr);

    // Ends with an ill-formed sequence
    str = (FlByte*)"\x41\x42\x43\x44\x00\xF1\x80";
    size = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF8, str, str+7, &validstr);
    flut_expect_compat("UTF-8: Validated form of <41 42 43 44 00 F1 80> has 8 bytes", size == 8);
    flut_expect_compat("UTF-8: Validated form of <41 42 43 44 00 F1 80> has 6 code points", fl_unicode_codepoint_sequence_length(FL_ENCODING_UTF8, validstr, validstr+size) == 6);
    flut_expect_compat("UTF-8: Validated form of <41 42 43 44 00 F1 80> is <41 42 43 44 00 EF BF BD>", fl_equals((const FlByte*)"\x41\x42\x43\x44\x00\xEF\xBF\xBD", validstr, 8));
    free(validstr);

    // Ends with an ill-formed sequence
    str = (FlByte*)"\x41\x42\x43\x44\x00\x80";
    size = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF8, str, str+6, &validstr);
    flut_expect_compat("UTF-8: Validated form of <41 42 43 44 00 80> has 8 bytes", size == 8);
    flut_expect_compat("UTF-8: Validated form of <41 42 43 44 00 80> has 6 code points", fl_unicode_codepoint_sequence_length(FL_ENCODING_UTF8, validstr, validstr+size) == 6);
    flut_expect_compat("UTF-8: Validated form of <41 42 43 44 00 80> is <41 42 43 44 00 EF BF BD>", fl_equals((const FlByte*)"\x41\x42\x43\x44\x00\xEF\xBF\xBD", validstr, 8));
    free(validstr);

    // Full ill-formed sequence
    str = (FlByte*)"\xE0\x9F\x80\xC0\xAF\xF1\x80\x80";
    size = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF8, str, str+8, &validstr);
    flut_expect_compat("UTF-8: Validated form of <E0 9F 80 C0 AF F1 80 80> has 18 bytes", size == 18);
    flut_expect_compat("UTF-8: Validated form of <E0 9F 80 C0 AF F1 80 80> has 6 code points", fl_unicode_codepoint_sequence_length(FL_ENCODING_UTF8, validstr, validstr+size) == 6);
    flut_expect_compat("UTF-8: Validated form of <E0 9F 80 C0 AF F1 80 80> is <EF BF BD EF BF BD EF BF BD EF BF BD EF BF BD EF BF BD>", fl_equals((const FlByte*)"\xEF\xBF\xBD\xEF\xBF\xBD\xEF\xBF\xBD\xEF\xBF\xBD\xEF\xBF\xBD\xEF\xBF\xBD", validstr, 18));
    free(validstr);   
}

void test_fl_unicode_data()
{
    uint32_t destination[50] = {0};
    size_t mappingsize = fl_unicode_get_decomposition_mapping(0x0005, true, destination);
    flut_expect_compat("Code point U+0005 <control> Canonical Decomposition_Mapping is its default value 0x0005", mappingsize == 1 && destination[0] == 0x0005);
    
    memset(destination, 0, sizeof(uint32_t) * 50);
    mappingsize = fl_unicode_get_decomposition_mapping(0x0005, false, destination);
    flut_expect_compat("Code point U+0005 <control> Compatibility Decomposition_Mapping is its default value 0x0005", mappingsize == 1 && destination[0] == 0x0005);

    //
    memset(destination, 0, sizeof(uint32_t) * 50);
    mappingsize = fl_unicode_get_decomposition_mapping(0x1DA07, true, destination);
    flut_expect_compat("Code point U+1DA07 SIGNWRITING FACE DIRECTION POSITION NOSE FORWARD TILTING Canonical Decomposition_Mapping is its default value 0x1DA07", mappingsize == 1 && destination[0] == 0x1DA07);

    memset(destination, 0, sizeof(uint32_t) * 50);
    mappingsize = fl_unicode_get_decomposition_mapping(0x1DA07, false, destination);
    flut_expect_compat("Code point U+1DA07 SIGNWRITING FACE DIRECTION POSITION NOSE FORWARD TILTING Compatibility Decomposition_Mapping is its default value 0x1DA07", mappingsize == 1 && destination[0] == 0x1DA07);

    //
    memset(destination, 0, sizeof(uint32_t) * 50);
    mappingsize = fl_unicode_get_decomposition_mapping(0x3060, true, destination);
    flut_expect_compat("Code point U+3060 HIRAGANA LETTER DA Canonical Decomposition_Mapping is {U+305F, U+3099}", mappingsize == 2 && fl_equals(destination, ((const uint32_t[]){0x305F, 0x3099}), mappingsize));

    memset(destination, 0, sizeof(uint32_t) * 50);
    mappingsize = fl_unicode_get_decomposition_mapping(0x3060, false, destination);
    flut_expect_compat("Code point U+3060 HIRAGANA LETTER DA Compatiblity Decomposition_Mapping is its default value 0x3060", mappingsize == 1 && destination[0] == 0x3060);

    //
    memset(destination, 0, sizeof(uint32_t) * 50);
    mappingsize = fl_unicode_get_decomposition_mapping(0x3200, false, destination);
    flut_expect_compat("Code point U+3200 PARENTHESIZED HANGUL KIYEOK Compatibility Decomposition_Mapping is {U+0028, U+1100, U+0029}", mappingsize == 3 && fl_equals(destination, ((const uint32_t[]){0x0028, 0x1100, 0x0029}), mappingsize));

    memset(destination, 0, sizeof(uint32_t) * 50);
    mappingsize = fl_unicode_get_decomposition_mapping(0x3200, true, destination);
    flut_expect_compat("Code point U+3200 PARENTHESIZED HANGUL KIYEOK Canonical Decomposition_Mapping is its default value 0x3200", mappingsize == 1 && destination[0] == 0x3200);

    //
    memset(destination, 0, sizeof(uint32_t) * 50);
    mappingsize = fl_unicode_get_decomposition_mapping(0x2057, false, destination);
    flut_expect_compat("Code point U+2057 QUADRUPLE PRIME Compatibility Decomposition_Mapping is {U+2032, U+2032, U+2032, U+2032}", mappingsize == 4 && fl_equals(destination, ((const uint32_t[]){0x2032, 0x2032, 0x2032, 0x2032}), mappingsize));

    memset(destination, 0, sizeof(uint32_t) * 50);
    mappingsize = fl_unicode_get_decomposition_mapping(0x2057, true, destination);
    flut_expect_compat("Code point U+2057 QUADRUPLE PRIME Canonical Decomposition_Mapping is its default value 0x2057", mappingsize == 1 && destination[0] == 0x2057);

    //
    memset(destination, 0, sizeof(uint32_t) * 50);
    mappingsize = fl_unicode_get_decomposition_mapping(0x321D, false, destination);
    flut_expect_compat("Code point U+321D PARENTHESIZED KOREAN CHARACTER OJEON Compatibility Decomposition_Mapping is {U+0028, U+110B, U+1169, U+110C, U+1165, U+11AB, U+0029}", mappingsize == 7 && fl_equals(destination, ((const uint32_t[]){0x0028, 0x110B, 0x1169, 0x110C, 0x1165, 0x11AB, 0x0029}), mappingsize));

    memset(destination, 0, sizeof(uint32_t) * 50);
    mappingsize = fl_unicode_get_decomposition_mapping(0x321D, true, destination);
    flut_expect_compat("Code point U+321D PARENTHESIZED KOREAN CHARACTER OJEON Canonical Decomposition_Mapping is its default value 0x321D", mappingsize == 1 && destination[0] == 0x321D);

    //
    memset(destination, 0, sizeof(uint32_t) * 50);
    mappingsize = fl_unicode_get_decomposition_mapping(0x321E, false, destination);
    flut_expect_compat("Code point U+321E PARENTHESIZED KOREAN CHARACTER O HU Compatibility Decomposition_Mapping is {U+0028, U+110B, U+1169, U+1112, U+116E, U+0029}", mappingsize == 6 && fl_equals(destination, ((const uint32_t[]){0x0028, 0x110B, 0x1169, 0x1112, 0x116E, 0x0029}), mappingsize));

    memset(destination, 0, sizeof(uint32_t) * 50);
    mappingsize = fl_unicode_get_decomposition_mapping(0x321E, true, destination);
    flut_expect_compat("Code point U+321E PARENTHESIZED KOREAN CHARACTER O HU Canonical Decomposition_Mapping is its default value 0x321E", mappingsize == 1 && destination[0] == 0x321E);

    //
    memset(destination, 0, sizeof(uint32_t) * 50);
    mappingsize = fl_unicode_get_decomposition_mapping(0xFDFA, false, destination);
    flut_expect_compat("Code point U+FDFA ARABIC LIGATURE SALLALLAHOU ALAYHE WASALLAM Compatibility Decomposition_Mapping is {U+0635, U+0644, U+0649, U+0020, U+0627, U+0644, U+0644, U+0647, U+0020, U+0639, U+0644, U+064A, U+0647, U+0020, U+0648, U+0633, U+0644, U+0645}", mappingsize == 18 && fl_equals(destination, ((const uint32_t[]){0x0635, 0x0644, 0x0649, 0x0020, 0x0627, 0x0644, 0x0644, 0x0647, 0x0020, 0x0639, 0x0644, 0x064A, 0x0647, 0x0020, 0x0648, 0x0633, 0x0644, 0x0645}), mappingsize));

    memset(destination, 0, sizeof(uint32_t) * 50);
    mappingsize = fl_unicode_get_decomposition_mapping(0xFDFA, true, destination);
    flut_expect_compat("Code point U+FDFA ARABIC LIGATURE SALLALLAHOU ALAYHE WASALLAM Canonical Decomposition_Mapping is its default value 0xFDFA", mappingsize == 1 && destination[0] == 0xFDFA);

    //
    memset(destination, 0, sizeof(uint32_t) * 50);
    mappingsize = fl_unicode_get_decomposition_mapping(0xFDFB, false, destination);
    flut_expect_compat("Code point U+FDFB ARABIC LIGATURE JALLAJALALOUHOU Compatibility Decomposition_Mapping is {U+062C, U+0644, U+0020, U+062C, U+0644, U+0627, U+0644, U+0647}", mappingsize == 8 && fl_equals(destination, ((const uint32_t[]){0x062C, 0x0644, 0x0020, 0x062C, 0x0644, 0x0627, 0x0644, 0x0647}), mappingsize));

    memset(destination, 0, sizeof(uint32_t) * 50);
    mappingsize = fl_unicode_get_decomposition_mapping(0xFDFB, true, destination);
    flut_expect_compat("Code point U+FDFB ARABIC LIGATURE JALLAJALALOUHOU Canonical Decomposition_Mapping is its default value 0xFDFB", mappingsize == 1 && destination[0] == 0xFDFB);
}
