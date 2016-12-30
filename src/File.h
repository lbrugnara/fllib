#ifndef FL_FILE_H
#define FL_FILE_H

#include "Types.h"
#include "Std.h"
#include "Array.h"

typedef FlByte* FlByteArray;

bool fl_file_exists(FlCstr filename);
bool fl_file_create_dir(FlCstr pathname);
void fl_file_write_all_bytes(FlCstr filename, FlByteArray content);
FlByteArray fl_file_read_all_bytes(FlCstr filename);


#endif /* FL_FILE_H */
