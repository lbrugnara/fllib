#ifndef FL_FILE_H
#define FL_FILE_H

#include "Types.h"
#include "Std.h"
#include "Array.h"

typedef FILE FlFile;

FlFile* fl_file_open(char *filename, char *mode);
void fl_file_close(FlFile *fd);
bool fl_file_exists(char *filename);
long fl_file_size(FlFile *fd);
size_t fl_file_read_bytes(FlFile *file, size_t bytesToRead, FlByte* dst);
void fl_file_write_all_bytes(char *filename, FlByte* content);
FlByte* fl_file_read_all_bytes(char *filename);

bool fl_file_create_dir(char *pathname);


#endif /* FL_FILE_H */
