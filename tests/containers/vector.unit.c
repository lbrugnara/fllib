#include <fllib.h>
#include <math.h>
#include <flut/flut.h>

flut_suite(vector) {
    flut_suite_description("Vector functions");
    flut_suite_register_test(vector_new, "fl_vector_new function");
    flut_suite_register_test(vector_add, "fl_vector_add function");
    flut_suite_register_test(vector_insert, "fl_vector_insert function");
    flut_suite_register_test(vector_first, "fl_vector_ref_first function");
    flut_suite_register_test(vector_last, "fl_vector_ref_last function");
    flut_suite_register_test(vector_contains, "fl_vector_contains function");
    flut_suite_register_test(vector_shift, "fl_vector_shift function");
    flut_suite_register_test(vector_pop, "fl_vector_pop function");
    flut_suite_register_test(vector_get, "fl_vector_get function");
    flut_suite_register_test(vector_value_writer, "fl_container_write function in vectors");
    flut_suite_register_test(vector_resize, "Vector resize operation");
    flut_suite_register_test(vector_max_capacity, "Vector max. capacity");
    flut_suite_register_test(vector_cleanup_fn, "Vector cleanup functions");
}

flut_test(vector_new) {
    flut_describe("Passing a list of designated initializers to flm_vector_new_with must return a valid vector (.capacity = 1)") {
        FlVector *vector = flm_vector_new_with(.capacity = 1);

        flut_assert_is_not_null(vector);
        flut_assert_size_t_is_equals(0, fl_vector_length(vector));
        flut_assert_size_t_is_equals(1, fl_vector_capacity(vector));
        flut_assert_size_t_is_equals(SIZE_MAX / sizeof(void*), fl_vector_max_capacity(vector));
        flut_assert_is_true(fl_vector_growth_factor(vector) == 2.0);
        flut_assert_size_t_is_equals(sizeof(void*), fl_vector_element_size(vector));

        fl_vector_free(vector);
    }

    flut_describe("Passing a list of positional initializers to flm_vector_new_with must return a valid vector (fl_container_writer, fl_container_cleaner_pointer, 1.5, 16, 15, 30)") {
        FlVector *vector = flm_vector_new_with(fl_container_writer, fl_container_cleaner_pointer, 1.5, 16, 15, 30);

        flut_assert_is_not_null(vector);
        flut_assert_size_t_is_equals(0, fl_vector_length(vector));
        flut_assert_size_t_is_equals(15, fl_vector_capacity(vector));
        flut_assert_size_t_is_equals(30, fl_vector_max_capacity(vector));
        flut_assert_is_true(fl_vector_growth_factor(vector) == 1.5);
        flut_assert_size_t_is_equals(16, fl_vector_element_size(vector));

        fl_vector_free(vector);
    }

    flut_describe("flm_vector_new_with(.capacity = 10) should create a valid vector") {
        FlVector *vector = flm_vector_new_with(.capacity = 10);

        flut_assert_is_not_null(vector);
        flut_assert_size_t_is_equals(0, fl_vector_length(vector));
        flut_assert_size_t_is_equals(10, fl_vector_capacity(vector));
        flut_assert_size_t_is_equals(SIZE_MAX / sizeof(void*), fl_vector_max_capacity(vector));
        flut_assert_is_true(fl_vector_growth_factor(vector) == 2.0);
        flut_assert_size_t_is_equals(sizeof(void*), fl_vector_element_size(vector));

        fl_vector_free(vector);
    }

    flut_describe("flm_vector_new_with(.capacity = 100) should create a valid vector") {
        FlVector *vector = flm_vector_new_with(.capacity = 100);

        flut_assert_is_not_null(vector);
        flut_assert_size_t_is_equals(0, fl_vector_length(vector));
        flut_assert_size_t_is_equals(100, fl_vector_capacity(vector));
        flut_assert_size_t_is_equals(SIZE_MAX / sizeof(void*), fl_vector_max_capacity(vector));
        flut_assert_is_true(fl_vector_growth_factor(vector) == 2.0);
        flut_assert_size_t_is_equals(sizeof(void*), fl_vector_element_size(vector));

        fl_vector_free(vector);
    }

    flut_describe("flm_vector_new_with(.capacity = 0) should create a valid vector") {
        FlVector *vector = flm_vector_new_with(.capacity = 0);

        flut_assert_is_not_null(vector);
        flut_assert_size_t_is_equals(0, fl_vector_length(vector));
        flut_assert_size_t_is_equals(1, fl_vector_capacity(vector));
        flut_assert_size_t_is_equals(SIZE_MAX / sizeof(void*), fl_vector_max_capacity(vector));
        flut_assert_is_true(fl_vector_growth_factor(vector) == 2.0);
        flut_assert_size_t_is_equals(sizeof(void*), fl_vector_element_size(vector));

        fl_vector_free(vector);
    }

    flut_describe("flm_vector_new_with(.element_size = sizeof(int*), .capacity = 20, .max_capacity = 40, .growth_factor = 1.6) should create a valid vector") {
        FlVector *vector = flm_vector_new_with(.element_size = sizeof(int*), .capacity = 20, .max_capacity = 40, .growth_factor = 1.6);

        flut_assert_is_not_null(vector);
        flut_assert_size_t_is_equals(0, fl_vector_length(vector));
        flut_assert_size_t_is_equals(20, fl_vector_capacity(vector));
        flut_assert_size_t_is_equals(40, fl_vector_max_capacity(vector));
        flut_assert_is_true(fl_vector_growth_factor(vector) == 1.6);
        flut_assert_size_t_is_equals(sizeof(int*), fl_vector_element_size(vector));

        fl_vector_free(vector);
    }

    flut_describe("flm_vector_new_with(.element_size = sizeof(double), .capacity = 20, .growth_factor = 1.6) should create a valid vector") {
        FlVector *vector = flm_vector_new_with(.element_size = sizeof(double), .capacity = 20, .growth_factor = 1.6);

        flut_assert_is_not_null(vector);
        flut_assert_size_t_is_equals(0, fl_vector_length(vector));
        flut_assert_size_t_is_equals(20, fl_vector_capacity(vector));
        flut_assert_size_t_is_equals(SIZE_MAX / sizeof(double), fl_vector_max_capacity(vector));
        flut_assert_is_true(fl_vector_growth_factor(vector) == 1.6);
        flut_assert_size_t_is_equals(sizeof(double), fl_vector_element_size(vector));

        fl_vector_free(vector);
    }

    flut_describe("flm_vector_new_with(.capacity = SIZE_MAX, .element_size = sizeof(void*)) must return NULL (cannot be allocated)") {
        FlVector *vector = flm_vector_new_with(.capacity = SIZE_MAX, .element_size = sizeof(void*));
        flut_assert_is_null(vector);
    }

    flut_describe("flm_vector_new_with(.element_size = SIZE_MAX, .max_capacity = 10) must return NULL (cannot be allocated") {
        FlVector *vector = flm_vector_new_with(.element_size = SIZE_MAX, .max_capacity = 10);
        flut_assert_is_null(vector);
    }

    flut_describe("flm_vector_new_with(.capacity = 10, .max_capacity = 5) must return NULL") {
        FlVector *vector = flm_vector_new_with(.capacity = 10, .max_capacity = 5);
        flut_assert_is_null(vector);
    }
}

