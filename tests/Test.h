#ifndef FL_TEST_H
#define FL_TEST_H
#include <setjmp.h>
#include "../src/os/Signal.h"
#include "../src/Std.h"

typedef bool(*FlExpect)(const char*, bool);
typedef bool(*FlExpectError)(const char*, bool);
typedef void(*FlTestHandler)();

typedef struct FlTest
{
    char name[50];
    FlTestHandler run;
} FlTest;
typedef struct FlTestSuite FlTestSuite;

#define fl_test_suite_new(name, ...) _fl_test_suite_new(name, ((const FlTest[]){__VA_ARGS__}), sizeof(((const FlTest[]){__VA_ARGS__})) / sizeof(((const FlTest[]){__VA_ARGS__}))[0]);
FlTestSuite* _fl_test_suite_new(const char* suitename, const FlTest[], size_t);

void fl_test_suite_delete(FlTestSuite* suite);
void fl_test_suite_run(FlTestSuite*);

bool fl_expect(const char* name, bool condition);

#endif