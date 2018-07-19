#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "Mem.h"
#include "IO.h"
#include "Error.h"
#include "Cstring.h"

#ifdef _WIN32
    #include <direct.h>
    #include <io.h>
    #define access _access
    #define mkdir(path, mode) _mkdir((path))
    #define FL_IO_DIR_SEPARATOR "\\"
#elif defined(__linux__) || defined(__CYGWIN__)
    #include <unistd.h>
    #include <sys/stat.h>
    #include <sys/types.h>
    #define FL_IO_DIR_SEPARATOR "/"
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
    flm_assert(pathname, "pathname cannot be NULL");

    int res = mkdir(pathname, 0775);
    if (res == 0)
        return true;
    char errmsg[64];
    fl_error_push(errno, fl_errno_str(errno, errmsg, 64));
    return false;
}

bool fl_io_dir_create_recursive(const char *pathname)
{
    flm_assert(pathname, "pathname cannot be NULL");

    bool status = true;

    char *path = fl_cstring_replace(pathname, "/", FL_IO_DIR_SEPARATOR);
    FlVector parts = fl_cstring_split_by(path, FL_IO_DIR_SEPARATOR);
    size_t count = fl_vector_length(parts);

    // TODO: Handle errors
    char *current = fl_cstring_dup("");
    for (size_t i=0; i < count; i++)
    {
        fl_cstring_append(&current, *(const char **)fl_vector_get(parts, i));
        fl_cstring_append(&current, FL_IO_DIR_SEPARATOR);
        
        if (fl_io_file_exists(current))
            continue;
        
        if (!fl_io_dir_create(current))
        {
            // TODO: Delete path
            status = false;
            goto CLEANUP;
        }
    }

CLEANUP:
    fl_cstring_delete(current);
    fl_vector_delete_ptrs(parts);
    fl_cstring_delete(path);
    return status;
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

size_t fl_io_file_read_bytes(FlFile *file, size_t bytesToRead, FlByte *dst)
{
    flm_assert(file != NULL, "File descriptor cannot be NULL");
    size_t read = fread(dst, 1, bytesToRead, file);
    return read;
}

bool fl_io_file_write_bytes(const char *filename, size_t nbytes, const FlByte *bytes)
{
    FlFile *fd = fl_io_file_open(filename, "wb");

    if (!fd)
        return false;

    fwrite(bytes, nbytes, 1, fd);
    fl_io_file_close(fd);

    return true;
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

bool fl_io_file_write_all_bytes(const char *filename, const FlByte *bytes)
{
    return fl_io_file_write_bytes(filename, fl_array_length(bytes), bytes);
}

char* fl_io_file_read_all_text(const char *filename)
{
    FlFile *fd = fl_io_file_open(filename, "r");
    if (!fd)
        return NULL;
    fseek(fd, 0, SEEK_END);
    int length = ftell(fd);
    char *buffer = fl_cstring_new(length+1);
    fseek(fd, 0, SEEK_SET);
    fread(buffer, length, 1, fd);
    buffer[length] = '\0';
    fl_io_file_close(fd);
    return buffer;
}

bool fl_io_file_write_all_text(const char *filename, const char *content)
{
    FlFile *fd = fl_io_file_open(filename, "w");

    if (!fd)
        return false;

    fwrite(content, strlen(content), 1, fd);
    fl_io_file_close(fd);

    return true;
}