flut_test(vector_add) {
    flut_describe("fl_vector_add should work with primitive types") {
        FlVector *vector = flm_vector_new_with(.element_size = sizeof(size_t), .capacity = 10);
        flut_assert_is_true(flm_vector_add(vector, int, 0));
        flut_assert_is_true(flm_vector_add(vector, int, 1));
        flut_assert_is_true(flm_vector_add(vector, int, 2));
        flut_assert_is_true(flm_vector_add(vector, int, 3));
        flut_assert_is_true(flm_vector_add(vector, int, 4));
        flut_assert_is_true(flm_vector_add(vector, int, 5));
        flut_assert_is_true(flm_vector_add(vector, int, 6));
        flut_assert_is_true(flm_vector_add(vector, int, 7));
        flut_assert_is_true(flm_vector_add(vector, int, 8));
        flut_assert_is_true(flm_vector_add(vector, int, 9));
        flut_assert_size_t_is_equals(10, fl_vector_length(vector));
        flut_assert_int_is_equals(0, flm_vector_get(vector, int, 0));
        flut_assert_int_is_equals(1, flm_vector_get(vector, int, 1));
        flut_assert_int_is_equals(2, flm_vector_get(vector, int, 2));
        flut_assert_int_is_equals(3, flm_vector_get(vector, int, 3));
        flut_assert_int_is_equals(4, flm_vector_get(vector, int, 4));
        flut_assert_int_is_equals(5, flm_vector_get(vector, int, 5));
        flut_assert_int_is_equals(6, flm_vector_get(vector, int, 6));
        flut_assert_int_is_equals(7, flm_vector_get(vector, int, 7));
        flut_assert_int_is_equals(8, flm_vector_get(vector, int, 8));
        flut_assert_int_is_equals(9, flm_vector_get(vector, int, 9));
        fl_vector_free(vector);
    }

    flut_describe("fl_vector_add should work with pointers") {
        FlVector *vector = flm_vector_new_with(.element_size = sizeof(char*), .cleaner = fl_container_cleaner_pointer, .capacity = 10);
        flut_assert_is_true(flm_vector_add(vector, char*, fl_cstring_vdup("Element %d", 0)));
        flut_assert_is_true(flm_vector_add(vector, char*, fl_cstring_vdup("Element %d", 1)));
        flut_assert_is_true(flm_vector_add(vector, char*, fl_cstring_vdup("Element %d", 2)));
        flut_assert_is_true(flm_vector_add(vector, char*, fl_cstring_vdup("Element %d", 3)));
        flut_assert_is_true(flm_vector_add(vector, char*, fl_cstring_vdup("Element %d", 4)));
        flut_assert_is_true(flm_vector_add(vector, char*, fl_cstring_vdup("Element %d", 5)));
        flut_assert_is_true(flm_vector_add(vector, char*, fl_cstring_vdup("Element %d", 6)));
        flut_assert_is_true(flm_vector_add(vector, char*, fl_cstring_vdup("Element %d", 7)));
        flut_assert_is_true(flm_vector_add(vector, char*, fl_cstring_vdup("Element %d", 8)));
        flut_assert_is_true(flm_vector_add(vector, char*, fl_cstring_vdup("Element %d", 9)));
        flut_assert_size_t_is_equals(10, fl_vector_length(vector));
        flut_assert_string_is_equals("Element 0", flm_vector_get(vector, char*, 0), false);
        flut_assert_string_is_equals("Element 1", flm_vector_get(vector, char*, 1), false);
        flut_assert_string_is_equals("Element 2", flm_vector_get(vector, char*, 2), false);
        flut_assert_string_is_equals("Element 3", flm_vector_get(vector, char*, 3), false);
        flut_assert_string_is_equals("Element 4", flm_vector_get(vector, char*, 4), false);
        flut_assert_string_is_equals("Element 5", flm_vector_get(vector, char*, 5), false);
        flut_assert_string_is_equals("Element 6", flm_vector_get(vector, char*, 6), false);
        flut_assert_string_is_equals("Element 7", flm_vector_get(vector, char*, 7), false);
        flut_assert_string_is_equals("Element 8", flm_vector_get(vector, char*, 8), false);
        flut_assert_string_is_equals("Element 9", flm_vector_get(vector, char*, 9), false);
        fl_vector_free(vector);
    }

    flut_describe("Add should not succeed if max. capacity has been reached") {
        FlVector *vector = flm_vector_new_with(.max_capacity = 2, .element_size = sizeof(size_t));
        flut_assert_is_true(flm_vector_add(vector, size_t, 0));
        flut_assert_is_true(flm_vector_add(vector, size_t, 1));
        flut_assert_size_t_is_equals(fl_vector_max_capacity(vector), fl_vector_length(vector));
        flut_assert_is_false(flm_vector_add(vector, size_t, 0));
        fl_vector_free(vector);
    }
}

