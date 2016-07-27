#include "Test.h"
#include "../src/Cstr.h"

void test_cstr_new(FlExpectFunc expect)
{
	FlCstr str = fl_cstr_new(5);
	expect("str not null", str != NULL);
	expect("str == \"Hello\"", flm_cstr_equals(str, "Hello"));
	fl_cstr_delete(str);
}

void test_cstr_dup(FlExpectFunc expect)
{
	FlCstr str = fl_cstr_dup("Hello");
	expect("str not null", str != NULL);
	expect("str == \"Hello\"", flm_cstr_equals(str, "Hello"));
	fl_cstr_delete(str);
}

int main(void)
{
	FlTestSuite *suite_cstr = fl_test_suite_new("Module Cstr", (FlTest*[]){ 
		fl_test_new("new", &test_cstr_new), 
		fl_test_new("dup", &test_cstr_new),
		NULL
	});
	fl_test_run_suite(suite_cstr);
	fl_test_suite_delete(suite_cstr);
	return 0;
}