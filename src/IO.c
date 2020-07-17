#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>

#include "Mem.h"
#include "IO.h"
#include "Error.h"
#include "Cstring.h"
#include "Array.h"
#include "containers/List.h"
#include "containers/Vector.h"
#include "text/Regex.h"
#include "os/System.h"

#ifdef _WIN32
    #include <windows.h>
    #include <direct.h>
    #include <io.h>
    #define access _access
    #define mkdir(path, mode) _mkdir((path))
#elif defined(__linux__) || defined(__CYGWIN__)
    #include <unistd.h>
    #include <dirent.h>
#endif

#include <sys/stat.h>
#include <sys/types.h>

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

bool fl_io_is_file(const char *path)
{
    struct stat info;

    if (stat(path, &info) != 0)
        return false;

    #ifdef S_IFREG
    return (info.st_mode & S_IFREG);
    #else
    return S_ISREG(info.st_mode);
    #endif
}

bool fl_io_is_dir(const char *path)
{
    #ifdef _WIN32
    {
        DWORD file_attrs = GetFileAttributes(path);

        return file_attrs != INVALID_FILE_ATTRIBUTES && (file_attrs & FILE_ATTRIBUTE_DIRECTORY);
    }
    #else
    {
        struct stat info;

        if (stat(path, &info) != 0)
            return false;

        #ifdef S_IFREG
        return (info.st_mode & S_IFDIR) ? true : false;
        #else
        return S_ISDIR(info.st_mode) ? true : false;
        #endif
    }
    #endif
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
    FlVector *parts = fl_cstring_split_by(path, FL_IO_DIR_SEPARATOR);
    size_t count = fl_vector_length(parts);

    // TODO: Handle errors
    char *current = fl_cstring_dup("");
    for (size_t i=0; i < count; i++)
    {
        fl_cstring_append(&current, *(const char**) fl_vector_ref_get(parts, i));
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
    fl_cstring_free(current);
    fl_vector_free(parts);
    fl_cstring_free(path);
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

FlArray* fl_io_file_read_all_bytes(const char *filename)
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
        fl_array_free(buffer);
        return NULL;
    }
    
    fl_io_file_close(fd);

    return buffer;
}

bool fl_io_file_write_all_bytes(const char *filename, const FlArray * const bytes)
{
    FILE *fd = fl_io_file_open(filename, "wb");
    
    size_t nbytes = fl_array_length(bytes);
    size_t wbytes = fl_io_file_write_bytes(fd, nbytes, bytes);

    fl_io_file_close(fd);

    return nbytes == wbytes;
}

char* fl_io_file_read_all_text(const char *filename)
{
    FILE * fd = fl_io_file_open(filename, "rb");

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
        fl_cstring_free(buffer);
        fl_io_file_close(fd);
        return NULL;
    }

    buffer[length] = '\0';

    fl_io_file_close(fd);

    return buffer;
}

bool fl_io_file_write_all_text(const char *filename, const char *content)
{
    FILE * fd = fl_io_file_open(filename, "wb");

    if (!fd)
        return false;

    size_t length = strlen(content);
    size_t written = fwrite(content, 1, length, fd);
    
    fl_io_file_close(fd);

    return length == written;
}

