#include <stdlib.h>
#include <errno.h>

#include "System.h"
#include "../Error.h"
#include "../Cstr.h"

#ifdef _WIN32
    #include <windows.h>
    #include <direct.h>
    #define getcwd _getcwd
    #define chdir _chdir
#elif defined(__linux__) || defined(__CYGWIN__)
    #include <unistd.h>
#endif

bool fl_system_set_working_dir(FlCstr path)
{
    flm_assert(path != NULL, "New working directory path is not NULL");
    if (chdir(path) == 0)
        return true;
    char errmsg[64];
    fl_error_push(errno, fl_errno_str(errno, errmsg, 64));
    return false;
}

FlCstr fl_system_get_working_dir()
{
    size_t max_path_length = 255;
    FlCstr cwd = fl_cstr_new(max_path_length);
    if (getcwd(cwd, max_path_length) == NULL)
        return NULL;
    return cwd;
}
