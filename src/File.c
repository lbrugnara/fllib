#include <stdlib.h> 
#include <stdio.h>
#include <errno.h>

#include "Mem.h"
#include "File.h"

#ifdef _WIN32
    #include <io.h>
    #define access _access
    #include <direct.h>
    #define mkdir _mkdir
#elif defined(__linux__)
    #include <unistd.h>
    #include <sys/stat.h>
    #include <sys/types.h>
#endif

bool fl_file_exists(FlCstr filename)
{
    return access(filename, 0) != -1;
}

bool fl_file_create_dir(FlCstr pathname)
{
    int res = mkdir(pathname, 0775);
    if (res == 0)
        return true;
    fl_error_push(errno, strerror(errno));
    return false;
}

void fl_file_write_all_bytes(FlCstr filename, FlByteArray bytes)
{
    FILE *fd = fopen(filename, "wb");
    if (!fd)
        return;
    int length = fl_array_length(bytes);
    fwrite(bytes, length, 1, fd);
    fclose(fd);
}

FlByteArray fl_file_read_all_bytes(FlCstr filename)
{
    FILE *fd = fopen(filename, "rb");
    if (!fd)
        return NULL;
    fseek(fd, 0, SEEK_END);
    int length = ftell(fd);
    FlByteArray buffer = (FlByteArray)fl_array_new(1, length+1);
    fseek(fd, 0, SEEK_SET);
    fread(buffer, length, 1, fd);
    fclose(fd);
    buffer[length] = FL_EOS;
    return buffer;
}