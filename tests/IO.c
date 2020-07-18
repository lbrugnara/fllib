#include <fllib.h>
#include <flut/flut.h>

#include "IO.h"

void test_io_path(FlutContext *ctx, FlutAssertUtils *assert)
{
    #ifdef _WIN32
    flut_describe(ctx, "Windows paths should be all relative paths")
    {
        flut_expect(ctx, assert->is_true(fl_io_path_is_relative(".")));
        flut_expect(ctx, assert->is_false(fl_io_path_is_absolute(".")));

        flut_expect(ctx, assert->is_true(fl_io_path_is_relative("..")));
        flut_expect(ctx, assert->is_false(fl_io_path_is_absolute("..")));

        flut_expect(ctx, assert->is_true(fl_io_path_is_relative(".\\")));
        flut_expect(ctx, assert->is_false(fl_io_path_is_absolute(".\\")));

        flut_expect(ctx, assert->is_true(fl_io_path_is_relative("..\\")));
        flut_expect(ctx, assert->is_false(fl_io_path_is_absolute("..\\")));

        flut_expect(ctx, assert->is_true(fl_io_path_is_relative(".\\test")));
        flut_expect(ctx, assert->is_false(fl_io_path_is_absolute(".\\test")));

        flut_expect(ctx, assert->is_true(fl_io_path_is_relative("..\\test")));
        flut_expect(ctx, assert->is_false(fl_io_path_is_absolute("..\\test")));

        flut_expect(ctx, assert->is_true(fl_io_path_is_relative(".\\test\\2")));
        flut_expect(ctx, assert->is_false(fl_io_path_is_absolute(".\\test\\2")));

        flut_expect(ctx, assert->is_true(fl_io_path_is_relative("..\\test\\2")));
        flut_expect(ctx, assert->is_false(fl_io_path_is_absolute("..\\test\\2")));

        flut_expect(ctx, assert->is_true(fl_io_path_is_relative("C:test\\2")));
        flut_expect(ctx, assert->is_false(fl_io_path_is_absolute("C:test\\2")));

        flut_expect(ctx, assert->is_true(fl_io_path_is_relative("test\\2")));
        flut_expect(ctx, assert->is_false(fl_io_path_is_absolute("test\\2")));
    }

    flut_describe(ctx, "Windows paths should be all absolute paths")
    {
        flut_expect(ctx, assert->is_true(fl_io_path_is_absolute("C:\\test")));
        flut_expect(ctx, assert->is_false(fl_io_path_is_relative("C:\\test")));

        flut_expect(ctx, assert->is_true(fl_io_path_is_absolute("\\test")));
        flut_expect(ctx, assert->is_false(fl_io_path_is_relative("\\test")));

        flut_expect(ctx, assert->is_true(fl_io_path_is_absolute("\\\\?\\UNC")));
        flut_expect(ctx, assert->is_false(fl_io_path_is_relative("\\\\?\\UNC")));

        flut_expect(ctx, assert->is_true(fl_io_path_is_absolute("\\\\?\\D:\\something")));
        flut_expect(ctx, assert->is_false(fl_io_path_is_relative("\\\\?\\D:\\something")));

        flut_expect(ctx, assert->is_true(fl_io_path_is_absolute("\\\\.\\pipe\\my-pipe")));
        flut_expect(ctx, assert->is_false(fl_io_path_is_relative("\\\\.\\pipe\\my-pipe")));
    }

    #endif
}

void test_file_rw_all_bytes()
{
    // Retrieve the current working dir and save it to restore it later
    char *wdir = fl_system_get_working_dir();

    // Use a temporary path
    char *path = NULL;
    path = "/tmp";
    #if defined(_WIN32)
    path = "c:\\tmp";
    #endif
    // Create the temporary dir
    fl_io_dir_create(path);
    // Change the working dir to the tmp path
    fl_system_set_working_dir(path);

    // fl_cstring_to_array returns an array of characters, cast it to an array of FlByte, that is syntactic sugar
    // for unsiged char
    char *str = "Hello world.\nBye.\n";
    FlByte *arr = (FlByte*)fl_cstring_to_array(str);

    // Write all thebytes to the test_file.txt file and check if it exists
    fl_io_file_write_all_bytes("test_file.txt", arr);
    #ifndef _WIN32
    flut_expect_compat("File /tmp/test_file.txt exists", fl_io_file_exists("/tmp/test_file.txt"));
    #else
    flut_expect_compat("File C:\\tmp\\test_file.txt exists", fl_io_file_exists("C:\\tmp\\test_file.txt"));
    #endif

    // Read all the bytes back, and check if the contet is the same as the str variable
    FlArray *bytes = fl_io_file_read_all_bytes("test_file.txt");
    flut_expect_compat("Amount of bytes written are the same amount of bytes read", strlen(str) == fl_array_length(bytes));
    flut_expect_compat("File content equals to 'Hello world.\nBye.\n'", flm_cstring_equals_n(str, (char*)bytes, strlen(str)));

    // Cleanup
    // Restore working dir
    if (wdir != NULL)
    {
        fl_system_set_working_dir(wdir);
        fl_cstring_free(wdir);
    }
    fl_array_free(arr);
    fl_array_free(bytes);

}
