#ifndef FL_UNICODE_H
#define FL_UNICODE_H
/*
 * file: UNICODE
 *
 * Contains a set of function to work with sequence of {FlByte}s representing
 * unicode codepoints.
 * This modules is a partial implementation of the Unicode Standard version 9.0.
 * 
 * TODO
 * ====
 * 
 * Normalization
 * Case-Folding
 * UTF-16 support
 *
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

/*
 * define: FL_UNICODE_INVALID_SIZE
 *
 * Represents an invalid Unicode character's size. The size of a single
 * code unit could be between 1 and 4 bytes (depending on the encoding).
 * Using SIZE_MAX we represent an invalid size for a unicode code point.
 *
 */
#define FL_UNICODE_INVALID_SIZE SIZE_MAX

/*
 * define: FL_UNICODE_REPLACEMENT_CHARACTER
 *
 * It is the Unicode Character (U+FFFD)
 *
 */
#define FL_UNICODE_REPLACEMENT_CHARACTER 0xFFFD

/*
 * UNICODE CODE POINT AND UNIT FUNCTIONS (FlByte*)
 *
 */

/*
 * Function: fl_unicode_codepoint_convert
 *
 * Converts code point units between Unicode representations.
 *
 * FlEncoding encoding} Encoding used by {src -
 * const FlByte* src} Source code point under encoding {srcencoding -
 * const FlByte* end} A pointer to a byte or NULL to stop while computing {src - code point size
 * FlEncoding dstencoding} Target encoding to convert {src - to
 * FlByte* dst} Destination to pointer to write the bytes of {src} encoding with {dstencoding -
 *
 * {return: size_t} Number of bytes written in {dst}
 *
 */
size_t fl_unicode_codepoint_convert(FlEncoding srcencoding, const FlByte *src, const FlByte *end, FlEncoding dstencoding, FlByte *dst);

/*
 * Function: fl_unicode_codepoint_size
 *
 * Returns the size in bytes of the {src} input. If {src} is an invalid
 * unicode code point, this function returns FL_UNICODE_INVALID_SIZE, 
 * otherwise >= 0.
 *
 * FlEncoding encoding} {src - encoding
 * const FlByte* src - Input bytes to get the size
 * const FlByte* end - A pointer to a byte or NULL to stop while 
 * computing the code point size
 *
 * {return: size_t} Size in bytes of input bytes {src}. If {src} is an invalid
 * unicode char it returns FL_UNICODE_INVALID_SIZE
 *
 */
size_t fl_unicode_codepoint_size(FlEncoding encoding, const FlByte *src, const FlByte *end);

/*
 * Function: fl_unicode_codeunit_sequence_size
 *
 * Returns the size in bytes of the code unit sequence represented by the
 * byte array {sequence}, interpreted with the provided encoding. {end}
 * could be NULL and the bytes count will stop when the first
 * NULL character is found, or a pointer to byte where the algorithm
 * should stop.
 * This function returns the number of well-formed bytes under encoding
 * {encoding} that are found in {sequence}. If any ill-formed character is found
 * the algorithm will stop returning the number of valid bytes before the
 * ill-formed character occurrence.
 *
 * FlEncoding encoding - Encoding used to interpret the bytes array
 * const FlByte* sequence - Source bytes array
 * const FlByte* end} A pointer to a byte or NULL to stop processing {sequence -.
 *
 * {return: size_t} Number of valid code units in sequence {sequence}
 *
 */
size_t fl_unicode_codeunit_sequence_size(FlEncoding encoding, const FlByte* sequence, const FlByte *end);

/*
 * Function: fl_unicode_codepoint_sequence_length
 *
 * Returns the count of valid code points in sequence represented by the
 * byte array {sequence}, interpreted with the provided encoding. {end}
 * could be NULL and the bytes count will stop when the first
 * NULL character is found, or a pointer to byte where the algorithm
 * should stop.
 * This function returns the number of well-formed code points under encoding
 * {encoding} that are found in {sequence}. If any ill-formed character is found
 * the algorithm will stop returning the number of valid code points before the
 * ill-formed character occurrence.
 *
 * FlEncoding encoding - Encoding used to interpret the bytes array
 * const FlByte* sequence - Source bytes array
 * const FlByte* end} A pointer to a byte or NULL to stop processing {sequence -.
 *
 * {return: size_t} Number of valid code points in sequence {sequence}
 *
 */
size_t fl_unicode_codepoint_sequence_length(FlEncoding encoding, const FlByte* sequence, const FlByte *end);

/*
 * Function: fl_unicode_codepoint_at
 *
 * Copies the {at}-th character from {str} with encoding {encoding} 
 * into {dst}. If the unicode string and target character is valid, it will return the
 * amount of bytes copied into {dst}. If any character inside {str} between 0 and {at}
 * is invalid this function returns FL_UNICODE_INVALID_SIZE
 *
 * FlEncoding encoding} Encoding used to interpret {str -
 * const FlByte* str} String to retrieve the {at --th character
 * const FlByte* end} A pointer to a byte or NULL to stop processing {str -.
 * size_t at - Position in string to retrieve the character
 * FlByte* dst} Destination allocation for the {at --character of
 * {str}. User must assign enough space in {dst} to hold a code point (Up to 4 bytes)
 *
 * {return: size_t} Count of bytes populated in {dst} or FL_UNICODE_INVALID_SIZE
 *
 */
size_t fl_unicode_codepoint_at(FlEncoding encoding, const FlByte *str, const FlByte *end, size_t at, FlByte *dst);

/*
 * Function: fl_unicode_codepoint_is_valid
 *
 * Returns true if {src} is a valid code point under encoding
 * {encoding}.
 *
 * FlEncoding encoding} Encoding of {src -
 * const FlByte* src - Bytes that compose the codepoint
 * const FlByte* end} Byte address from {src - where to stop processing,
 * or NULL to stop at the first 0x0 value.
 *
 * {return: bool} True if the codepoint encoded in {src} is valid under
 * encoding {encoding}
 *
 */
bool fl_unicode_codepoint_is_valid(FlEncoding encoding, const FlByte *src, const FlByte *end);

/*
 * Function: fl_unicode_codepoint_sequence_is_valid
 *
 * Returns true if {src} is a valid sequence of code points under encoding
 * {encoding}.
 *
 * FlEncoding encoding} Encoding of {src -
 * const FlByte* src - Bytes that compose the code unit sequence
 * const FlByte* end} Byte address from {src - where to stop processing,
 * or NULL to stop at the first 0x0 value.
 *
 * {return: bool} True if the codepoint sequence encoded in {src} is valid under
 * encoding {encoding}
 *
 */
bool fl_unicode_codepoint_sequence_is_valid(FlEncoding encoding, const FlByte* src, const FlByte* end);

/*
 * Function: fl_unicode_codepoint_sequence_validate
 *
 * Takes {sequence} and replace all the ill-formed code points by
 * REPLACEMENT CHARACTER (U+FFFD). The new sequence will be allocated
 * into {destination}, and the size of the new sequence in bytes will be
 * returned by the function.
 *
 * FlEncoding encoding} Encoding of the source {sequence -
 * const FlByte* sequence - Source sequence to convert to a valid Unicode sequence
 * const FlByte* end} Byte address from {src - where to stop processing,
 * FlByte** destination} The new sequence will be allocated into {destination -
 *
 * {return: size_t} Number of bytes allocated into destination.
 *
 */
size_t fl_unicode_codepoint_sequence_validate(FlEncoding encoding, const FlByte *sequence, const FlByte *end, FlByte **destination);

#endif /* FL_UNICODE_H */
