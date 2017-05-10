#ifndef FL_UNICODE_H
#define FL_UNICODE_H
/* =============================================================
* {module: UNICODE}
* =============================================================
* Contains a set of function to work with unicode characters
* and strings. The two most important data types in this module are:
*
* {FlByte} Used to represents each byte of a Unicode code point.
* Generally, when working with stream of bytes that contains
* unicode strings, is recommended to work with FlByte. This module
* provides functions to work with stream of unicode bytes.
*
* {FlUnicodeChar} It is a typedef uint32_t, it can represent any
* unicode point in any encoding. If you want the UTF-32 representation
* of code point U+0181, you simply write `FlUnicodeChar u181 = 0x0181; // UTF-32`,
* or if you want its UTF-8 representaiton you write `FlUnicodeChar u181 = 0xc681; // UTF-8`.
*
* This module provides functions to convert FlUnicodeChar between encodings and to get
* the bytes from that FlUnicodeChar. Keep in mind that FlUnicodeChar IS NOT INTERCHANGEABLE
* with the byte representation of a unicode point, except for UTF-32. That means, if you convert
* an stream of unicode bytes representing a UTF-8 string to an array of FlUnicodeChar, you MUST
* convert it back to unicode bytes before sharing it to be a valid unicode string. FlUnicodeChar
* is a handy way to handle unicode code points inside your library.
* This module provides functions to convert from bytes to FlUnicodeChar and vice versa. Alongside
* each function related with FlUnicodeChar declared in this header, you will find (if exists)
* its FlByte version.
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
* users to keep track of the encoding of each FlUnicodeChar and to
* convert it to an stream of bytes representing valid unicode char
* -------------------------------------------------------------
*/
typedef uint32_t FlUnicodeChar;

/* -------------------------------------------------------------
* {macro: FL_UNICODE_INVALID_CHAR}
* -------------------------------------------------------------
* Represents an invalid Unicode character. FlUnicodeChar is an
* unsigned 32 bits int, so to represent an invalid unicode code point
* we used 2^32-1 bits (too much greater than the maximum valid code point)
* -------------------------------------------------------------
*/
#define FL_UNICODE_INVALID_CHAR UINT32_MAX

#define FL_UNICODE_INVALID_SIZE SIZE_MAX

/* -------------------------------------------------------------
* {function: fl_unicode_codepoint_to_char}
* -------------------------------------------------------------
* It receives an array of bytes that represents a unicode code point, 
* and returns its FlUnicodeChar representation with the same encoding. 
* If {src} is composed by more than one character (more than one code point)
* it will return the FlUnicodeChar representation of the first code 
* point of src. src will be interpreted using the encoding provided
* by the user. The encoding of the returned FlUnicodeChar is the same as the
* used used to interpret the input bytes.
* -------------------------------------------------------------
* {param: const FlString src} The string to retrieve the first character
* it is pointing to as an FlUnicodeChar
* {param: FlEncoding encoding} src bytes encoding
* -------------------------------------------------------------
* {return: FlUnicodeChar} FlUnicodeChar of the first
* character of the string {src}
* -------------------------------------------------------------
*/
FlUnicodeChar fl_unicode_codepoint_to_char(const FlByte* src, FlEncoding encoding);

/* -------------------------------------------------------------
* {function: fl_unicode_char_to_codepoint}
* -------------------------------------------------------------
* It takes a unicode char represented by an FlUnicodeChar
* encoded with {encoding} and populates {dst} with the code
* points of the unicode char. If {chr} is an invalid unicode
* code point this function returns false.
* -------------------------------------------------------------
* {param: FlUnicodeChar chr} Unicode char to get the bytes from
* {param: FlEncoding encoding} Encoding used by {chr}
* {param: FlByte* dst} Target to save the bytes from {chr}. Caller MUST
* ensure {dst} to have space to allocate a code point (1 to 4 bytes)
* -------------------------------------------------------------
* {return: bool} If {chr} is an invalid unicode code point returns false,
* if not true.
* -------------------------------------------------------------
*/
bool fl_unicode_char_to_codepoint(FlUnicodeChar chr, FlEncoding encoding, FlByte* dst);

/* -------------------------------------------------------------
* {function: fl_unicode_codepoint_to_encoding_to_char}
* -------------------------------------------------------------
* Similar to fl_unicode_codepoint_to_char, but it converts the input
* bytes array ('srcencoding' encoded) to an FlUnicodeChar encoded
* with 'dstencoding'.
* -------------------------------------------------------------
* {param: const FlByte* src} Source bytes
* {param: FlEncoding srcencoding} Encoding of the bytes array
* {param: FlEncoding dstencoding} Encoding used for the returned FlUnicodeChar
* -------------------------------------------------------------
* {return: FlUnicodeChar} Result of convert src (encoded as 'srcencoding') to an FlUnicodeChar 
* with encoding 'dstencoding'
* -------------------------------------------------------------
*/
FlUnicodeChar fl_unicode_codepoint_to_encoding_to_char(const FlByte* src, FlEncoding srcencoding, FlEncoding dstencoding);

