#ifndef FL_UNICODEDATA_H
#define FL_UNICODEDATA_H

#include <stdint.h>
#include <stdbool.h>

size_t fl_unicode_get_decomposition_mapping(uint32_t codepoint, bool canonical, uint32_t *destination);

#endif /* FL_UNICODEDATA_H */
