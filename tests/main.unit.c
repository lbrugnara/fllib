#include <flut/flut.h>

int main(int argc, char **argv) 
{
    bool success;
    flut_run_tests(
        argc,
        argv,
        &success,
        flut_register_suite(std),
        flut_register_suite(array),
        flut_register_suite(slice),
        flut_register_suite(cstring),
        flut_register_suite(io),
        flut_register_suite(unicode),
        flut_register_suite(string),
        flut_register_suite(regex),
        flut_register_suite(vector),
        flut_register_suite(hashtable),
        flut_register_suite(list),
        flut_register_suite(binheap)
    );
    return success ? 0 : -1;
}
