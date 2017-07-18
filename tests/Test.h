#ifndef FL_TEST_H
#define FL_TEST_H

#include "../src/Std.h"

typedef void(*FlTestHandler)();

typedef struct FlTest
{
    char name[100];
    FlTestHandler run;
} FlTest;

typedef struct FlTestSuite* FlTestSuite;

#define fl_test_suite(name, ...) fl_test_suite_new(name, ((const FlTest[]){__VA_ARGS__}), sizeof(((const FlTest[]){__VA_ARGS__})) / sizeof(((const FlTest[]){__VA_ARGS__}))[0])
FlTestSuite fl_test_suite_new(const char* suitename, const FlTest[], size_t);

void fl_test_suite_delete(FlTestSuite suite);
size_t fl_test_suite_run(FlTestSuite suite);

#define fl_test_run_all_suites(...) fl_test_run_all(((FlTestSuite[]){__VA_ARGS__}), sizeof(((const FlTestSuite[]){__VA_ARGS__})) / sizeof(((const FlTestSuite[]){__VA_ARGS__}))[0])
void fl_test_run_all(FlTestSuite *suites, size_t number_of_suites);

bool fl_expect(const char* name, bool condition);
bool fl_vexpect(bool conditionResult, const char* format, ...);

#endif