flut_test(vector_insert) {
    flut_describe("Insert of primitive types in position 0 must result in an ordedered vector") {
        FlVector *vector = flm_vector_new_with(.capacity = 10, .element_size = sizeof(size_t));
        flut_assert_is_true(flm_vector_insert(vector, size_t, 9, 0));
        flut_assert_is_true(flm_vector_insert(vector, size_t, 8, 0));
        flut_assert_is_true(flm_vector_insert(vector, size_t, 7, 0));
        flut_assert_is_true(flm_vector_insert(vector, size_t, 6, 0));
        flut_assert_is_true(flm_vector_insert(vector, size_t, 5, 0));
        flut_assert_is_true(flm_vector_insert(vector, size_t, 4, 0));
        flut_assert_is_true(flm_vector_insert(vector, size_t, 3, 0));
        flut_assert_is_true(flm_vector_insert(vector, size_t, 2, 0));
        flut_assert_is_true(flm_vector_insert(vector, size_t, 1, 0));
        flut_assert_is_true(flm_vector_insert(vector, size_t, 0, 0));
        flut_assert_size_t_is_equals(10, fl_vector_length(vector));
        flut_assert_int_is_equals(0, flm_vector_get(vector, size_t, 0));
        flut_assert_int_is_equals(1, flm_vector_get(vector, size_t, 1));
        flut_assert_int_is_equals(2, flm_vector_get(vector, size_t, 2));
        flut_assert_int_is_equals(3, flm_vector_get(vector, size_t, 3));
        flut_assert_int_is_equals(4, flm_vector_get(vector, size_t, 4));
        flut_assert_int_is_equals(5, flm_vector_get(vector, size_t, 5));
        flut_assert_int_is_equals(6, flm_vector_get(vector, size_t, 6));
        flut_assert_int_is_equals(7, flm_vector_get(vector, size_t, 7));
        flut_assert_int_is_equals(8, flm_vector_get(vector, size_t, 8));
        flut_assert_int_is_equals(9, flm_vector_get(vector, size_t, 9));
        fl_vector_free(vector);
    }

    flut_describe("Insert of pointers in position 0 must result in an ordedered vector") {
        FlVector *vector = flm_vector_new_with(.element_size = sizeof(char*), .cleaner = fl_container_cleaner_pointer, .capacity = 10);
        flut_assert_is_true(flm_vector_insert(vector, char*, fl_cstring_vdup("Element %d", 9), 0));
        flut_assert_is_true(flm_vector_insert(vector, char*, fl_cstring_vdup("Element %d", 8), 0));
        flut_assert_is_true(flm_vector_insert(vector, char*, fl_cstring_vdup("Element %d", 7), 0));
        flut_assert_is_true(flm_vector_insert(vector, char*, fl_cstring_vdup("Element %d", 6), 0));
        flut_assert_is_true(flm_vector_insert(vector, char*, fl_cstring_vdup("Element %d", 5), 0));
        flut_assert_is_true(flm_vector_insert(vector, char*, fl_cstring_vdup("Element %d", 4), 0));
        flut_assert_is_true(flm_vector_insert(vector, char*, fl_cstring_vdup("Element %d", 3), 0));
        flut_assert_is_true(flm_vector_insert(vector, char*, fl_cstring_vdup("Element %d", 2), 0));
        flut_assert_is_true(flm_vector_insert(vector, char*, fl_cstring_vdup("Element %d", 1), 0));
        flut_assert_is_true(flm_vector_insert(vector, char*, fl_cstring_vdup("Element %d", 0), 0));
        flut_assert_size_t_is_equals(10, fl_vector_length(vector));
        flut_assert_string_is_equals("Element 0", flm_vector_get(vector, char*, 0), false);
        flut_assert_string_is_equals("Element 1", flm_vector_get(vector, char*, 1), false);
        flut_assert_string_is_equals("Element 2", flm_vector_get(vector, char*, 2), false);
        flut_assert_string_is_equals("Element 3", flm_vector_get(vector, char*, 3), false);
        flut_assert_string_is_equals("Element 4", flm_vector_get(vector, char*, 4), false);
        flut_assert_string_is_equals("Element 5", flm_vector_get(vector, char*, 5), false);
        flut_assert_string_is_equals("Element 6", flm_vector_get(vector, char*, 6), false);
        flut_assert_string_is_equals("Element 7", flm_vector_get(vector, char*, 7), false);
        flut_assert_string_is_equals("Element 8", flm_vector_get(vector, char*, 8), false);
        flut_assert_string_is_equals("Element 9", flm_vector_get(vector, char*, 9), false);
        fl_vector_free(vector);
    }

    flut_describe("Insert should not succeed if max. capacity has been reached") {
        FlVector *vector = flm_vector_new_with(.max_capacity = 5, .element_size = sizeof(size_t));
        flut_explain(flut_assert_is_true(flm_vector_insert(vector, size_t, 0, 4)), "An insert in the last position should fill the vector");
        flut_assert_size_t_is_equals(fl_vector_max_capacity(vector), fl_vector_length(vector));
        flut_assert_is_false(flm_vector_insert(vector, size_t, 0, 0));
        flut_assert_is_false(flm_vector_insert(vector, size_t, 0, 1));
        flut_assert_is_false(flm_vector_insert(vector, size_t, 0, 2));
        flut_assert_is_false(flm_vector_insert(vector, size_t, 0, 3));
        fl_vector_free(vector);
    }
}

