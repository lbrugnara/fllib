#ifndef FL_STRING_H
#define FL_STRING_H
/* -------------------------------------------------------------
* {macro: FlString}
* -------------------------------------------------------------
* Allows user to create and manipulate UTF-8 strings.
* -------------------------------------------------------------
*/

#include "../Types.h"
#include "Unicode.h"

/* -------------------------------------------------------------
* {function: fl_string_new}
* -------------------------------------------------------------
* Creates an FlString from c string. It accepts ASCII, ANSI and
* UTF-8 multybytes strings.
* -------------------------------------------------------------
* {param: FlCstr str} An string that could be ASCII, ANSI or UTF-8 (multibyte)
* -------------------------------------------------------------
* {return: FlString} A new instance of an FlString that represents
* an UTF-8 string
* -------------------------------------------------------------
*/
FlString fl_string_new(const FlString str);

/* -------------------------------------------------------------
* {function: fl_string_new_from_bytes}
* -------------------------------------------------------------
* Creates a new instance of an FlString object using decoding the
* byte array based on the "encoding" parameter 
* -------------------------------------------------------------
* {param: const FlByte* bytes} Input bytes to be decoded
* {param: size_t length} Number of bytes in "bytes"
* {param: FlEncoding encoding} Encoding used to decode the bytes
* -------------------------------------------------------------
* {return: FlString} A new instance of an FlString that represents
* an UTF-8 string
* -------------------------------------------------------------
*/
FlString fl_string_new_from_bytes(const FlByte *bytes, size_t length, FlEncoding encoding);

/* -------------------------------------------------------------
* {function: fl_string_delete}
* -------------------------------------------------------------
* Releases the memory used by the object str
* -------------------------------------------------------------
* {param: FlString str} Target object to release the memory
* -------------------------------------------------------------
* {return: void}
* -------------------------------------------------------------
*/
void fl_string_delete(FlString str);

/* -------------------------------------------------------------
* {function: fl_string_length}
* -------------------------------------------------------------
* Return the number of characters that compose the string str.
* 'end' is used as a marker to stop the counting. If end is NULL,
* this function will search for the first NULL character and will
* stop the counting. If end is a valid pointer, it will count until
* reach the provided address.
* -------------------------------------------------------------
* {param: const FlString str} Target string to retrieve the length
* {param: const FlByte* end} Marker to stop the counting
* -------------------------------------------------------------
* {return: size_t} Number of characters that compose the string
* -------------------------------------------------------------
*/
size_t fl_string_length(const FlString str, const FlByte* end);

/* -------------------------------------------------------------
* {function: fl_string_size}
* -------------------------------------------------------------
* Returns the number of bytes of the UTF-8 string str. 'end' is
* a marker to stop the counting of bytes. If end is NULL,
* this function will search for the first NULL character and will
* stop the counting. If end is a valid pointer, it will count until
* reach the provided address.
* -------------------------------------------------------------
* {param: const FlString str} Target string to retrieve the size_t
* {param: const FlByte* end} Marker to stop the byte counting
* -------------------------------------------------------------
* {return: size_t} Number of bytes used to represent the string str
* -------------------------------------------------------------
*/
size_t fl_string_size(const FlString str, const FlByte* end);

FlUnicodeChar fl_char(const FlString str);

FlUnicodeChar fl_string_char_at(const FlString str, size_t at);

#endif /* FL_STRING_H */