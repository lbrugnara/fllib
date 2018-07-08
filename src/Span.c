#include "Std.h"
#include "Span.h"

struct FlSpan fl_span_new(const FlByte * const sequence, size_t dtsize, size_t offset, size_t length)
{
    return (struct FlSpan) {
        .sequence = sequence + dtsize * offset,
        .dtsize = dtsize,
        .length = length
    };
}

bool fl_span_equals(struct FlSpan *span1, struct FlSpan *span2)
{
    return span1->sequence == span2->sequence 
            && span1->dtsize == span2->dtsize
            && span1->length == span2->length;
}

bool fl_span_equals_sequence(struct FlSpan *span, const FlByte * const sequence)
{
    return fl_equals(span->sequence, sequence, span->length * span->dtsize);
}

/* -------------------------------------------------------------
 * FlIterator support
 * -------------------------------------------------------------
 */
typedef struct FlSpanIterator {
    const FlByte * const sequence;
    unsigned int current;
    size_t dtsize;  
} FlSpanIterator;

static void it_next(void *it)
{
    FlSpanIterator *spanIterator = (FlSpanIterator*)it;
    spanIterator->current += spanIterator->dtsize;
}

static void it_prev(void *it)
{
    FlSpanIterator *spanIterator = (FlSpanIterator*)it;
    spanIterator->current -= spanIterator->dtsize;
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
    return spanIterator->sequence + spanIterator->current == span->sequence + (span->dtsize * span->length);
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
        .dtsize = span->dtsize
    }, sizeof(*spanIterator));
    //spanIterator->sequence = span->sequence;
    //spanIterator->current = 0;
    //spanIterator->dtsize = span->dtsize;
    return fl_iterator_new(IT_SPAN, spanIterator, &it_next, &it_prev, &it_value, &it_equals, &it_is_start, &it_is_end, &it_delete);
}

FlIterator fl_span_end(const struct FlSpan *span)
{
    FlSpanIterator *spanIterator = fl_calloc(1, sizeof(struct FlSpanIterator));

    memcpy(spanIterator, &(struct FlSpanIterator){
        .sequence = span->sequence,
        .current = span->dtsize * span->length,
        .dtsize = span->dtsize
    }, sizeof(*spanIterator));

    //spanIterator->sequence = span->sequence;
    //spanIterator->current = span->dtsize * span->length;
    //spanIterator->dtsize = span->dtsize;
    return fl_iterator_new(IT_SPAN, spanIterator, &it_next, &it_prev, &it_value, &it_equals, &it_is_start, &it_is_end, &it_delete);
}