flut_test(vector_first) {
    flut_describe("Vector should return the first element") {
        FlVector *vector = flm_vector_new_with(.max_capacity = 5, .element_size = sizeof(size_t));
        flut_assert_is_true(flm_vector_add(vector, size_t, 0));
        flut_assert_is_true(flm_vector_add(vector, size_t, 1));
        flut_assert_is_true(flm_vector_add(vector, size_t, 2));
        flut_assert_size_t_is_equals(3, fl_vector_length(vector));
        flut_assert_size_t_is_equals(0, flm_vector_first(vector, size_t));
        fl_vector_free(vector);
    }

    flut_describe("fl_vector_ref_first should return NULL if there are no elements") {
        FlVector *vector = fl_vector_new();
        flut_assert_is_null(fl_vector_ref_first(vector));
        fl_vector_free(vector);
    }
}

flut_test(vector_last) {
    flut_describe("Vector should return the last element") {
        FlVector *vector = flm_vector_new_with(.max_capacity = 5, .element_size = sizeof(size_t));
        flut_assert_is_true(flm_vector_add(vector, size_t, 0));
        flut_assert_is_true(flm_vector_add(vector, size_t, 1));
        flut_assert_is_true(flm_vector_add(vector, size_t, 2));
        flut_assert_size_t_is_equals(3, fl_vector_length(vector));
        flut_assert_size_t_is_equals(2, flm_vector_last(vector, size_t));
        fl_vector_free(vector);
    }

    flut_describe("fl_vector_ref_last should return NULL if there are no elements") {
        FlVector *vector = fl_vector_new();
        flut_assert_is_null(fl_vector_ref_last(vector));
        fl_vector_free(vector);
    }
}

