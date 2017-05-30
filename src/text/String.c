#include "../Std.h"
#include "String.h"

/* -------------------------------------------------------------
* PUBLIC API
* -------------------------------------------------------------
*/

FlString fl_string_new(const FlString str)
{
    //TODO: Validate and create a new UTF-8 string from {str} (null-terminated string)
    return NULL;
}

FlString fl_string_new_from_bytes(const FlByte *bytes, size_t length, FlEncoding encoding)
{
    //TODO: Validate and create a new UTF-8 string from {bytes} with encoding {encoding}
    return NULL;
}

void fl_string_delete(FlString str)
{
    // TODO
}


size_t fl_string_length(const FlString str, const FlByte* end)
{
    if (!end && !*str)
        return 0;
    size_t l=0;
    size_t c=0;
    size_t tmp = 0;
    do {
        tmp = fl_unicode_codeunit_sequence_size((FlByte*)str + c, FL_ENCODING_UTF8, (FlByte*)str + c +1);
        if (tmp == 0)
            break;
        c += tmp;
        l++;
    } while((end ? (FlByte*)(str+c+1) < end : str[c]));
    return l;
}

size_t fl_string_size(const FlString str, const FlByte* end)
{
    return fl_unicode_codeunit_sequence_size((const FlByte*)str, FL_ENCODING_UTF8, end);
}

FlUnicodeChar fl_char(const FlString str)
{
    return fl_unicode_codepoint_to_unichar((const FlByte*)str, FL_ENCODING_UTF8);
}

FlUnicodeChar fl_string_char_at(const FlString str, size_t at)
{
    return fl_unicode_codepoint_at((const FlByte*) str, FL_ENCODING_UTF8, at);    
}
