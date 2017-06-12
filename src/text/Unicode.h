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
* All the functions using the keywords /codepoint/ and /codeunit/, are functions
* that work with stream of bytes representing code units.
*
* {FlUnicodeChar} It is a typedef uint32_t, it can represent any
* unicode point in any encoding. If you want the UTF-32 representation
* of code point U+0181, you simply write `FlUnicodeChar u181 = 0x0181; // UTF-32`,
* or if you want its UTF-8 representaiton you write `FlUnicodeChar u181 = 0xc681; // UTF-8`.
* In this module the set of functions that work with FlUnicodeChar are the functions that contains
* in its name the work /unichar/. unichar is used as a synonym of FlUnicodeChar.
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
*
* TODO
* ====
* - Add fl_unicode_unichar_validate: If the input is invalid, return FL_UNICODE_REPLACEMENT_CHARACTER
* - Add fl_unicode_unichar_sequence_validate: If the input is invalid, replace ill-formed characters by FL_UNICODE_REPLACEMENT_CHARACTER
* - Add fl_unicode_codepoint_validate: If the input is invalid, return REPLACEMENT CHARACTER
* - Add fl_unicode_codepoint_sequence_validate: If the input is invalid, replace ill-formed characters by REPLACEMENT CHARACTER
*
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
* {define: FL_UNICODE_INVALID_CHAR}
* -------------------------------------------------------------
* Represents an invalid Unicode character. FlUnicodeChar is an
* unsigned 32 bits int, so to represent an invalid unicode code point
* we used 2^32-1 bits (too much greater than the maximum valid code point)
* -------------------------------------------------------------
*/
#define FL_UNICODE_INVALID_CHAR UINT32_MAX

/* -------------------------------------------------------------
* {define: FL_UNICODE_INVALID_SIZE}
* -------------------------------------------------------------
* Represents an invalid Unicode character's size. The size of a single
* code unit could be between 1 and 4 bytes (depending on the encoding).
* Using SIZE_MAX we represent an invalid size for a unicode code point.
* -------------------------------------------------------------
*/
#define FL_UNICODE_INVALID_SIZE SIZE_MAX

/* -------------------------------------------------------------
* {define: FL_UNICODE_REPLACEMENT_CHARACTER}
* -------------------------------------------------------------
* It is the Unicode Character (U+FFFD)
* -------------------------------------------------------------
*/
#define FL_UNICODE_REPLACEMENT_CHARACTER 0xFFFD

#define fl_unicode_codeunit_sequence_equals(a,b,s) (memcmp(a,b,s) == 0)

/* -------------------------------------------------------------
* UNICODE CODE POINT AND UNIT FUNCTIONS (FlByte*)
* -------------------------------------------------------------
*/

size_t fl_unicode_codepoint_convert(FlEncoding srcencoding, const FlByte *src, const FlByte *end, FlEncoding dstencoding, FlByte *dst);

/* -------------------------------------------------------------
* {function: fl_unicode_codepoint_size}
* -------------------------------------------------------------
* Returns the size in bytes of the {src} input. If {src} is an invalid
* unicode code point, this function returns FL_UNICODE_INVALID_SIZE, 
* otherwise >= 0.
* -------------------------------------------------------------
* {param: FlEncoding encoding} {src} encoding
* {param: const FlByte* src} Input bytes to get the size
* {param: const FlByte* end} A pointer to a byte or NULL to stop while 
* computing the code point size
* -------------------------------------------------------------
* {return: size_t} Size in bytes of input bytes {src}. If {src} is an invalid
* unicode char it returns FL_UNICODE_INVALID_SIZE
* -------------------------------------------------------------
*/
size_t fl_unicode_codepoint_size(FlEncoding encoding, const FlByte *src, const FlByte *end);

