#include "Std.h"
#include "Span.h"

struct FlSpan fl_span_new(const FlByte * const sequence, size_t element_size, size_t offset, size_t length)
{
    return (struct FlSpan) {
        .sequence = sequence + element_size * offset,
        .element_size = element_size,
        .length = length
    };
}

bool fl_span_equals(const struct FlSpan *span1, const struct FlSpan *span2)
{
    return span1->sequence == span2->sequence 
            && span1->element_size == span2->element_size
            && span1->length == span2->length;
}

bool fl_span_equals_sequence(const struct FlSpan *span, const FlByte * const sequence, size_t sequence_length)
{
    if (span->length != sequence_length)
        return false;

    return fl_equals(span->sequence, sequence, span->length * span->element_size);
}

/*
 * FlIterator support
 *
 */
typedef struct FlSpanIterator {
    const FlByte * const sequence;
    unsigned int current;
    size_t element_size;  
} FlSpanIterator;

static void it_next(void *it)
{
    FlSpanIterator *spanIterator = (FlSpanIterator*)it;
    spanIterator->current += spanIterator->element_size;
}

static void it_prev(void *it)
{
    FlSpanIterator *spanIterator = (FlSpanIterator*)it;
    spanIterator->current -= spanIterator->element_size;
}

static void* it_value(void *it)
{
    FlSpanIterator *spanIterator = (FlSpanIterator*)it;
    return (void*)(spanIterator->sequence + spanIterator->current);
}

static bool it_equals(void *it1, void *it2)
{
    FlSpanIterator *spanIt1 = (FlSpanIterator*)it1;
    FlSpanIterator *spanIt2 = (FlSpanIterator*)it2;
    return spanIt1->sequence+spanIt1->current == spanIt2->sequence + spanIt2->current;
}

static bool it_is_start(void *it, void *container)
{
    FlSpanIterator *spanIterator = it;
    struct FlSpan *span = container;
    return spanIterator->sequence + spanIterator->current == span->sequence;
}

static bool it_is_end(void *it, void *container)
{
    FlSpanIterator *spanIterator = it;
    struct FlSpan *span = container;
    return spanIterator->sequence + spanIterator->current == span->sequence + (span->element_size * span->length);
}

static void it_delete(void *it)
{
    fl_free(it);
}

FlIterator fl_span_begin(const struct FlSpan *span)
{
    FlSpanIterator *spanIterator = fl_calloc(1, sizeof(struct FlSpanIterator));

    memcpy(spanIterator, &(struct FlSpanIterator){
        .sequence = span->sequence,
        .current = 0,
        .element_size = span->element_size
    }, sizeof(*spanIterator));

    return fl_iterator_new(IT_SPAN, spanIterator, &it_next, &it_prev, &it_value, &it_equals, &it_is_start, &it_is_end, &it_delete);
}

FlIterator fl_span_end(const struct FlSpan *span)
{
    FlSpanIterator *spanIterator = fl_calloc(1, sizeof(struct FlSpanIterator));

    memcpy(spanIterator, &(struct FlSpanIterator){
        .sequence = span->sequence,
        .current = span->element_size * span->length,
        .element_size = span->element_size
    }, sizeof(*spanIterator));

    return fl_iterator_new(IT_SPAN, spanIterator, &it_next, &it_prev, &it_value, &it_equals, &it_is_start, &it_is_end, &it_delete);
}
