#include "Test.h"

// Tests
#include "Std.h"
#include "Cstr.h"
#include "File.h"

#include <fllib.h>
#include <stdio.h>

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

    return 0;
}