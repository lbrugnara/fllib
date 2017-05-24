#include <string.h>

#include "../Std.h"
#include "../Mem.h"
#include "Unicode.h"

/* -------------------------------------------------------------
* Last code point (inclusive) of each UTF-8 range (1-4 bytes)
* -------------------------------------------------------------
*/
#define UTF8_LAST_CODEPOINT_1 0x00007F
#define UTF8_LAST_CODEPOINT_2 0x0007FF
#define UTF8_LAST_CODEPOINT_3 0x00FFFF
#define UTF8_LAST_CODEPOINT_4 0x10FFFF

/* -------------------------------------------------------------
* Lead byte of each UTF-8 and max value of each lead byte
* -------------------------------------------------------------
*/
#define UTF8_CODEPOINT_LEADBYTE_2 0xC0
#define UTF8_CODEPOINT_LEADBYTE_2_MAX 0xDF
#define UTF8_CODEPOINT_LEADBYTE_3 0xE0
#define UTF8_CODEPOINT_LEADBYTE_3_MAX 0xEF
#define UTF8_CODEPOINT_LEADBYTE_4 0xF0
#define UTF8_CODEPOINT_LEADBYTE_4_MAX 0xF7

// UTF32
#define UTF32_BYTES_SIZE 4


/* -------------------------------------------------------------
* PRIVATE API
* -------------------------------------------------------------
* Multibyte Strings: All these functions assume input-charset 
* and exec-charset are UTF-8
* -------------------------------------------------------------
*/
static inline bool utf8_mb_str_is_bigendian();
size_t utf8_bytes_count(const FlByte* src);
bool utf8_unicode_char_to_bytes(FlUnicodeChar chr, FlByte* dst);
FlUnicodeChar utf8_bytes_to_unicode_char(const FlByte *src);
FlUnicodeChar fl_unicode_utf32_to_utf8(FlUnicodeChar src);
FlUnicodeChar fl_unicode_utf8_to_utf32(FlUnicodeChar src);

/* -------------------------------------------------------------
* Checks if a multibyte character is stored as big-endiann. It
* is implementation dependant, maybe to make it compatible with
* common chars.
* -------------------------------------------------------------
*/
static inline bool utf8_mb_str_is_bigendian()
{
    FlString oldac = "𐌀"; // Hex 0xF0908C80
    int32_t oldah = 0xF0908C80;
    return ((FlByte*)oldac)[0] != ((FlByte*)&oldah)[0];
}

/* -------------------------------------------------------------
* Helper function to convert between multibyte and FlUnicodeChar
* representations.
* -------------------------------------------------------------
*/
void swap_representations(const FlByte *src, FlByte *dst, size_t nbytes)
{
    bool mbUsesBigEndian = utf8_mb_str_is_bigendian();
    for (size_t index=0, j=0; j < nbytes; j++)
    {
        if (mbUsesBigEndian)
            index = nbytes-1-j;
        dst[index] = src[j];
    }
}

