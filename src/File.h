#ifndef FL_FILE_H
#define FL_FILE_H

#include "Types.h"
#include "Std.h"
#include "Array.h"

typedef FILE FlFile;

FlFile* fl_file_open(FlCstr filename, FlCstr mode);
void fl_file_close(FlFile *fd);
bool fl_file_exists(FlCstr filename);
long fl_file_size(FlFile *fd);
size_t fl_file_read_bytes(FlFile *file, size_t bytesToRead, FlByteArray dst);
void fl_file_write_all_bytes(FlCstr filename, FlByteArray content);
FlByteArray fl_file_read_all_bytes(FlCstr filename);

bool fl_file_create_dir(FlCstr pathname);


#endif /* FL_FILE_H */
