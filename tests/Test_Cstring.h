#ifndef fl_cstring_TEST_H
#define fl_cstring_TEST_H

#include <flut/flut.h>

void test_cstring_new();
void test_cstring_dup();
void test_cstring_split();
void test_cstring_replace_char();
void test_cstring_replace();
void test_cstring_append();
void test_cstring_join();
void test_cstring_misc();
void test_cstring_find(FlutContext *ctx, FlutAssertUtils *assert);

#endif