/* -------------------------------------------------------------
* Returns the size of a UTF-8 valid character represented by a
* multi-byte character
* -------------------------------------------------------------
*/
size_t utf8_bytes_count(const FlByte* src)
{
    flm_assert(src != NULL, "The source byte array cannot be NULL.");
    
    // U+0000 to U+007F occupy 1 byte
    if (src[0] <= UTF8_LAST_CODEPOINT_1)
    {
        return 1;
    }
    
    // Check if first byte is the lead byte of 2-bytes code points
    // Check if first byte is a valid lead byte value for 2-bytes code points
    // Check if second byte is a continuation byte (0x80)
    // Check for invalid octets C0 and C1
    if ((src[0] & UTF8_CODEPOINT_LEADBYTE_2) && src[0] <= UTF8_CODEPOINT_LEADBYTE_2_MAX && (src[1] & 0x80) && src[0] != 0xC0 && src[0] != 0xC1)
    {
        return 2;
    }
    
    // Check if first byte is the lead byte of 3-bytes code points
    // Check if first byte is a valid lead byte value for 3-bytes code points
    // Check if second and third bytes are continuation bytes (0x80)
    // (inside) Check UTF-16 surrogates
    if ((src[0] & UTF8_CODEPOINT_LEADBYTE_3) && src[0] <= UTF8_CODEPOINT_LEADBYTE_3_MAX && ((src[0] == UTF8_CODEPOINT_LEADBYTE_3 && src[1] >= 0xA0) || (src[0] > UTF8_CODEPOINT_LEADBYTE_3 && src[1] >= 0x80)) && (src[2] & 0x80))
    {
        FlUnicodeChar chr = 0;
        swap_representations(src, (FlByte*)&chr, 3);
        // Surrogates (UTF-8 encoded)
        if (chr < 0xeda080 || chr > 0xedbfbf)
            return 3;
        return FL_UNICODE_INVALID_SIZE;
    }
    
    // Check if first byte is the lead byte of 4-bytes code points
    // Check if first byte is a valid lead byte value for 4-bytes code points
    // Check if second, third and fourth bytes are continuation bytes (0x80)
    // Check for invalid octets greather than F5 (F5-FF)
    if ((src[0] & UTF8_CODEPOINT_LEADBYTE_4) && src[0] <= UTF8_CODEPOINT_LEADBYTE_4_MAX && ((src[0] == UTF8_CODEPOINT_LEADBYTE_4 && src[1] >= 0x90) || (src[0] > UTF8_CODEPOINT_LEADBYTE_4 && src[1] >= 0x80)) && (src[2] & 0x80) && (src[3] & 0x80) && src[0] < 0xF5)
    {
        return 4;
    }

    return FL_UNICODE_INVALID_SIZE;
}

/* -------------------------------------------------------------
* Converts a FlUnicodeChar into its multi-byte representation. The
* result is assigned into {dst}. Caller MUST free the memory used
* by {dst}. {dst} MAY NOT be null-terminated
* -------------------------------------------------------------
*/
bool utf8_unicode_char_to_bytes(FlUnicodeChar chr, FlByte* dst)
{
    size_t size = fl_unicode_unichar_size(chr, FL_ENCODING_UTF8);
    if (size == FL_UNICODE_INVALID_SIZE)
        return false;
    swap_representations((FlByte*)&chr, dst, size);
    return true;
}

/* -------------------------------------------------------------
* Converts a multi-byte char into its FlUnicodeChar representation
* -------------------------------------------------------------
*/
FlUnicodeChar utf8_bytes_to_unicode_char(const FlByte *src)
{
    FlUnicodeChar chr = 0;
    size_t bs = utf8_bytes_count(src);

    if (bs == FL_UNICODE_INVALID_SIZE)
        return FL_UNICODE_INVALID_CHAR;

    swap_representations(src, (FlByte*)&chr, bs);
    return chr;
}

