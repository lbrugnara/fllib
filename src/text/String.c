#include "../Std.h"
#include "String.h"

/*
 * PUBLIC API
 *
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

void fl_string_free(FlString str)
{
    // TODO
}


size_t fl_string_length(const FlString str, const void* end)
{
    return fl_unicode_codepoint_sequence_length(FL_ENCODING_UTF8, (FlByte*)str, end);
}

size_t fl_string_size(const FlString str, const void* end)
{
    return fl_unicode_codeunit_sequence_size(FL_ENCODING_UTF8, (const FlByte*)str, end);
}

FlChar fl_char(const FlString str)
{
    // TODO: fix all this
    FlByte char_buffer[4] = { 0 };
    size_t size = fl_unicode_codepoint_at(FL_ENCODING_UTF8, (const FlByte*) str, 0x00, 0, char_buffer);
    if (size == FL_UNICODE_INVALID_SIZE)
    {
        return FL_UNICODE_INVALID_SIZE;
    }

    return (FlChar) (char_buffer[3] | char_buffer[2] | char_buffer[1] | char_buffer[0]);
}

FlChar fl_string_char_at(const FlString str, size_t at)
{
    // TODO: fix all this
    FlByte char_buffer[4] = { 0 };
    size_t size = fl_unicode_codepoint_at(FL_ENCODING_UTF8, (const FlByte*)str, 0x00, at, char_buffer);
    if (size == FL_UNICODE_INVALID_SIZE)
    {
        return FL_UNICODE_INVALID_SIZE;
    }

    return (FlChar) (char_buffer[3] | char_buffer[2] | char_buffer[1] | char_buffer[0]);
}