flut_test(vector_contains) {
    flut_describe("fl_vector_contains should find primitive types") {
        FlVector *vector = flm_vector_new_with(.max_capacity = 5, .element_size = sizeof(size_t));

        for (size_t i=0; i < 5; i++) {
            flut_explainv(flut_assert_is_true(flm_vector_add(vector, size_t, i)), "Add element %zu", i);
        }

        flut_assert_is_true(flm_vector_contains(vector, size_t, 0));
        flut_assert_is_true(flm_vector_contains(vector, size_t, 1));
        flut_assert_is_true(flm_vector_contains(vector, size_t, 2));
        flut_assert_is_true(flm_vector_contains(vector, size_t, 3));
        flut_assert_is_true(flm_vector_contains(vector, size_t, 4));

        flut_assert_is_false(flm_vector_contains(vector, size_t, 5));
        flut_assert_is_false(flm_vector_contains(vector, size_t, 10));
        flut_assert_is_false(flm_vector_contains(vector, size_t, SIZE_MAX));
        
        fl_vector_free(vector);
    }

    flut_describe("fl_vector_contains should find pointers") {
        FlVector *vector = flm_vector_new_with(.max_capacity = 5, .element_size = sizeof(size_t*));
        size_t pointers[5][1] = { { 0 }, { 1 }, { 2 }, { 3 }, { 4 } };

        for (size_t i=0; i < 3; i++) {
            flut_explainv(flut_assert_is_true(flm_vector_add(vector, size_t*, pointers[i])), "Add pointer to element %zu", *pointers[i]);
        }

        flut_assert_is_true(flm_vector_contains(vector, size_t*, pointers[0]));
        flut_assert_is_true(flm_vector_contains(vector, size_t*, pointers[1]));
        flut_assert_is_true(flm_vector_contains(vector, size_t*, pointers[2]));

        flut_assert_is_false(flm_vector_contains(vector, size_t*, pointers[3]));
        flut_assert_is_false(flm_vector_contains(vector, size_t*, pointers[4]));
        flut_assert_is_false(flm_vector_contains(vector, size_t*, NULL));
        
        fl_vector_free(vector);
    }

    flut_describe("fl_vector_contains should return false if there are no elements") {
        FlVector *vector = fl_vector_new();
        flut_assert_is_false(flm_vector_contains(vector, void*, NULL));
        fl_vector_free(vector);
    }
}

