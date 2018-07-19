#ifndef FL_IO_H
#define FL_IO_H

#include "Types.h"
#include "Std.h"
#include "Array.h"

typedef FILE FlFile;

FlFile* fl_io_file_open(const char *filename, const char *mode);
void fl_io_file_close(FlFile *fd);
bool fl_io_file_exists(const char *filename);
long fl_io_file_size(FlFile *fd);

bool fl_io_dir_create(const char *pathname);
bool fl_io_dir_create_recursive(const char *pathname);

size_t fl_io_file_read_bytes(FlFile *file, size_t bytesToRead, FlByte* dst);
bool fl_io_file_write_bytes(const char *filename, size_t nbytes, const FlByte *bytes);

FlByte *fl_io_file_read_all_bytes(const char *filename);
bool fl_io_file_write_all_bytes(const char *filename, const FlByte *content);

char* fl_io_file_read_all_text(const char *filename);
bool fl_io_file_write_all_text(const char *filename, const char *content);


#endif /* FL_IO_H */