bool fl_io_file_get_modified_timestamp(const char *filename, unsigned long long *timestamp)
{
    #ifdef _WIN32
    {
        HANDLE fh = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

        if (fh == INVALID_HANDLE_VALUE)
        {
            char *wdir = fl_system_get_working_dir();
            char *fullname = fl_cstring_vdup("%s%s%s", wdir, FL_IO_DIR_SEPARATOR, filename);

            fh = CreateFile(fullname, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

            fl_cstring_free(fullname);
            fl_cstring_free(wdir);

            if (fh == INVALID_HANDLE_VALUE)
                return false;
        }

        FILETIME modtime;
        if (GetFileTime(fh, NULL, NULL, &modtime) == 0)
            return false;

        ULARGE_INTEGER integer = {
            .LowPart = modtime.dwLowDateTime,
            .HighPart = modtime.dwHighDateTime
        };
        
        *timestamp = integer.QuadPart;

        CloseHandle(fh);

        return true;
    }
    #else
    {
        struct stat attr;
        memset(&attr, 0, sizeof(attr));
        if (stat(filename, &attr) == 1)
            return false;

        *timestamp = (unsigned long long)attr.st_mtime;
        return true;
    }
    #endif
}

char** fl_io_dir_list(const char *directory)
{
    if (!fl_io_file_exists(directory))
        return NULL;

    char **files = fl_array_new(sizeof(char*), 0);

    #ifdef _WIN32
    {
        char *directory_pattern = fl_cstring_dup(directory);

        if (!flm_cstring_ends_with_char(directory_pattern, '\\') && !flm_cstring_ends_with_char(directory_pattern, '/'))
            fl_cstring_append(&directory_pattern, "\\");

        fl_cstring_append(&directory_pattern, "*");

        WIN32_FIND_DATA data;
        memset(&data, 0, sizeof(WIN32_FIND_DATA));
        HANDLE find_handle = FindFirstFile(directory_pattern, &data);

        fl_cstring_free(directory_pattern);

        if (find_handle == INVALID_HANDLE_VALUE)
            return files;

        do {
            if (flm_cstring_equals(data.cFileName, ".") || flm_cstring_equals(data.cFileName, ".."))
                continue;

            char *file = fl_cstring_dup(data.cFileName);
            files = fl_array_append(files, &file);

        } while (FindNextFile(find_handle, &data));

        FindClose(find_handle);
    }
    #else
    {
        DIR *dir = opendir (directory);

        if (dir == NULL)
            return files;

        struct dirent *dir_entry;
        
        while ((dir_entry = readdir(dir)) != NULL)
        {
            if (flm_cstring_equals(dir_entry->d_name, ".") || flm_cstring_equals(dir_entry->d_name, ".."))
                continue;

            char *file = fl_cstring_dup(dir_entry->d_name);
            files = fl_array_append(files, &file);
        }

        closedir(dir);
    }
    #endif

    return files;
}

static inline FlVector* split_regex_by_path_separator(const char *regex, const char separator)
{
    flm_assert(regex != NULL, "char* argument to split cannot be NULL");

    FlVector *parts = flm_vector_new_with(.capacity = 1, .cleaner = fl_container_cleaner_pointer);

    size_t length = strlen(regex);
    size_t i=0;
    size_t l=0;
    for (; i < length; i++)
    {
        if (regex[i] == '[' && i > 0 && regex[i-1] != '\\')
        {
            while (i < length && regex[i] != ']')
                i++;
        }
        else if (regex[i] == separator)
        {
            // Starting path separator
            if (i == 0)
            {
                l++;
                continue;
            }
            else
            {
                char *part = fl_cstring_dup_n(regex + l, i-l);
                fl_vector_add(parts, &part);
                l = i+1; // Consume the path separator
            }
        }
    }

    if (i != l) {
        char *part = fl_cstring_dup_n(regex + l, i-l);
        fl_vector_add(parts, &part);
    }

    return parts;
}

char** fl_io_file_find(const char *pattern, const char *path_separator)
{
    if (!path_separator || !path_separator[0])
        return NULL;

    char **files = fl_array_new(sizeof(char*), 0);

    FlList *matching_files = fl_list_new_args((struct FlListArgs){ .value_cleaner = fl_container_cleaner_pointer });

    FlVector *parts = split_regex_by_path_separator(pattern, path_separator[0]);

    // Our starting point is the current directory
    char *base_dir = fl_cstring_vdup("%s%s", *(char**) fl_vector_ref_get(parts, 0), path_separator);
    char **dir_files = fl_io_dir_list(base_dir[0] == '^' ? base_dir + 1 : base_dir);

    for (size_t i=0; i < fl_array_length(dir_files); i++)
        fl_list_append(matching_files, fl_cstring_vdup("%s%s", base_dir, dir_files[i]));
   
    fl_array_free_each_pointer(dir_files, (FlArrayFreeElementFunc)fl_cstring_free);
    fl_cstring_free(base_dir);

    // Now we need to start the match proccess.
    char *current_path = fl_cstring_new(0);
    for (size_t i=0; i < fl_vector_length(parts); i++)
    {
        if (i == 0)
            fl_cstring_append(&current_path, *(char**) fl_vector_ref_get(parts, i));
        else
            fl_cstring_vappend(&current_path, "%s%s", path_separator, *(char**) fl_vector_ref_get(parts, i));

        FlRegex *regex = fl_regex_compile(current_path);

        if (regex == NULL)
            break;

        struct FlListNode *matching_file = fl_list_head(matching_files);

        for (size_t i=0; matching_file; i++)
        {
            char *filepath = (char*)matching_file->value;
            bool is_match = fl_regex_match(regex, filepath);
            bool is_dir = fl_io_is_dir(filepath);
            
            if (is_dir)
            {
                char **dir_files = fl_io_dir_list(filepath);

                for (size_t j=0; j < fl_array_length(dir_files); j++)
                    fl_list_insert_before(matching_files, matching_file, fl_cstring_vdup("%s%s%s", filepath, path_separator, dir_files[j]));

                fl_array_free_each_pointer(dir_files, (FlArrayFreeElementFunc)fl_cstring_free);
            }

            struct FlListNode *to_remove = matching_file;
            matching_file = matching_file->next;

            if (!is_match || is_dir)
                fl_list_remove(matching_files, to_remove);
        }

        fl_regex_free(regex);
    }
    fl_cstring_free(current_path);
    fl_vector_free(parts);

    struct FlListNode *tmp = fl_list_head(matching_files);
    FlRegex *regex = fl_regex_compile((char*)pattern);
    while (tmp)
    {
        // We need to make a last check against the full pattern
        if (fl_regex_match(regex, (char*)tmp->value))
        {
            char *filename = fl_cstring_dup((char*)tmp->value);
            files = fl_array_append(files, &filename);
        }
        tmp = tmp->next;
    }

    fl_regex_free(regex);
    fl_list_free(matching_files);

    return files;
}

bool fl_io_file_unlink(const char *filename)
{
    #ifdef _WIN32
    return _unlink(filename) == 0;
    #else
    return unlink(filename) == 0;
    #endif
}