flut_test(vector_shift) {
    flut_describe("Shift primitive types from vector") {
        FlVector *vector = flm_vector_new_with(.capacity = 10, .element_size = sizeof(int));
        flut_assert_is_true(flm_vector_add(vector, int, 0));
        flut_assert_is_true(flm_vector_add(vector, int, 1));
        flut_assert_is_true(flm_vector_add(vector, int, 2));
        flut_assert_is_true(flm_vector_add(vector, int, 3));
        flut_assert_size_t_is_equals(4, fl_vector_length(vector));
        flut_assert_is_true(fl_vector_shift(vector, NULL));
        flut_assert_is_true(fl_vector_shift(vector, NULL));
        flut_assert_is_true(fl_vector_shift(vector, NULL));
        flut_assert_is_true(fl_vector_shift(vector, NULL));
        flut_assert_size_t_is_equals(0, fl_vector_length(vector));
        flut_assert_is_false(fl_vector_shift(vector, NULL));
        fl_vector_free(vector);
    }
    flut_describe("Shift pointers from vector") {
        FlVector *vector = flm_vector_new_with(.element_size = sizeof(char*), .capacity = 10, .cleaner = fl_container_cleaner_pointer);
        flut_assert_is_true(flm_vector_add(vector, char*, fl_cstring_vdup("Element %d", 0)));
        flut_assert_is_true(flm_vector_add(vector, char*, fl_cstring_vdup("Element %d", 1)));
        flut_assert_is_true(flm_vector_add(vector, char*, fl_cstring_vdup("Element %d", 2)));
        flut_assert_is_true(flm_vector_add(vector, char*, fl_cstring_vdup("Element %d", 3)));
        flut_assert_size_t_is_equals(4, fl_vector_length(vector));
        flut_assert_is_true(fl_vector_shift(vector, NULL));
        flut_assert_is_true(fl_vector_shift(vector, NULL));
        flut_assert_is_true(fl_vector_shift(vector, NULL));
        flut_assert_is_true(fl_vector_shift(vector, NULL));
        flut_assert_size_t_is_equals(0, fl_vector_length(vector));
        flut_assert_is_false(fl_vector_shift(vector, NULL));
        fl_vector_free(vector);
    }
    flut_describe("Shift pointers from vector without cleaning its memory") {
        FlVector *vector = flm_vector_new_with(.element_size = sizeof(char*), .capacity = 10, .cleaner = fl_container_cleaner_pointer);
        flut_assert_is_true(flm_vector_add(vector, char*, fl_cstring_vdup("Element %d", 0)));
        flut_assert_is_true(flm_vector_add(vector, char*, fl_cstring_vdup("Element %d", 1)));
        flut_assert_is_true(flm_vector_add(vector, char*, fl_cstring_vdup("Element %d", 2)));
        flut_assert_is_true(flm_vector_add(vector, char*, fl_cstring_vdup("Element %d", 3)));
        flut_assert_size_t_is_equals(4, fl_vector_length(vector));
        char *element = NULL;
        flut_assert_is_true(fl_vector_shift(vector, &element));
        flut_assert_string_is_equals("Element 0", element, true);
        flut_assert_is_true(fl_vector_shift(vector, &element));
        flut_assert_string_is_equals("Element 1", element, true);
        flut_assert_is_true(fl_vector_shift(vector, &element));
        flut_assert_string_is_equals("Element 2", element, true);
        flut_assert_is_true(fl_vector_shift(vector, &element));
        flut_assert_string_is_equals("Element 3", element, true);
        flut_assert_size_t_is_equals(0, fl_vector_length(vector));
        fl_vector_free(vector);
    }
}

