#ifndef FL_IO_H
#define FL_IO_H

#include "Types.h"
#include "Std.h"
#include "Array.h"

/*
 * Function: fl_io_file_open
 *  Wrapper for the fopen function.
 *
 * Parameters:
 *  filename - File to be opened 
 *  mode - File access mode
 *
 * Returns:
 *  FILE* - A handle to the opened file
 *
 */
FILE* fl_io_file_open(const char *filename, const char *mode);

/*
 * Function: fl_io_file_close
 *  Wrapper for the fclose function
 *
 * Parameters:
 *  fd - File descriptor to be closed
 *
 * Returns:
 *  bool - *true* on success, otherwise *false*.
 */
bool fl_io_file_close(FILE *fd);

/*
 * Function: fl_io_file_exists
 *  Check if a file exists.
 *
 * Parameters:
 *  filename - File to check if exists
 *
 * Returns:
 *  bool - *true* if the file exists, otherwise *false*
 *
 */
bool fl_io_file_exists(const char *filename);


/*
 * Function: fl_io_file_size
 *  Returns the size of an opened file.
 *
 * Parameters:
 *  fd - File descriptor to an opened file
 *
 * Returns:
 *  long - On error this function returns -1L. Otherwise the size of a file (greater than 0)
 *
 */
long fl_io_file_size(FILE *fd);

/*
 * Function: fl_io_dir_create
 *  Creates the directory specified in *pathname*
 *
 * Parameters:
 *  pathname - Path to the directory to be created
 *
 * Returns:
 *  bool - *true* on success, otherwise *false*.
 *
 */
bool fl_io_dir_create(const char *pathname);

/*
 * Function: fl_io_dir_create_recursive
 *  Creates all the directories that do not exist in the provided *pathname*
 *
 * Parameters:
 *  pathname - Path to the directories to be created
 *
 * Returns:
 *  bool - *true* on success, otherwise *false*.
 *
 */
bool fl_io_dir_create_recursive(const char *pathname);

/*
 * Function: fl_io_file_read_bytes
 *  This function is a wrapper of the fread function. It reads *nbytes* bytes from the *file* 
 *  and copies it to the *destination* pointer. This function *does not* allocate memory, 
 *  *destination* must point to a valid memory location able to hold *nbytes* in it.
 *
 * Parameters:
 *  file - file to read from
 *  nbytes - Number of bytes to read from the file
 *  dst - Destination memory to hold the copied bytes
 *
 * Returns:
 *  size_t - Number of bytes read from the file.
 *
 */
size_t fl_io_file_read_bytes(FILE *file, size_t nbytes, FlByte *destination);

/*
 * Function: fl_io_file_write_bytes
 *  This function is a wrapper of the fwrite function. It writes *nbytes* from the memory 
 *  pointed by *bytes* to the *file*.
 * 
 * Parameters:
 *  file - File to write to.
 *  nbytes - Number of bytes to write to the file.
 *  bytes - Pointer to the memory region to be copied to the file.
 *
 * Returns:
 *  size_t - Number of bytes written to the file
 *
 */
size_t fl_io_file_write_bytes(FILE *file, size_t nbytes, const FlByte *bytes);

/*
 * Function: fl_io_file_read_all_bytes
 *  Reads all the bytes from the file pointed by *filename* and return a pointer
 *  to the allocated memory containing the file's content, or NULL on error.
 *
 * Parameters:
 *  filename - File to be read
 *
 * Returns:
 *  FlByte* - Pointer to a memory region containing the file's content, or NULL on failure
 * 
 * Notes:
 *  The returned pointer is an array allocated with the <fl_array_new> function,
 *  therefore after usage it needs to be deleted using the <fl_array_delete> function
 *
 */
FlArray fl_io_file_read_all_bytes(const char *filename);

/*
 * Function: fl_io_file_write_all_bytes
 *  Reads all the bytes from an array allocated with the <fl_array_new> function
 *  and writes the content to the file *filename*
 *
 * Parameters:
 *  filename - Destination file
 *  content - Source <FlArray> to read the bytes from
 *
 * Returns:
 *  bool - *true* on success, otherwise *false*.
 *
 */
bool fl_io_file_write_all_bytes(const char *filename, const FlArray content);

/*
 * Function: fl_io_file_read_all_text
 *  Reads all the text in the file *filename* and return a pointer to a memory region
 *  containing all the text as a null terminated string.
 *
 * Parameters:
 *  filename - File to read all the text from
 *
 * Returns:
 *  char* - Pointer to the allocated memory containing all the text read from the file.
 * 
 * Notes:
 *  This function allocates memory therefore the returned pointer must be freed by the caller
 *  using the <fl_free> or the <fl_cstring_delete> function.
 *
 */
char* fl_io_file_read_all_text(const char *filename);

/*
 * Function: fl_io_file_write_all_text
 *  Copies all the text in the *content* variable (null terminated string) to the file
 *  *filename*
 *
 * Parameters:
 *  filename - File to write all the text to
 *  content - Null terminated string to be copied to the file
 *
 * Returns:
 *  bool - *true* on success, otherwise *false*
 *
 */
bool fl_io_file_write_all_text(const char *filename, const char *content);

#endif /* FL_IO_H */
