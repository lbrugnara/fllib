#include <fllib.h>

#include "File.h"
#include "Test.h"

void test_file_rw_all_bytes()
{
    // Retrieve the current working dir and save it to restore it later
    FlCstr wdir = fl_system_get_working_dir();

    // Use a temporary path
    FlCstr path = NULL;
    path = "/tmp";
    #if defined(_WIN32)
    path = "c:\\tmp";
    #endif
    // Create the temporary dir
    fl_file_create_dir(path);
    // Change the working dir to the tmp path
    fl_system_set_working_dir(path);

    // fl_cstr_to_array returns an array of characters, cast it to an array of FlByte, that is syntactic sugar
    // for unsiged char
    FlCstr str = "Hello world.\nBye.\n";
    FlByteArray arr = (FlByteArray)fl_cstr_to_array(str);

    // Write all thebytes to the test_file.txt file and check if it exists
    fl_file_write_all_bytes("test_file.txt", arr);
    #ifndef _WIN32
    fl_expect("File /tmp/test_file.txt exists", fl_file_exists("/tmp/test_file.txt"));
    #else
    fl_expect("File C:\\tmp\\test_file.txt exists", fl_file_exists("C:\\tmp\\test_file.txt"));
    #endif

    // Read all the bytes back, and check if the contet is the same as the str variable
    FlByteArray bytes = fl_file_read_all_bytes("test_file.txt");
    fl_expect("Amount of bytes written are the same amount of bytes read", strlen(str) == fl_array_length(bytes));
    fl_expect("File content equals to 'Hello world.\\nBye.\\n'", flm_cstr_nequals(str, (FlCstr)bytes, strlen(str)));

    // Restore working dir
    fl_system_set_working_dir(wdir);
    // Cleanup
    fl_cstr_delete(wdir);
    fl_array_delete(arr);
    fl_array_delete(bytes);

}