flut_test(vector_pop) {
    flut_describe("Pop primitive types from vector") {
        FlVector *vector = flm_vector_new_with(.capacity = 10, .element_size = sizeof(int));
        flut_assert_is_true(flm_vector_add(vector, int, 0));
        flut_assert_is_true(flm_vector_add(vector, int, 1));
        flut_assert_is_true(flm_vector_add(vector, int, 2));
        flut_assert_is_true(flm_vector_add(vector, int, 3));
        flut_assert_size_t_is_equals(4, fl_vector_length(vector));
        flut_assert_is_true(fl_vector_pop(vector, NULL));
        flut_assert_is_true(fl_vector_pop(vector, NULL));
        flut_assert_is_true(fl_vector_pop(vector, NULL));
        flut_assert_is_true(fl_vector_pop(vector, NULL));
        flut_assert_size_t_is_equals(0, fl_vector_length(vector));
        flut_assert_is_false(fl_vector_pop(vector, NULL));
        fl_vector_free(vector);
    }
    flut_describe("Pop pointers from vector") {
        FlVector *vector = flm_vector_new_with(.element_size = sizeof(char*), .capacity = 10, .cleaner = fl_container_cleaner_pointer);
        flut_assert_is_true(flm_vector_add(vector, char*, fl_cstring_vdup("Element %d", 0)));
        flut_assert_is_true(flm_vector_add(vector, char*, fl_cstring_vdup("Element %d", 1)));
        flut_assert_is_true(flm_vector_add(vector, char*, fl_cstring_vdup("Element %d", 2)));
        flut_assert_is_true(flm_vector_add(vector, char*, fl_cstring_vdup("Element %d", 3)));
        flut_assert_size_t_is_equals(4, fl_vector_length(vector));
        flut_assert_is_true(fl_vector_pop(vector, NULL));
        flut_assert_is_true(fl_vector_pop(vector, NULL));
        flut_assert_is_true(fl_vector_pop(vector, NULL));
        flut_assert_is_true(fl_vector_pop(vector, NULL));
        flut_assert_size_t_is_equals(0, fl_vector_length(vector));
        flut_assert_is_false(fl_vector_pop(vector, NULL));
        fl_vector_free(vector);
    }
    flut_describe("Pop pointers from vector without cleaning its memory") {
        FlVector *vector = flm_vector_new_with(.element_size = sizeof(char*), .capacity = 10, .cleaner = fl_container_cleaner_pointer);
        flut_assert_is_true(flm_vector_add(vector, char*, fl_cstring_vdup("Element %d", 0)));
        flut_assert_is_true(flm_vector_add(vector, char*, fl_cstring_vdup("Element %d", 1)));
        flut_assert_is_true(flm_vector_add(vector, char*, fl_cstring_vdup("Element %d", 2)));
        flut_assert_is_true(flm_vector_add(vector, char*, fl_cstring_vdup("Element %d", 3)));
        flut_assert_size_t_is_equals(4, fl_vector_length(vector));
        char *element = NULL;
        flut_assert_is_true(fl_vector_pop(vector, &element));
        flut_assert_string_is_equals("Element 3", element, true);
        flut_assert_is_true(fl_vector_pop(vector, &element));
        flut_assert_string_is_equals("Element 2", element, true);
        flut_assert_is_true(fl_vector_pop(vector, &element));
        flut_assert_string_is_equals("Element 1", element, true);
        flut_assert_is_true(fl_vector_pop(vector, &element));
        flut_assert_string_is_equals("Element 0", element, true);
        flut_assert_size_t_is_equals(0, fl_vector_length(vector));
        fl_vector_free(vector);
    }
}

flut_test(vector_get) {
    flut_describe("Get primitive types from vector") {
        FlVector *vector = flm_vector_new_with(.capacity = 10, .element_size = sizeof(int));
        flut_assert_is_true(flm_vector_add(vector, int, 0));
        flut_assert_is_true(flm_vector_add(vector, int, 1));
        flut_assert_is_true(flm_vector_add(vector, int, 2));
        flut_assert_is_true(flm_vector_add(vector, int, 3));
        flut_assert_size_t_is_equals(4, fl_vector_length(vector));
        flut_assert_int_is_equals(0, *(int*) fl_vector_ref_get(vector, 0));
        flut_assert_int_is_equals(1, *(int*) fl_vector_ref_get(vector, 1));
        flut_assert_int_is_equals(2, *(int*) fl_vector_ref_get(vector, 2));
        flut_assert_int_is_equals(3, *(int*) fl_vector_ref_get(vector, 3));
        flut_assert_is_null(fl_vector_ref_get(vector, 4));
        fl_vector_free(vector);
    }
    flut_describe("Get pointers from vector") {
        FlVector *vector = flm_vector_new_with(.element_size = sizeof(char*), .capacity = 10, .cleaner = fl_container_cleaner_pointer);
        flut_assert_is_true(flm_vector_add(vector, char*, fl_cstring_vdup("Element %d", 0)));
        flut_assert_is_true(flm_vector_add(vector, char*, fl_cstring_vdup("Element %d", 1)));
        flut_assert_is_true(flm_vector_add(vector, char*, fl_cstring_vdup("Element %d", 2)));
        flut_assert_is_true(flm_vector_add(vector, char*, fl_cstring_vdup("Element %d", 3)));
        flut_assert_size_t_is_equals(4, fl_vector_length(vector));
        flut_assert_string_is_equals("Element 0", *(char**) fl_vector_ref_get(vector, 0), false);
        flut_assert_string_is_equals("Element 1", *(char**) fl_vector_ref_get(vector, 1), false);
        flut_assert_string_is_equals("Element 2", *(char**) fl_vector_ref_get(vector, 2), false);
        flut_assert_string_is_equals("Element 3", *(char**) fl_vector_ref_get(vector, 3), false);
        flut_assert_is_null(fl_vector_ref_get(vector, 4));
        fl_vector_free(vector);
    }
}

