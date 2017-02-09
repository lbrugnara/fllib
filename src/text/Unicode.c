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
        if (chr <= UTF8_CP_MAXB_LOCK_1)
        {
            return 1;
        }
        else if ((chrb[1] & UTF8_CP_LEADBYTE_2) && chrb[1] <= UTF8_CP_LEADBYTE_2_MAX)
        {
            return 2;
        }
        else if ((chrb[2] & UTF8_CP_LEADBYTE_3) && chrb[2] <= UTF8_CP_LEADBYTE_3_MAX)
        {
            return 3;
        }
        else if ((chrb[3] & UTF8_CP_LEADBYTE_4) && chrb[3] <= UTF8_CP_LEADBYTE_4_MAX)
        {
            return 4;
        }
    }
    return 0; // Not uf8
}

FlUnicodeChar fl_unicode_char_at(const FlByte* str, FlEncoding encoding, size_t at)
{
    FlUnicodeChar chr = 0;
    size_t offset = 0;
    for (int i=0; i < at; i++)
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

size_t fl_unicode_str_size(const FlByte* chr, FlEncoding encoding, const FlByte* end)
{
    if (chr == 0x00)
        return 0;
    size_t i=0;
    size_t size = 0, tmp = 0;
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
            return -1; // Not uf8
        }
        size += tmp;
        i += tmp;
    } while ((end == NULL && chr[i]) || (end != NULL && chr+i < end));    
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
        dst[mbUsesBigEndian ? 1 : 0] = UTF8_CP_LEADBYTE_2 | ((src & 0x7c0) >> 6);
        dst[mbUsesBigEndian ? 0 : 1] = 0x80 |  (src & 0x3f);
    }    
    else if (src <= UTF8_CP_MAXB_LOCK_3) 
    {
        dst[mbUsesBigEndian ? 2 : 0] = UTF8_CP_LEADBYTE_3 | ((src & 0xf000) >> 12);
        dst[1                      ] = 0x80 | ((src & 0xfc0) >>  6);
        dst[mbUsesBigEndian ? 0 : 2] = 0x80 |  (src & 0x3f);
    }
    else if (src <= UTF8_CP_MAXB_LOCK_4) 
    {
        dst[mbUsesBigEndian ? 3 : 0] = UTF8_CP_LEADBYTE_4 | ((src & 0x1c0000) >> 18);
        dst[mbUsesBigEndian ? 2 : 1] = 0x80 | ((src & 0x03f000) >> 12);
        dst[mbUsesBigEndian ? 1 : 2] = 0x80 | ((src & 0xfc0) >>  6);
        dst[mbUsesBigEndian ? 0 : 3] = 0x80 | (src & 0x3f);
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
    FlByte* srcp = (FlByte*)&src;
    bool mbUsesBigEndian = fl_mbstring_is_bigendian();
    if (l == 2)
    {
        if (mbUsesBigEndian)
        {
        }
        else
        {
        }
    }
    return chr;
}
