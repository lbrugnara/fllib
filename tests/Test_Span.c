#include <fllib.h>

#include "Test_Span.h"
#include "Test.h"

void test_span()
{
    const char * const sequence = "Some text we want to work with as contiguous sequence of bytes";

    struct FlSpan span = fl_span_new((const FlByte*)sequence, 1, 0, 4);
    fl_expect("Sequence span[0,4] is equals to 'Some'", flm_cstring_nequals((const char*)span.sequence, "Some", span.length));
    fl_expect("Span[0,4] is equals to sequence", fl_span_equals_sequence(&span, (const FlByte*)sequence));

    struct FlSpan span2 = fl_span_new((const FlByte*)sequence, 1, 34, 10);
    fl_expect("Sequence span[36,10] is equals to 'contiguous'", flm_cstring_nequals((const char*)span2.sequence, "contiguous", span.length));
    fl_expect("Span[36,10] is equals to sequence + 34", fl_span_equals_sequence(&span2, (const FlByte*)sequence + 34));

    struct FlSpan span3 = fl_span_new((const FlByte*)sequence, 1, 57, 5);
    fl_expect("Sequence span[57,5] is equals to 'bytes'", flm_cstring_nequals((const char*)span3.sequence, "bytes", span.length));
    fl_expect("Span[57,5] is equals to sequence + 57", fl_span_equals_sequence(&span3, (const FlByte*)sequence + 57));
    
    struct FlSpan span4 = fl_span_new((const FlByte*)sequence, 1, 34, 10);
    fl_expect("Two spans[36,10] must be equals", fl_span_equals(&span2, &span4));
}

void test_span_iterator()
{
    int numbers[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

    struct FlSpan span = fl_span_new((const FlByte*)numbers, sizeof(int), 0, 10);

    // Forward
    FlIterator spanit = fl_span_begin(&span);

    for (int j=10; !fl_iterator_is_end(spanit, &span); j--, fl_iterator_next(spanit))
        fl_vexpect(j == flm_iterator_value(spanit, int), "Iterator current value in span must be equals to %d (j)", j);

    fl_iterator_delete(spanit);

    // Backward
    spanit = fl_span_end(&span);

    for (int j=1; !fl_iterator_is_start(spanit, &span); j++)
    {
        fl_iterator_prev(spanit);
        fl_vexpect(j == flm_iterator_value(spanit, int), "Iterator current value in span must be equals to %d (j)", j);
    }

    fl_iterator_delete(spanit);


    // Subspan Forward
    struct FlSpan subspan = fl_span_new((const FlByte*)numbers, sizeof(int), 5, 5);
    
    spanit = fl_span_begin(&subspan);

    for (int j=5; !fl_iterator_is_end(spanit, &subspan); j--, fl_iterator_next(spanit))
        fl_vexpect(j == flm_iterator_value(spanit, int), "Iterator current value in subspan must be equals to %d (j)", j);

    fl_iterator_delete(spanit);

    // Subspan Backward
    spanit = fl_span_end(&subspan);

    for (int j=1; !fl_iterator_is_start(spanit, &subspan); j++)
    {
        fl_iterator_prev(spanit);
        fl_vexpect(j == flm_iterator_value(spanit, int), "Iterator current value in subspan must be equals to %d (j)", j);
    }

    fl_iterator_delete(spanit);
}