/* -------------------------------------------------------------
* Converts an UTF-32 FlUnicodeChar to its UTF-8 representation
* -------------------------------------------------------------
*/
FlUnicodeChar fl_unicode_utf32_to_utf8(FlUnicodeChar src)
{
    if (src <= UTF8_LAST_CODEPOINT_1)
    {
        return (FlUnicodeChar)src;
    }
    
    FlUnicodeChar chr = 0;
    FlByte* dst = (FlByte*)&chr;
    bool mbUsesBigEndian = utf8_mb_str_is_bigendian();
    if (src <= UTF8_LAST_CODEPOINT_2)
    {
        // Control 10xxxxxx = 0x80 | 0x3f: Get last 6 bit
        dst[mbUsesBigEndian ? 0 : 1] = 0x80 |  (src & 0x3f);
        // Lead 110xxxxx => Shift 6 bits used in a continuation byte and get last five (0x1f) bits
        dst[mbUsesBigEndian ? 1 : 0] = UTF8_CODEPOINT_LEADBYTE_2 | ((src >> 6) & 0x1F);
    }    
    else if (src <= UTF8_LAST_CODEPOINT_3) 
    {
        // Control 10xxxxxx = 0x80 | 0x3f: Get last 6 bit
        dst[mbUsesBigEndian ? 0 : 2] = 0x80 |  (src & 0x3f);
        // Control 10xxxxxx = 0x80 | 0x3f: Shift 6 and get last 6 bits
        dst[1                      ] = 0x80 | ((src >> 6) & 0x3f );
        // Lead 1110xxxx => Shift 12 bits used in two continuation bytes and get last four (0xf) bits
        dst[mbUsesBigEndian ? 2 : 0] = UTF8_CODEPOINT_LEADBYTE_3 | ((src >> 12) & 0xf);
    }
    else if (src <= UTF8_LAST_CODEPOINT_4) 
    {
        // Control 10xxxxxx = 0x80 | 0x3f: Get last 6 bit
        dst[mbUsesBigEndian ? 0 : 3] = 0x80 | (src & 0x3f);
        // Control 10xxxxxx = 0x80 | 0x3f: Shift 6 and get last 6 bits
        dst[mbUsesBigEndian ? 1 : 2] = 0x80 | ((src >> 6) & 0x3f);
        // Control 10xxxxxx = 0x80 | 0x3f: Shift 12 and get last 6 bits
        dst[mbUsesBigEndian ? 2 : 1] = 0x80 | ((src >> 12) & 0x3f);
        // Lead 1110xxxx => Shift 18 bits used in three continuation bytes and get last three (0x7) bits
        dst[mbUsesBigEndian ? 3 : 0] = UTF8_CODEPOINT_LEADBYTE_4 | ((src >> 18) & 0x7);
    }
    else
    {
        return FL_UNICODE_INVALID_CHAR;
    }
    return chr;
}

/* -------------------------------------------------------------
* Converts an UTF-8 FlUnicodeChar to its UTF-32 representation
* -------------------------------------------------------------
*/
FlUnicodeChar fl_unicode_utf8_to_utf32(FlUnicodeChar src)
{
    size_t l = fl_unicode_unichar_size(src, FL_ENCODING_UTF8);
    if (l == FL_UNICODE_INVALID_SIZE)
        return FL_UNICODE_INVALID_CHAR;

    if (l == 1) 
    {
        return (FlUnicodeChar)src;
    }

    FlUnicodeChar chr = 0;
    FlByte* dst = (FlByte*)&chr;
    bool mbUsesBigEndian = utf8_mb_str_is_bigendian();
    if (l == 2)
    {
        // LB: 110xxxxx CB: 10xxxxxx => We take the last two bytes from LB and 6 bits from CB (0x3f)
        dst[mbUsesBigEndian ? 0 : 1] = ((src >> 2) & 0xC0) | (src & 0x3f);
        // Remove CB (>> 8) and take the three first bits after 110 (avoid last two already taken in previous step)
        dst[mbUsesBigEndian ? 1 : 0] = ((src >> 8) & 0x1c) >> 2;
    }
    else if (l == 3)
    {
        // Format: LB: 1110xxxx CB1: 10xxxxxx CB2: 10xxxxxx
        // Shift 2 bits from CB1 to replace 10 from CB2. Get last 2 bits from CB1 | Get last 6 bits from CB2
        dst[mbUsesBigEndian ? 0 : 1] = ((src >> 2) & 0xC0 ) | (src & 0x3f);
        // Shift CB2 and the LS half of CB1. Get 4 MS bits from the result | Shift CB2 bits and the two LS bits from CB1. Get remaining 4 bits from CB1
        dst[mbUsesBigEndian ? 1 : 0] = ((src >> 12) & 0xF0 ) | ((src >> 10) & 0xF);
    }
    else if (l == 4)
    {
        // Format: LB: 11110xxx CB1: 10xxxxxx CB2: 10xxxxxx CB3: 10xxxxxx
        // Shift 2 bits from CB2 to replace 10 from CB3. Get last 2 bits from CB2 | Get last 6 bits from CB3
        dst[mbUsesBigEndian ? 0 : 2] = ((src >> 2) & 0xC0 ) | (src & 0x3f);
        dst[                      1] = ((src >> 12) & 0xF0 ) | ((src >> 10) & 0xF);
        dst[mbUsesBigEndian ? 2 : 0] = ((src >> 22) & 0x1C ) | ((src >> 20) & 0x3);
    }
    else
    {
        return FL_UNICODE_INVALID_CHAR;
    }
    return chr;
}

