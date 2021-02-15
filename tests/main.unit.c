#include <flut/flut.h>

int main(int argc, char **argv) 
{
    bool success;
    flut_run_suites(
        argc,
        argv,
        &success,
        std,
        array,
        slice,
        cstring,
        io,
        unicode,
        string,
        regex,
        vector,
        hashtable,
        list,
        binheap
    );
    return success ? 0 : -1;
}
