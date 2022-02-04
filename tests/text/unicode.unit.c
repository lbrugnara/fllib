#include <locale.h>
#include <fllib.h>
#include <string.h>

#include <flut/flut.h>

#define fl_unicode_sequence_equals(string, offset, unichar, unichar_size) \
    fl_slice_equals_sequence(&flm_slice_new(string, sizeof(FlByte), offset, unichar_size), unichar, unichar_size)

flut_suite(unicode) {
    flut_suite_description("Unicode functions");
    flut_suite_register_test(unicode_codepoint_at, "fl_unicode_codepoint_at function");
    flut_suite_register_test(unicode_codepoint_size, "fl_unicode_codepoint_size function");
    flut_suite_register_test(unicode_codeunit_sequence_size, "");
    flut_suite_register_test(unicode_codepoint_validity, "");
    flut_suite_register_test(unicode_codepoint_convert, "");
    flut_suite_register_test(unicode_codepoint_sequence_validate, "");
    flut_suite_register_test(unicode_data, "");
}

flut_test(unicode_codepoint_at) {
    #define CODEPOINT_OFFSET(offset, size) offset * size
    // All the code points size in this string is two-bytes
    const FlByte *string = FL_UNICODE_SEQUENCE("ĀāĂăĄąĆćĈĉĊċČčĎďĐđĒēĔĕĖėĘęĚěĜĝĞğĠġĢģĤĥĦħĨĩĪīĬĭĮįİıĲĳĴĵĶķĸĹĺĻļĽľĿŀŁłŃńŅņŇňŉŊŋŌōŎŏŐőŒœŔŕŖŗŘřŚśŜŝŞşŠšŢţŤťŦŧŨũŪūŬŭŮůŰűŲųŴŵŶŷŸŹźŻżŽž\x00");
    flut_assert_size_t_is_equals(127, fl_unicode_codepoint_sequence_length(FL_ENCODING_UTF8, string, 0x00));
    flut_assert_size_t_is_equals(254, fl_unicode_codeunit_sequence_size(FL_ENCODING_UTF8, string, 0x00));

    FlByte unichar[4] = {0,0,0,0};
    size_t unichar_size = fl_unicode_codepoint_at(FL_ENCODING_UTF8, string, 0x00, 0, unichar);
    flut_assert_size_t_is_equals(2, unichar_size);
    flut_assert_is_true(fl_unicode_sequence_equals(string, 0, unichar, unichar_size));

    memset(unichar, 0, 4);
    unichar_size = fl_unicode_codepoint_at(FL_ENCODING_UTF8, string, 0x00, 28, unichar);
    flut_assert_size_t_is_equals(2, unichar_size);
    flut_assert_is_true(fl_unicode_sequence_equals(string, CODEPOINT_OFFSET(28, 2), unichar, unichar_size));

    memset(unichar, 0, 4);
    unichar_size = fl_unicode_codepoint_at(FL_ENCODING_UTF8, string, 0x00, 40, unichar);
    flut_assert_size_t_is_equals(2, unichar_size);
    flut_assert_is_true(fl_unicode_sequence_equals(string, CODEPOINT_OFFSET(40, 2), unichar, unichar_size));

    memset(unichar, 0, 4);
    unichar_size = fl_unicode_codepoint_at(FL_ENCODING_UTF8, string, 0x00, 60, unichar);
    flut_assert_size_t_is_equals(2, unichar_size);
    flut_assert_is_true(fl_unicode_sequence_equals(string, CODEPOINT_OFFSET(60, 2), unichar, unichar_size));

    memset(unichar, 0, 4);
    unichar_size = fl_unicode_codepoint_at(FL_ENCODING_UTF8, string, 0x00, 48, unichar);
    flut_assert_size_t_is_equals(2, unichar_size);
    flut_assert_is_true(fl_unicode_sequence_equals(string, CODEPOINT_OFFSET(48, 2), unichar, unichar_size));

    memset(unichar, 0, 4);
    unichar_size = fl_unicode_codepoint_at(FL_ENCODING_UTF8, string, 0x00, 92, unichar);
    flut_assert_size_t_is_equals(2, unichar_size);
    flut_assert_is_true(fl_unicode_sequence_equals(string, CODEPOINT_OFFSET(92, 2), unichar, unichar_size));

    memset(unichar, 0, 4);
    unichar_size = fl_unicode_codepoint_at(FL_ENCODING_UTF8, string, 0x00, 126, unichar);
    flut_assert_size_t_is_equals(2, unichar_size);
    flut_assert_is_true(fl_unicode_sequence_equals(string, CODEPOINT_OFFSET(126, 2), unichar, unichar_size));

    // All the code points size in this string is four-bytes
    string = FL_UNICODE_SEQUENCE("\x00\x00\x00\x41\x00\x00\x00\x42");
    memset(unichar, 0, 4);
    unichar_size = fl_unicode_codepoint_at(FL_ENCODING_UTF32, string, 0x00, 0, unichar);
    flut_assert_size_t_is_equals(4, unichar_size);
    flut_assert_is_true(fl_unicode_sequence_equals(string, CODEPOINT_OFFSET(0, 4), unichar, unichar_size));

    // 
    memset(unichar, 0, 4);
    unichar_size = fl_unicode_codepoint_at(FL_ENCODING_UTF32, string, 0x00, 1, unichar);
    flut_assert_size_t_is_equals(4, unichar_size);
    flut_assert_is_true(fl_unicode_sequence_equals(string, CODEPOINT_OFFSET(1, 4), unichar, unichar_size));
}

flut_test(unicode_codepoint_size) {
    flut_assert_size_t_is_equals(FL_UNICODE_INVALID_SIZE, fl_unicode_codepoint_size(FL_ENCODING_UTF32, FL_UNICODE_SEQUENCE("\x00\xD8\x00\x00\x00"), 0x00));
    flut_assert_size_t_is_equals(FL_UNICODE_INVALID_SIZE, fl_unicode_codepoint_size(FL_ENCODING_UTF32, FL_UNICODE_SEQUENCE("\x00\xDF\xFF\xFF\x00"), 0x00));
    flut_assert_size_t_is_equals(2, fl_unicode_codepoint_size(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("Ɓ"), 0x00));
    flut_assert_size_t_is_equals(4, fl_unicode_codepoint_size(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("兔"), 0x00));
}

flut_test(unicode_codeunit_sequence_size) {
    flut_assert_size_t_is_equals(6, fl_unicode_codeunit_sequence_size(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("abc123"), 0x00));
    flut_assert_size_t_is_equals(7, fl_unicode_codeunit_sequence_size(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("aƁc123"), 0x00));
    flut_assert_size_t_is_equals(9, fl_unicode_codeunit_sequence_size(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("a兔c123"), 0x00));
    flut_assert_size_t_is_equals(4, fl_unicode_codeunit_sequence_size(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("兔"), 0x00));
    flut_assert_size_t_is_equals(4, fl_unicode_codeunit_sequence_size(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("ԱԲ"), 0x00));
    flut_assert_size_t_is_equals(2, fl_unicode_codeunit_sequence_size(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("Ɓ"), 0x00));
    flut_assert_size_t_is_equals(4, fl_unicode_codeunit_sequence_size(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("兔"), 0x00));
}

