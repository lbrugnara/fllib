#include <fllib.h>
#include <flut/flut.h>

flut_suite(array) {
    flut_suite_description("Dynamic array functions");
    flut_suite_register_test(array_combine, "Array combine");
    flut_suite_register_test(array_append, "Array append");
    flut_suite_register_test(array_contains_n, "Array contains N");
}

flut_test(array_combine) {
    {
        int *zero_to_nine = fl_array_new(sizeof(int), 10);
        int *ten_to_nineteen = fl_array_new(sizeof(int), 10);

        for (int i=0; i < 10; i++)
        {
            zero_to_nine[i] = i;
            ten_to_nineteen[i] = i + 10;
        }

        int *zero_to_nineteen = fl_array_new(sizeof(int), 0);
        zero_to_nineteen = fl_array_combine(zero_to_nineteen, zero_to_nine);
        zero_to_nineteen = fl_array_combine(zero_to_nineteen, ten_to_nineteen);

        flut_assert_is_true(fl_array_length(zero_to_nineteen) == 20);

        for (int i=0; i < 20; i++)
            flut_assert_int_is_equals(i, zero_to_nineteen[i]);

        fl_array_free(zero_to_nine);
        fl_array_free(ten_to_nineteen);
        fl_array_free(zero_to_nineteen);
    }

    {
        int numbers[20];

        int **zero_to_nine = fl_array_new(sizeof(int*), 10);
        int **ten_to_nineteen = fl_array_new(sizeof(int*), 10);

        for (int i=0; i < 10; i++)
        {
            numbers[i] = i;
            numbers[i + 10] = i + 10;
            zero_to_nine[i] = numbers + i;
            ten_to_nineteen[i] = numbers + i + 10;
        }

        int **zero_to_nineteen = fl_array_new(sizeof(int*), 0);
        zero_to_nineteen = fl_array_combine(zero_to_nineteen, zero_to_nine);
        zero_to_nineteen = fl_array_combine(zero_to_nineteen, ten_to_nineteen);

        flut_assert_is_true(fl_array_length(zero_to_nineteen) == 20);

        for (int i=0; i < 20; i++)
        {
            flut_assert_pointer_is_equals(numbers + i, zero_to_nineteen[i]);
            flut_assert_int_is_equals(i, *zero_to_nineteen[i]);
        }

        fl_array_free(zero_to_nine);
        fl_array_free(ten_to_nineteen);
        fl_array_free(zero_to_nineteen);
    }
}

flut_test(array_append) {

    int *numbers = fl_array_new(sizeof(int), 0);

    for (int i=0; i < 10; i++)
        numbers = fl_array_append(numbers, &i);

    flut_assert_is_true(fl_array_length(numbers) == 10);

    for (int i=0; i < 10; i++)
            flut_assert_int_is_equals(i, numbers[i]);

    fl_array_free(numbers);
}

flut_test(array_contains_n) {

    flut_describe("fl_array_contains_n should work for array of characters") {
        char array[] = { 'a', 'e', 'i', 'o', 'u' };

        #define MAKE_CHAR_PTR(c)   (char[]) { c }
        flut_assert_is_true(fl_array_contains_n(array, flm_array_length(array), MAKE_CHAR_PTR('a'), sizeof(char)));
        flut_assert_is_true(fl_array_contains_n(array, flm_array_length(array), MAKE_CHAR_PTR('e'), sizeof(char)));
        flut_assert_is_true(fl_array_contains_n(array, flm_array_length(array), MAKE_CHAR_PTR('i'), sizeof(char)));
        flut_assert_is_true(fl_array_contains_n(array, flm_array_length(array), MAKE_CHAR_PTR('o'), sizeof(char)));
        flut_assert_is_true(fl_array_contains_n(array, flm_array_length(array), MAKE_CHAR_PTR('u'), sizeof(char)));
        flut_assert_is_false(fl_array_contains_n(array, flm_array_length(array), MAKE_CHAR_PTR('b'), sizeof(char)));
        #undef MAKE_CHAR_PTR
    }

    flut_describe("fl_array_contains_n should work for array of integers") {
        int array[] = { 0, 2, 4, 6, 8, 10 };

        #define MAKE_INT_PTR(i)   (int[]) { i }
        flut_assert_is_true(fl_array_contains_n(array, flm_array_length(array), MAKE_INT_PTR(0), sizeof(int)));
        flut_assert_is_true(fl_array_contains_n(array, flm_array_length(array), MAKE_INT_PTR(2), sizeof(int)));
        flut_assert_is_true(fl_array_contains_n(array, flm_array_length(array), MAKE_INT_PTR(4), sizeof(int)));
        flut_assert_is_true(fl_array_contains_n(array, flm_array_length(array), MAKE_INT_PTR(6), sizeof(int)));
        flut_assert_is_true(fl_array_contains_n(array, flm_array_length(array), MAKE_INT_PTR(8), sizeof(int)));
        flut_assert_is_true(fl_array_contains_n(array, flm_array_length(array), MAKE_INT_PTR(10), sizeof(int)));
        flut_assert_is_false(fl_array_contains_n(array, flm_array_length(array), MAKE_INT_PTR(1), sizeof(int)));
        #undef MAKE_INT_PTR
    }

    flut_describe("fl_array_contains_n should work for structs") {
        struct ItemTest
        {
            int i;
            char c;
            float f;
        };
        
        struct ItemTest template[] = { { 0, 'a', 0.0 }, { 1, 'b', 0.1 }, { 2, 'c', 0.2 } };
        size_t array_length = flm_array_length(template);

        struct ItemTest *array = fl_malloc(sizeof(struct ItemTest) * array_length);
        
        for (size_t i=0; i < array_length; i++)
        {
            array[i].i = template[i].i;
            array[i].c = template[i].c;
            array[i].f = template[i].f;
        }
        
        struct ItemTest *values = fl_malloc(sizeof(struct ItemTest) * 4);
        struct ItemTest *current = values - 1;
        #define MAKE_STRUCT_PTR(vi, vc, vf)    (current++, current->i = vi, current->c = vc, current->f = vf, current)
        flut_assert_is_true(fl_array_contains_n(array, array_length, MAKE_STRUCT_PTR(0, 'a', 0.0), sizeof(struct ItemTest)));
        flut_assert_is_true(fl_array_contains_n(array, array_length, MAKE_STRUCT_PTR(1, 'b', 0.1), sizeof(struct ItemTest)));
        flut_assert_is_true(fl_array_contains_n(array, array_length, MAKE_STRUCT_PTR(2, 'c', 0.2), sizeof(struct ItemTest)));
        flut_assert_is_false(fl_array_contains_n(array, array_length, MAKE_STRUCT_PTR(3, 'd', 0.3), sizeof(struct ItemTest)));
        #undef MAKE_STRUCT_PTR
        fl_free(values);
        fl_free(array);
    }
}
