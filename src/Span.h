#ifndef FL_SPAN_H
#define FL_SPAN_H

/*
 * File: Span
 *  Provides primitives to work with contiguous sequence of bytes
 *
 */

#include <stdlib.h>
#include "Types.h"
#include "containers/Iterator.h"

/*
 * Type: struct FlSpan
 *   Represents a contiguous read-only sequence of bytes
 *
 * ===== C =====
 *   struct FlSpan {
 *       const FlByte * const sequence;
 *       const size_t element_size;
 *       const size_t length;
 *   };
 */
struct FlSpan {
    const FlByte * const sequence;
    const size_t element_size;
    const size_t length;
};

/*
 * Function: fl_span_new
 *   Creates a new struct FlSpan that holds a contiguous read-only sequence of bytes
 *
 * Parameters: 
 *  const FlByte * const sequence - Pointer to the sequence to hold within the struct
 *  size_t length - Number of bytes this span holds
 *
 * Return: 
 *  <struct FlSpan> - Representation of the contiguous read-only sequence of bytes
 *
 */
struct FlSpan fl_span_new(const FlByte * const sequence, size_t element_size, size_t offset, size_t length);

/*
 * Function: fl_span_equals
 *   Compare two spans and return true if they point to the same contiguous sequence of bytes
 *
 * Parameters:
 *  <struct FlSpan> *span1 - Span to compare
 *  <struct FlSpan> *span2 - Span to compare
 *
 * Return: 
 *  bool - True if both spans point to the same sequence and have the same length. False otherwise
 *
 */
bool fl_span_equals(const struct FlSpan *span1, const struct FlSpan *span2);

/*
 * Function: fl_span_equals_sequence
 *  Compare the span with the sequence of bytes.
 *
 * Parameters:
 *  <struct FlSpan> *span - Span to compare with the sequence of bytes
 *  const FlByte * const sequence - Sequence of bytes
 *  size_t sequence_length - Length of the sequence to compare
 *
 * Return:
 *  bool - True if the span is within the sequence of bytes
 *
 */
bool fl_span_equals_sequence(const struct FlSpan *span, const FlByte * const sequence, size_t sequence_length);

/*
 * Function: fl_span_begin
 *  Returns an <FlIterator> that points to the first element in the span
 *
 * Parameters:
 *  const struct FlSpan *span - The iterator will point to the begining of the span
 *
 * Return: 
 *  <FlIterator> - Iterator pointing to the first element in the span
 *
 */
FlIterator fl_span_begin(const struct FlSpan *span);

/*
 * Function: fl_span_end
 *  Returns an <FlIterator> that points to the past-last-nth element in the span
 *  The FlIterator returned by this function MUST NOT be dereferenced by <fl_iterator_value>.
 *
 * Parameters:
 *  const <struct FlSpan> *span - The iterator will point to the past-last-nth element in the span
 *
 * Return:
 *  <FlIterator> - Iterator pointing beyond the last element in the span
 *
 */
FlIterator fl_span_end(const struct FlSpan *span);

#endif /* FL_SPAN_H */