/* -------------------------------------------------------------
* {function: fl_unicode_char_to_encoding_to_codepoint}
* -------------------------------------------------------------
* Similar to fl_unicode_char_to_codepoint, but instead of return the bytes
* of the input {chr}, it previously encode {chr} to {dstencoding}
* and copies the resulting bytes into {dst}. {chr} is encoded
* with {srcencoding}.
* -------------------------------------------------------------
* {param: FlUnicodeChar chr} Input FlUnicodeChar to get the bytes from
* {param: FlEncoding srcencoding} Encoding of {chr}
* {param: FlByte* dst} Where to put the bytes of {chr}. Caller MUST ensure
* {dst} to have enough space to save a code point (1 to 4 bytes)
* {param: FlEncoding dstencoding} Encoding of {dst}
* -------------------------------------------------------------
* {return: <Data type>} <Return description>
* -------------------------------------------------------------
*/
bool fl_unicode_char_to_encoding_to_codepoint(FlUnicodeChar chr, FlEncoding srcencoding, FlByte* dst, FlEncoding dstencoding);

/* -------------------------------------------------------------
* {function: fl_unicode_char_size}
* -------------------------------------------------------------
* Returns the size in bytes of the code point represented by
* FlUnicodeChar 'chr'. If {chr} is invalid, it returns -1, other
* cases >= 0
* -------------------------------------------------------------
* {param: FlUnicodeChar chr} target Unicode character to retrieve its bytes size
* -------------------------------------------------------------
* {return: size_t} Size in bytes of character 'chr'. If chr is an invalid
* unicode char it returns -1
* -------------------------------------------------------------
*/
size_t fl_unicode_char_size(const FlUnicodeChar chr, FlEncoding encoding);

/* -------------------------------------------------------------
* {function: fl_unicode_codepoint_size}
* -------------------------------------------------------------
* Returns the size in bytes of the {src} input. If {src} is an invalid
* unicode code point, this function returns -1, other cases >= 0.
* -------------------------------------------------------------
* {param: const FlByte* src} Input bytes to get the size
* {param: FlEncoding encoding} {src} encoding
* -------------------------------------------------------------
* {return: size_t} Size in bytes of input bytes {src}. If {src} is an invalid
* unicode char it returns -1
* -------------------------------------------------------------
*/
size_t fl_unicode_codepoint_size(const FlByte* src, FlEncoding encoding);

/* -------------------------------------------------------------
* {function: fl_unicode_char_array_size}
* -------------------------------------------------------------
* Returns the size in bytes of the unicode string represented by
* an array of {FlUnicodeChar}s. The encoding of the chars is determined
* by {encoding}. If {end} is NULL, the count of bytes will stop when
* one of the elements of {string} is 0x0. If {end} has a value, it should
* be a pointer to an element of string where the function should stop counting.
* -------------------------------------------------------------
* {param: const FlUnicodeChar* string} Input string
* {param: FlEncoding encoding} Encoding used by {string}
* {param: FlUnicodeChar* end} Element from {string} where to stop counting,
* or NULL to stop at the first 0x0 value.
* -------------------------------------------------------------
* {return: size_t} Number of valid bytes in {string}
* -------------------------------------------------------------
*/
size_t fl_unicode_char_array_size(const FlUnicodeChar *string, FlEncoding encoding, FlUnicodeChar *end);

/* -------------------------------------------------------------
* {function: fl_unicode_codeunit_sequence_size}
* -------------------------------------------------------------
* Returns the size in bytes of the code unit sequence represented by the
* byte array {sequence}, interpreted with the provided encoding. {end}
* could be NULL and the bytes count will stop when the first
* NULL character is found, or a pointer to byte where the algorithm
* should stop.
* -------------------------------------------------------------
* {param: const FlByte* sequence} Source bytes array
* {param: FlEncoding encoding} Encoding used to interpret the bytes array
* {param: const FlByte* end} A pointer to a byte or NULL.
* -------------------------------------------------------------
* {return: size_t} Number of valid code units in sequence {sequence}
* -------------------------------------------------------------
*/
size_t fl_unicode_codeunit_sequence_size(const FlByte* sequence, FlEncoding encoding, const FlByte *end);

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
* {function: fl_unicode_char_encode_to}
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
FlUnicodeChar fl_unicode_char_encode_to(const FlUnicodeChar src, const FlEncoding srcencoding, const FlEncoding dstencoding);

#endif /* FL_UNICODE_H */
