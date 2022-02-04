#include <fllib.h>
#include <flut/flut.h>

flut_suite(io) {
    flut_suite_description("File, dirs, and path related functions");
    flut_suite_register_test(io_realpath, "fl_io_realpath function");
    flut_suite_register_test(io_path, "Relative and absolute paths");
    flut_suite_register_test(io_rw_all_bytes, "read/write all bytes");
}

flut_test(io_realpath) {
    flut_describe("realpath should convert all paths to absolute paths and all should be valid.") {
        const char *files[][3] = { 
            // File     Directory                                                   Full path
            { "a.test", NULL,                                                       NULL        },
            { "b.test", NULL,                                                       NULL        },
            { "c.test", NULL,                                                       NULL        },
            { "d.test", "folder",                                                   NULL        },
            { "e.test", "folder" FL_IO_DIR_SEPARATOR "1",                           NULL        },
            { "f.test", "folder" FL_IO_DIR_SEPARATOR "1" FL_IO_DIR_SEPARATOR "2",   NULL        },
            { NULL,     NULL,                                                       NULL        },
        };

        for (size_t i=0; files[i][0] != NULL; i++) {
            if (files[i][1]) {
                fl_io_dir_create_recursive(files[i][1]);
            }

            const char *folder = (files[i][1] != NULL ? files[i][1] : "");
            const char *separator = (files[i][1] != NULL ? FL_IO_DIR_SEPARATOR : "");
            files[i][2] = fl_cstring_vdup("%s%s%s", folder, separator, files[i][0]);

            fl_io_file_write_all_text(files[i][2], files[i][0]);
        }
        
        for (size_t i=0; files[i][0] != NULL; i++)
        {
            const char *file_abspath = fl_io_realpath(files[i][2]);

            flut_assert_string_is_not_null(file_abspath, false);
            flut_assert_string_is_equals(files[i][0], fl_io_file_read_all_text(file_abspath), true);

            fl_cstring_free(file_abspath);
        }

        for (size_t i=0; files[i][0] != NULL; i++)
        {
            fl_io_file_unlink(files[i][2]);
            fl_cstring_free(files[i][2]);
            if (files[i][1] != NULL) {
                fl_io_dir_remove_recursive(files[i][1]);
            }
        }
    }
}

flut_test(io_path) {
    #ifdef _WIN32
    flut_describe("Windows paths should be all relative paths") {
        flut_assert_is_true(fl_io_path_is_relative("."));
        flut_assert_is_false(fl_io_path_is_absolute("."));

        flut_assert_is_true(fl_io_path_is_relative(".."));
        flut_assert_is_false(fl_io_path_is_absolute(".."));

        flut_assert_is_true(fl_io_path_is_relative(".\\"));
        flut_assert_is_false(fl_io_path_is_absolute(".\\"));

        flut_assert_is_true(fl_io_path_is_relative("..\\"));
        flut_assert_is_false(fl_io_path_is_absolute("..\\"));

        flut_assert_is_true(fl_io_path_is_relative(".\\test"));
        flut_assert_is_false(fl_io_path_is_absolute(".\\test"));

        flut_assert_is_true(fl_io_path_is_relative("..\\test"));
        flut_assert_is_false(fl_io_path_is_absolute("..\\test"));

        flut_assert_is_true(fl_io_path_is_relative(".\\test\\2"));
        flut_assert_is_false(fl_io_path_is_absolute(".\\test\\2"));

        flut_assert_is_true(fl_io_path_is_relative("..\\test\\2"));
        flut_assert_is_false(fl_io_path_is_absolute("..\\test\\2"));

        flut_assert_is_true(fl_io_path_is_relative("C:test\\2"));
        flut_assert_is_false(fl_io_path_is_absolute("C:test\\2"));

        flut_assert_is_true(fl_io_path_is_relative("test\\2"));
        flut_assert_is_false(fl_io_path_is_absolute("test\\2"));
    }

    flut_describe("Windows paths should be all absolute paths") {
        flut_assert_is_true(fl_io_path_is_absolute("C:\\test"));
        flut_assert_is_false(fl_io_path_is_relative("C:\\test"));

        flut_assert_is_true(fl_io_path_is_absolute("\\test"));
        flut_assert_is_false(fl_io_path_is_relative("\\test"));

        flut_assert_is_true(fl_io_path_is_absolute("\\\\?\\UNC"));
        flut_assert_is_false(fl_io_path_is_relative("\\\\?\\UNC"));

        flut_assert_is_true(fl_io_path_is_absolute("\\\\?\\D:\\something"));
        flut_assert_is_false(fl_io_path_is_relative("\\\\?\\D:\\something"));

        flut_assert_is_true(fl_io_path_is_absolute("\\\\.\\pipe\\my-pipe"));
        flut_assert_is_false(fl_io_path_is_relative("\\\\.\\pipe\\my-pipe"));
    }

    #else
    flut_describe("Linux paths should be all relative paths") {
        flut_assert_is_true(fl_io_path_is_relative("."));
        flut_assert_is_false(fl_io_path_is_absolute("."));

        flut_assert_is_true(fl_io_path_is_relative(".."));
        flut_assert_is_false(fl_io_path_is_absolute(".."));

        flut_assert_is_true(fl_io_path_is_relative("./"));
        flut_assert_is_false(fl_io_path_is_absolute("./"));

        flut_assert_is_true(fl_io_path_is_relative("../"));
        flut_assert_is_false(fl_io_path_is_absolute("../"));

        flut_assert_is_true(fl_io_path_is_relative("./test"));
        flut_assert_is_false(fl_io_path_is_absolute("./test"));

        flut_assert_is_true(fl_io_path_is_relative("../test"));
        flut_assert_is_false(fl_io_path_is_absolute("../test"));

        flut_assert_is_true(fl_io_path_is_relative("./test/2"));
        flut_assert_is_false(fl_io_path_is_absolute("./test/2"));

        flut_assert_is_true(fl_io_path_is_relative("../test/2"));
        flut_assert_is_false(fl_io_path_is_absolute("../test/2"));

        flut_assert_is_true(fl_io_path_is_relative("test/2"));
        flut_assert_is_false(fl_io_path_is_absolute("test/2"));
    }

    flut_describe("Linux paths should be all absolute paths") {
        flut_assert_is_true(fl_io_path_is_absolute("/test"));
        flut_assert_is_false(fl_io_path_is_relative("/test"));

        flut_assert_is_true(fl_io_path_is_absolute("/test/something"));
        flut_assert_is_false(fl_io_path_is_relative("/test/something"));

        flut_assert_is_true(fl_io_path_is_absolute("/var/something/my-pipe"));
        flut_assert_is_false(fl_io_path_is_relative("/var/something/my-pipe"));
    }
    #endif
}

flut_test(io_rw_all_bytes) {
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
    flut_assert_is_true(fl_io_file_exists("/tmp/test_file.txt"));
    #else
    flut_assert_is_true(fl_io_file_exists("C:\\tmp\\test_file.txt"));
    #endif

    // Read all the bytes back, and check if the contet is the same as the str variable
    FlArray *bytes = fl_io_file_read_all_bytes("test_file.txt");
    flut_assert_size_t_is_equals(strlen(str), fl_array_length(bytes));
    flut_assert_string_is_equals_n(str, fl_cstring_vdup("%.*s", strlen(str), (char*) bytes), strlen(str), true);

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
