#include <stdlib.h>
#include <string.h>

#include "Test.h"

#define TEST_NAME_MAX_LENGTH 50

struct FlTest
{
	char name[TEST_NAME_MAX_LENGTH];
	FlTestHandler test;
};

struct FlTestSuite
{
	char name[TEST_NAME_MAX_LENGTH];
	FlTest **tests;
};

FlTest* fl_test_new(const char *name, FlTestHandler test)
{
	FlTest *t = calloc(1, sizeof(FlTest));
	memcpy(t->name, name, TEST_NAME_MAX_LENGTH);
	t->test = test;
	return t;
}

void fl_test_delete(FlTest* test)
{
	free(test);
}

FlTestSuite* fl_test_suite_new(const char *name, FlTest *tests[])
{
	FlTestSuite *t = calloc(1, sizeof(FlTestSuite));
	memcpy(t->name, name, TEST_NAME_MAX_LENGTH);
	t->tests = tests;
	return t;
}

void fl_test_suite_delete(FlTestSuite* suite)
{
	for (int i=0; ;i++)
	{
		if (suite->tests[i] == NULL)
			break;
		fl_test_delete(suite->tests[i]);
	}
	free(suite);
}

void fl_test_run_suite(FlTestSuite *suite)
{

}