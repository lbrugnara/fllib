#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>

#include "System.h"
#include "../Error.h"
#include "../Cstring.h"

#ifdef _WIN32
    #include <windows.h>
    #include <direct.h>
    #define getcwd _getcwd
    #define chdir _chdir
#elif defined(__linux__) || defined(__CYGWIN__)
    #include <unistd.h>
#endif

bool fl_system_set_working_dir(char *path)
{
    flm_assert(path != NULL, "New working directory path is not NULL");
    if (chdir(path) == 0)
        return true;
    char errmsg[64];
    fl_error_push(errno, fl_errno_str(errno, errmsg, 64));
    return false;
}

char* fl_system_get_working_dir(void)
{
    size_t max_path_length = 255;
    char *cwd = fl_cstring_new(max_path_length);
    if (getcwd(cwd, max_path_length) == NULL)
        return NULL;
    return cwd;
}

bool fl_system_is_little_endian (void)
{
    int i = 1;
    char *p = (char *)&i;
    return p[0] == 1;
}

void fl_system_sleep_ms(unsigned long milliseconds)
{
    #ifdef _WIN32
    Sleep(milliseconds);
    #elif defined(__linux__)
    usleep(milliseconds * 1000);
    #endif
}

void fl_system_sleep_us(unsigned long microseconds)
{
    #ifdef _WIN32

    HANDLE timer;
	LARGE_INTEGER ft;

	ft.QuadPart = -(10 * microseconds);

	timer = CreateWaitableTimer(NULL, TRUE, NULL);
	SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
	WaitForSingleObject(timer, INFINITE);
	CloseHandle(timer);

    #elif defined(__linux__)
    usleep(microseconds);
    #endif
}
