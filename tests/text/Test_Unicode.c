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

void test_fl_unicode_unichar_encode_to()
{
    /*// UTF8 1-byte
    uint32_t u32 = 0x0000;
    uint32_t u8 = 0x0;
    fl_expect("UTF32 -> UTF8: U+0000 equals to 0x0", fl_unicode_unichar_encode_to(FL_ENCODING_UTF32, u32, FL_ENCODING_UTF8) == u8);
    fl_expect("UTF8 -> UTF32: 0x0 equals to U+0000", fl_unicode_unichar_encode_to(FL_ENCODING_UTF8, u8, FL_ENCODING_UTF32) == u32);

    u32 = 0x0048;
    u8 = 0x48;
    fl_expect("UTF32 -> UTF8: U+0048 equals to 0x48", fl_unicode_unichar_encode_to(FL_ENCODING_UTF32, u32, FL_ENCODING_UTF8) == u8);
    fl_expect("UTF8 -> UTF32: 0x48 equals to U+0048", fl_unicode_unichar_encode_to(FL_ENCODING_UTF8, u8, FL_ENCODING_UTF32) == u32);

    u32 = 0x007F;
    u8 = 0x7f;
    fl_expect("UTF32 -> UTF8: U+007F equals to 0x7f", fl_unicode_unichar_encode_to(FL_ENCODING_UTF32, u32, FL_ENCODING_UTF8) == u8);
    fl_expect("UTF8 -> UTF32: 0x7f equals to U+007F", fl_unicode_unichar_encode_to(FL_ENCODING_UTF8, u8, FL_ENCODING_UTF32) == u32);

    // UTF8 2-bytes
    u32 = 0x0080;
    u8 = 0xC280;
    fl_expect("UTF32 -> UTF8: U+0080 equals to 0xC280", fl_unicode_unichar_encode_to(FL_ENCODING_UTF32, u32, FL_ENCODING_UTF8) == u8);
    fl_expect("UTF8 -> UTF32: 0xC280 equals to U+0080", fl_unicode_unichar_encode_to(FL_ENCODING_UTF8, u8, FL_ENCODING_UTF32) == u32);

    u32 = 0x0100;
    u8 = 0xC480;
    fl_expect("UTF32 -> UTF8: U+0100 equals to 0xC480", fl_unicode_unichar_encode_to(FL_ENCODING_UTF32, u32, FL_ENCODING_UTF8) == u8);
    fl_expect("UTF8 -> UTF32: 0xC480 equals to U+0100", fl_unicode_unichar_encode_to(FL_ENCODING_UTF8, u8, FL_ENCODING_UTF32) == u32);

    u32 = 0x07FF;
    u8 = 0xdfbf;
    fl_expect("UTF32 -> UTF8: U+07FF equals to 0xdfbf", fl_unicode_unichar_encode_to(FL_ENCODING_UTF32, u32, FL_ENCODING_UTF8) == u8);
    fl_expect("UTF8 -> UTF32: 0xdfbf equals to U+07FF", fl_unicode_unichar_encode_to(FL_ENCODING_UTF8, u8, FL_ENCODING_UTF32) == u32);

    // UTF8 3-bytes
    u32 = 0x0800;
    u8 = 0xe0a080;
    fl_expect("UTF32 -> UTF8: U+0800 equals to 0xe0a080", fl_unicode_unichar_encode_to(FL_ENCODING_UTF32, u32, FL_ENCODING_UTF8) == u8);
    fl_expect("UTF8 -> UTF32: 0xe0a080 equals to U+0800", fl_unicode_unichar_encode_to(FL_ENCODING_UTF8, u8, FL_ENCODING_UTF32) == u32);

    u32 = 0xA8F9;
    u8 = 0xeaa3b9;
    fl_expect("UTF32 -> UTF8: U+A8F9 equals to 0xeaa3b9", fl_unicode_unichar_encode_to(FL_ENCODING_UTF32, u32, FL_ENCODING_UTF8) == u8);
    fl_expect("UTF8 -> UTF32: 0xeaa3b9 equals to U+A8F9", fl_unicode_unichar_encode_to(FL_ENCODING_UTF8, u8, FL_ENCODING_UTF32) == u32);

    u32 = 0xFFFF;
    u8 = 0xefbfbf;
    fl_expect("UTF32 -> UTF8: U+FFFF equals to 0xefbfbf", fl_unicode_unichar_encode_to(FL_ENCODING_UTF32, u32, FL_ENCODING_UTF8) == u8);
    fl_expect("UTF8 -> UTF32: 0xefbfbf equals to U+FFFF", fl_unicode_unichar_encode_to(FL_ENCODING_UTF8, u8, FL_ENCODING_UTF32) == u32);

    // UTF8 4-bytes
    u32 = 0x10000;
    u8 = 0xf0908080;
    fl_expect("UTF32 -> UTF8: U+10000 equals to 0xf0908080", fl_unicode_unichar_encode_to(FL_ENCODING_UTF32, u32, FL_ENCODING_UTF8) == u8);
    fl_expect("UTF8 -> UTF32: 0xf0908080 equals to U+10000", fl_unicode_unichar_encode_to(FL_ENCODING_UTF8, u8, FL_ENCODING_UTF32) == u32);

    u32 = 0x1F615;
    u8 = 0xf09f9895;
    fl_expect("UTF32 -> UTF8: U+1F615 equals to 0xf09f9895", fl_unicode_unichar_encode_to(FL_ENCODING_UTF32, u32, FL_ENCODING_UTF8) == u8);
    fl_expect("UTF8 -> UTF32: 0xf09f9895 equals to U+1F615", fl_unicode_unichar_encode_to(FL_ENCODING_UTF8, u8, FL_ENCODING_UTF32) == u32);

    u32 = 0x10FFFF;
    u8 = 0xf48fbfbf;
    fl_expect("UTF32 -> UTF8: U+FFFF equals to 0xf48fbfbf", fl_unicode_unichar_encode_to(FL_ENCODING_UTF32, u32, FL_ENCODING_UTF8) == u8);
    fl_expect("UTF8 -> UTF32: 0xf48fbfbf equals to U+FFFF", fl_unicode_unichar_encode_to(FL_ENCODING_UTF8, u8, FL_ENCODING_UTF32) == u32);
    
    // Invalid
    u32 = 0x110000;
    fl_expect("UTF8 -> UTF32: 0xf48fbfc0 is an invalid Unicode char", fl_unicode_unichar_encode_to(FL_ENCODING_UTF32, u32, FL_ENCODING_UTF8) == FL_UNICODE_INVALID_CHAR);

    // From string
    u8 = fl_unicode_codepoint_to_unichar(FL_ENCODING_UTF8, (FlByte*)"Ɓ\x00", 0x00);
    u32 = 0x0181;
    fl_expect("UTF32 -> UTF8: U+0181 equals to 0xC681", fl_unicode_unichar_encode_to(FL_ENCODING_UTF32, u32, FL_ENCODING_UTF8) == u8);
    fl_expect("UTF8 -> UTF32: 0xC681 equals to U+0181", fl_unicode_unichar_encode_to(FL_ENCODING_UTF8, u8, FL_ENCODING_UTF32) == u32);*/
}