/* -------------------------------------------------------------
* {function: fl_unicode_codeunit_sequence_size}
* -------------------------------------------------------------
* Returns the size in bytes of the code unit sequence represented by the
* byte array {sequence}, interpreted with the provided encoding. {end}
* could be NULL and the bytes count will stop when the first
* NULL character is found, or a pointer to byte where the algorithm
* should stop.
* This function returns the number of well-formed bytes under encoding
* {encoding} that are found in {sequence}.
* -------------------------------------------------------------
* {param: FlEncoding encoding} Encoding used to interpret the bytes array
* {param: const FlByte* sequence} Source bytes array
* {param: const FlByte* end} A pointer to a byte or NULL to stop processing {sequence}.
* -------------------------------------------------------------
* {return: size_t} Number of valid code units in sequence {sequence}
* -------------------------------------------------------------
*/
size_t fl_unicode_codeunit_sequence_size(FlEncoding encoding, const FlByte* sequence, const FlByte *end);

/* -------------------------------------------------------------
* {function: fl_unicode_codepoint_sequence_length}
* -------------------------------------------------------------
* Returns the count of valid code points in sequence represented by the
* byte array {sequence}, interpreted with the provided encoding. {end}
* could be NULL and the bytes count will stop when the first
* NULL character is found, or a pointer to byte where the algorithm
* should stop.
* This function returns the number of well-formed code points under encoding
* {encoding} that are found in {sequence}.
* -------------------------------------------------------------
* {param: FlEncoding encoding} Encoding used to interpret the bytes array
* {param: const FlByte* sequence} Source bytes array
* {param: const FlByte* end} A pointer to a byte or NULL to stop processing {sequence}.
* -------------------------------------------------------------
* {return: size_t} Number of valid code points in sequence {sequence}
* -------------------------------------------------------------
*/
size_t fl_unicode_codepoint_sequence_length(FlEncoding encoding, const FlByte* sequence, const FlByte *end);

/* -------------------------------------------------------------
* {function: fl_unicode_codepoint_at}
* -------------------------------------------------------------
* Returns the 'at'-th character in the string {str}. The char
* is an FlUnicodeChar (a UTF-8 representation of the caracter)
* that could be easily used to compare it with another UTF-8
* character that could be generated using the fl_unicode_char_from_xxx
* functions family
* -------------------------------------------------------------
* {param: FlEncoding encoding} Encoding used to interpret {str}
* {param: const FlByte* str} String to retrieve the 'at'-th character
* {param: const FlByte* end} A pointer to a byte or NULL to stop processing {str}.
* {param: size_t at} Position in string to retrieve the character
* {param: FlByte* dst} Destination allocation for the {at}-character of
* {str}. User must assign enough space in {dst} to hold a code point (Up to 4 bytes)
* -------------------------------------------------------------
* {return: size_t} Count of bytes populated in {dst}
* -------------------------------------------------------------
*/
size_t fl_unicode_codepoint_at(FlEncoding encoding, const FlByte *str, const FlByte *end, size_t at, FlByte *dst);

/* -------------------------------------------------------------
* {function: fl_unicode_codepoint_is_valid}
* -------------------------------------------------------------
* Returns true if {src} is a valid code point under encoding
* {encoding}.
* -------------------------------------------------------------
* {param: FlEncoding encoding} Encoding of {src}
* {param: const FlByte* src} Bytes that compose the codepoint
* {param: const FlByte* end} Byte address from {src} where to stop processing,
* or NULL to stop at the first 0x0 value.
* -------------------------------------------------------------
* {return: bool} True if the codepoint encoded in {src} is valid under
* encoding {encoding}
* -------------------------------------------------------------
*/
bool fl_unicode_codepoint_is_valid(FlEncoding encoding, const FlByte *src, const FlByte *end);

/* -------------------------------------------------------------
* {function: fl_unicode_codeunit_sequence_is_valid}
* -------------------------------------------------------------
* Returns true if {src} is a valid sequence of code points under encoding
* {encoding}.
* -------------------------------------------------------------
* {param: FlEncoding encoding} Encoding of {src}
* {param: const FlByte* src} Bytes that compose the code unit sequence
* {param: const FlByte* end} Byte address from {src} where to stop processing,
* or NULL to stop at the first 0x0 value.
* -------------------------------------------------------------
* {return: bool} True if the codepoint sequence encoded in {src} is valid under
* encoding {encoding}
* -------------------------------------------------------------
*/
bool fl_unicode_codeunit_sequence_is_valid(FlEncoding encoding, const FlByte* src, const FlByte* end);

#endif /* FL_UNICODE_H */
