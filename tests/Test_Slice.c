#include <fllib.h>

#include "Test_Slice.h"
#include <flut/flut.h>

void test_slice()
{
    const char * const sequence = "Some text we want to work with as contiguous sequence of bytes";

    struct FlSlice slice = fl_slice_new((const FlByte*)sequence, 1, 0, 4);
    flut_expect_compat("Sequence slice[0,4] is equals to 'Some'", flm_cstring_equals_n((const char*)slice.sequence, "Some", slice.length));
    flut_expect_compat("Slice[0,4] is equals to sequence", fl_slice_equals_sequence(&slice, (const FlByte*)sequence, slice.length));

    struct FlSlice slice2 = fl_slice_new((const FlByte*)sequence, 1, 34, 10);
    flut_expect_compat("Sequence slice[36,10] is equals to 'contiguous'", flm_cstring_equals_n((const char*)slice2.sequence, "contiguous", slice.length));
    flut_expect_compat("Slice[36,10] is equals to sequence + 34", fl_slice_equals_sequence(&slice2, (const FlByte*)sequence + 34, slice2.length));

    struct FlSlice slice3 = fl_slice_new((const FlByte*)sequence, 1, 57, 5);
    flut_expect_compat("Sequence slice[57,5] is equals to 'bytes'", flm_cstring_equals_n((const char*)slice3.sequence, "bytes", slice.length));
    flut_expect_compat("Slice[57,5] is equals to sequence + 57", fl_slice_equals_sequence(&slice3, (const FlByte*)sequence + 57, slice3.length));
    
    struct FlSlice slice4 = fl_slice_new((const FlByte*)sequence, 1, 34, 10);
    flut_expect_compat("Two slices[36,10] must be equals", fl_slice_equals(&slice2, &slice4));
}

void test_slice_iterator()
{
    int numbers[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

    struct FlSlice slice = fl_slice_new((const FlByte*)numbers, sizeof(int), 0, 10);

    // Forward
    FlIterator *sliceit = fl_slice_begin(&slice);

    for (int j=10; !fl_iterator_is_end(sliceit, &slice); j--, fl_iterator_next(sliceit))
        flut_vexpect_compat(j == flm_iterator_value(sliceit, int), "Iterator current value in slice must be equals to %d (j)", j);

    fl_iterator_free(sliceit);

    // Backward
    sliceit = fl_slice_end(&slice);

    for (int j=1; !fl_iterator_is_start(sliceit, &slice); j++)
    {
        fl_iterator_prev(sliceit);
        flut_vexpect_compat(j == flm_iterator_value(sliceit, int), "Iterator current value in slice must be equals to %d (j)", j);
    }

    fl_iterator_free(sliceit);


    // Subslice Forward
    struct FlSlice subslice = fl_slice_new((const FlByte*)numbers, sizeof(int), 5, 5);
    
    sliceit = fl_slice_begin(&subslice);

    for (int j=5; !fl_iterator_is_end(sliceit, &subslice); j--, fl_iterator_next(sliceit))
        flut_vexpect_compat(j == flm_iterator_value(sliceit, int), "Iterator current value in subslice must be equals to %d (j)", j);

    fl_iterator_free(sliceit);

    // Subslice Backward
    sliceit = fl_slice_end(&subslice);

    for (int j=1; !fl_iterator_is_start(sliceit, &subslice); j++)
    {
        fl_iterator_prev(sliceit);
        flut_vexpect_compat(j == flm_iterator_value(sliceit, int), "Iterator current value in subslice must be equals to %d (j)", j);
    }

    fl_iterator_free(sliceit);
}
