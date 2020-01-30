#ifndef fl_slice_H
#define fl_slice_H

/*
 * File: Slice
 *  Provides primitives to work with contiguous sequence of bytes
 *
 */

#include <stdlib.h>
#include "Types.h"
#include "containers/Iterator.h"

/*
 * Type: struct FlSlice
 *   Represents a contiguous read-only sequence of bytes
 *
 * ===== C =====
 *   struct FlSlice {
 *       const FlByte * const sequence;
 *       const size_t element_size;
 *       const size_t length;
 *   };
 */
struct FlSlice {
    const FlByte * const sequence;
    const size_t element_size;
    const size_t length;
};

/*
 * Function: fl_slice_new
 *   Creates a new struct FlSlice that holds a contiguous read-only sequence of bytes
 *
 * Parameters: 
 *  const FlByte * const sequence - Pointer to the sequence to hold within the struct
 *  size_t length - Number of bytes this slice holds
 *
 * Return: 
 *  <struct FlSlice> - Representation of the contiguous read-only sequence of bytes
 *
 */
struct FlSlice fl_slice_new(const FlByte * const sequence, size_t element_size, size_t offset, size_t length);

/*
 * Function: fl_slice_equals
 *   Compare two slices and return true if they point to the same contiguous sequence of bytes
 *
 * Parameters:
 *  <struct FlSlice> *slice1 - Slice to compare
 *  <struct FlSlice> *slice2 - Slice to compare
 *
 * Return: 
 *  bool - True if both slices point to the same sequence and have the same length. False otherwise
 *
 */
bool fl_slice_equals(const struct FlSlice *slice1, const struct FlSlice *slice2);

/*
 * Function: fl_slice_equals_sequence
 *  Compare the slice with the sequence of bytes.
 *
 * Parameters:
 *  <struct FlSlice> *slice - Slice to compare with the sequence of bytes
 *  const FlByte * const sequence - Sequence of bytes
 *  size_t sequence_length - Length of the sequence to compare
 *
 * Return:
 *  bool - True if the slice is within the sequence of bytes
 *
 */
bool fl_slice_equals_sequence(const struct FlSlice *slice, const FlByte * const sequence, size_t sequence_length);

/*
 * Function: fl_slice_begin
 *  Returns an <FlIterator> that points to the first element in the slice
 *
 * Parameters:
 *  const struct FlSlice *slice - The iterator will point to the begining of the slice
 *
 * Return: 
 *  <FlIterator*> - Iterator pointing to the first element in the slice
 *
 */
FlIterator* fl_slice_begin(const struct FlSlice *slice);

/*
 * Function: fl_slice_end
 *  Returns an <FlIterator> that points to the past-last-nth element in the slice
 *  The FlIterator returned by this function MUST NOT be dereferenced by <fl_iterator_value>.
 *
 * Parameters:
 *  const <struct FlSlice> *slice - The iterator will point to the past-last-nth element in the slice
 *
 * Return:
 *  <FlIterator*> - Iterator pointing beyond the last element in the slice
 *
 */
FlIterator* fl_slice_end(const struct FlSlice *slice);

#endif /* fl_slice_H */