/* -------------------------------------------------------------
* PUBLIC API
* -------------------------------------------------------------
*/

/* -------------------------------------------------------------
* Returns an FlUnicodeChar from a byte array. The byte array encoding
* must match the 'encoding' parameter
* -------------------------------------------------------------
*/
FlUnicodeChar fl_unicode_codepoint_to_unichar(const FlByte* src, FlEncoding encoding)
{
    return fl_unicode_unichar_at(src, encoding, 0);
}

bool fl_unicode_unichar_to_codepoint(FlUnicodeChar chr, FlEncoding encoding, FlByte* dst)
{
    if (encoding == FL_ENCODING_UTF32)
    {
        memcpy(dst, &chr, UTF32_BYTES_SIZE);
        return true;
    }
    else if (encoding == FL_ENCODING_UTF8)
    {
        return utf8_unicode_char_to_bytes(chr, dst);
    }
    return false;
}

/* -------------------------------------------------------------
* Returns an FlUnicodeChar from a bytes array with 'dstencoding' encoding. 
* The bytes array encoding must match the 'encoding' parameter
* -------------------------------------------------------------
*/
FlUnicodeChar fl_unicode_codepoint_to_encoded_unichar(const FlByte* src, FlEncoding srcencoding, FlEncoding dstencoding)
{
    FlUnicodeChar chr = fl_unicode_unichar_at(src, srcencoding, 0);
    return fl_unicode_unichar_encode_to(chr, srcencoding, dstencoding);
}

bool fl_unicode_unichar_to_encoded_codepoint(FlUnicodeChar chr, FlEncoding srcencoding, FlByte* dst, FlEncoding dstencoding)
{
    FlUnicodeChar dstchr = fl_unicode_unichar_encode_to(chr, srcencoding, dstencoding);
    return fl_unicode_unichar_to_codepoint(dstchr, dstencoding, dst);
}