flut_test(unicode_unichar_validity) {
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

flut_test(unicode_codepoint_validity) {
    // NOTE: All the 0x00 (NULL) chars at the end are used to mark the end of the sequence

    flut_explain(flut_assert_is_true(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xef\xbf\xbd\x00"), 0x00)), "Replacement char U+FFFD is valid UTF-8");
    
    // Overlon encodings
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xC0\x80\x00"), 0x00)), "Overlong encoding of U+0000 (0xC080) is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xc0\xaf\x00"), 0x00)), "Overlong encoding of U+002F (0xc0af) is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xe0\x80\xaf\x00"), 0x00)), "Overlong encoding of U+002F (0xe080af) is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xf0\x80\x80\xaf\x00"), 0x00)), "Overlong encoding of U+002F (0xf08080af) is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xc1\xbf\x00"), 0x00)), "Overlong encoding of U+007F (0xc1bf) is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xe0\x9f\xbf\x00"), 0x00)), "Overlong encoding of U+07FF (0xe09fbf) is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xf0\x8f\xbf\xbf\x00"), 0x00)), "Overlong encoding of U+FFFF (0xf08fbfbf) is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xc0\x80\x00"), 0x00)), "Overlong encoding of U+0000 (0xc080) is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xe0\x80\x80\x00"), 0x00)), "Overlong encoding of U+0000 (0xe08080) is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xf0\x80\x80\x80\x00"), 0x00)), "Overlong encoding of U+0000 (0xf0808080) is not valid UTF-8");
    
    // UTF-16 Surrogates
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xed\xa0\x80\x00"), 0x00)), "UTF-16 Surrogate U+D800 (0xeda080) is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xed\xad\xbf\x00"), 0x00)), "UTF-16 Surrogate U+DB7F (0xedadbf) is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xed\xae\x80\x00"), 0x00)), "UTF-16 Surrogate U+DB80 (0xedae80) is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xed\xaf\xbf\x00"), 0x00)), "UTF-16 Surrogate U+DBFF (0xedafbf) is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xed\xb0\x80\x00"), 0x00)), "UTF-16 Surrogate U+DC00 (0xedb080) is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xed\xbe\x80\x00"), 0x00)), "UTF-16 Surrogate U+DF80 (0xedbe80) is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xed\xbf\xbf\x00"), 0x00)), "UTF-16 Surrogate U+DFFF (0xedbfbf) is not valid UTF-8");

    // Boundaries
    #define MAKE_UINT32_PTR(u) (const FlByte*)(uint32_t[]) { u }
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xED\xA1\x8C\x00"), 0x00)), "High surrogate U+D84C is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF32, MAKE_UINT32_PTR(0xD84C), 0x00)), "High surrogate U+D84C is not valid UTF-32");
    flut_explain(flut_assert_is_true(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xed\x9f\xbf\x00"), 0x00)), "High surrogate boundary U+D7FF is valid UTF-8");
    flut_explain(flut_assert_is_true(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xf4\x8f\xbf\xbf\x00"), 0x00)), "Max boundary U+10FFFF is valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xf4\x90\x80\x80\x00"), 0x00)), "Max boundary+1 U+110000 is not valid UTF-8");

    flut_explain(flut_assert_is_true(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\x00\x00\x00"), 0x00)), "U+0000 is valid UTF-8 (0x0000)");
    flut_explain(flut_assert_is_true(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xC2\x80\x00"), 0x00)), "U+0080 is valid UTF-8 (0xC280)");
    flut_explain(flut_assert_is_true(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xe0\xa0\x80\x00"), 0x00)), "U+0800 is valid UTF-8 (0xe0a080)");
    flut_explain(flut_assert_is_true(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xf0\x90\x80\x80\x00"), 0x00)), "U+10000 is valid UTF-8 (0xf0908080)");

    flut_explain(flut_assert_is_true(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\x00\x7F\x00"), 0x00)), "U+007F is valid UTF-8 (0x007F)");
    flut_explain(flut_assert_is_true(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xdf\xbf\x00"), 0x00)), "U+07FF is valid UTF-8 (0xdfbf)");
    flut_explain(flut_assert_is_true(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xef\xbf\xbf\x00"), 0x00)), "U+FFFF is valid UTF-8 (0xefbfbf)");
    
    flut_explain(flut_assert_is_true(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xe0\xb8\x80\x00"), 0x00)), "U+0E00 is valid UTF-8 (0xe0b880)");
    #undef MAKE_UINT32_PTR


    // Unexpected continuation bytes
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\x80\x00"), 0x00)), "Continuation byte 0x80 is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xbf\x00"), 0x00)), "Continuation byte 0xbf is not valid UTF-8");

    // 2-bytes sequence start bytes
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xC0\x00"), 0x00)), "0xC0 is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xC5\x00"), 0x00)), "0xC5 is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xCA\x00"), 0x00)), "0xCA is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xCF\x00"), 0x00)), "0xCF is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xD0\x00"), 0x00)), "0xD0 is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xD5\x00"), 0x00)), "0xD5 is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xDA\x00"), 0x00)), "0xDA is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xDF\x00"), 0x00)), "0xDF is not valid UTF-8");

    // 3-bytes sequence start bytes
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xE0\x00"), 0x00)), "0xE0 is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xE5\x00"), 0x00)), "0xE5 is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xEA\x00"), 0x00)), "0xEA is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xEF\x00"), 0x00)), "0xEF is not valid UTF-8");

    // 4-bytes sequence start bytes
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xF0\x00"), 0x00)), "0xF0 is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xF5\x00"), 0x00)), "0xF5 is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xFA\x00"), 0x00)), "0xFA is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xFE\x00"), 0x00)), "0xFE is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xFF\x00"), 0x00)), "0xFF is not valid UTF-8");

    // Sequences
    
    // Last byte missing
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_sequence_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xC0\x00"), NULL)), "Sequence xC0 is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_sequence_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xDF\x00"), NULL)), "Sequence xDF is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_sequence_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xE0\x80\x00"), NULL)), "Sequence xE0x80 is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_sequence_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xEF\xBF\x00"), NULL)), "Sequence xEFxBF is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_sequence_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xF0\x80\x80\x00"), NULL)), "Sequence xF0x80x80 is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_sequence_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xF7\xBF\xBF\x00"), NULL)), "Sequence xF7xBFxBF is not valid UTF-8");

    // Surrogates
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_sequence_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xed\xa0\x80\xed\xb0\x80\x00"), NULL)), "Surrogates sequence U+D800 U+DC00 is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_sequence_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xed\xa0\x80\xed\xbf\xbf\x00"), NULL)), "Surrogates sequence U+D800 U+DFFF is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_sequence_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xed\xad\xbf\xed\xb0\x80\x00"), NULL)), "Surrogates sequence U+DB7F U+DC00 is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_sequence_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xed\xad\xbf\xed\xbf\xbf\x00"), NULL)), "Surrogates sequence U+DB7F U+DFFF is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_sequence_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xed\xae\x80\xed\xb0\x80\x00"), NULL)), "Surrogates sequence U+DB80 U+DC00 is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_sequence_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xed\xae\x80\xed\xbf\xbf\x00"), NULL)), "Surrogates sequence U+DB80 U+DFFF is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_sequence_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xed\xaf\xbf\xed\xb0\x80\x00"), NULL)), "Surrogates sequence U+DBFF U+DC00 is not valid UTF-8");
    flut_explain(flut_assert_is_false(fl_unicode_codepoint_sequence_is_valid(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xed\xaf\xbf\xed\xbf\xbf\x00"), NULL)), "Surrogates sequence U+DBFF U+DFFF is not valid UTF-8");
}

flut_test(unicode_unichar_sequence_validate) {
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

flut_test(unicode_codepoint_convert) {
    FlByte codepoint[4] = {0,0,0,0};
    size_t bytes = 0;

    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, FL_UNICODE_SEQUENCE("\x00\x00\x00\x41\x00"), NULL, FL_ENCODING_UTF32, codepoint);
    flut_assert_size_t_is_equals(4, bytes);
    flut_assert_is_true(fl_unicode_sequence_equals(FL_UNICODE_SEQUENCE("\x00\x00\x00\x41\x00"), 0, codepoint, bytes));
    memset(codepoint, 0, 4);

    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\x41\x00"), NULL, FL_ENCODING_UTF8, codepoint);
    flut_assert_size_t_is_equals(1, bytes);
    flut_assert_is_true(fl_unicode_sequence_equals(FL_UNICODE_SEQUENCE("\x41"), 0, codepoint, bytes));
    memset(codepoint, 0, 4);

    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, FL_UNICODE_SEQUENCE("\x00\x00\x00\x41\x00"), NULL, FL_ENCODING_UTF8, codepoint);
    flut_assert_size_t_is_equals(1, bytes);
    flut_assert_is_true(fl_unicode_sequence_equals(FL_UNICODE_SEQUENCE("\x41"), 0, codepoint, bytes));
    memset(codepoint, 0, 4);

    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\x41\x00"), NULL, FL_ENCODING_UTF32, codepoint);
    flut_assert_size_t_is_equals(4, bytes);
    flut_assert_is_true(fl_unicode_sequence_equals(FL_UNICODE_SEQUENCE("\x00\x00\x00\x41"), 0, codepoint, bytes));
    memset(codepoint, 0, 4);

    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, FL_UNICODE_SEQUENCE("\x00\x00\x01\x00"), NULL, FL_ENCODING_UTF8, codepoint);
    flut_assert_size_t_is_equals(2, bytes);
    flut_assert_is_true(fl_unicode_sequence_equals(FL_UNICODE_SEQUENCE("Ā\x00"), 0, codepoint, bytes));
    memset(codepoint, 0, 4);

    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("Ā\x00"), NULL, FL_ENCODING_UTF32, codepoint);
    flut_assert_size_t_is_equals(4, bytes);
    flut_assert_is_true(fl_unicode_sequence_equals(FL_UNICODE_SEQUENCE("\x00\x00\x01\x00"), 0, codepoint, bytes));
    memset(codepoint, 0, 4);

    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, FL_UNICODE_SEQUENCE("\x00\x00\x00\x5A"), NULL, FL_ENCODING_UTF8, codepoint);
    flut_assert_size_t_is_equals(1, bytes);
    flut_assert_is_true(fl_unicode_sequence_equals(FL_UNICODE_SEQUENCE("\x5A\x00"), 0, codepoint, bytes));
    memset(codepoint, 0, 4);

    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("Z\x00"), NULL, FL_ENCODING_UTF32, codepoint);
    flut_assert_size_t_is_equals(4, bytes);
    flut_assert_is_true(fl_unicode_sequence_equals(FL_UNICODE_SEQUENCE("\x00\x00\x00\x5A"), 0, codepoint, bytes));
    memset(codepoint, 0, 4);

    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, FL_UNICODE_SEQUENCE("\x00\x00\x03\xA0"), NULL, FL_ENCODING_UTF8, codepoint);
    flut_assert_size_t_is_equals(2, bytes);
    flut_assert_is_true(fl_unicode_sequence_equals(FL_UNICODE_SEQUENCE("\xCE\xA0"), 0, codepoint, bytes));
    memset(codepoint, 0, 4);

    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xCE\xA0\x00"), NULL, FL_ENCODING_UTF32, codepoint);
    flut_assert_size_t_is_equals(4, bytes);
    flut_assert_is_true(fl_unicode_sequence_equals(FL_UNICODE_SEQUENCE("\x00\x00\x03\xA0"), 0, codepoint, bytes));
    memset(codepoint, 0, 4);

    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, FL_UNICODE_SEQUENCE("\x00\x00\xFF\x21"), NULL, FL_ENCODING_UTF8, codepoint);
    flut_assert_size_t_is_equals(3, bytes);
    flut_assert_is_true(fl_unicode_sequence_equals(FL_UNICODE_SEQUENCE("\xEF\xBC\xA1"), 0, codepoint, bytes));
    memset(codepoint, 0, 4);

    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xEF\xBC\xA1\x00"), NULL, FL_ENCODING_UTF32, codepoint);
    flut_assert_size_t_is_equals(4, bytes);
    flut_assert_is_true(fl_unicode_sequence_equals(FL_UNICODE_SEQUENCE("\x00\x00\xFF\x21"), 0, codepoint, bytes));
    memset(codepoint, 0, 4);

    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, FL_UNICODE_SEQUENCE("\x00\x02\xF8\x0F\x00"), NULL, FL_ENCODING_UTF8, codepoint);
    flut_assert_size_t_is_equals(4, bytes);
    flut_assert_is_true(fl_unicode_sequence_equals(FL_UNICODE_SEQUENCE("\xF0\xAF\xA0\x8F"), 0, codepoint, bytes));
    memset(codepoint, 0, 4);

    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xF0\xAF\xA0\x8F\x00"), NULL, FL_ENCODING_UTF32, codepoint);
    flut_assert_size_t_is_equals(4, bytes);
    flut_assert_is_true(fl_unicode_sequence_equals(FL_UNICODE_SEQUENCE("\x00\x02\xF8\x0F"), 0, codepoint, bytes));
    memset(codepoint, 0, 4);

    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, FL_UNICODE_SEQUENCE("\x00\x00\xD8\x00\x00"), NULL, FL_ENCODING_UTF8, codepoint);
    flut_explain(flut_assert_size_t_is_equals(FL_UNICODE_INVALID_SIZE, bytes), "UTF-32 U+D800 encoded to UTF-8 is invalid because it is a High Surrogate");

    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xED\xA0\x80"), NULL, FL_ENCODING_UTF32, codepoint);
    flut_explain(flut_assert_size_t_is_equals(FL_UNICODE_INVALID_SIZE, bytes), "UTF-8 0xEDA080 encoded to UTF-32 is invalid because it is a High Surrogate");

    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, FL_UNICODE_SEQUENCE("\x00\x00\xD8\x00\x00"), NULL, FL_ENCODING_UTF8, codepoint);
    flut_explain(flut_assert_size_t_is_equals(FL_UNICODE_INVALID_SIZE, bytes), "UTF-32 U+D800 encoded to UTF-8 is invalid because it is the first High Surrogate");

    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xED\xA0\x80"), NULL, FL_ENCODING_UTF32, codepoint);
    flut_explain(flut_assert_size_t_is_equals(FL_UNICODE_INVALID_SIZE, bytes), "UTF-8 0xEDA080 encoded to UTF-32 is invalid because it is the first High Surrogate U+D800");

    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, FL_UNICODE_SEQUENCE("\x00\x00\xDF\xFF\x00"), NULL, FL_ENCODING_UTF8, codepoint);
    flut_explain(flut_assert_size_t_is_equals(FL_UNICODE_INVALID_SIZE, bytes), "UTF-32 U+DFFF encoded to UTF-8 is invalid because it is the last Low Surrogate");

    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xED\xBF\xBF"), NULL, FL_ENCODING_UTF32, codepoint);
    flut_explain(flut_assert_size_t_is_equals(FL_UNICODE_INVALID_SIZE, bytes), "UTF-8 0xEDBFBF encoded to UTF-32 is invalid because it is the last Low Surrogate U+DFFF");

    // Clean codepoint
    memset(codepoint, 0, 4);

    // UTF8 1-byte
    const FlByte *str = FL_UNICODE_SEQUENCE("\x00\x00\x00\x00");
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, str, str+4, FL_ENCODING_UTF8, codepoint);
    flut_assert_size_t_is_equals(1, bytes);
    flut_assert_is_true(fl_unicode_sequence_equals(FL_UNICODE_SEQUENCE("\x00"), 0, codepoint, bytes));
    memset(codepoint, 0, 4);

    str = FL_UNICODE_SEQUENCE("\x00");
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, str, str+1, FL_ENCODING_UTF32, codepoint);
    flut_assert_size_t_is_equals(4, bytes);
    flut_assert_is_true(fl_unicode_sequence_equals(FL_UNICODE_SEQUENCE("\x00\x00\x00\x00"), 0, codepoint, bytes));
    memset(codepoint, 0, 4);

    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, FL_UNICODE_SEQUENCE("\x00\x00\x00\x48\x00"), NULL, FL_ENCODING_UTF8, codepoint);
    flut_assert_size_t_is_equals(1, bytes);
    flut_assert_is_true(fl_unicode_sequence_equals(FL_UNICODE_SEQUENCE( "H\x00"), 0, codepoint, bytes));
    memset(codepoint, 0, 4);

    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("H\x00"), NULL, FL_ENCODING_UTF32, codepoint);
    flut_assert_size_t_is_equals(4, bytes);
    flut_assert_is_true(fl_unicode_sequence_equals(FL_UNICODE_SEQUENCE("\x00\x00\x00\x48"), 0, codepoint, bytes));
    memset(codepoint, 0, 4);

    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, FL_UNICODE_SEQUENCE("\x00\x00\x00\x7F\x00"), NULL, FL_ENCODING_UTF8, codepoint);
    flut_assert_size_t_is_equals(1, bytes);
    flut_assert_is_true(fl_unicode_sequence_equals(FL_UNICODE_SEQUENCE( "\x7F\x00"), 0, codepoint, bytes));
    memset(codepoint, 0, 4);

    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\x7F\x00"), NULL, FL_ENCODING_UTF32, codepoint);
    flut_assert_size_t_is_equals(4, bytes);
    flut_assert_is_true(fl_unicode_sequence_equals(FL_UNICODE_SEQUENCE("\x00\x00\x00\x7F"), 0, codepoint, bytes));
    memset(codepoint, 0, 4);

    // UTF8 2-bytes
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, FL_UNICODE_SEQUENCE("\x00\x00\x00\x80\x00"), NULL, FL_ENCODING_UTF8, codepoint);
    flut_assert_size_t_is_equals(2, bytes);
    flut_assert_is_true(fl_unicode_sequence_equals(FL_UNICODE_SEQUENCE("\xC2\x80"), 0, codepoint, bytes));
    memset(codepoint, 0, 4);

    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xC2\x80\x00"), NULL, FL_ENCODING_UTF32, codepoint);
    flut_assert_size_t_is_equals(4, bytes);
    flut_assert_is_true(fl_unicode_sequence_equals(FL_UNICODE_SEQUENCE("\x00\x00\x00\x80"), 0, codepoint, bytes));
    memset(codepoint, 0, 4);

    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, FL_UNICODE_SEQUENCE("\x00\x00\x01\x81\x00"), NULL, FL_ENCODING_UTF8, codepoint);
    flut_assert_size_t_is_equals(2, bytes);
    flut_assert_is_true(fl_unicode_sequence_equals(FL_UNICODE_SEQUENCE("\xC6\x81"), 0, codepoint, bytes));
    memset(codepoint, 0, 4);

    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xC6\x81\x00"), NULL, FL_ENCODING_UTF32, codepoint);
    flut_assert_size_t_is_equals(4, bytes);
    flut_assert_is_true(fl_unicode_sequence_equals(FL_UNICODE_SEQUENCE("\x00\x00\x01\x81"), 0, codepoint, bytes));
    memset(codepoint, 0, 4);

    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, FL_UNICODE_SEQUENCE("\x00\x00\x07\xFF\x00"), NULL, FL_ENCODING_UTF8, codepoint);
    flut_assert_size_t_is_equals(2, bytes);
    flut_assert_is_true(fl_unicode_sequence_equals(FL_UNICODE_SEQUENCE("\xDF\xBF"), 0, codepoint, bytes));
    memset(codepoint, 0, 4);

    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xDF\xBF\x00"), NULL, FL_ENCODING_UTF32, codepoint);
    flut_assert_size_t_is_equals(4, bytes);
    flut_assert_is_true(fl_unicode_sequence_equals(FL_UNICODE_SEQUENCE("\x00\x00\x07\xFF"), 0, codepoint, bytes));
    memset(codepoint, 0, 4);

    // UTF8 3-bytes
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, FL_UNICODE_SEQUENCE("\x00\x00\x08\x00\x00"), NULL, FL_ENCODING_UTF8, codepoint);
    flut_assert_size_t_is_equals(3, bytes);
    flut_assert_is_true(fl_unicode_sequence_equals(FL_UNICODE_SEQUENCE("\xE0\xA0\x80"), 0, codepoint, bytes));
    memset(codepoint, 0, 4);

    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xE0\xA0\x80\x00"), NULL, FL_ENCODING_UTF32, codepoint);
    flut_assert_size_t_is_equals(4, bytes);
    flut_assert_is_true(fl_unicode_sequence_equals(FL_UNICODE_SEQUENCE("\x00\x00\x08\x00"), 0, codepoint, bytes));
    memset(codepoint, 0, 4);

    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, FL_UNICODE_SEQUENCE("\x00\x00\xA8\xF9\x00"), NULL, FL_ENCODING_UTF8, codepoint);
    flut_assert_size_t_is_equals(3, bytes);
    flut_assert_is_true(fl_unicode_sequence_equals(FL_UNICODE_SEQUENCE("\xEA\xA3\xB9"), 0, codepoint, bytes));
    memset(codepoint, 0, 4);

    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xEA\xA3\xB9\x00"), NULL, FL_ENCODING_UTF32, codepoint);
    flut_assert_size_t_is_equals(4, bytes);
    flut_assert_is_true(fl_unicode_sequence_equals(FL_UNICODE_SEQUENCE("\x00\x00\xA8\xF9"), 0, codepoint, bytes));
    memset(codepoint, 0, 4);

    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, FL_UNICODE_SEQUENCE("\x00\x00\xFF\xFF\x00"), NULL, FL_ENCODING_UTF8, codepoint);
    flut_assert_size_t_is_equals(3, bytes);
    flut_assert_is_true(fl_unicode_sequence_equals(FL_UNICODE_SEQUENCE("\xEF\xBF\xBF"), 0, codepoint, bytes));
    memset(codepoint, 0, 4);

    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xEF\xBF\xBF\x00"), NULL, FL_ENCODING_UTF32, codepoint);
    flut_assert_size_t_is_equals(4, bytes);
    flut_assert_is_true(fl_unicode_sequence_equals(FL_UNICODE_SEQUENCE("\x00\x00\xFF\xFF"), 0, codepoint, bytes));
    memset(codepoint, 0, 4);

    // UTF8 4-bytes
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, FL_UNICODE_SEQUENCE("\x00\x01\x00\x00\x00"), NULL, FL_ENCODING_UTF8, codepoint);
    flut_assert_size_t_is_equals(4, bytes);
    flut_assert_is_true(fl_unicode_sequence_equals(FL_UNICODE_SEQUENCE("\xF0\x90\x80\x80"), 0, codepoint, bytes));
    memset(codepoint, 0, 4);

    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xF0\x90\x80\x80\x00"), NULL, FL_ENCODING_UTF32, codepoint);
    flut_assert_size_t_is_equals(4, bytes);
    flut_assert_is_true(fl_unicode_sequence_equals(FL_UNICODE_SEQUENCE("\x00\x01\x00\x00"), 0, codepoint, bytes));
    memset(codepoint, 0, 4);

    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, FL_UNICODE_SEQUENCE("\x00\x01\xF6\x15\x00"), NULL, FL_ENCODING_UTF8, codepoint);
    flut_assert_size_t_is_equals(4, bytes);
    flut_assert_is_true(fl_unicode_sequence_equals(FL_UNICODE_SEQUENCE("\xF0\x9F\x98\x95"), 0, codepoint, bytes));
    memset(codepoint, 0, 4);

    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xF0\x9F\x98\x95\x00"), NULL, FL_ENCODING_UTF32, codepoint);
    flut_assert_size_t_is_equals(4, bytes);
    flut_assert_is_true(fl_unicode_sequence_equals(FL_UNICODE_SEQUENCE("\x00\x01\xF6\x15"), 0, codepoint, bytes));
    memset(codepoint, 0, 4);

    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, FL_UNICODE_SEQUENCE("\x00\x10\xFF\xFF\x00"), NULL, FL_ENCODING_UTF8, codepoint);
    flut_assert_size_t_is_equals(4, bytes);
    flut_assert_is_true(fl_unicode_sequence_equals(FL_UNICODE_SEQUENCE("\xF4\x8F\xBF\xBF"), 0, codepoint, bytes));
    memset(codepoint, 0, 4);

    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xF4\x8F\xBF\xBF\x00"), NULL, FL_ENCODING_UTF32, codepoint);
    flut_assert_size_t_is_equals(4, bytes);
    flut_assert_is_true(fl_unicode_sequence_equals(FL_UNICODE_SEQUENCE("\x00\x10\xFF\xFF"), 0, codepoint, bytes));
    memset(codepoint, 0, 4);

    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, FL_UNICODE_SEQUENCE("\x00\x11\x00\x00\x00"), NULL, FL_ENCODING_UTF8, codepoint);
    flut_explain(flut_assert_size_t_is_equals(FL_UNICODE_INVALID_SIZE, bytes), "UTF-32 U+110000 encoded to UTF-8 is invalid because it is beyond the upper bound");

    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, FL_UNICODE_SEQUENCE("\xF4\x8F\xBF\xC0\x00"), NULL, FL_ENCODING_UTF32, codepoint);
    flut_explain(flut_assert_size_t_is_equals(FL_UNICODE_INVALID_SIZE, bytes), "UTF-8 0xF48FBFC0 encoded to UTF-32 is invalid because it is beyond the upper bound");

    // Clean codepoint
    memset(codepoint, 0, 4);

    str = FL_UNICODE_SEQUENCE("你\x00");
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF8, str, NULL, FL_ENCODING_UTF32, codepoint);
    flut_assert_size_t_is_equals(4, bytes);
    flut_assert_is_true(fl_unicode_sequence_equals(FL_UNICODE_SEQUENCE("\x00\x02\xF8\x04"), 0, codepoint, bytes));
    
    FlByte codepoint_utf8[4] = { 0 };
    bytes = fl_unicode_codepoint_convert(FL_ENCODING_UTF32, codepoint, NULL, FL_ENCODING_UTF8, codepoint_utf8);
    flut_assert_size_t_is_equals(4, bytes);
    flut_assert_is_true(fl_unicode_sequence_equals(FL_UNICODE_SEQUENCE("\xF0\xAF\xA0\x84"), 0, codepoint_utf8, bytes));
}