void test_fl_unicode_codepoint_to_unichar()
{
    /*uint32_t u8fromStr = fl_unicode_codepoint_to_unichar(FL_ENCODING_UTF8, (FlByte*)"Ɓ\x00", 0x00);
    FlByte u32bytes[] = {0,0,0,0,0}; 
    if (fl_system_is_little_endian()) 
    {
        u32bytes[0] = 129;
        u32bytes[1] = 1;
    }
    else
    {
         u32bytes[2] = 1;
         u32bytes[3] = 129;
    }
    uint32_t u8fromBytes = fl_unicode_codepoint_to_encoded_unichar(FL_ENCODING_UTF32, u32bytes, 0x00, FL_ENCODING_UTF8);

    uint32_t u32fromBytes = 0;
    memcpy(&u32fromBytes, u32bytes, sizeof(uint32_t));
    uint32_t u32fromStr = fl_unicode_codepoint_to_encoded_unichar(FL_ENCODING_UTF8, (FlByte*)"Ɓ\x00", 0x00, FL_ENCODING_UTF32);

    fl_expect("Character Ɓ is compound of bytes {1,129}", u8fromStr == u8fromBytes && u32fromStr == u32fromBytes);


    char *b = "Ɓ";
    char b2[5];
    uint32_t ucharB = fl_unicode_codepoint_to_encoded_unichar(FL_ENCODING_UTF8, (FlByte*)b, 0x00, FL_ENCODING_UTF32);
    fl_unicode_unichar_to_encoded_codepoint(FL_ENCODING_UTF32, ucharB, FL_ENCODING_UTF8, (FlByte*)b2);
    fl_expect("Bytes after conversion UTF-8 bytes -> uint32_t UTF-32 -> UTF-8 bytes are the same for character Ɓ", memcmp(b, b2, 2) == 0);

    ucharB = fl_unicode_codepoint_to_unichar(FL_ENCODING_UTF8, (FlByte*)b, 0x00);
    fl_unicode_unichar_to_codepoint(FL_ENCODING_UTF8, ucharB, (FlByte*)b2);
    fl_expect("Bytes after conversion UTF-8 bytes -> uint32_t UTF-8 -> UTF-8 bytes are the same for character Ɓ", memcmp(b, b2, 2) == 0);

    char *b3 = "你";
    ucharB = fl_unicode_codepoint_to_encoded_unichar(FL_ENCODING_UTF8, (FlByte*)b3, 0x00, FL_ENCODING_UTF32);
    fl_unicode_unichar_to_codepoint(FL_ENCODING_UTF32, ucharB, (FlByte*)b2);
    fl_expect("Bytes after conversion UTF-8 bytes -> uint32_t UTF-32 -> UTF-32 bytes are the same for character Ɓ", memcmp(&ucharB, b2, 4) == 0);

    FlCstr wdir = fl_system_get_working_dir();
    FlCstr path = fl_cstr_concat((FlCstr[]){ wdir, "\\tmp\\b.txt", 0x0 });
    FlByteArray bytes = fl_file_read_all_bytes(path);
    fl_cstr_delete(wdir);
    fl_cstr_delete(path);
    if (bytes)
        fl_array_delete(bytes);*/
}

