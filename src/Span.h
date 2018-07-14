#ifndef FL_SPAN_H
#define FL_SPAN_H

/* =============================================================
 * {module: Span}
 * =============================================================
 * Provides primitives to work with contiguous sequence of bytes
 * -------------------------------------------------------------
 */

#include <stdlib.h>
#include "Types.h"
#include "containers/Iterator.h"

/* -------------------------------------------------------------
* {datatype: struct FlSpan}
* -------------------------------------------------------------
* Represents a contiguous read-only sequence of bytes
* -------------------------------------------------------------
*/
struct FlSpan {
    const FlByte * const sequence;
    const size_t dtsize;
    const size_t length;
};

/* -------------------------------------------------------------
* {function: fl_span_new}
* -------------------------------------------------------------
* Creates a new struct FlSpan that holds a contiguous read-only
* sequence of bytes
* -------------------------------------------------------------
* {param: const FlByte * const sequence} Pointer to the sequence to hold within the struct
* {param: size_t length} Number of bytes this span holds
* -------------------------------------------------------------
* {return: struct FlSpan} Representation of the contiguous read-only sequence of bytes
* -------------------------------------------------------------
*/
struct FlSpan fl_span_new(const FlByte * const sequence, size_t dtsize, size_t offset, size_t length);

/* -------------------------------------------------------------
* {function: fl_span_equals}
* -------------------------------------------------------------
* Compare two spans and return true if they point to the same
* contiguous sequence of bytes
* -------------------------------------------------------------
* {param: struct FlSpan *span1} Span to compare
* {param: struct FlSpan *span2} Span to compare
* -------------------------------------------------------------
* {return: bool} True if both spans point to the same sequence 
* and have the same length. False otherwise
* -------------------------------------------------------------
*/
bool fl_span_equals(const struct FlSpan *span1, const struct FlSpan *span2);

/* -------------------------------------------------------------
* {function: fl_span_equals_sequence}
* -------------------------------------------------------------
* * Compare the span with the sequence of bytes.
* -------------------------------------------------------------
* {param: struct FlSpan *span} Span to compare with the sequence of bytes
* {param: const FlByte * const sequence} Sequence of bytes
* -------------------------------------------------------------
* {return: bool} True if the span is within the sequence of bytes
* -------------------------------------------------------------
*/
bool fl_span_equals_sequence(const struct FlSpan *span, const FlByte * const sequence);

/* -------------------------------------------------------------
 * {function: fl_span_begin}
 * -------------------------------------------------------------
 * Returns an {FlIterator} that points to the first element in the span
 * -------------------------------------------------------------
 * {param: const struct FlSpan *span} The iterator will point to the begining of the span
 * -------------------------------------------------------------
 * {return: FlIterator} Iterator pointing to the first element in the span
 * -------------------------------------------------------------
 */
FlIterator fl_span_begin(const struct FlSpan *span);

/* -------------------------------------------------------------
 * {function: fl_span_end}
 * -------------------------------------------------------------
 * Returns an {FlIterator} that points to the past-last-nth element in the span
 * The FlIterator returned by this function MUST NOT be dereferenced by fl_iterator_value.
 * -------------------------------------------------------------
 * {param: const struct FlSpan *span} The iterator will point to the past-last-nth element in the span
 * -------------------------------------------------------------
 * {return: FlIterator} Iterator pointing beyond the last element in the span
 * -------------------------------------------------------------
 */
FlIterator fl_span_end(const struct FlSpan *span);

#endif /* FL_SPAN_H */
