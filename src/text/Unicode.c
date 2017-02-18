#include <string.h>

#include "../Std.h"
#include "../Mem.h"
#include "Unicode.h"

/* -------------------------------------------------------------
* UNICODE
* -------------------------------------------------------------
*/
#define UTF8_CP_MAXB_LOCK_1 0x00007F
#define UTF8_CP_MAXB_LOCK_2 0x0007FF
#define UTF8_CP_MAXB_LOCK_3 0x00FFFF
#define UTF8_CP_MAXB_LOCK_4 0x10FFFF

#define UTF8_CP_LEADBYTE_2 0xC0
#define UTF8_CP_LEADBYTE_2_MAX 0xDF
#define UTF8_CP_LEADBYTE_3 0xE0
#define UTF8_CP_LEADBYTE_3_MAX 0xEF
#define UTF8_CP_LEADBYTE_4 0xF0
#define UTF8_CP_LEADBYTE_4_MAX 0xF7

// UTF32
#define REPLACEMENT_CHARACTER 0xFFFD

FlUnicodeChar fl_unicode_utf32_to_utf8(FlUnicodeChar src);
FlUnicodeChar fl_unicode_utf8_to_utf32(FlUnicodeChar src);

// MultiByte string functions. ALl these functions assume UTF-8 for input and exec charset
int32_t utf8_bytes_count(const FlByte* src);
bool utf8_unicode_char_to_bytes(FlUnicodeChar chr, FlByte* dst);
FlUnicodeChar utf8_bytes_to_unicode_char(const FlByte *src);
bool utf8_mb_str_is_bigendian();

// Public API

/* -------------------------------------------------------------
* Returns an FlUnicodeChar from a bytes array. The bytes array encoding
* must match the 'encoding' parameter
* -------------------------------------------------------------
*/
FlUnicodeChar fl_unicode_codepoint_to_char(const FlByte* src, FlEncoding encoding)
{
    return fl_unicode_char_at(src, encoding, 0);
}