void test_fl_unicode_codeunit_sequence_size()
{
    fl_expect("abc123 has 6 bytes", fl_unicode_codeunit_sequence_size(FL_ENCODING_UTF8, (const FlByte*)"abc123", NULL) == 6);
    fl_expect("aƁc123 has 7 bytes", fl_unicode_codeunit_sequence_size(FL_ENCODING_UTF8, (const FlByte*)"aƁc123", NULL) == 7);
    fl_expect("a兔c123 has 9 bytes", fl_unicode_codeunit_sequence_size(FL_ENCODING_UTF8, (const FlByte*)"a兔c123", NULL) == 9);
    fl_expect("兔 has 4 bytes", fl_unicode_codeunit_sequence_size(FL_ENCODING_UTF8, (const FlByte*)"兔", NULL) == 4);
    fl_expect("ԱԲ has 4 bytes", fl_unicode_codeunit_sequence_size(FL_ENCODING_UTF8, (const FlByte*)"ԱԲ", NULL) == 4);
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
    fl_expect("UTF-32 U+0041 encoded to UTF-8 is equals to 0x41 and occupies 1 byte", bytes == 1 && fl_unicode_codeunit_sequence_equals(dst, "\x41", bytes));

    //
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"\x41\x00", NULL, FL_ENCODING_UTF32, dst);
    fl_expect("UTF-8 0x41 encoded to UTF-32 is equals to 0x00000041 and occupies 4 bytes", bytes == 4 && fl_unicode_codeunit_sequence_equals(dst, "\x00\x00\x00\x41", bytes));

    //
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x00\x01\x00", NULL, FL_ENCODING_UTF8, dst);
    fl_expect("UTF-32 U+0100 encoded to UTF-8 is equals to 0xC480 and occupies 2 bytes", bytes == 2 && fl_unicode_codeunit_sequence_equals(dst, "Ā\x00", bytes));

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"Ā\x00", NULL, FL_ENCODING_UTF32, dst);
    fl_expect("UTF-8 Ā encoded to UTF-32 is equals to 0x00000100 and occupies 4 bytes", bytes == 4 && fl_unicode_codeunit_sequence_equals(dst, "\x00\x00\x01\x00", bytes));

    //
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x00\x00\x5A", NULL, FL_ENCODING_UTF8, dst);
    fl_expect("UTF-32 U+005A encoded to UTF-8 is equals to 0x005A and occupies 1 byte", bytes == 1 && fl_unicode_codeunit_sequence_equals(dst, "\x5A\x00", bytes));

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"Z\x00", NULL, FL_ENCODING_UTF32, dst);
    fl_expect("UTF-8 Z encoded to UTF-32 is equals to 0x0000005A and occupies 4 bytes", bytes == 4 && fl_unicode_codeunit_sequence_equals(dst, "\x00\x00\x00\x5A", bytes));

    //
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x00\x03\xA0", NULL, FL_ENCODING_UTF8, dst);
    fl_expect("UTF-32 U+03A0 encoded to UTF-8 is equals to 0xCEA0 and occupies 2 bytes", bytes == 2 && fl_unicode_codeunit_sequence_equals(dst, "\xCE\xA0", bytes));

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"\xCE\xA0\x00", NULL, FL_ENCODING_UTF32, dst);
    fl_expect("UTF-8 0xCEA000 encoded to UTF-32 is equals to 0x000003A0 and occupies 4 bytes", bytes == 4 && fl_unicode_codeunit_sequence_equals(dst, "\x00\x00\x03\xA0", bytes));

    //
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x00\xFF\x21", NULL, FL_ENCODING_UTF8, dst);
    fl_expect("UTF-32 U+FF21 encoded to UTF-8 is equals to 0xEFBCA1 and occupies 3 bytes", bytes == 3 && fl_unicode_codeunit_sequence_equals(dst, "\xEF\xBC\xA1", bytes));

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"\xEF\xBC\xA1\x00", NULL, FL_ENCODING_UTF32, dst);
    fl_expect("UTF-8 0xEFBCA1 encoded to UTF-32 is equals to 0x0000FF21 and occupies 4 bytes", bytes == 4 && fl_unicode_codeunit_sequence_equals(dst, "\x00\x00\xFF\x21", bytes));

    //
    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, (FlByte*)"\x00\x02\xF8\x0F\x00", NULL, FL_ENCODING_UTF8, dst);
    fl_expect("UTF-32 U+2F80F encoded to UTF-8 is equals to 0xF0AFA08F and occupies 3 bytes", bytes == 4 && fl_unicode_codeunit_sequence_equals(dst, "\xF0\xAF\xA0\x8F", bytes));

    memset(dst, 0, 4);
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, (FlByte*)"\xF0\xAF\xA0\x8F\x00", NULL, FL_ENCODING_UTF32, dst);
    fl_expect("UTF-8 0xF0AFA08F encoded to UTF-32 is equals to 0x00002F80F and occupies 4 bytes", bytes == 4 && fl_unicode_codeunit_sequence_equals(dst, "\x00\x02\xF8\x0F", bytes));

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
}