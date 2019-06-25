#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "Mem.h"
#include "IO.h"
#include "Error.h"
#include "Cstring.h"
#include "Array.h"

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

FILE * fl_io_file_open(const char *filename, const char *mode)
{
    #if defined(_WIN32) && __STDC_WANT_SECURE_LIB__
    {
        FILE * fd;
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
    fl_vector_delete(parts);
    fl_cstring_delete(path);
    return status;
}

bool fl_io_file_close(FILE * fd)
{
    return fclose(fd) == 0;
}

long fl_io_file_size(FILE * fd)
{
    flm_assert(fd != NULL, "File descriptor cannot be NULL");

    long originalPos = ftell(fd);

    if (originalPos == -1L)
        return -1L;
    
    fseek(fd, 0, SEEK_END);
    
    long size = ftell(fd);

    if (size == -1L)
        return -1L;

    fseek(fd, originalPos, SEEK_SET);
    
    return size;
}

size_t fl_io_file_read_bytes(FILE * file, size_t nbytes, FlByte *destination)
{
    flm_assert(file != NULL, "File descriptor cannot be NULL");
    size_t read = fread(destination, 1, nbytes, file);
    return read;
}

size_t fl_io_file_write_bytes(FILE *file, size_t nbytes, const FlByte *bytes)
{
    flm_assert(file != NULL, "File descriptor cannot be NULL");

    return fwrite(bytes, 1, nbytes, file);
}

FlArray fl_io_file_read_all_bytes(const char *filename)
{
    FILE * fd = fl_io_file_open(filename, "rb");

    if (!fd)
        return NULL;

    fseek(fd, 0, SEEK_END);
    long llength = ftell(fd);

    if(llength == -1L)
    {
        fl_io_file_close(fd);
        return NULL;
    }

    size_t length = (size_t)llength;

    FlByte *buffer = (FlByte*)fl_array_new(1, length);

    if (!buffer)
    {
        fl_io_file_close(fd);
        return NULL;
    }

    fseek(fd, 0, SEEK_SET);

    size_t read = fread(buffer, 1, length, fd);

    if (read != length)
    {
        fl_io_file_close(fd);
        fl_array_delete(buffer);
        return NULL;
    }
    
    fl_io_file_close(fd);

    return buffer;
}

bool fl_io_file_write_all_bytes(const char *filename, const FlArray bytes)
{
    FILE *fd = fl_io_file_open(filename, "wb");
    
    size_t nbytes = fl_array_length(bytes);
    size_t wbytes = fl_io_file_write_bytes(fd, nbytes, bytes);

    fl_io_file_close(fd);

    return nbytes == wbytes;
}

char* fl_io_file_read_all_text(const char *filename)
{
    FILE * fd = fl_io_file_open(filename, "r");

    if (!fd)
        return NULL;

    fseek(fd, 0, SEEK_END);
    
    long llength = ftell(fd);

    if (llength == -1L)
    {
        fl_io_file_close(fd);
        return NULL;
    }

    size_t length = (size_t)llength;

    char *buffer = fl_cstring_new(length);

    if (buffer == NULL)
    {
        fl_io_file_close(fd);
        return NULL;
    }

    fseek(fd, 0, SEEK_SET);
    
    size_t read = fread(buffer, 1, length, fd);

    if (read != length)
    {
        fl_cstring_delete(buffer);
        fl_io_file_close(fd);
        return NULL;
    }

    buffer[length] = '\0';

    fl_io_file_close(fd);

    return buffer;
}

bool fl_io_file_write_all_text(const char *filename, const char *content)
{
    FILE * fd = fl_io_file_open(filename, "w");

    if (!fd)
        return false;

    size_t length = strlen(content);
    size_t written = fwrite(content, 1, length, fd);
    
    fl_io_file_close(fd);

    return length == written;
}