flut_test(vector_value_writer) {
    flut_describe("A container writer should copy the inserted value instead of saving pointers") {
        size_t numbers[3] = { 0, 1, 2 };
        FlVector *vector = flm_vector_new_with(.capacity = 10, .writer = fl_container_writer);

        for (size_t i=0; i < flm_array_length(numbers); i++) {
            flm_vector_add(vector, size_t, numbers[i]);
        }

        (*(size_t*) fl_vector_ref_get(vector, 0))++;
        (*(size_t*) fl_vector_ref_get(vector, 1))++;
        (*(size_t*) fl_vector_ref_get(vector, 2))++;
        flut_explain(flut_assert_size_t_is_not_equals(numbers[0], flm_vector_get(vector, size_t, 0)), "Element 0 within the vector should have been incremented by 1");
        flut_explain(flut_assert_size_t_is_not_equals(numbers[1], flm_vector_get(vector, size_t, 1)), "Element 1 within the vector should have been incremented by 1");
        flut_explain(flut_assert_size_t_is_not_equals(numbers[2], flm_vector_get(vector, size_t, 2)), "Element 2 within the vector should have been incremented by 1");

        fl_vector_free(vector);
    }
}

flut_test(vector_resize) {
    flut_describe("On resize, the capacity must be round to the next higher integer after applying the growth factor") {
        FlVector *vector = flm_vector_new_with(.element_size = sizeof(int), .capacity = 10, .growth_factor = 1.5);

        flut_assert_is_true(flm_vector_insert(vector, int, 0, 16));
        flut_assert_size_t_is_equals(23, fl_vector_capacity(vector));

        flut_assert_is_true(flm_vector_insert(vector, int, 0, 26));
        flut_assert_size_t_is_equals(35, fl_vector_capacity(vector));

        fl_vector_free(vector);
    }

    flut_describe("Resize based on growth factor should not exceed the max. capacity") {
        // Check growth factor not exceeds max. capacity
        size_t capacity = 5;
        size_t max_capacity = 45;
        double growth_factor = 2.0;

        FlVector *vector = flm_vector_new_with(.capacity = capacity, .max_capacity = max_capacity, .growth_factor = growth_factor, .element_size = sizeof(size_t));

        flut_assert_size_t_is_equals(5, fl_vector_capacity(vector));
        flut_assert_size_t_is_equals(45, fl_vector_max_capacity(vector));
        flut_assert_double_is_equals(2.0, fl_vector_growth_factor(vector), 0.01);

        flut_assert_is_true(flm_vector_insert(vector, size_t, 0, 5));
        flut_assert_size_t_is_equals(10, fl_vector_capacity(vector));

        flut_assert_is_true(flm_vector_insert(vector, size_t, 0, 10));
        flut_assert_size_t_is_equals(20, fl_vector_capacity(vector));

        flut_assert_is_true(flm_vector_insert(vector, size_t, 0, 20));
        flut_assert_size_t_is_equals(40, fl_vector_capacity(vector));

        flut_assert_is_true(flm_vector_insert(vector, size_t, 0, 41));
        flut_assert_size_t_is_equals(45, fl_vector_capacity(vector));

        fl_vector_free(vector);
    }    
}

flut_test(vector_max_capacity) {
    flut_describe("Once the vector has reached its maximum capacity, inserts on positions greater than max_capacity-1 should not thrive") {
        FlVector *vector = flm_vector_new_with(.capacity = 5, .max_capacity = 5, .growth_factor = 1.5);

        flut_assert_is_true(flm_vector_insert(vector, void*, NULL, 0));
        flut_assert_is_true(flm_vector_insert(vector, void*, NULL, 4));
        flut_assert_is_false(flm_vector_insert(vector, void*, NULL, 10));
        flut_assert_is_false(flm_vector_insert(vector, void*, NULL, 1));

        fl_vector_free(vector);
    }
}

flut_test(vector_cleanup_fn) {
    FlVector *vector = flm_vector_new_with(.cleaner = fl_container_cleaner_pointer);
    flut_assert_is_true(fl_container_cleaner_pointer == fl_vector_cleanup_fn_get(vector));
    FlContainerCleanupFn old_fn = fl_vector_cleanup_fn_set(vector, (FlContainerCleanupFn) fl_free);
    flut_assert_is_true(fl_container_cleaner_pointer == old_fn);
    flut_assert_is_true((FlContainerCleanupFn) fl_free == fl_vector_cleanup_fn_get(vector));
    fl_vector_free(vector);
}
