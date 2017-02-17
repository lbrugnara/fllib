#include "../Std.h"
#include "String.h"

/* -------------------------------------------------------------
* PUBLIC API
* -------------------------------------------------------------
*/

FlString fl_string_new(const FlString str)
{
    //TODO
    return NULL;
}

FlString fl_string_new_from_bytes(const FlByte *bytes, size_t length, FlEncoding encoding)
{
    //TODO
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
    do {
        c += fl_unicode_str_size((FlByte*)str + c, FL_ENCODING_UTF8, (FlByte*)str + c +1);
        l++;
    } while((end ? (FlByte*)(str+c+1) < end : str[c]));
    return l;
}

size_t fl_string_size(const FlString str, const FlByte* end)
{
    return fl_unicode_str_size((const FlByte*)str, FL_ENCODING_UTF8, end);
}

FlUnicodeChar fl_char(const FlString str)
{
    return fl_unicode_char_from_bytes((const FlByte*)str, FL_ENCODING_UTF8);
}

FlUnicodeChar fl_string_char_at(const FlString str, size_t at)
{
    return fl_unicode_char_at((const FlByte*) str, FL_ENCODING_UTF8, at);    
}
