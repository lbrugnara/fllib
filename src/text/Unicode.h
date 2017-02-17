#ifndef FL_UNICODE_H
#define FL_UNICODE_H
/* =============================================================
* {module: UNICODE}
* =============================================================
* Contains a set of function to work with unicode characters
* and strings.
* -------------------------------------------------------------
*/

#if defined(_WIN32)
#   ifndef UNICODE
#       define UNICODE
#   endif
#   ifndef _UNICODE
#       define _UNICODE
#   endif
#endif

#include <stdint.h>

#include "../Types.h"
#include "Encoding.h"

/* -------------------------------------------------------------
* {datatype: uint32_t FlUnicodeChar}
* -------------------------------------------------------------
* Represents a Unicode char. To make it easy to work with Unicode
* characters, the type of FlUnicodeChar is an uint32_t, to be able
* to represents all the unicode code points. It is up to library's
* users to keep track of the encoding of each FlUnicodeChar
* -------------------------------------------------------------
*/
typedef uint32_t FlUnicodeChar;

#define FL_UNICODE_INVALID_CHAR UINT32_MAX

/* -------------------------------------------------------------
* {function: fl_unicode_char_from_bytes}
* -------------------------------------------------------------
* It receives an array of bytes that represents a multibyte (or single-byte)
* character, and returns its FlUnicodeChar representation with the same encoding. 
* If 'src' is composed by more than one character (more than one code point)
* it will return the FlUnicodeChar representation of the first code 
* point of src. src will be interpreted using the encoding provided
* by the user. The encoding of the returned FlUnicodeChar is the same as the used
* to interpret the bytes array.
* -------------------------------------------------------------
* {param: const FlString src} The string to retrieve the first character
* it is pointing to as an FlUnicodeChar
* {param: FlEncoding encoding} src bytes encoding
* -------------------------------------------------------------
* {return: FlUnicodeChar} FlUnicodeChar of the first
* character of the string 'src'
* -------------------------------------------------------------
*/
FlUnicodeChar fl_unicode_char_from_bytes(const FlByte* src, FlEncoding encoding);

/* -------------------------------------------------------------
* {function: fl_unicode_char_from_bytes_to}
* -------------------------------------------------------------
* Similar to fl_unicode_char_from_bytes, but it converts the input
* bytes array ('srcencoding' encoded) to an FlUnicodeChar encoded
* using 'dstencoding'.
* -------------------------------------------------------------
* {param: const FlByte* src} Source bytes
* {param: FlEncoding srcencoding} Encoding of the bytes array
* {param: FlEncoding dstencoding} Encoding used for the returned FlUnicodeChar
* -------------------------------------------------------------
* {return: FlUnicodeChar} Result of convert src (encoded as 'srcencoding') to an FlUnicodeChar 
* with encoding 'dstencoding'
* -------------------------------------------------------------
*/
FlUnicodeChar fl_unicode_char_from_bytes_to(const FlByte* src, FlEncoding srcencoding, FlEncoding dstencoding);

/* -------------------------------------------------------------
* {function: fl_unicode_char_size}
* -------------------------------------------------------------
* Returns the size in bytes of the character represented by
* FlUnicodeChar 'chr'.
* -------------------------------------------------------------
* {param: FlUnicodeChar chr} target Unicode character to retrieve its bytes size
* -------------------------------------------------------------
* {return: size_t} Size in bytes of character 'chr'. If chr is an invalid
* unicode char it returns -1
* -------------------------------------------------------------
*/
int32_t fl_unicode_char_size(const FlUnicodeChar chr, FlEncoding encoding);

/* -------------------------------------------------------------
* {function: fl_unicode_char_at}
* -------------------------------------------------------------
* Returns the 'at'-th character in the string 'str'. The char
* is an FlUnicodeChar (a UTF-8 representation of the caracter)
* that could be easily used to compare it with another UTF-8
* character that could be generated using the fl_unicode_char_from_xxx
* functions family
* -------------------------------------------------------------
* {param: const FlByte* str} String to retrieve the 'at'-th character
* {param: size_t at} Position in string to retrieve the character
* -------------------------------------------------------------
* {return: FlUnicodeChar} UTF-8 representation of the character
* in the 'at'-th position of 'str'
* -------------------------------------------------------------
*/
FlUnicodeChar fl_unicode_char_at(const FlByte* str, FlEncoding encoding, size_t at);

/* -------------------------------------------------------------
* {function: fl_unicode_str_size}
* -------------------------------------------------------------
* * Returns the size in bytes of the string represente1d by the
* byte array arr, interpreted with the provided encoding. end
* could be NULL and the bytes count will stop when the first
* NULL character is found, or a pointer to byte where the algorithm
* should stop.
* -------------------------------------------------------------
* {param: const FlByte* arr} Source bytes array
* {param: FlEncoding encoding} Encoding used to interpret the bytes array
* {param: const FlByte* end} A pointer to a byte or NULL.
* -------------------------------------------------------------
* {return: size_t} Size in bytes of the byte array arr
* -------------------------------------------------------------
*/
size_t fl_unicode_str_size(const FlByte* arr, FlEncoding encoding, const FlByte *end);

/* -------------------------------------------------------------
* {function: fl_unicode_encode_char_to}
* -------------------------------------------------------------
* Encodes src character with encoding {srcencoding} to thread_error
* encoding specified in {dstencoding}
* -------------------------------------------------------------
* {param: FlUnicodeChar src} Source character
* {param: FlEncoding srcencoding} Encoding used by {src}
* {param: FlEncoding dstencoding} Encoding of the return value
* -------------------------------------------------------------
* {return: FlUnicodeChar} Unicode character that represents
* {src} character encoded as {dstencoding}
* -------------------------------------------------------------
*/
FlUnicodeChar fl_unicode_encode_char_to(const FlUnicodeChar src, const FlEncoding srcencoding, const FlEncoding dstencoding);

#endif /* FL_UNICODE_H */
