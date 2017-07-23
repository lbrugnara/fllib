#ifndef FL_SYSTEM_H
#define FL_SYSTEM_H

#include "../Types.h"
#include "../Std.h"

bool fl_system_set_working_dir(char *path);

char* fl_system_get_working_dir();

static bool fl_system_is_little_endian ()
{
    int i = 1;
    char *p = (char *)&i;
    return p[0] == 1;
}

#endif /* FL_SYSTEM_H */
