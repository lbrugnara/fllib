#include <stdbool.h>
#include "Std.h"
#include "Slice.h"

struct FlSlice fl_slice_new(const FlByte * const sequence, size_t element_size, size_t offset, size_t length)
{
    return (struct FlSlice) {
        .sequence = sequence + element_size * offset,
        .element_size = element_size,
        .length = length
    };
}

bool fl_slice_equals(const struct FlSlice *slice1, const struct FlSlice *slice2)
{
    return slice1->sequence == slice2->sequence 
            && slice1->element_size == slice2->element_size
            && slice1->length == slice2->length;
}

bool fl_slice_equals_sequence(const struct FlSlice *slice, const FlByte * const sequence, size_t sequence_length)
{
    if (slice->length != sequence_length)
        return false;

    return fl_equals(slice->sequence, sequence, slice->length * slice->element_size);
}

/*
 * FlIterator support
 *
 */
typedef struct FlSliceIterator {
    const FlByte * const sequence;
    unsigned int current;
    size_t element_size;  
} FlSliceIterator;

static void it_next(void *it)
{
    FlSliceIterator *sliceIterator = (FlSliceIterator*)it;
    sliceIterator->current += sliceIterator->element_size;
}

static void it_prev(void *it)
{
    FlSliceIterator *sliceIterator = (FlSliceIterator*)it;
    sliceIterator->current -= sliceIterator->element_size;
}

static void* it_value(void *it)
{
    FlSliceIterator *sliceIterator = (FlSliceIterator*)it;
    return (void*)(sliceIterator->sequence + sliceIterator->current);
}

static bool it_equals(void *it1, void *it2)
{
    FlSliceIterator *sliceIt1 = (FlSliceIterator*)it1;
    FlSliceIterator *sliceIt2 = (FlSliceIterator*)it2;
    return sliceIt1->sequence+sliceIt1->current == sliceIt2->sequence + sliceIt2->current;
}

static bool it_is_start(void *it, void *container)
{
    FlSliceIterator *sliceIterator = it;
    struct FlSlice *slice = container;
    return sliceIterator->sequence + sliceIterator->current == slice->sequence;
}

static bool it_is_end(void *it, void *container)
{
    FlSliceIterator *sliceIterator = it;
    struct FlSlice *slice = container;
    return sliceIterator->sequence + sliceIterator->current == slice->sequence + (slice->element_size * slice->length);
}

static void it_delete(void *it)
{
    fl_free(it);
}

FlIterator* fl_slice_begin(const struct FlSlice *slice)
{
    FlSliceIterator *sliceIterator = fl_calloc(1, sizeof(struct FlSliceIterator));

    memcpy(sliceIterator, &(struct FlSliceIterator){
        .sequence = slice->sequence,
        .current = 0,
        .element_size = slice->element_size
    }, sizeof(*sliceIterator));

    return fl_iterator_new(IT_SPAN, sliceIterator, &it_next, &it_prev, &it_value, &it_equals, &it_is_start, &it_is_end, &it_delete);
}

FlIterator* fl_slice_end(const struct FlSlice *slice)
{
    FlSliceIterator *sliceIterator = fl_calloc(1, sizeof(struct FlSliceIterator));

    memcpy(sliceIterator, &(struct FlSliceIterator){
        .sequence = slice->sequence,
        .current = slice->element_size * slice->length,
        .element_size = slice->element_size
    }, sizeof(*sliceIterator));

    return fl_iterator_new(IT_SPAN, sliceIterator, &it_next, &it_prev, &it_value, &it_equals, &it_is_start, &it_is_end, &it_delete);
}
