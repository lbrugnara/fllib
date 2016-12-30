#include <fllib.h>

#include "File.h"
#include "Test.h"

void test_file_rw_all_bytes()
{
    #ifdef __linux__
    FlCstr path = "/tmp";
    #elif define(_WIN32)
    FlCstr path = "c:\\tmp";
    #endif

    FlCstr wdir = fl_system_get_working_dir();
    fl_file_create_dir(path);
    fl_system_set_working_dir(path);

    FlCstr str = "Hello world.\nBye.\n";
    FlByteArray arr = (FlByteArray)fl_cstr_to_array(str);

    fl_file_write_all_bytes("test_file.txt", arr);
    #ifdef __linux__
    fl_expect("File /tmp/test_file.txt exists", fl_file_exists("/tmp/test_file.txt"));
    #elif define(_WIN32)
    fl_expect("File C:\\tmp\\test_file.txt exists", fl_file_exists("C:\\tmp\\test_file.txt"));
    #endif
    FlByteArray bytes = fl_file_read_all_bytes("test_file.txt");
    fl_expect("File content equals to 'Hello world.\\nBye.\\n'", flm_cstr_ncmp(str, (FlCstr)bytes, strlen(str)));

    fl_array_delete(arr);
    fl_array_delete(bytes);

    fl_system_set_working_dir(wdir);
    fl_cstr_delete(wdir);
}