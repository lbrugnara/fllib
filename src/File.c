#include <stdlib.h>
#include <stdio.h>

#include "Mem.h"
#include "File.h"
#include "Error.h"

#ifdef _WIN32
    #include <direct.h>
    #include <io.h>
    #define access _access
    #define mkdir(path, mode) _mkdir((path))
#elif defined(__linux__)
    #include <unistd.h>
    #include <sys/stat.h>
    #include <sys/types.h>
#endif

FILE *fl_file_open(FlCstr filename, FlCstr mode)
{
    #if defined(_WIN32) && __STDC_WANT_SECURE_LIB__
    {
        FILE *fd;
        if (fopen_s(&fd, filename, mode) != 0)
            return NULL;
        return fd;
    }
    #else
    {
        return fopen(filename, mode);
    }
    #endif
}


bool fl_file_exists(FlCstr filename)
{
    return access(filename, 0) != -1;
}

bool fl_file_create_dir(FlCstr pathname)
{
    int res = mkdir(pathname, 0775);
    if (res == 0)
        return true;
    char errmsg[64];
    fl_error_push(errno, fl_errno_str(errno, errmsg, 64));
    return false;
}

void fl_file_close(FILE *fd)
{
    fclose(fd);
}

void fl_file_write_all_bytes(FlCstr filename, FlByteArray bytes)
{
    FILE *fd = fl_file_open(filename, "wb");
    if (!fd)
        return;
    int length = fl_array_length(bytes);
    fwrite(bytes, length, 1, fd);
    fl_file_close(fd);
}

FlByteArray fl_file_read_all_bytes(FlCstr filename)
{
    FILE *fd = fl_file_open(filename, "rb");
    if (!fd)
        return NULL;
    fseek(fd, 0, SEEK_END);
    int length = ftell(fd);
    FlByteArray buffer = (FlByteArray)fl_array_new(1, length + 1);
    fseek(fd, 0, SEEK_SET);
    fread(buffer, length, 1, fd);
    fl_file_close(fd);
    buffer[length] = FL_EOS;
    return buffer;
}
