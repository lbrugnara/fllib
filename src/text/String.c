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
    return fl_unicode_codepoint_sequence_length(FL_ENCODING_UTF8, (FlByte*)str, end);
}

size_t fl_string_size(const FlString str, const FlByte* end)
{
    return fl_unicode_codeunit_sequence_size(FL_ENCODING_UTF8, (const FlByte*)str, end);
}

FlUnicodeChar fl_char(const FlString str)
{
    return fl_unicode_codepoint_to_unichar(FL_ENCODING_UTF8, (const FlByte*)str, 0x00);
}

FlUnicodeChar fl_string_char_at(const FlString str, size_t at)
{
    return fl_unicode_codepoint_at(FL_ENCODING_UTF8, (const FlByte*)str, 0x00, at);    
}
