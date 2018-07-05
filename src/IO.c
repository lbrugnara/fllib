#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "Mem.h"
#include "IO.h"
#include "Error.h"

#ifdef _WIN32
    #include <direct.h>
    #include <io.h>
    #define access _access
    #define mkdir(path, mode) _mkdir((path))
#elif defined(__linux__) || defined(__CYGWIN__)
    #include <unistd.h>
    #include <sys/stat.h>
    #include <sys/types.h>
#endif

FlFile *fl_io_file_open(const char *filename, const char *mode)
{
    #if defined(_WIN32) && __STDC_WANT_SECURE_LIB__
    {
        FlFile *fd;
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

bool fl_io_file_exists(const char *filename)
{
    return access(filename, 0) != -1;
}

bool fl_io_dir_create(const char *pathname)
{
    int res = mkdir(pathname, 0775);
    if (res == 0)
        return true;
    char errmsg[64];
    fl_error_push(errno, fl_errno_str(errno, errmsg, 64));
    return false;
}

void fl_io_file_close(FlFile *fd)
{
    fclose(fd);
}

long fl_io_file_size(FlFile *fd)
{
    flm_assert(fd != NULL, "File descriptor cannot be NULL");
    long originalPos = ftell(fd);
    fseek(fd, 0, SEEK_END);
    long size = ftell(fd);
    fseek(fd, originalPos, SEEK_SET);
    return size;
}

void fl_io_file_write_all_bytes(const char *filename, FlByte *bytes)
{
    FlFile *fd = fl_io_file_open(filename, "wb");
    if (!fd)
        return;
    int length = fl_array_length(bytes);
    fwrite(bytes, length, 1, fd);
    fl_io_file_close(fd);
}

FlByte *fl_io_file_read_all_bytes(const char *filename)
{
    FlFile *fd = fl_io_file_open(filename, "rb");
    if (!fd)
        return NULL;
    fseek(fd, 0, SEEK_END);
    int length = ftell(fd);
    FlByte *buffer = (FlByte*)fl_array_new(1, length);
    fseek(fd, 0, SEEK_SET);
    fread(buffer, length, 1, fd);
    fl_io_file_close(fd);
    return buffer;
}

size_t fl_io_file_read_bytes(FlFile *file, size_t bytesToRead, FlByte *dst)
{
    flm_assert(file != NULL, "File descriptor cannot be NULL");
    size_t read = fread(dst, 1, bytesToRead, file);
    return read;
}
