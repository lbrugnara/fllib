#include <string.h>

#include "../Std.h"
#include "../Mem.h"
#include "Unicode.h"

/* -------------------------------------------------------------
* Last code point (inclusive) of each UTF-8 range (1-4 bytes)
* -------------------------------------------------------------
*/
#define UNICODE_LAST_CODEPOINT_1 0x0000007F
#define UNICODE_LAST_CODEPOINT_2 0x000007FF
#define UNICODE_LAST_CODEPOINT_3 0x0000FFFF
#define UNICODE_LAST_CODEPOINT_4 0x0010FFFF

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

// UTF-8 (codepoint)
#define UTF8_MAX_CODEPOINT ((FlByte*)"\xf4\x8f\xbf\xbf")

// UTF-8 (uint32_t)
#define UTF8_REPLACEMENT_CHARACTER 0xEFBFBD

// UTF-32
#define UTF32_BYTES_SIZE 4

#define UTF32_LAST_CODEPOINT_1 ((FlByte*)"\x00\x00\x00\x7F")
#define UTF32_LAST_CODEPOINT_2 ((FlByte*)"\x00\x00\x07\xFF")
#define UTF32_LAST_CODEPOINT_3 ((FlByte*)"\x00\x00\xFF\xFF")
#define UTF32_LAST_CODEPOINT_4 ((FlByte*)"\x00\x10\xFF\xFF")

/* -------------------------------------------------------------
* PRIVATE API
* -------------------------------------------------------------
* Multibyte Strings: All these functions assume input-charset 
* and exec-charset are UTF-8
* -------------------------------------------------------------
*/
static inline void swap_representations(const FlByte *src, FlByte *dst, size_t nbytes);
static inline bool utf8_mb_str_is_bigendian();
static inline size_t utf8_codepoint_size(const FlByte* src, const FlByte *end);
static inline size_t utf32_codepoint_size(const FlByte* src);
static inline uint32_t utf8_to_utf32(uint32_t src);
static inline uint32_t utf32_to_utf8(uint32_t src);

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
* Helper function to convert between multibyte and uint32_t
* representations.
* -------------------------------------------------------------
*/
static inline void swap_representations(const FlByte *src, FlByte *dst, size_t nbytes)
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
* Returns the size of a UTF-32 valid character represented by a
* multi-byte character.
* -------------------------------------------------------------
*/
static inline size_t utf32_codepoint_size(const FlByte *src)
{
    uint32_t srcchr = 0x0;
    swap_representations(src, (FlByte*)&srcchr, UTF32_BYTES_SIZE);
    if (srcchr <= UNICODE_LAST_CODEPOINT_4 && (srcchr < 0xD800 || srcchr > 0xDFFF)) // Check surrogates
    {
        return UTF32_BYTES_SIZE;
    }
    return FL_UNICODE_INVALID_SIZE;
}

