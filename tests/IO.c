#include <fllib.h>
#include <flut/flut.h>

#include "IO.h"

void test_io_realpath(FlutContext *flut__internal_ctx, FlutAssertUtils *assert)
{
    #ifdef _WIN32
    flut_describe("realpath should convert all paths to absolute paths and all should be valid.")
    {
        const char *files[][3] = { 
            { "a.test", NULL,                               NULL    },
            { "b.test", NULL,                               NULL    },
            { "c.test", NULL,                               NULL    },
            { "d.test", "folder",                           NULL    },
            { "e.test", "folder" FL_IO_DIR_SEPARATOR "1",   NULL    },
            { NULL,     NULL,                               NULL    },
        };

        for (size_t i=0; files[i][0] != NULL; i++)
        {
            if (files[i][1])
                fl_io_dir_create_recursive(files[i][1]);

            const char *folder = (files[i][1] != NULL ? files[i][1] : "");
            const char *separator = (files[i][1] != NULL ? FL_IO_DIR_SEPARATOR : "");
            files[i][2] = fl_cstring_vdup("%s%s%s", folder, separator, files[i][0]);

            fl_io_file_write_all_text(files[i][2], files[i][0]);
        }
        
        for (size_t i=0; files[i][0] != NULL; i++)
        {
            const char *file_abspath = fl_io_realpath(files[i][2]);

            flut_expect_vexplain(assert->not_null((void*) file_abspath), "File \"%s\" must be a valid path: \"%s\"", files[i][2], file_abspath);
            
            const char *text = fl_io_file_read_all_text(file_abspath);
            flut_expect_vexplain(assert->str.equals(files[i][0], text, true), "Content of file \"%s\" must be equals to \"%s\"", file_abspath, files[i][0]);

            fl_cstring_free(file_abspath);
        }

        for (size_t i=0; files[i][0] != NULL; i++)
        {
            fl_io_file_unlink(files[i][2]);
            fl_cstring_free(files[i][2]);
        }
    }
    #endif
}

void test_io_path(FlutContext *flut__internal_ctx, FlutAssertUtils *assert)
{
    #ifdef _WIN32
    flut_describe("Windows paths should be all relative paths")
    {
        flut_expect(assert->is_true(fl_io_path_is_relative(".")));
        flut_expect(assert->is_false(fl_io_path_is_absolute(".")));

        flut_expect(assert->is_true(fl_io_path_is_relative("..")));
        flut_expect(assert->is_false(fl_io_path_is_absolute("..")));

        flut_expect(assert->is_true(fl_io_path_is_relative(".\\")));
        flut_expect(assert->is_false(fl_io_path_is_absolute(".\\")));

        flut_expect(assert->is_true(fl_io_path_is_relative("..\\")));
        flut_expect(assert->is_false(fl_io_path_is_absolute("..\\")));

        flut_expect(assert->is_true(fl_io_path_is_relative(".\\test")));
        flut_expect(assert->is_false(fl_io_path_is_absolute(".\\test")));

        flut_expect(assert->is_true(fl_io_path_is_relative("..\\test")));
        flut_expect(assert->is_false(fl_io_path_is_absolute("..\\test")));

        flut_expect(assert->is_true(fl_io_path_is_relative(".\\test\\2")));
        flut_expect(assert->is_false(fl_io_path_is_absolute(".\\test\\2")));

        flut_expect(assert->is_true(fl_io_path_is_relative("..\\test\\2")));
        flut_expect(assert->is_false(fl_io_path_is_absolute("..\\test\\2")));

        flut_expect(assert->is_true(fl_io_path_is_relative("C:test\\2")));
        flut_expect(assert->is_false(fl_io_path_is_absolute("C:test\\2")));

        flut_expect(assert->is_true(fl_io_path_is_relative("test\\2")));
        flut_expect(assert->is_false(fl_io_path_is_absolute("test\\2")));
    }

    flut_describe("Windows paths should be all absolute paths")
    {
        flut_expect(assert->is_true(fl_io_path_is_absolute("C:\\test")));
        flut_expect(assert->is_false(fl_io_path_is_relative("C:\\test")));

        flut_expect(assert->is_true(fl_io_path_is_absolute("\\test")));
        flut_expect(assert->is_false(fl_io_path_is_relative("\\test")));

        flut_expect(assert->is_true(fl_io_path_is_absolute("\\\\?\\UNC")));
        flut_expect(assert->is_false(fl_io_path_is_relative("\\\\?\\UNC")));

        flut_expect(assert->is_true(fl_io_path_is_absolute("\\\\?\\D:\\something")));
        flut_expect(assert->is_false(fl_io_path_is_relative("\\\\?\\D:\\something")));

        flut_expect(assert->is_true(fl_io_path_is_absolute("\\\\.\\pipe\\my-pipe")));
        flut_expect(assert->is_false(fl_io_path_is_relative("\\\\.\\pipe\\my-pipe")));
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
