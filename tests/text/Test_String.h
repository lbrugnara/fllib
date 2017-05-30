#ifndef FL_STRING_TEST_H
#define FL_STRING_TEST_H

//FlString fl_string_new(const FlString str);

//FlString fl_string_new_from_bytes(const FlByte *bytes, size_t length, FlEncoding encoding);

// void fl_string_delete(FlString str);

void test_fl_string_length();

void test_fl_string_size();

void test_fl_string_char_at();

void test_fl_unicode_codepoint_at();

void test_fl_unicode_char();


#endif /* FL_STRING_TEST_H */
