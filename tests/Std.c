#include <stdio.h>

#include "../src/Std.h"
#include "../src/Cstr.h"
#include "../src/threading/Thread.h"
#include "Std.h"

#include "Test.h"

void thread_error()
{
    FlError error;
    FlCstr str;
    FlThreadId id = fl_thread_current_id();
    fl_error_push(1, "ID=%d | Testing error 1", id);
    error = fl_error_last();
    str = fl_cstr_vdup("ID=%d | Testing error 1", id);
    fl_expect("Error ID=1", error.id == 1 && flm_cstr_equals(error.message, str));
    fl_cstr_delete(str);
    
    fl_error_push(2, "ID=%d | Testing error 2", id);
    error = fl_error_last();
    str = fl_cstr_vdup("ID=%d | Testing error 2", id);
    fl_expect("Error ID=2", error.id == 2 && flm_cstr_equals(error.message, str));
    fl_cstr_delete(str);
    
    fl_error_push(3, "ID=%d | Testing error 3", id);
    error = fl_error_last();
    str = fl_cstr_vdup("ID=%d | Testing error 3", id);
    fl_expect("Error ID=3", error.id == 3 && flm_cstr_equals(error.message, str));
    fl_cstr_delete(str);
    
    fl_error_push(4, "ID=%d | Testing error 4", id);
    error = fl_error_last();
    str = fl_cstr_vdup("ID=%d | Testing error 4", id);
    fl_expect("Error ID=4", error.id == 4 && flm_cstr_equals(error.message, str));
    fl_cstr_delete(str);
    
    fl_error_push(5, "ID=%d | Testing error 5", id);
    error = fl_error_last();
    str = fl_cstr_vdup("ID=%d | Testing error 5", id);
    fl_expect("Error ID=5", error.id == 5 && flm_cstr_equals(error.message, str));
    fl_cstr_delete(str);
}

void test_errors()
{
    int nthreads = 5;
    FlThread threads[nthreads];
    for (int i=0; i < nthreads; i++)
    {
        threads[i] = fl_thread_create(thread_error, NULL);
    }
    fl_thread_join_all(threads, nthreads);
}

static FlTryContext simpletest;

void a();
void b();
void c();
void d();
void e();

void a() 
{
    b();
}

void b() 
{
    c();
}

void c() 
{ 
    volatile FlTryContext prevctx = simpletest;
    Try(&simpletest)
    {   
        d();
    }
    Rest
    {
        fl_expect("Catched exception in c() should be 1", simpletest.exception == 1);
        simpletest = prevctx;
        Throw(&simpletest, 2);
    }
    EndTry;
}

void d() 
{
    e();
}

void e() 
{
    Throw(&simpletest, 1);
}

void test_std_exception()
{
    Try(&simpletest)
    {
        a();
    }
    Rest
    {
        fl_expect("Catched exception in test_std_exception() should be 2", simpletest.exception == 2);
    }
    EndTry;
}