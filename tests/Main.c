#include "Test.h"

// Tests
#include "Std.h"
#include "Cstr.h"
#include "File.h"
#include "text/Test_Unicode.h"
#include "text/String.h"

#include <fllib.h>
#include <stdio.h>
#include <locale.h>
#include <time.h>

int counter;

void routine(FlThreadArgs args)
{
    FlMutex *mutex = (FlMutex*)args;
    fl_mutex_lock(mutex);
    unsigned long i = 0;
    counter += 1;
    printf("\n Job %d started\n", counter);

    for(i=0; i<(100);i++);
    printf("\n Job %d finished\n", counter);
    fl_mutex_unlock(mutex);
}

int main(void)
{
    FlTestSuite suite_cstr = fl_test_suite_new("Module Cstr", 
        { "fl_cstr_new",            &test_cstr_new          }, 
        { "fl_cstr_dup",            &test_cstr_dup          },
        { "fl_cstr_split",          &test_cstr_split        },
        { "fl_cstr_replace_char",   &test_cstr_replace_char },
        { "fl_cstr_append{_char}",  &test_cstr_append       },
        { "fl_cstr_join",           &test_cstr_join         }        
    );
    fl_test_suite_run(suite_cstr);
    fl_test_suite_delete(suite_cstr);

    FlTestSuite suite_std = fl_test_suite_new("Module Std", 
        { "Exception handling", &test_std_exception },
        { "Global error handling thread safety", &test_errors }
    );
    fl_test_suite_run(suite_std);
    fl_test_suite_delete(suite_std);

    FlTestSuite suite_file = fl_test_suite_new("Module File", 
        { "fl_file_write_all_bytes and fl_file_read_all_bytes", &test_file_rw_all_bytes }
    );
    fl_test_suite_run(suite_file);
    fl_test_suite_delete(suite_file);


    FlTestSuite suite_unicode = fl_test_suite_new("Module Unicode",
        { "fl_unicode_char_at", &test_fl_unicode_char_at },
        { "fl_unicode_char", &test_fl_unicode_char },
        { "fl_unicode_encode_char_to", &test_fl_unicode_encode_char_to }
    );
    fl_test_suite_run(suite_unicode);
    fl_test_suite_delete(suite_unicode);

    FlTestSuite suite_string = fl_test_suite_new("Module String", 
        { "fl_string_length", &test_fl_string_length },
        { "fl_string_size", &test_fl_string_size },
        { "fl_string_char_at", &test_fl_string_char_at }
    );
    fl_test_suite_run(suite_string);
    fl_test_suite_delete(suite_string);


    int nthreads = 5;    
    FlMutex mutex;
    fl_mutex_init(&mutex);
    FlThread threads[nthreads];
    for (int i=0; i < nthreads; i++)
    {
        threads[i] = fl_thread_create(routine, &mutex);
    }
    fl_thread_join_all(threads, nthreads);
    fl_mutex_destroy(&mutex);
    /*
    setlocale(LC_ALL, "");
    FlString str = "ASCII";
    printf("Str %s | Length %zu characters | Size %zu bytes\n", str, fl_string_length(str, NULL), fl_string_size(str, NULL));
  
    FlString str2 = "\x41\x00\x42";
    printf("Str %s | Length %zu characters | Size %zu bytes\n", str2, fl_string_length(str2, (FlByte*)str2+4), fl_string_size(str2, (FlByte*)str2+4));
  
    FlString eos = "\0";
    printf("Str %s | Length %zu characters | Size %zu bytes\n", "NULL", fl_string_length(eos, (FlByte*)eos+1), fl_string_size(eos, (FlByte*)eos+1));

    FlString eos2 = "\0";
    printf("Str %s | Length %zu characters | Size %zu bytes\n", "NULL", fl_string_length(eos2, NULL), fl_string_size(eos2, NULL));
    
    FlByteArray bytes = fl_file_read_all_bytes("c:\\tmp\\utf8.txt");
    printf("Str %s | Length %zu characters | Size %zu bytes\n", bytes, fl_string_length((FlString)bytes, NULL), fl_string_size((FlString)bytes, NULL));
    fl_array_delete(bytes);
    
    FlString unistr = "兔¡¢£¤¥¦§¨©ª«¬­®";
    printf("Str %s | Length %zu characters | Size %zu bytes\n", unistr, fl_string_length(unistr, NULL), fl_string_size(unistr, NULL));
    FlString test = "兔";
    printf("%x | Equals to 0x2F80F? %d\n", *(int32_t*)test, *(int32_t*)test == 0x2F80F);

    int a = 11259375;
    printf("%04x\n", a);

    unistr = "𐌀";
    FlUnicodeChar chr = fl_string_char_at(unistr, 0);
    printf("%x | Equals to 0xF0908C80? %d\n", chr, chr == 0xF0908C80);*/
/*
    for (int i=0; i < fl_string_length(unistr, NULL); i++)
    {
        FlUnicodeChar chr = fl_string_char_at(unistr, i);
        printf("Chr %04x\n", chr);
    }
*/
    return 0;
}
