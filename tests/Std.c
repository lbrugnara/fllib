#include <stdio.h>

#include "../src/Std.h"
#include "Std.h"

#include "Test.h"

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