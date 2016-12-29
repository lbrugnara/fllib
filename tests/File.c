#include <fllib.h>

#include "File.h"
#include "Test.h"

void test_file_rw_all_bytes()
{
    FlCstr wdir = fl_system_get_working_dir();
    fl_file_create_dir("c:\\tmp", NULL);
    fl_system_set_working_dir("c:\\tmp", NULL);

    FlCstr str = "Hello world.\nBye.\n";
    FlByteArray arr = (FlByteArray)fl_cstr_to_array(str);

    fl_file_write_all_bytes("test_file.txt", arr);
    fl_expect("File C:\\tmp\\test_file.txt exists", fl_file_exists("C:\\tmp\\test_file.txt"));
    FlByteArray bytes = fl_file_read_all_bytes("test_file.txt");
    fl_expect("File content equals to 'Hello world.\\nBye.\\n'", flm_cstr_ncmp(str, (FlCstr)bytes, strlen(str)));

    fl_array_delete(arr);
    fl_array_delete(bytes);

    fl_system_set_working_dir(wdir, NULL);
    fl_cstr_delete(wdir);
}