/* -------------------------------------------------------------
* Returns the size of a UTF-8 valid character represented by a
* multi-byte character
* -------------------------------------------------------------
*/
static inline size_t utf8_codepoint_size(const FlByte *src, const FlByte *end)
{
    flm_assert(src != NULL, "The source byte array cannot be NULL.");

    // If end is NULL and src[0], we are NOT handling U+0000
    if ((end != NULL && src == end))
    {
        return FL_UNICODE_INVALID_SIZE;
    }
    
    // U+0000 to U+007F occupy 1 byte
    if (src[0] <= UNICODE_LAST_CODEPOINT_1)
    {
        return 1;
    }

    if ((end != NULL && src+1 == end) || src[1] == 0x0)
    {
        return FL_UNICODE_INVALID_SIZE;
    }
    
    // Check if first byte is the lead byte of 2-bytes code points
    // Check if first byte is a valid lead byte value for 2-bytes code points
    // Check if second byte is a continuation byte (0x80)
    // Check for invalid octets C0 and C1
    if ((src[0] & UTF8_CODEPOINT_LEADBYTE_2) && src[0] <= UTF8_CODEPOINT_LEADBYTE_2_MAX && (src[1] & 0x80) && src[0] != 0xC0 && src[0] != 0xC1)
    {
        return 2;
    }

    if ((end != NULL && src+2 == end) || src[2] == 0x0)
    {
        return FL_UNICODE_INVALID_SIZE;
    }
    
    // Check if first byte is the lead byte of 3-bytes code points
    // Check if first byte is a valid lead byte value for 3-bytes code points
    // Check if second and third bytes are continuation bytes (0x80)
    // (inside) Check UTF-16 surrogates
    if ((src[0] & UTF8_CODEPOINT_LEADBYTE_3) && src[0] <= UTF8_CODEPOINT_LEADBYTE_3_MAX && ((src[0] == UTF8_CODEPOINT_LEADBYTE_3 && src[1] >= 0xA0) || (src[0] > UTF8_CODEPOINT_LEADBYTE_3 && src[1] >= 0x80)) && (src[2] & 0x80))
    {
        uint32_t chr = 0;
        swap_representations(src, (FlByte*)&chr, 3);
        // Surrogates (UTF-8 encoded)
        if (chr < 0xeda080 || chr > 0xedbfbf)
            return 3;
        return FL_UNICODE_INVALID_SIZE;
    }
 
    if ((end != NULL && src+3 == end) || src[3] == 0x0)
    {
        return FL_UNICODE_INVALID_SIZE;
    }

    // Check if first byte is the lead byte of 4-bytes code points
    // Check if first byte is a valid lead byte value for 4-bytes code points
    // Check if second, third and fourth bytes are continuation bytes (0x80)
    // Check for invalid octets greather than F5 (F5-FF)
    if ((src[0] & UTF8_CODEPOINT_LEADBYTE_4) && src[0] <= UTF8_CODEPOINT_LEADBYTE_4_MAX && ((src[0] == UTF8_CODEPOINT_LEADBYTE_4 && src[1] >= 0x90) || (src[0] > UTF8_CODEPOINT_LEADBYTE_4 && src[1] >= 0x80)) && (src[2] & 0x80) && (src[3] & 0x80) && src[0] < 0xF5)
    {
        if (memcmp(src, UTF8_MAX_CODEPOINT, 4) > 0)
            return FL_UNICODE_INVALID_SIZE;
        return 4;
    }

    return FL_UNICODE_INVALID_SIZE;
}

/* -------------------------------------------------------------
* Converts an UTF-32 uint32_t to its UTF-8 representation
* -------------------------------------------------------------
*/
static inline size_t _utf32_to_utf8(const FlByte *src, FlByte *dst)
{
    if (memcmp(src, UTF32_LAST_CODEPOINT_1, 4) <= 0)
    {
        dst[0] = src[3];
        return 1;
    }

    if (memcmp(src, UTF32_LAST_CODEPOINT_2, 4) <= 0)
    {
        // Control 10xxxxxx = 0x80 | 0x3f: Get last 6 bit
        dst[1] = 0x80 | (src[3] & 0x3f);
        // Lead 110xxxxx => Shift 6 bits used in a continuation byte and get last five (0x1f) bits
        dst[0] = UTF8_CODEPOINT_LEADBYTE_2 | ((src[2] << 2) & 0x1C) | ((src[3] >> 6) & 0x3);
        
        if (dst[0] == 0xC0 || dst[0] == 0xC1)
            return FL_UNICODE_INVALID_CHAR;
        return 2;
    }    
    if (memcmp(src, UTF32_LAST_CODEPOINT_3, 4) <= 0)
    {
        // Control 10xxxxxx = 0x80 | 0x3f: Get last 6 bit
        dst[2] = 0x80 | (src[3] & 0x3f);
        // Control 10xxxxxx = 0x80 | 0x3f: Shift 6 and get last 6 bits
        dst[1] = 0x80 | ((src[2] << 2) & 0x3C) | ((src[3] >> 6) & 0x3);
        // Lead 1110xxxx => Shift 12 bits used in two continuation bytes and get last four (0xf) bits
        dst[0] = UTF8_CODEPOINT_LEADBYTE_3 | ((src[2] >> 4) & 0xF);

        // Surrogates
        if (memcmp(src, "\x00\x00\xD8\x00", 4) >= 0 && memcmp(src, "\x00\x00\xDF\xFF", 4) <= 0)
            return FL_UNICODE_INVALID_CHAR;
        return 3;
    }
    if (memcmp(src, UTF32_LAST_CODEPOINT_4, 4) <= 0)
    {
        // Control 10xxxxxx = 0x80 | 0x3f: Get last 6 bit
        dst[3] = 0x80 | (src[3] & 0x3f);
        // Control 10xxxxxx = 0x80 | 0x3f: Shift 6 and get last 6 bits
        dst[2] = 0x80 | ((src[2] << 2) & 0x3C) | ((src[3] >> 6) & 0x3);
        // Control 10xxxxxx = 0x80 | 0x3f: Shift 12 and get last 6 bsits
        dst[1] = 0x80 | ((src[1] << 4) & 0x30) | ((src[2] >> 4) & 0xF);
        // Lead 11110xxx => Shift 18 bits used in three continuation bytes and get last three (0x7) bits
        dst[0] = UTF8_CODEPOINT_LEADBYTE_4 | ((src[1] >> 2) & 0x7);
        return 4;
    }
    return FL_UNICODE_INVALID_CHAR;
}

