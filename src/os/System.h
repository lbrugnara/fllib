#ifndef FL_SYSTEM_H
#define FL_SYSTEM_H

#include "../Types.h"
#include "../Std.h"

bool fl_system_is_little_endian ();

bool fl_system_set_working_dir(char *path);
char* fl_system_get_working_dir();

#endif /* FL_SYSTEM_H */