bool fl_unicode_char_to_codepoint(FlUnicodeChar chr, FlEncoding encoding, FlByte* dst)
{
    if (encoding == FL_ENCODING_UTF32)
    {
        dst[5] = 0x0;
        memcpy(dst, &chr, 4);
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
FlUnicodeChar fl_unicode_codepoint_to_encoding_to_char(const FlByte* src, FlEncoding srcencoding, FlEncoding dstencoding)
{
    FlUnicodeChar chr = fl_unicode_char_at(src, srcencoding, 0);
    return fl_unicode_char_encode_to(chr, srcencoding, dstencoding);
}

bool fl_unicode_char_to_encoding_to_codepoint(FlUnicodeChar chr, FlEncoding srcencoding, FlByte* dst, FlEncoding dstencoding)
{
    FlUnicodeChar dstchr = fl_unicode_char_encode_to(chr, srcencoding, dstencoding);
    return fl_unicode_char_to_codepoint(dstchr, dstencoding, dst);
}

/* -------------------------------------------------------------
* Returns the size of the FlUnicodeChar 'chr' under provided encoding
* -------------------------------------------------------------
*/
int32_t fl_unicode_char_size(const FlUnicodeChar chr, FlEncoding encoding)
{
    if (encoding == FL_ENCODING_UTF8)
    {
        FlByte *chrb = (FlByte*)&chr;
        if (chrb[3] > UTF8_CP_LEADBYTE_3_MAX && (chrb[3] & UTF8_CP_LEADBYTE_4) && chrb[3] <= UTF8_CP_LEADBYTE_4_MAX && chrb[3] != 0xF5)
        {
            return 4;
        }
        else if (chrb[2] > UTF8_CP_LEADBYTE_2_MAX && (chrb[2] & UTF8_CP_LEADBYTE_3) && chrb[2] <= UTF8_CP_LEADBYTE_3_MAX)
        {
            // Surrogates
            if (chr < 0xeda080 || chr > 0xedbfbf)
                return 3;
            return -1;
        }
        else if (chrb[1] > UTF8_CP_MAXB_LOCK_1 && (chrb[1] & UTF8_CP_LEADBYTE_2) && chrb[1] <= UTF8_CP_LEADBYTE_2_MAX && chrb[1] != 0xC0 && chrb[1] != 0xC1)
        {
            return 2;
        }
        else if (chr <= UTF8_CP_MAXB_LOCK_1)
        {
            return 1;
        }
    }
    else if (encoding == FL_ENCODING_UTF32 && chr <= 0x10FFFF)
    {
        return 4;
    }
    return -1; // Not unicode
}

int32_t fl_unicode_codepoint_size(const FlByte* src, FlEncoding encoding)
{
    if (encoding == FL_ENCODING_UTF32)
    {
        FlUnicodeChar srcchr = fl_unicode_codepoint_to_char(src, FL_ENCODING_UTF32);
        if (srcchr > UTF8_CP_MAXB_LOCK_4)
        {
            return -1;
        }
        return 4;
    }
    else if (encoding == FL_ENCODING_UTF8)
    {
        return utf8_bytes_count(src);
    }
    return -1;
}

size_t fl_unicode_char_array_size(const FlUnicodeChar *string, FlEncoding encoding, FlUnicodeChar *end)
{
    if (string == 0x00)
        return 0;
    
    size_t size = 0;
    size_t i = 0;
    do
    {
        int32_t tmp = fl_unicode_char_size(string[i], encoding);
        if (tmp == -1)
        {
            // Invalid char
            break;
        }
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
    if (sequence == 0x00)
        return 0;
    
    size_t size = 0;
    if (encoding == FL_ENCODING_UTF32)
    {
        size_t i = 0;
        const uint32_t *src = (const uint32_t*)&sequence;
        do
        {
            size += 4;
            i++;
        } while ((end == NULL && src[i]) || (end != NULL && ((FlByte*)src)+i < end));
    }
    else if (encoding == FL_ENCODING_UTF8)
    {
        size_t i=0;
        do
        {
            int32_t tmp = utf8_bytes_count(sequence+i);
            if (tmp == -1)
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
FlUnicodeChar fl_unicode_char_at(const FlByte* str, FlEncoding encoding, size_t at)
{
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
        bool mbUsesBigEndian = utf8_mb_str_is_bigendian();
        for (size_t index=0, j=0; j < bs; j++)
        {
            if (mbUsesBigEndian)
                index = bs-1-j;
            ((FlByte*)&chr)[index] = (str+offset)[j];
        }
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
FlUnicodeChar fl_unicode_char_encode_to(const FlUnicodeChar src, const FlEncoding srcencoding, const FlEncoding dstencoding)
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

/* -------------------------------------------------------------
* Converts an UTF-32 FlUnicodeChar to its UTF-8 representation
* -------------------------------------------------------------
*/
FlUnicodeChar fl_unicode_utf32_to_utf8(FlUnicodeChar src)
{
    FlUnicodeChar chr = 0;
    FlByte* dst = (FlByte*)&chr;
    if (src <= UTF8_CP_MAXB_LOCK_1)
    {
        chr = (FlUnicodeChar)src;
        return chr;
    }
    
    bool mbUsesBigEndian = utf8_mb_str_is_bigendian();
    if (src <= UTF8_CP_MAXB_LOCK_2)
    {
        // Control 10xxxxxx = 0x80 | 0x3f
        dst[mbUsesBigEndian ? 0 : 1] = 0x80 |  (src & 0x3f);
        // Lead 110xxxxx => Shift 6 bits used in a continuation byte and get last five bits
        dst[mbUsesBigEndian ? 1 : 0] = UTF8_CP_LEADBYTE_2 | ((src >> 6) & 0x1F);
    }    
    else if (src <= UTF8_CP_MAXB_LOCK_3) 
    {
        // Control 10xxxxxx = 0x80 | 0x3f: Get last 6 bit
        dst[mbUsesBigEndian ? 0 : 2] = 0x80 |  (src & 0x3f);
        // Control 10xxxxxx = 0x80 | 0x3f: Shift 6 and get last 6 bits
        dst[1                      ] = 0x80 | ((src >> 6) & 0x3f );
        // Lead 1110xxxx => Shift 12 bits used in two continuation bytes and get last four bits
        dst[mbUsesBigEndian ? 2 : 0] = UTF8_CP_LEADBYTE_3 | ((src >> 12) & 0xf);
    }
    else if (src <= UTF8_CP_MAXB_LOCK_4) 
    {
        // Control 10xxxxxx = 0x80 | 0x3f: Get last 6 bit
        dst[mbUsesBigEndian ? 0 : 3] = 0x80 | (src & 0x3f);
        // Control 10xxxxxx = 0x80 | 0x3f: Shift 6 and get last 6 bits
        dst[mbUsesBigEndian ? 1 : 2] = 0x80 | ((src >> 6) & 0x3f);
        // Control 10xxxxxx = 0x80 | 0x3f: Shift 6 and get last 12 bits
        dst[mbUsesBigEndian ? 2 : 1] = 0x80 | ((src >> 12) & 0x3f);
        // Lead 1110xxxx => Shift 18 bits used in three continuation bytes and get last three bits
        dst[mbUsesBigEndian ? 3 : 0] = UTF8_CP_LEADBYTE_4 | ((src >> 18) & 0x7);
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
    FlUnicodeChar chr = 0;
    int32_t l = fl_unicode_char_size(src, FL_ENCODING_UTF8);
    if (l == -1)
        return FL_UNICODE_INVALID_CHAR;
    if (l == 1) 
    {
        chr = (FlUnicodeChar)src;
        return chr;
    }
    // TODO
    FlByte* dst = (FlByte*)&chr;
    FlByte* srcp = (FlByte*)&src;
    bool mbUsesBigEndian = utf8_mb_str_is_bigendian();
    if (l == 2)
    {
        // LB: 110xxxxx CB: 10xxxxxx => We take the last two bytes from LB (0x300, but we need to shift it to join with the other 6) and 6 bits from CB (0x3f) and
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
* MULTI-BYTE STRINGS
* -------------------------------------------------------------
* All these functions assume input-charset and exec-charset are UTF-8
* -------------------------------------------------------------
*/

/* -------------------------------------------------------------
* Checks if a multibyte character is stored as big-endiann. It
* is implementation dependant, maybe to make it compatible with
* common chars.
* -------------------------------------------------------------
*/
bool utf8_mb_str_is_bigendian()
{
    FlString oldac = "𐌀"; // Hex 0xF0908C80
    int32_t oldah = 0xF0908C80;
    return ((FlByte*)oldac)[0] != ((FlByte*)&oldah)[0];
}

/* -------------------------------------------------------------
* Converts a FlUnicodeChar into its multi-byte representation. The
* result is assigned into 'dst'. Caller MUST free the memory used
* by 'dst'
* -------------------------------------------------------------
*/
bool utf8_unicode_char_to_bytes(FlUnicodeChar chr, FlByte* dst)
{
    int32_t size = fl_unicode_char_size(chr, FL_ENCODING_UTF8);
    if (size == -1)
        return false;
    dst[size] = 0x0;
    FlByte* src = (FlByte*)&chr;
    bool mbUsesBigEndian = utf8_mb_str_is_bigendian();
    for (int32_t index=0, j=0; j < size; j++)
    {
        if (mbUsesBigEndian)
            index = size-1-j;
        dst[index] = src[j];
    }
    return true;
}

/* -------------------------------------------------------------
* Converts a multi-byte char into its FlUnicodeChar representation
* -------------------------------------------------------------
*/
FlUnicodeChar utf8_bytes_to_unicode_char(const FlByte *src)
{
    FlUnicodeChar chr = 0;
    int32_t bs = utf8_bytes_count(src);

    if (bs == -1)
        return FL_UNICODE_INVALID_CHAR;

    bool mbUsesBigEndian = utf8_mb_str_is_bigendian();
    for (int32_t index=0, j=0; j < bs; j++)
    {
        if (mbUsesBigEndian)
            index = bs-1-j;
        ((FlByte*)&chr)[index] = src[j];
    }
    return chr;
}

/* -------------------------------------------------------------
* Returns the size of a UTF-8 valida character represented by a
* multi-byte character
* -------------------------------------------------------------
*/
int32_t utf8_bytes_count(const FlByte* src)
{
    if (src == 0x0)
        return 0;
    if (src[0] == 0x00)
    {
        return 1;
    }
    else if (src[0] <= UTF8_CP_MAXB_LOCK_1)
    {
        return 1;
    }
    else if ((src[0] & UTF8_CP_LEADBYTE_2) && src[0] <= UTF8_CP_LEADBYTE_2_MAX && (src[1] & 0x80) && src[0] != 0xC0 && src[0] != 0xC1)
    {
        return 2;
    }
    else if ((src[0] & UTF8_CP_LEADBYTE_3) && src[0] <= UTF8_CP_LEADBYTE_3_MAX && (src[1] & 0x80) && (src[2] & 0x80))
    {
        FlUnicodeChar chr = 0;
        int bs = 3;
        bool mbUsesBigEndian = utf8_mb_str_is_bigendian();
        for (int index=0, j=0; j < bs; j++)
        {
            if (mbUsesBigEndian)
                index = bs-1-j;
            ((FlByte*)&chr)[index] = src[j];
        }
        // Surrogates
        if (chr < 0xeda080 || chr > 0xedbfbf)
            return 3;
        return -1;
    }
    else if ((src[0] & UTF8_CP_LEADBYTE_4) && src[0] <= UTF8_CP_LEADBYTE_4_MAX && (src[1] & 0x80) && (src[2] & 0x80) && (src[3] & 0x80) && src[0] != 0xF5)
    {
        return 4;
    }
    return -1;
}
