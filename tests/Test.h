#ifndef FL_TEST_H
#define FL_TEST_H

#include "../src/Types.h"

typedef struct FlTest FlTest;
typedef struct FlTestSuite FlTestSuite;
typedef bool(*FlExpectFunc)(const char*, bool);
typedef void(*FlTestHandler)(FlExpectFunc);

FlTest* fl_test_new(const char* name, FlTestHandler test);
void fl_test_delete(FlTest* test);

FlTestSuite* fl_test_suite_new(const char* suitename, FlTest*[]);
void fl_test_suite_delete(FlTestSuite* suite);
void fl_test_run_suite(FlTestSuite*);

#endif