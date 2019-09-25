#ifndef FL_TEST_H
#define FL_TEST_H

#include "../src/Std.h"

#define fl_test_suite(name, ...) fl_test_suite_new(name, ((const FlTest[]){__VA_ARGS__}), sizeof(((const FlTest[]){__VA_ARGS__})) / sizeof(((const FlTest[]){__VA_ARGS__}))[0])
#define fl_test_run_all_suites(argc, argv, ...) fl_test_run_all(argc, argv, ((FlTestSuite[]){__VA_ARGS__}))
#define fl_test_description(descr) printf(" |\n | [%s]\n |\n", (descr));

typedef void(*FlTestHandler)();

typedef struct FlTest
{
    char name[100];
    FlTestHandler run;
} FlTest;

typedef struct FlTestSuite* FlTestSuite;

FlTestSuite fl_test_suite_new(const char* suitename, const FlTest[], size_t ntests);

void fl_test_suite_free(FlTestSuite suite);

void fl_test_run_all(int argc, char **argv, FlTestSuite *suites);

bool fl_expect(const char* name, bool condition);

bool fl_vexpect(bool conditionResult, const char* format, ...);

#endif
