#ifndef FL_REGEX_H
#define FL_REGEX_H

#include "../Std.h"

typedef struct FlRegex* FlRegex;

/**
 * Creates an {FlRegex} using the pattern {pattern}. If {error}
 * is not null, it will contain any information regarding to regex
 * compilation errors
 */
FlRegex fl_regex_compile(FlCstr pattern, FlError *error);

/**
 * Using {regex} check if {text} matches the pattern
 */
bool fl_regex_match(FlRegex regex, FlCstr text);

/**
 * Deletes the memory used by a compiled {FlRegex}
 */
void fl_regex_delete(FlRegex regex);

#endif /* FL_REGEX_H */