/* -------------------------------------------------------------
* Returns the size of the FlUnicodeChar {chr} encoded as {encoding}
* -------------------------------------------------------------
*/
size_t fl_unicode_unichar_size(const FlUnicodeChar chr, FlEncoding encoding)
{
    if (encoding == FL_ENCODING_UTF8)
    {
        // {todo: Check this block to see if we can use utf8_bytes_count directly after changing representaitons}
        // FlByte dst[] = {0x0,0x0,0x0,0x0};
        // swap_representations((FlByte*)&chr, dst, 4);
        // size_t i=0;
        // while (dst[i] == 0x0) i++;
        // return utf8_bytes_count(dst+i);
        
        FlByte *src = (FlByte*)&chr;
        // Check if first byte is the lead byte of 4-bytes code points
        // Check if first byte is a valid lead byte value for 4-bytes code points
        // Check if second, third and fourth bytes are continuation bytes (0x80)
        // Check for invalid octets greather than F5 (F5-FF)
        if ((src[3] & UTF8_CODEPOINT_LEADBYTE_4) && src[3] <= UTF8_CODEPOINT_LEADBYTE_4_MAX && ((src[3] == UTF8_CODEPOINT_LEADBYTE_4 && src[2] >= 0x90) || (src[3] > UTF8_CODEPOINT_LEADBYTE_4 && src[2] >= 0x80)) && (src[1] & 0x80) && (src[0] & 0x80) && src[3] < 0xF5)
        {
            return 4;
        }

        // Check if first byte is the lead byte of 3-bytes code points
        // Check if first byte is a valid lead byte value for 3-bytes code points
        // Check if second and third bytes are continuation bytes (0x80)
        // (inside) Check UTF-16 surrogates
        if (src[3] == 0x0 && (src[2] & UTF8_CODEPOINT_LEADBYTE_3) && src[2] <= UTF8_CODEPOINT_LEADBYTE_3_MAX && ((src[2] == UTF8_CODEPOINT_LEADBYTE_3 && src[1] >= 0xA0) || (src[2] > UTF8_CODEPOINT_LEADBYTE_3 && src[1] >= 0x80)) && (src[0] & 0x80))
        {
            // Surrogates (UTF-8 encoded)
            if (chr < 0xeda080 || chr > 0xedbfbf)
                return 3;
            return FL_UNICODE_INVALID_SIZE;
        }

        // Check if first byte is the lead byte of 2-bytes code points
        // Check if first byte is a valid lead byte value for 2-bytes code points
        // Check if second byte is a continuation byte (0x80)
        // Check for invalid octets C0 and C1
        if (src[3] == 0x0 && src[2] == 0x0 && (src[1] & UTF8_CODEPOINT_LEADBYTE_2) && src[1] <= UTF8_CODEPOINT_LEADBYTE_2_MAX && (src[0] & 0x80) && src[1] != 0xC0 && src[1] != 0xC1)
        {
            return 2;
        }

        if (src[3] == 0x0 && src[2] == 0x0 && src[1] == 0x0 && chr <= UTF8_LAST_CODEPOINT_1)
        {
            return 1;
        }
    }
    else if (encoding == FL_ENCODING_UTF32 && chr <= UTF8_LAST_CODEPOINT_4 && (chr < 0xD800 || chr > 0xDFFF))
    {
        return 4;
    }
    return FL_UNICODE_INVALID_SIZE; // Not unicode
}

size_t fl_unicode_codepoint_size(const FlByte* src, FlEncoding encoding)
{
    if (encoding == FL_ENCODING_UTF32)
    {
        FlUnicodeChar srcchr = fl_unicode_codepoint_to_unichar(src, FL_ENCODING_UTF32);
        if (srcchr <= UTF8_LAST_CODEPOINT_4)
        {
            return UTF32_BYTES_SIZE;
        }
        return FL_UNICODE_INVALID_SIZE;
    }
    else if (encoding == FL_ENCODING_UTF8)
    {
        return utf8_bytes_count(src);
    }
    return FL_UNICODE_INVALID_SIZE;
}

size_t fl_unicode_unichar_sequence_size(const FlUnicodeChar *string, FlEncoding encoding, FlUnicodeChar *end)
{
    flm_assert(string != NULL, "Source string cannot be NULL.");
    
    size_t size = 0;
    size_t i = 0;
    do
    {
        size_t tmp = fl_unicode_unichar_size(string[i], encoding);
        if (tmp == FL_UNICODE_INVALID_SIZE)
            break;
        size += tmp;
        i++;
    } while ((end == NULL && string[i]) || (end != NULL && (string+i) < end));
    return size;
}

// TODO: Fix for UTF16
/* -------------------------------------------------------------
* Returns the size of a Unicode string
* -------------------------------------------------------------
*/
size_t fl_unicode_codeunit_sequence_size(const FlByte* sequence, FlEncoding encoding, const FlByte* end)
{
    flm_assert(sequence != NULL, "Code unit sequence cannot be NULL.");
    
    size_t size = 0;
    if (encoding == FL_ENCODING_UTF32)
    {
        size_t i = 0;
        const uint32_t *src = (const uint32_t*)sequence;
        do
        {
            size += UTF32_BYTES_SIZE;
            i++;
        } while ((end == NULL && src[i]) || (end != NULL && ((FlByte*)src)+i < end));
    }
    else if (encoding == FL_ENCODING_UTF8)
    {
        size_t i=0;
        do
        {
            size_t tmp = utf8_bytes_count(sequence+i);
            if (tmp == FL_UNICODE_INVALID_SIZE)
                break; // truncated string
            // If we are NOT taking in consideration the NULL character for the size (it is the end marker), set tmp to 0
            if (tmp == 1 && sequence[i] == 0x0 && end == NULL)
                tmp = 0;
            size += tmp;
            i += tmp;
        } while ((end == NULL && sequence[i]) || (end != NULL && sequence+i < end));
    }
    return size;
}

