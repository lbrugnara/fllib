#include "../Std.h"
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

FlUnicodeChar fl_unicode_utf32_to_utf8(FlUnicodeChar src);
FlUnicodeChar fl_unicode_utf8_to_utf32(FlUnicodeChar src);
bool fl_mbstring_is_bigendian();

/* -------------------------------------------------------------
* Checks if a multibyte character is stored as big-endiann. It
* is implementation dependant, maybe to make it compatible with
* common chars.
* -------------------------------------------------------------
*/
bool fl_mbstring_is_bigendian()
{
    FlString oldac = "𐌀"; // Hex 0xF0908C80
    int32_t oldah = 0xF0908C80;
    return ((FlByte*)oldac)[0] != ((FlByte*)&oldah)[0];
}


FlUnicodeChar fl_unicode_char_from_str(const FlByte* src, FlEncoding encoding)
{
    return fl_unicode_char_at(src, encoding, 0);
}

FlUnicodeChar fl_unicode_char_from_hex(const unsigned long src)
{
    if (fl_unicode_char_size(src, FL_ENCODING_UTF8) > 0)
        return (FlUnicodeChar)src;
        
    return fl_unicode_utf32_to_utf8((FlUnicodeChar)src);
}

size_t fl_unicode_char_size(const FlUnicodeChar chr, FlEncoding encoding)
{
    if (encoding == FL_ENCODING_UTF8)
    {
        FlByte *chrb = (FlByte*)&chr;
        if (chrb[3] > UTF8_CP_LEADBYTE_3_MAX && (chrb[3] & UTF8_CP_LEADBYTE_4) && chrb[3] <= UTF8_CP_LEADBYTE_4_MAX)
        {
            return 4;
        }
        else if (chrb[2] > UTF8_CP_LEADBYTE_2_MAX && (chrb[2] & UTF8_CP_LEADBYTE_3) && chrb[2] <= UTF8_CP_LEADBYTE_3_MAX)
        {
            return 3;
        }
        else if (chrb[1] > UTF8_CP_MAXB_LOCK_1 && (chrb[1] & UTF8_CP_LEADBYTE_2) && chrb[1] <= UTF8_CP_LEADBYTE_2_MAX)
        {
            return 2;
        }
        else if (chr <= UTF8_CP_MAXB_LOCK_1)
        {
            return 1;
        }
    }
    return 0; // Not uf8
}

FlUnicodeChar fl_unicode_char_at(const FlByte* str, FlEncoding encoding, size_t at)
{
    FlUnicodeChar chr = 0;
    size_t offset = 0;
    for (size_t i=0; i < at; i++)
    {
        offset += fl_unicode_str_size(str+offset, encoding, str+offset+1);
    }
    size_t bs = fl_unicode_str_size(str+offset, encoding, str+offset+1);
    bool mbUsesBigEndian = fl_mbstring_is_bigendian();
    for (size_t index=0, j=0; j < bs; j++)
    {
        if (mbUsesBigEndian)
            index = bs-1-j;
        ((FlByte*)&chr)[index] = (str+offset)[j];
    }
    return chr;
}

// TODO: Fix for UTF32 and UTF16
size_t fl_unicode_str_size(const FlByte* chr, FlEncoding encoding, const FlByte* end)
{
    if (chr == 0x00)
        return 0;
    
    size_t size = 0;
    if (encoding == FL_ENCODING_UTF32)
    {
        size_t i = 0;
        const FlUnicodeChar *src = (const FlUnicodeChar*)&chr;
        do
        {
            size += 4;
            i++;
        } while ((end == NULL && src[i]) || (end != NULL && ((FlByte*)src)+i < end));
    }
    else if (encoding == FL_ENCODING_UTF8)
    {
        size_t i=0;
        size_t tmp = 0;
        do
        {
            tmp = 0;
            if (chr[i] == 0x00)
            {
                if (end != NULL)
                    tmp = 1;
            }
            else if (chr[i] <= UTF8_CP_MAXB_LOCK_1)
            {
                tmp = 1;
            }
            else if ((chr[i] & UTF8_CP_LEADBYTE_2) && chr[i] <= UTF8_CP_LEADBYTE_2_MAX)
            {
                tmp = 2;
            }
            else if ((chr[i] & UTF8_CP_LEADBYTE_3) && chr[i] <= UTF8_CP_LEADBYTE_3_MAX)
            {
                tmp = 3;
            }
            else if ((chr[i] & UTF8_CP_LEADBYTE_4) && chr[i] <= UTF8_CP_LEADBYTE_4_MAX)
            {
                tmp = 4;
            }
            else
            {
                return FL_UNICODE_INVALID_CHAR; // Not uf8
            }
            size += tmp;
            i += tmp;
        } while ((end == NULL && chr[i]) || (end != NULL && chr+i < end));
    }
    return size;
}

FlUnicodeChar fl_unicode_encode_char_to(const FlUnicodeChar src, const FlEncoding srcencoding, const FlEncoding dstencoding)
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

FlUnicodeChar fl_unicode_utf32_to_utf8(FlUnicodeChar src)
{
    FlUnicodeChar chr = 0;
    FlByte* dst = (FlByte*)&chr;
    if (src <= UTF8_CP_MAXB_LOCK_1)
    {
        chr = (FlUnicodeChar)src;
        return chr;
    }
    
    bool mbUsesBigEndian = fl_mbstring_is_bigendian();
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

FlUnicodeChar fl_unicode_utf8_to_utf32(FlUnicodeChar src)
{
    FlUnicodeChar chr = 0;
    size_t l = fl_unicode_char_size(src, FL_ENCODING_UTF8);
    if (l == 1) 
    {
        chr = (FlUnicodeChar)src;
        return chr;
    }
    // TODO
    FlByte* dst = (FlByte*)&chr;
    FlByte* srcp = (FlByte*)&src;
    bool mbUsesBigEndian = fl_mbstring_is_bigendian();
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
