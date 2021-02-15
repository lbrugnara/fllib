#include <fllib.h>
#include <flut/flut.h>

flut_suite(slice) {
    flut_suite_description("slice module");
    flut_suite_register_test(slice_new, "Creating slices of a sequence of bytes");
    flut_suite_register_test(slice_iterator, "Working with slice iterators");
}

flut_test(slice_new) {

    const char * const string = "Some text we want to work with as contiguous sequence of bytes";

    flut_describev("Working with slices of the string: '%s'", string) {
        struct FlSlice slice = fl_slice_new((const FlByte*) string, 1, 0, 4);
        flut_assert_string_is_equals_n("Some", (const char*) slice.sequence, slice.length, false);
        flut_assert_is_true(fl_slice_equals_sequence(&slice, (const FlByte*) string, slice.length));

        struct FlSlice slice2 = fl_slice_new((const FlByte*) string, 1, 34, 10);
        flut_assert_string_is_equals_n("contiguous", (const char*) slice2.sequence, slice2.length, false);
        flut_assert_is_true(fl_slice_equals_sequence(&slice2, (const FlByte*) string + 34, slice2.length));

        struct FlSlice slice3 = fl_slice_new((const FlByte*) string, 1, 57, 5);
        flut_assert_string_is_equals_n("bytes", (const char*) slice3.sequence, slice3.length, false);
        flut_assert_is_true(fl_slice_equals_sequence(&slice3, (const FlByte*) string + 57, slice3.length));
        
        struct FlSlice slice4 = fl_slice_new((const FlByte*) string, 1, 34, 10);
        flut_assert_is_true(fl_slice_equals(&slice2, &slice4));
    }
}

flut_test(slice_iterator) {
    int numbers[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

    struct FlSlice slice = fl_slice_new((const FlByte*)numbers, sizeof(int), 0, 10);
    struct FlSlice subslice = fl_slice_new((const FlByte*)numbers, sizeof(int), 5, 5);

    flut_describe("Moving forward through all the slice with iterators") {
        FlIterator *slice_it = fl_slice_begin(&slice);

        for (int j=10; !fl_iterator_is_end(slice_it, &slice); j--, fl_iterator_next(slice_it)) {
            flut_assert_int_is_equals(j, flm_iterator_value(slice_it, int));
        }

        fl_iterator_free(slice_it);
    }

    flut_describe("Moving backward through all the slice with iterators") {
        FlIterator *slice_it = fl_slice_end(&slice);

        for (int j=1; !fl_iterator_is_start(slice_it, &slice); j++)
        {
            fl_iterator_prev(slice_it);
            flut_assert_int_is_equals(j, flm_iterator_value(slice_it, int));
        }

        fl_iterator_free(slice_it);
    }

    flut_describe("Moving forward through part of the slice with iterators") {    
        FlIterator *slice_it = fl_slice_begin(&subslice);

        for (int j=5; !fl_iterator_is_end(slice_it, &subslice); j--, fl_iterator_next(slice_it))
            flut_assert_int_is_equals(j, flm_iterator_value(slice_it, int));

        fl_iterator_free(slice_it);
    }

    flut_describe("Moving barward through part of the slice with iterators") {    
        FlIterator *slice_it = fl_slice_end(&subslice);

        for (int j=1; !fl_iterator_is_start(slice_it, &subslice); j++)
        {
            fl_iterator_prev(slice_it);
            flut_assert_int_is_equals(j, flm_iterator_value(slice_it, int));
        }

        fl_iterator_free(slice_it);
    }
}
