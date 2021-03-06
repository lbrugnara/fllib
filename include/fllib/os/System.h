#ifndef FL_SYSTEM_H
#define FL_SYSTEM_H

#include <stdbool.h>
#include "../Types.h"
#include "../Std.h"

bool fl_system_is_little_endian(void);

bool fl_system_set_working_dir(char *path);
char* fl_system_get_working_dir(void);
void fl_system_sleep_ms(unsigned long milliseconds);
void fl_system_sleep_us(unsigned long microseconds);

#endif /* FL_SYSTEM_H */