/* -------------------------------------------------------------
* Returns the 'at'-th character of a Unicode string encoded as 'encoding'
* -------------------------------------------------------------
*/
FlUnicodeChar fl_unicode_unichar_at(const FlByte* str, FlEncoding encoding, size_t at)
{
    // Initialize chr to 0 to "clear" it
    FlUnicodeChar chr = 0;
    if (encoding == FL_ENCODING_UTF32)
    {
        chr = ((FlUnicodeChar*)str)[at];
    }
    else if (encoding == FL_ENCODING_UTF8)
    {
        size_t offset = 0;
        for (size_t i=0; i < at; i++)
        {
            offset += fl_unicode_codeunit_sequence_size(str+offset, encoding, str+offset+1);
        }
        size_t bs = fl_unicode_codeunit_sequence_size(str+offset, encoding, str+offset+1);
        swap_representations(str+offset, (FlByte*)&chr, bs);
    }
    else
    {
        chr = FL_UNICODE_INVALID_CHAR;
    }
    return chr;
}

/* -------------------------------------------------------------
* Takes a source character encoded under 'srcencoding' and Returns
* a new character representing 'src' version under encoding 'dstencoding'
* -------------------------------------------------------------
*/
FlUnicodeChar fl_unicode_unichar_encode_to(const FlUnicodeChar src, const FlEncoding srcencoding, const FlEncoding dstencoding)
{
    if (srcencoding == FL_ENCODING_UTF32)
    {
        if (dstencoding == FL_ENCODING_UTF8)
        {
            return fl_unicode_utf32_to_utf8(src);
        }
    }
    else if (srcencoding == FL_ENCODING_UTF8)
    {
        if (dstencoding == FL_ENCODING_UTF32)
        {
            return fl_unicode_utf8_to_utf32(src);
        }
    }
    return 0;
}

bool fl_unicode_unichar_is_valid(const FlUnicodeChar chr, FlEncoding encoding)
{
    return fl_unicode_unichar_size(chr, encoding) != FL_UNICODE_INVALID_SIZE;
}

bool fl_unicode_codepoint_is_valid(const FlByte* src, FlEncoding encoding)
{
    return fl_unicode_codepoint_size(src, encoding) != FL_UNICODE_INVALID_SIZE;
}

bool fl_unicode_unichar_sequence_is_valid(const FlUnicodeChar *sequence, FlEncoding encoding, FlUnicodeChar *end)
{
    flm_assert(sequence != NULL, "Source sequence cannot be NULL.");

    size_t i = 0;
    while ((end == NULL && sequence[i]) || (end != NULL && (sequence+i) < end))
    {
        if (fl_unicode_unichar_size(sequence[i], encoding) == FL_UNICODE_INVALID_SIZE)
            return false;
    }
    return true;
}


bool fl_unicode_codeunit_sequence_is_valid(const FlByte* src, FlEncoding encoding, FlByte* end)
{
    flm_assert(src != NULL, "Source src cannot be NULL.");

    size_t i = 0;
    while ((end == NULL && src[i]) || (end != NULL && (src+i) < end))
    {
        size_t tmp = fl_unicode_codepoint_size(src+i, encoding);
        if (tmp == FL_UNICODE_INVALID_SIZE)
            return false;
        i += tmp;
    }
    return true;
}