flut_test(unicode_codepoint_sequence_validate) {
    const FlByte *string = NULL;
    FlByte *valid_string = NULL;
    size_t bytes = 0;

    /// UTF-32
    #define UTF32_SEQ_EQUALS(a, b, length) fl_equals((a), (b), (length) * 4)

    // Empty sequence
    string = FL_UNICODE_SEQUENCE("");
    bytes = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF32, string, string, &valid_string);
    flut_assert_size_t_is_equals(0, bytes);
    fl_free(valid_string);

    // 1-Byte length sequence
    string = FL_UNICODE_SEQUENCE("\x00\x00\x00\x01");
    bytes = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF32, string, string+4, &valid_string);
    flut_assert_size_t_is_equals(4, bytes);
    flut_explain(
        flut_assert_is_true(UTF32_SEQ_EQUALS(string, valid_string, 1)),
        "UTF-32: Validated Code Point Sequence {U+0001} is equals to {U+0001}"
    );
    fl_free(valid_string);

    // Sequence larger than 1 byte
    string = FL_UNICODE_SEQUENCE("\x00\x00\x00\x01\x00\x00\x00\x02\x00\x00\x00\x03\x00\x00\x00\x04");
    bytes = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF32, string, string+4*4, &valid_string);
    flut_assert_size_t_is_equals(16, bytes);
    flut_explain(
        flut_assert_is_true(UTF32_SEQ_EQUALS(string, valid_string, 4)),
        "UTF-32: Validated Code Point Sequence {U+0001,U+0002,U+0003,U+0004} is equals to {U+0001,U+0002,U+0003,U+0004}"
    );
    fl_free(valid_string);

    // One ill-formed code point in the middle of the sequence
    string = FL_UNICODE_SEQUENCE("\x00\x00\x00\x01\xFF\xFF\xFF\xFF\x00\x00\x00\x03\x00\x00\x00\x04");
    bytes = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF32, string, string+4*4, &valid_string);
    flut_assert_size_t_is_equals(16, bytes);
    flut_explain(
        flut_assert_is_true(UTF32_SEQ_EQUALS(FL_UNICODE_SEQUENCE("\x00\x00\x00\x01\x00\x00\xFF\xFD\x00\x00\x00\x03\x00\x00\x00\x04"), valid_string, 4)),
        "UTF-32: Validated Code Point Sequence {U+0001,U+FFFFFFFF,U+0003,U+0004} is equals to {U+0001,U+FFFD,U+0003,U+0004}"
    );
    fl_free(valid_string);

    // Two ill-formed code point in the middle of the sequence
    string = FL_UNICODE_SEQUENCE("\x00\x00\x00\x01\xFF\xFF\xFF\xFF\x00\x00\x00\x03\xFF\x10\xFF\xEE\x00\x00\x00\x05");
    bytes = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF32, string, string+4*5, &valid_string);
    flut_assert_size_t_is_equals(20, bytes);
    flut_explain(
        flut_assert_is_true(UTF32_SEQ_EQUALS(FL_UNICODE_SEQUENCE("\x00\x00\x00\x01\x00\x00\xFF\xFD\x00\x00\x00\x03\x00\x00\xFF\xFD\x00\x00\x00\x05"), valid_string, 5)),
        "UTF-32: Validated Code Point Sequence {U+0001,U+FFFFFFFF,U+0003,U+FF10FFEE,U+0005} is equals to {U+0001,U+FFFD,U+0003,U+FFFD,U+0005}"
    );
    fl_free(valid_string);

    // Two ill-formed code point, one in the middle of the sequence another in the upper bound
    string = FL_UNICODE_SEQUENCE("\x00\x00\x00\x01\xFF\xFF\xFF\xFF\x00\x00\x00\x03\xFF\x10\xFF\xEE");
    bytes = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF32, string, string+4*4, &valid_string);
    flut_assert_size_t_is_equals(16, bytes);
    flut_explain(
        flut_assert_is_true(UTF32_SEQ_EQUALS(FL_UNICODE_SEQUENCE("\x00\x00\x00\x01\x00\x00\xFF\xFD\x00\x00\x00\x03\x00\x00\xFF\xFD"), valid_string, 4)),
        "UTF-32: Validated Code Point Sequence {U+0001,U+FFFFFFFF,U+0003,U+FF10FFEE} is equals to {U+0001,U+FFFD,U+0003,U+FFFD}"
    );
    fl_free(valid_string);

    // Two ill-formed code point, one in the lower bound of the sequence another in the middle
    string = FL_UNICODE_SEQUENCE("\xFF\x10\xFF\xEE\x00\x00\x00\x02\x00\x00\x00\x03\xFF\xFF\xFF\xFF\x00\x00\x00\x05");
    bytes = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF32, string, string+4*5, &valid_string);
    flut_assert_size_t_is_equals(20, bytes);
    flut_explain(
        flut_assert_is_true(UTF32_SEQ_EQUALS(FL_UNICODE_SEQUENCE("\x00\x00\xFF\xFD\x00\x00\x00\x02\x00\x00\x00\x03\x00\x00\xFF\xFD\x00\x00\x00\x05"), valid_string, 5)),
        "UTF-32: Validated Code Point Sequence {U+FF10FFEE,U+0002,U+0003,U+FFFFFFFF,U+0005} is equals to {U+FFFD,U+0002,U+0003,U+FFFD,U+0005}"
    );
    fl_free(valid_string);

    // Two ill-formed code point at both bounds
    string = FL_UNICODE_SEQUENCE("\xFF\x10\xFF\xEE\x00\x00\x00\x02\x00\x00\x00\x03\xFF\xFF\xFF\xFF");
    bytes = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF32, string, string+4*4, &valid_string);
    flut_assert_size_t_is_equals(16, bytes);
    flut_explain(
        flut_assert_is_true(UTF32_SEQ_EQUALS(FL_UNICODE_SEQUENCE("\x00\x00\xFF\xFD\x00\x00\x00\x02\x00\x00\x00\x03\x00\x00\xFF\xFD"), valid_string, 4)),
        "UTF-32: Validated Code Point Sequence {U+FF10FFEE,U+0002,U+0003,U+FFFFFFFF} is equals to {U+FFFD,U+0002,U+0003,U+FFFD}"
    );
    fl_free(valid_string);

    // Interleaved ill-formed and well-formed code points
    string = FL_UNICODE_SEQUENCE("\xFF\x10\xFF\xEE\x00\x00\x00\x02\xFF\xFF\xFF\xFF\x00\x00\x00\x04\xFF\xFF\xFF\xDD\x00\x00\x00\x06");
    bytes = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF32, string, string+4*6, &valid_string);
    flut_assert_size_t_is_equals(24, bytes);
    flut_explain(
        flut_assert_is_true(UTF32_SEQ_EQUALS(FL_UNICODE_SEQUENCE("\x00\x00\xFF\xFD\x00\x00\x00\x02\x00\x00\xFF\xFD\x00\x00\x00\x04\x00\x00\xFF\xFD\x00\x00\x00\x06"), valid_string, 6)),
        "UTF-32: Validated Code Point Sequence {U+FF10FFEE,U+0002,U+FFFFFFFF,U+0004,U+FFFFFFDD,U+0006} is equals to {U+FFFD,U+0002,U+FFFD,U+0004,U+FFFD,U+0006}"
    );
    fl_free(valid_string);

    // Full ill-formed sequence
    string = FL_UNICODE_SEQUENCE("\xFF\x10\xFF\xEE\xFF\xEF\xBC\xED\xFF\xEA\xAF\xA3\xFF\xFF\xFF\xFF\xF9\xFF\xEF\xFF");
    bytes = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF32, string, string+4*5, &valid_string);
    flut_assert_size_t_is_equals(20, bytes);
    flut_explain(
        flut_assert_is_true(UTF32_SEQ_EQUALS(FL_UNICODE_SEQUENCE("\x00\x00\xFF\xFD\x00\x00\xFF\xFD\x00\x00\xFF\xFD\x00\x00\xFF\xFD\x00\x00\xFF\xFD"), valid_string, 5)),
        "UTF-32: Validated Code Point Sequence {U+FF10FFEE,U+FFEFBCED,U+FFEAAFA3,U+FFFFFFFF,U+F9FFEFFF} is equals to {U+FFFD,U+FFFD,U+FFFD,U+FFFD,U+FFFD}"
    );
    fl_free(valid_string);
    #undef UTF32_SEQ_EQUALS

    /// UTF-8
    #define UTF8_SEQ_EQUALS(a, b, length) fl_equals((a), (b), (length))

    // Empty sequence
    string = FL_UNICODE_SEQUENCE("");
    bytes = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF8, string, string, &valid_string);
    flut_assert_size_t_is_equals(0, bytes);
    fl_free(valid_string);

    string = FL_UNICODE_SEQUENCE("\x61\x62\x63\x64\x65\x66");
    bytes = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF8, string, string+6, &valid_string);
    flut_explain(flut_assert_size_t_is_equals(6, bytes), "UTF-8: Validated form of <61 62 63 64 65 66> has 6 bytes");
    flut_explain(flut_assert_size_t_is_equals(6, fl_unicode_codepoint_sequence_length(FL_ENCODING_UTF8, valid_string, valid_string+bytes)), "UTF-8: Validated form of <61 62 63 64 65 66> has 6 code points");
    flut_explain(flut_assert_is_true(UTF8_SEQ_EQUALS(string, valid_string, 6)), "UTF-8: Validated form of <61 62 63 64 65 66> is the same sequence");
    fl_free(valid_string);

    string = FL_UNICODE_SEQUENCE("\x61\x62\x63\x80\x64\x65\x66");
    bytes = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF8, string, string+7, &valid_string);
    flut_explain(flut_assert_size_t_is_equals(9, bytes), "UTF-8: Validated form of <61 62 63 80 64 65 66> has 9 bytes");
    flut_explain(flut_assert_size_t_is_equals(7, fl_unicode_codepoint_sequence_length(FL_ENCODING_UTF8, valid_string, valid_string+bytes)), "UTF-8: Validated form of <61 62 63 80 64 65 66> has 7 code points");
    flut_explain(flut_assert_is_true(UTF8_SEQ_EQUALS(FL_UNICODE_SEQUENCE("\x61\x62\x63\xEF\xBF\xBD\x64\x65\x66"), valid_string, 9)), "UTF-8: Validated form of <61 62 63 80 64 65 66> is <61 62 63 EF BF BD 64 65 66>");
    fl_free(valid_string);

    string = FL_UNICODE_SEQUENCE("\x61\xF1\x80\x80\xE1\x80\xC2\x62\x80\x63\x80\xBF\x64");
    bytes = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF8, string, string+13, &valid_string);
    flut_explain(flut_assert_size_t_is_equals(22, bytes), "UTF-8: Validated form of <61 F1 80 80 E1 80 C2 62 80 63 80 BF 64> has 22 bytes");
    flut_explain(flut_assert_size_t_is_equals(10, fl_unicode_codepoint_sequence_length(FL_ENCODING_UTF8, valid_string, valid_string+bytes)), "UTF-8: Validated form of <61 F1 80 80 E1 80 C2 62 80 63 80 BF 64> has 10 code points");
    flut_explain(flut_assert_is_true(UTF8_SEQ_EQUALS(FL_UNICODE_SEQUENCE("\x61\xEF\xBF\xBD\xEF\xBF\xBD\xEF\xBF\xBD\x62\xEF\xBF\xBD\x63\xEF\xBF\xBD\xEF\xBF\xBD\x64"), valid_string, 22)), "UTF-8: Validated form of <61 F1 80 80 E1 80 C2 62 80 63 80 BF 64> is <61 EF BF BD EF BF BD EF BF BD 62 EF BF BD 63 EF BF BD EF BF BD 64>");
    fl_free(valid_string);

    string = FL_UNICODE_SEQUENCE("\x41\xC0\xAF\x41\xF4\x80\x80\x41");
    bytes = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF8, string, string+8, &valid_string);
    flut_explain(flut_assert_size_t_is_equals(12, bytes), "UTF-8: Validated form of <41 C0 AF 41 F4 80 80 41> has 12 bytes");
    flut_explain(flut_assert_size_t_is_equals(6, fl_unicode_codepoint_sequence_length(FL_ENCODING_UTF8, valid_string, valid_string+bytes)), "UTF-8: Validated form of <41 C0 AF 41 F4 80 80 41> has 6 code points");
    flut_explain(flut_assert_is_true(UTF8_SEQ_EQUALS(FL_UNICODE_SEQUENCE("\x41\xEF\xBF\xBD\xEF\xBF\xBD\x41\xEF\xBF\xBD\x41"), valid_string, 12)), "UTF-8: Validated form of <41 C0 AF 41 F4 80 80 41> is <41 EF BF BD EF BF BD 41 EF BF BD 41>");
    fl_free(valid_string);

    string = FL_UNICODE_SEQUENCE("\x41\xE0\x9F\x80\x41");
    bytes = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF8, string, string+5, &valid_string);
    flut_explain(flut_assert_size_t_is_equals(11, bytes), "UTF-8: Validated form of <41 E0 9F 80 41> has 11 bytes");
    flut_explain(flut_assert_size_t_is_equals(5, fl_unicode_codepoint_sequence_length(FL_ENCODING_UTF8, valid_string, valid_string+bytes)), "UTF-8: Validated form of <41 E0 9F 80 41> has 5 code points");
    flut_explain(flut_assert_is_true(UTF8_SEQ_EQUALS(FL_UNICODE_SEQUENCE("\x41\xEF\xBF\xBD\xEF\xBF\xBD\xEF\xBF\xBD\x41"), valid_string, 11)), "UTF-8: Validated form of <41 E0 9F 80 41> is <41 EF BF BD EF BF BD EF BF BD 41>");
    fl_free(valid_string);   

    //TODO: Test edge cases like full ill-formed, starts with ill-formed code point or end with it

    // Starts with an ill-formed sequence
    string = FL_UNICODE_SEQUENCE("\xF1\x80\x80\x41\x42\x43\x44\x00");
    bytes = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF8, string, string+8, &valid_string);
    flut_explain(flut_assert_size_t_is_equals(8, bytes), "UTF-8: Validated form of <F1 80 80 41 42 43 44 00> has 8 bytes");
    flut_explain(flut_assert_size_t_is_equals(6, fl_unicode_codepoint_sequence_length(FL_ENCODING_UTF8, valid_string, valid_string+bytes)), "UTF-8: Validated form of <F1 80 80 41 42 43 44 00> has 6 code points");
    flut_explain(flut_assert_is_true(UTF8_SEQ_EQUALS(FL_UNICODE_SEQUENCE("\xEF\xBF\xBD\x41\x42\x43\x44\x00"), valid_string, 8)), "UTF-8: Validated form of <F1 80 80 41 42 43 44 00> is <EF BF BD 41 42 43 44 00>");
    fl_free(valid_string);

    // Ends with an ill-formed sequence
    string = FL_UNICODE_SEQUENCE("\x41\x42\x43\x44\x00\xF1\x80\x80");
    bytes = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF8, string, string+8, &valid_string);
    flut_explain(flut_assert_size_t_is_equals(8, bytes), "UTF-8: Validated form of <41 42 43 44 00 F1 80 80> has 8 bytes");
    flut_explain(flut_assert_size_t_is_equals(6, fl_unicode_codepoint_sequence_length(FL_ENCODING_UTF8, valid_string, valid_string+bytes)), "UTF-8: Validated form of <41 42 43 44 00 F1 80 80> has 6 code points");
    flut_explain(flut_assert_is_true(UTF8_SEQ_EQUALS(FL_UNICODE_SEQUENCE("\x41\x42\x43\x44\x00\xEF\xBF\xBD"), valid_string, 8)), "UTF-8: Validated form of <41 42 43 44 00 F1 80 80> is <41 42 43 44 00 EF BF BD>");
    fl_free(valid_string);

    // Ends with an ill-formed sequence
    string = FL_UNICODE_SEQUENCE("\x41\x42\x43\x44\x00\xF1\x80");
    bytes = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF8, string, string+7, &valid_string);
    flut_explain(flut_assert_size_t_is_equals(8, bytes), "UTF-8: Validated form of <41 42 43 44 00 F1 80> has 8 bytes");
    flut_explain(flut_assert_size_t_is_equals(6, fl_unicode_codepoint_sequence_length(FL_ENCODING_UTF8, valid_string, valid_string+bytes)), "UTF-8: Validated form of <41 42 43 44 00 F1 80> has 6 code points");
    flut_explain(flut_assert_is_true(UTF8_SEQ_EQUALS(FL_UNICODE_SEQUENCE("\x41\x42\x43\x44\x00\xEF\xBF\xBD"), valid_string, 8)), "UTF-8: Validated form of <41 42 43 44 00 F1 80> is <41 42 43 44 00 EF BF BD>");
    fl_free(valid_string);

    // Ends with an ill-formed sequence
    string = FL_UNICODE_SEQUENCE("\x41\x42\x43\x44\x00\x80");
    bytes = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF8, string, string+6, &valid_string);
    flut_explain(flut_assert_size_t_is_equals(8, bytes), "UTF-8: Validated form of <41 42 43 44 00 80> has 8 bytes");
    flut_explain(flut_assert_size_t_is_equals(6, fl_unicode_codepoint_sequence_length(FL_ENCODING_UTF8, valid_string, valid_string+bytes)), "UTF-8: Validated form of <41 42 43 44 00 80> has 6 code points");
    flut_explain(flut_assert_is_true(UTF8_SEQ_EQUALS(FL_UNICODE_SEQUENCE("\x41\x42\x43\x44\x00\xEF\xBF\xBD"), valid_string, 8)), "UTF-8: Validated form of <41 42 43 44 00 80> is <41 42 43 44 00 EF BF BD>");
    fl_free(valid_string);

    // Full ill-formed sequence
    string = FL_UNICODE_SEQUENCE("\xE0\x9F\x80\xC0\xAF\xF1\x80\x80");
    bytes = fl_unicode_codepoint_sequence_validate(FL_ENCODING_UTF8, string, string+8, &valid_string);
    flut_explain(flut_assert_size_t_is_equals(18, bytes), "UTF-8: Validated form of <E0 9F 80 C0 AF F1 80 80> has 18 bytes");
    flut_explain(flut_assert_size_t_is_equals(6, fl_unicode_codepoint_sequence_length(FL_ENCODING_UTF8, valid_string, valid_string+bytes)), "UTF-8: Validated form of <E0 9F 80 C0 AF F1 80 80> has 6 code points");
    flut_explain(flut_assert_is_true(UTF8_SEQ_EQUALS(FL_UNICODE_SEQUENCE("\xEF\xBF\xBD\xEF\xBF\xBD\xEF\xBF\xBD\xEF\xBF\xBD\xEF\xBF\xBD\xEF\xBF\xBD"), valid_string, 18)), "UTF-8: Validated form of <E0 9F 80 C0 AF F1 80 80> is <EF BF BD EF BF BD EF BF BD EF BF BD EF BF BD EF BF BD>");
    fl_free(valid_string);   
}

