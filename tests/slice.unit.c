#include <fllib.h>
#include <flut/flut.h>

flut_define_suite(slice) {
    flut_suite_description("slice module");
    flut_suite_register_test(slice_new, "fl_slice_new function");
    flut_suite_register_test(slice_iterator, "Iterators");
}

flut_define_test(slice_new) {

    const char * const sequence = "Some text we want to work with as contiguous sequence of bytes";

    flut_vdescribe("Working with slices of the string: '%s'", sequence) {
        struct FlSlice slice = fl_slice_new((const FlByte*)sequence, 1, 0, 4);
        flut_assert_explain(assert->str.equals_n("Some", (const char*)slice.sequence, slice.length, false), "Sequence slice[0,4] is equals to 'Some'");
        flut_assert_explain(assert->is_true(fl_slice_equals_sequence(&slice, (const FlByte*)sequence, slice.length)), "Slice[0,4] is equals to sequence");

        struct FlSlice slice2 = fl_slice_new((const FlByte*)sequence, 1, 34, 10);
        flut_assert_explain(assert->str.equals_n("contiguous", (const char*)slice2.sequence, slice2.length, false), "Sequence slice[36,10] is equals to 'contiguous'");
        flut_assert_explain(assert->is_true(fl_slice_equals_sequence(&slice2, (const FlByte*)sequence + 34, slice2.length)), "Slice[36,10] is equals to sequence + 34");

        struct FlSlice slice3 = fl_slice_new((const FlByte*)sequence, 1, 57, 5);
        flut_assert_explain(assert->str.equals_n("bytes", (const char*)slice3.sequence, slice3.length, false), "Sequence slice[57,5] is equals to 'bytes'");
        flut_assert_explain(assert->is_true(fl_slice_equals_sequence(&slice3, (const FlByte*)sequence + 57, slice3.length)), "Slice[57,5] is equals to sequence + 57");
        
        struct FlSlice slice4 = fl_slice_new((const FlByte*)sequence, 1, 34, 10);
        flut_assert_explain(assert->is_true(fl_slice_equals(&slice2, &slice4)), "Two slices[36,10] must be equals");
    }
}

flut_define_test(slice_iterator) {
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