static inline uint32_t utf32_to_utf8(uint32_t src)
{
    if (src <= UNICODE_LAST_CODEPOINT_1)
    {
        return (uint32_t)src;
    }
    
    uint32_t chr = 0;
    FlByte* dst = (FlByte*)&chr;
    bool mbUsesBigEndian = utf8_mb_str_is_bigendian();
    if (src <= UNICODE_LAST_CODEPOINT_2)
    {
        // Control 10xxxxxx = 0x80 | 0x3f: Get last 6 bit
        dst[mbUsesBigEndian ? 0 : 1] = 0x80 |  (src & 0x3f);
        // Lead 110xxxxx => Shift 6 bits used in a continuation byte and get last five (0x1f) bits
        dst[mbUsesBigEndian ? 1 : 0] = UTF8_CODEPOINT_LEADBYTE_2 | ((src >> 6) & 0x1F);
        
        if (dst[mbUsesBigEndian ? 1 : 0] == 0xC0 || dst[mbUsesBigEndian ? 1 : 0] == 0xC1)
            return FL_UNICODE_INVALID_CHAR;
    }    
    else if (src <= UNICODE_LAST_CODEPOINT_3) 
    {
        // Control 10xxxxxx = 0x80 | 0x3f: Get last 6 bit
        dst[mbUsesBigEndian ? 0 : 2] = 0x80 |  (src & 0x3f);
        // Control 10xxxxxx = 0x80 | 0x3f: Shift 6 and get last 6 bits
        dst[1                      ] = 0x80 | ((src >> 6) & 0x3f );
        // Lead 1110xxxx => Shift 12 bits used in two continuation bytes and get last four (0xf) bits
        dst[mbUsesBigEndian ? 2 : 0] = UTF8_CODEPOINT_LEADBYTE_3 | ((src >> 12) & 0xf);

        if (chr >= 0xeda080 && chr <= 0xedbfbf)
            return FL_UNICODE_INVALID_CHAR;
    }
    else if (src <= UNICODE_LAST_CODEPOINT_4) 
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
* Converts an UTF-8 uint32_t to its UTF-32 representation
* -------------------------------------------------------------
*/
static inline bool _utf8_to_utf32(const FlByte *src, const FlByte *end, FlByte *dst)
{
    size_t l = utf8_codepoint_size(src, end);
    if (l == FL_UNICODE_INVALID_SIZE)
    {
        return false;
    }
    else if (l == 1) 
    {
        dst[3] = src[0];
    }    
    else if (l == 2)
    {
        // LB: 110xxxxx CB: 10xxxxxx => We take the last two bytes from LB and 6 bits from CB (0x3f)
        dst[3] = ((src[0] << 6) & 0xC0) | (src[1] & 0x3f);
        // Remove CB (>> 8) and take the three first bits after 110 (avoid last two already taken in previous step)
        dst[2] = ((src[0] >> 2) & 0x7);
    }
    else if (l == 3)
    {
        // Format: LB: 1110xxxx CB1: 10xxxxxx CB2: 10xxxxxx
        // Shift 2 bits from CB1 to replace 10 from CB2. Get last 2 bits from CB1 | Get last 6 bits from CB2
        dst[3] = ((src[1] << 6) & 0xC0) | (src[2] & 0x3f);
        // Shift CB2 and the LS half of CB1. Get 4 MS bits from the result | Shift CB2 bits and the two LS bits from CB1. Get remaining 4 bits from CB1
        dst[2] = ((src[0] << 4) & 0xF0) | ((src[1] >> 2) & 0xF);
    }
    else if (l == 4)
    {
        // Format: LB: 11110xxx CB1: 10xxxxxx CB2: 10xxxxxx CB3: 10xxxxxx
        // Shift 2 bits from CB2 to replace 10 from CB3. Get last 2 bits from CB2 | Get last 6 bits from CB3
        dst[3] = ((src[2] << 6) & 0xC0) | (src[3] & 0x3f);
        dst[2] = ((src[1] << 4) & 0xF0) | ((src[2] >> 2) & 0xF);
        dst[1] = ((src[0] << 2) & 0x1C) | ((src[1] >> 4) & 0x3);
    }
    else
    {
        return false;
    }
    return true;
}

static inline uint32_t utf8_to_utf32(uint32_t src)
{
    FlByte tmp[4] = {0x0,0x0,0x0,0x0};
    swap_representations((FlByte*)&src, tmp, 4);
    size_t i=0;
    while (tmp[i] == 0x0 && i < 3) i++;
    size_t l = utf8_codepoint_size(tmp+i, tmp+4);
    if (l == FL_UNICODE_INVALID_SIZE)
        return FL_UNICODE_INVALID_CHAR;

    if (l == 1) 
    {
        return (uint32_t)src;
    }

    uint32_t chr = 0;
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

size_t fl_unicode_codepoint_convert(FlEncoding srcencoding, const FlByte *src, const FlByte *end, FlEncoding dstencoding, FlByte *dst)
{
    if (srcencoding == FL_ENCODING_UTF8)
    {
        if (dstencoding == FL_ENCODING_UTF32)
        {
            if (!_utf8_to_utf32(src, end, dst))
                return FL_UNICODE_INVALID_SIZE;
            return UTF32_BYTES_SIZE;
        }
    }
    else if (srcencoding == FL_ENCODING_UTF32)
    {
        if (dstencoding == FL_ENCODING_UTF8)
        {
            size_t nbytes = _utf32_to_utf8(src, dst);
            if (nbytes != FL_UNICODE_INVALID_SIZE)
            {
                return nbytes;
            }
        }
    }
    return FL_UNICODE_INVALID_SIZE;
}

/* -------------------------------------------------------------
* UNICODE CODE POINT AND UNIT FUNCTIONS (FlByte*)
* -------------------------------------------------------------
*/

size_t fl_unicode_codepoint_size(FlEncoding encoding, const FlByte *src, const FlByte *end)
{
    if (encoding == FL_ENCODING_UTF32)
    {
        return utf32_codepoint_size(src);
    }
    else if (encoding == FL_ENCODING_UTF8)
    {
        return utf8_codepoint_size(src, end);
    }
    return FL_UNICODE_INVALID_SIZE;
}

// TODO: Fix for UTF16
// TODO: Fix logic to retrieve UTF-8 code units size
/* -------------------------------------------------------------
* Returns the amount of valid code units in sequence
* -------------------------------------------------------------
*/
size_t fl_unicode_codeunit_sequence_size(FlEncoding encoding, const FlByte* sequence, const FlByte* end)
{
    flm_assert(sequence != NULL, "Code unit sequence cannot be NULL.");
    
    size_t size = 0;
    if (encoding == FL_ENCODING_UTF32)
    {
        // {todo: This needs to be tested}
        size_t i=0;
        bool isValid = true;
        while (isValid)
        {
            if (end == NULL && sequence[i] == 0x00)
                break;
            if (end != NULL && sequence+i >= end)
                break;
            if (!fl_unicode_codepoint_is_valid(encoding, sequence+i, end))
                break;
            size += UTF32_BYTES_SIZE;
            size_t tmp = UTF32_BYTES_SIZE;
            while (tmp-- > 0)
            {
                i++;
                if (end == NULL && sequence[i] == 0x00)
                {
                    isValid = false;
                    break;
                }
            }
        }
    }
    else if (encoding == FL_ENCODING_UTF8)
    {
        size_t i=0;
        bool isValid = true;
        while (isValid)
        {
            if (end == NULL && sequence[i] == 0x00)
                break;
            if (end != NULL && sequence+i >= end)
                break;

            size_t tmp = utf8_codepoint_size(sequence+i, end);
            if (tmp == FL_UNICODE_INVALID_SIZE)
                break; // truncated string
            size += tmp;
            while (tmp-- > 0)
            {
                i++;
                if (end == NULL && sequence[i] == 0x00)
                {
                    isValid = false;
                    break;
                }
            }
        }
    }
    return size;
}

// TODO: Fix for UTF16
// TODO: Fix logic to retrieve UTF-8 code points length
/* -------------------------------------------------------------
* Returns the amount of valid code points in sequence
* -------------------------------------------------------------
*/
size_t fl_unicode_codepoint_sequence_length(FlEncoding encoding, const FlByte* sequence, const FlByte* end)
{
    flm_assert(sequence != NULL, "Code unit sequence cannot be NULL.");
    
    size_t size = 0;
    if (encoding == FL_ENCODING_UTF32)
    {
        // {todo: This needs to be tested}
        size_t i=0;
        bool isValid = true;
        while (isValid)
        {
            if (end == NULL && sequence[i] == 0x00)
                break;
            if (end != NULL && sequence+i >= end)
                break;
            if (!fl_unicode_codepoint_is_valid(encoding, sequence+i, end))
                break;
            size++;
            size_t tmp = UTF32_BYTES_SIZE;
            while (tmp-- > 0)
            {
                i++;
                if (end == NULL && sequence[i] == 0x00)
                {
                    isValid = false;
                    break;
                }
            }
        }
    }
    else if (encoding == FL_ENCODING_UTF8)
    {
        size_t i=0;
        bool isValid = true;
        while (isValid)
        {
            if (end == NULL && sequence[i] == 0x00)
                break;
            if (end != NULL && sequence+i >= end)
                break;

            size_t tmp = utf8_codepoint_size(sequence+i, end);
            if (tmp == FL_UNICODE_INVALID_SIZE)
                break; // truncated string
            size++;
            while (tmp-- > 0)
            {
                i++;
                if (end == NULL && sequence[i] == 0x00)
                {
                    isValid = false;
                    break;
                }
            }
        }
    }
    return size;
}

/* -------------------------------------------------------------
* Returns the 'at'-th character of a Unicode string encoded as 'encoding'
* -------------------------------------------------------------
*/
size_t fl_unicode_codepoint_at(FlEncoding encoding, const FlByte *str, const FlByte *end, size_t at, FlByte *dst)
{
    if (encoding == FL_ENCODING_UTF32)
    {
        memcpy(dst, str+(at * UTF32_BYTES_SIZE), UTF32_BYTES_SIZE);
        return fl_unicode_codepoint_size(FL_ENCODING_UTF32, dst, dst + UTF32_BYTES_SIZE);
    }
    else if (encoding == FL_ENCODING_UTF8)
    {
        size_t offset = 0;
        for (size_t i=0; i < at; i++)
        {
            size_t tmp = fl_unicode_codepoint_size(encoding, str+offset, end);
            if (tmp == FL_UNICODE_INVALID_SIZE)
            {
                return FL_UNICODE_INVALID_SIZE;
            }
            offset += tmp;
        }
        size_t bs = fl_unicode_codepoint_size(encoding, str+offset, end);
        memcpy(dst, str+offset, bs);
        return bs;
    }
    return FL_UNICODE_INVALID_SIZE;
}

bool fl_unicode_codepoint_is_valid(FlEncoding encoding, const FlByte *src, const FlByte *end)
{
    return fl_unicode_codepoint_size(encoding, src, end) != FL_UNICODE_INVALID_SIZE;
}

bool fl_unicode_codeunit_sequence_is_valid(FlEncoding encoding, const FlByte* src, const FlByte* end)
{
    flm_assert(src != NULL, "Source src cannot be NULL.");

    size_t i = 0;
    while ((end == NULL && src[i]) || (end != NULL && (src+i) < end))
    {
        size_t tmp = fl_unicode_codepoint_size(encoding, src+i, end);
        if (tmp == FL_UNICODE_INVALID_SIZE)
            return false;
        i += tmp;
    }
    return true;
}