flut_test(unicode_data) {
    uint32_t destination[50] = {0};
    size_t mapping_size = UINT32_MAX;

    #define MAKE_CODEPOINT(...) ((const uint32_t[]){ __VA_ARGS__ })
    #define MAPPING_EQUALS(a, b, size) fl_equals(a, b, size)

    mapping_size = fl_unicode_get_decomposition_mapping(0x0005, true, destination);
    flut_explain(
        flut_assert_is_true(MAPPING_EQUALS(MAKE_CODEPOINT(0x0005), destination, mapping_size)),
        "Code point U+0005 <control> Canonical Decomposition_Mapping is its default value 0x0005"
    );
    flut_explain(flut_assert_size_t_is_equals(1, mapping_size), "Code point U+0005 <control> Canonical Decomposition_Mapping is its default value 0x0005");
    memset(destination, 0, sizeof(uint32_t) * 50);
    
    mapping_size = fl_unicode_get_decomposition_mapping(0x0005, false, destination);
    flut_explain(
        flut_assert_is_true(MAPPING_EQUALS(MAKE_CODEPOINT(0x0005), destination, mapping_size)),
        "Code point U+0005 <control> Compatibility Decomposition_Mapping is its default value 0x0005"
    );
    flut_explain(flut_assert_size_t_is_equals(1, mapping_size), "Code point U+0005 <control> Compatibility Decomposition_Mapping is its default value 0x0005");
    memset(destination, 0, sizeof(uint32_t) * 50);
    
    mapping_size = fl_unicode_get_decomposition_mapping(0x1DA07, true, destination);
    flut_explain(
        flut_assert_is_true(MAPPING_EQUALS(MAKE_CODEPOINT(0x1DA07), destination, mapping_size)),
        "Code point U+1DA07 SIGNWRITING FACE DIRECTION POSITION NOSE FORWARD TILTING Canonical Decomposition_Mapping is its default value 0x1DA07"
    );
    flut_explain(flut_assert_size_t_is_equals(1, mapping_size), "Code point U+1DA07 SIGNWRITING FACE DIRECTION POSITION NOSE FORWARD TILTING Canonical Decomposition_Mapping is its default value 0x1DA07");
    memset(destination, 0, sizeof(uint32_t) * 50);
    
    mapping_size = fl_unicode_get_decomposition_mapping(0x1DA07, false, destination);
    flut_explain(
        flut_assert_is_true(MAPPING_EQUALS(MAKE_CODEPOINT(0x1DA07), destination, mapping_size)),
        "Code point U+1DA07 SIGNWRITING FACE DIRECTION POSITION NOSE FORWARD TILTING Compatibility Decomposition_Mapping is its default value 0x1DA07"
    );
    flut_explain(flut_assert_size_t_is_equals(1, mapping_size), "Code point U+1DA07 SIGNWRITING FACE DIRECTION POSITION NOSE FORWARD TILTING Compatibility Decomposition_Mapping is its default value 0x1DA07");
    memset(destination, 0, sizeof(uint32_t) * 50);
    
    mapping_size = fl_unicode_get_decomposition_mapping(0x3060, true, destination);
    flut_explain(
        flut_assert_is_true(MAPPING_EQUALS(MAKE_CODEPOINT(0x305F, 0x3099), destination, mapping_size)),
        "Code point U+3060 HIRAGANA LETTER DA Canonical Decomposition_Mapping is {U+305F, U+3099}"
    );
    flut_explain(flut_assert_size_t_is_equals(2, mapping_size), "Code point U+3060 HIRAGANA LETTER DA Canonical Decomposition_Mapping is {U+305F, U+3099}");
    memset(destination, 0, sizeof(uint32_t) * 50);
    
    mapping_size = fl_unicode_get_decomposition_mapping(0x3060, false, destination);
    flut_explain(
        flut_assert_is_true(MAPPING_EQUALS(MAKE_CODEPOINT(0x3060), destination, mapping_size)),
        "Code point U+3060 HIRAGANA LETTER DA Compatiblity Decomposition_Mapping is its default value 0x3060"
    );
    flut_explain(flut_assert_size_t_is_equals(1, mapping_size), "Code point U+3060 HIRAGANA LETTER DA Compatiblity Decomposition_Mapping is its default value 0x3060");
    memset(destination, 0, sizeof(uint32_t) * 50);
    
    mapping_size = fl_unicode_get_decomposition_mapping(0x3200, false, destination);
    flut_explain(
        flut_assert_is_true(MAPPING_EQUALS(MAKE_CODEPOINT(0x0028, 0x1100, 0x0029), destination, mapping_size)),
        "Code point U+3200 PARENTHESIZED HANGUL KIYEOK Compatibility Decomposition_Mapping is {U+0028, U+1100, U+0029}"
    );
    flut_explain(flut_assert_size_t_is_equals(3, mapping_size), "Code point U+3200 PARENTHESIZED HANGUL KIYEOK Compatibility Decomposition_Mapping is {U+0028, U+1100, U+0029}");
    memset(destination, 0, sizeof(uint32_t) * 50);
    
    mapping_size = fl_unicode_get_decomposition_mapping(0x3200, true, destination);
    flut_explain(
        flut_assert_is_true(MAPPING_EQUALS(MAKE_CODEPOINT(0x3200), destination, mapping_size)),
        "Code point U+3200 PARENTHESIZED HANGUL KIYEOK Canonical Decomposition_Mapping is its default value 0x3200"
    );
    flut_explain(flut_assert_size_t_is_equals(1, mapping_size), "Code point U+3200 PARENTHESIZED HANGUL KIYEOK Canonical Decomposition_Mapping is its default value 0x3200");
    memset(destination, 0, sizeof(uint32_t) * 50);
    
    mapping_size = fl_unicode_get_decomposition_mapping(0x2057, false, destination);
    flut_explain(
        flut_assert_is_true(MAPPING_EQUALS(MAKE_CODEPOINT(0x2032, 0x2032, 0x2032, 0x2032), destination, mapping_size)),
        "Code point U+2057 QUADRUPLE PRIME Compatibility Decomposition_Mapping is {U+2032, U+2032, U+2032, U+2032}"
    );
    flut_explain(flut_assert_size_t_is_equals(4, mapping_size), "Code point U+2057 QUADRUPLE PRIME Compatibility Decomposition_Mapping is {U+2032, U+2032, U+2032, U+2032}");
    memset(destination, 0, sizeof(uint32_t) * 50);
    
    mapping_size = fl_unicode_get_decomposition_mapping(0x2057, true, destination);
    flut_explain(
        flut_assert_is_true(MAPPING_EQUALS(MAKE_CODEPOINT(0x2057), destination, mapping_size)),
        "Code point U+2057 QUADRUPLE PRIME Canonical Decomposition_Mapping is its default value 0x2057"
    );
    flut_explain(flut_assert_size_t_is_equals(1, mapping_size), "Code point U+2057 QUADRUPLE PRIME Canonical Decomposition_Mapping is its default value 0x2057");
    memset(destination, 0, sizeof(uint32_t) * 50);
    
    mapping_size = fl_unicode_get_decomposition_mapping(0x321D, false, destination);
    flut_explain(
        flut_assert_is_true(MAPPING_EQUALS(MAKE_CODEPOINT(0x0028, 0x110B, 0x1169, 0x110C, 0x1165, 0x11AB, 0x0029), destination, mapping_size)),
        "Code point U+321D PARENTHESIZED KOREAN CHARACTER OJEON Compatibility Decomposition_Mapping is {U+0028, U+110B, U+1169, U+110C, U+1165, U+11AB, U+0029}"
    );
    flut_explain(flut_assert_size_t_is_equals(7, mapping_size), "Code point U+321D PARENTHESIZED KOREAN CHARACTER OJEON Compatibility Decomposition_Mapping is {U+0028, U+110B, U+1169, U+110C, U+1165, U+11AB, U+0029}");
    memset(destination, 0, sizeof(uint32_t) * 50);
    
    mapping_size = fl_unicode_get_decomposition_mapping(0x321D, true, destination);
    flut_explain(
        flut_assert_is_true(MAPPING_EQUALS(MAKE_CODEPOINT(0x321D), destination, mapping_size)),
        "Code point U+321D PARENTHESIZED KOREAN CHARACTER OJEON Canonical Decomposition_Mapping is its default value 0x321D"
    );
    flut_explain(flut_assert_size_t_is_equals(1, mapping_size), "Code point U+321D PARENTHESIZED KOREAN CHARACTER OJEON Canonical Decomposition_Mapping is its default value 0x321D");
    memset(destination, 0, sizeof(uint32_t) * 50);
    
    mapping_size = fl_unicode_get_decomposition_mapping(0x321E, false, destination);
    flut_explain(
        flut_assert_is_true(MAPPING_EQUALS(MAKE_CODEPOINT(0x0028, 0x110B, 0x1169, 0x1112, 0x116E, 0x0029), destination, mapping_size)),
        "Code point U+321E PARENTHESIZED KOREAN CHARACTER O HU Compatibility Decomposition_Mapping is {U+0028, U+110B, U+1169, U+1112, U+116E, U+0029}"
    );
    flut_explain(flut_assert_size_t_is_equals(6, mapping_size), "Code point U+321E PARENTHESIZED KOREAN CHARACTER O HU Compatibility Decomposition_Mapping is {U+0028, U+110B, U+1169, U+1112, U+116E, U+0029}");
    memset(destination, 0, sizeof(uint32_t) * 50);
    
    mapping_size = fl_unicode_get_decomposition_mapping(0x321E, true, destination);
    flut_explain(
        flut_assert_is_true(MAPPING_EQUALS(MAKE_CODEPOINT(0x321E), destination, mapping_size)),
        "Code point U+321E PARENTHESIZED KOREAN CHARACTER O HU Canonical Decomposition_Mapping is its default value 0x321E"
    );
    flut_explain(flut_assert_size_t_is_equals(1, mapping_size), "Code point U+321E PARENTHESIZED KOREAN CHARACTER O HU Canonical Decomposition_Mapping is its default value 0x321E");
    memset(destination, 0, sizeof(uint32_t) * 50);
    
    mapping_size = fl_unicode_get_decomposition_mapping(0xFDFA, false, destination);
    flut_explain(
        flut_assert_is_true(MAPPING_EQUALS(MAKE_CODEPOINT(0x0635, 0x0644, 0x0649, 0x0020, 0x0627, 0x0644, 0x0644, 0x0647, 0x0020, 0x0639, 0x0644, 0x064A, 0x0647, 0x0020, 0x0648, 0x0633, 0x0644, 0x0645), destination, mapping_size)),
        "Code point U+FDFA ARABIC LIGATURE SALLALLAHOU ALAYHE WASALLAM Compatibility Decomposition_Mapping is {U+0635, U+0644, U+0649, U+0020, U+0627, U+0644, U+0644, U+0647, U+0020, U+0639, U+0644, U+064A, U+0647, U+0020, U+0648, U+0633, U+0644, U+0645}"
    );
    flut_explain(flut_assert_size_t_is_equals(18, mapping_size), "Code point U+FDFA ARABIC LIGATURE SALLALLAHOU ALAYHE WASALLAM Compatibility Decomposition_Mapping is {U+0635, U+0644, U+0649, U+0020, U+0627, U+0644, U+0644, U+0647, U+0020, U+0639, U+0644, U+064A, U+0647, U+0020, U+0648, U+0633, U+0644, U+0645}");
    memset(destination, 0, sizeof(uint32_t) * 50);
    
    mapping_size = fl_unicode_get_decomposition_mapping(0xFDFA, true, destination);
    flut_explain(
        flut_assert_is_true(MAPPING_EQUALS(MAKE_CODEPOINT(0xFDFA), destination, mapping_size)),
        "Code point U+FDFA ARABIC LIGATURE SALLALLAHOU ALAYHE WASALLAM Canonical Decomposition_Mapping is its default value 0xFDFA"
    );
    flut_explain(flut_assert_size_t_is_equals(1, mapping_size), "Code point U+FDFA ARABIC LIGATURE SALLALLAHOU ALAYHE WASALLAM Canonical Decomposition_Mapping is its default value 0xFDFA");
    memset(destination, 0, sizeof(uint32_t) * 50);
    
    mapping_size = fl_unicode_get_decomposition_mapping(0xFDFB, false, destination);
    flut_explain(
        flut_assert_is_true(MAPPING_EQUALS(MAKE_CODEPOINT(0x062C, 0x0644, 0x0020, 0x062C, 0x0644, 0x0627, 0x0644, 0x0647), destination, mapping_size)),
        "Code point U+FDFB ARABIC LIGATURE JALLAJALALOUHOU Compatibility Decomposition_Mapping is {U+062C, U+0644, U+0020, U+062C, U+0644, U+0627, U+0644, U+0647}"
    );
    flut_explain(flut_assert_size_t_is_equals(8, mapping_size), "Code point U+FDFB ARABIC LIGATURE JALLAJALALOUHOU Compatibility Decomposition_Mapping is {U+062C, U+0644, U+0020, U+062C, U+0644, U+0627, U+0644, U+0647}");
    memset(destination, 0, sizeof(uint32_t) * 50);
    
    mapping_size = fl_unicode_get_decomposition_mapping(0xFDFB, true, destination);
    flut_explain(
        flut_assert_is_true(MAPPING_EQUALS(MAKE_CODEPOINT(0xFDFB), destination, mapping_size)),
        "Code point U+FDFB ARABIC LIGATURE JALLAJALALOUHOU Canonical Decomposition_Mapping is its default value 0xFDFB"
    );
    flut_explain(flut_assert_size_t_is_equals(1, mapping_size), "Code point U+FDFB ARABIC LIGATURE JALLAJALALOUHOU Canonical Decomposition_Mapping is its default value 0xFDFB");

    #undef MAKE_CODEPOINT
    #undef MAPPING_EQUALS
}
