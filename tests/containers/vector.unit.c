#include <fllib.h>
#include <math.h>
#include <flut/flut.h>

flut_define_suite(vector) {
    flut_suite_description("Vector functions");
    flut_suite_register_test(vector_new, "fl_vector_new function");
    flut_suite_register_test(vector_add, "fl_vector_add function");
    flut_suite_register_test(vector_insert, "fl_vector_insert function");
    flut_suite_register_test(vector_shift, "fl_vector_shift function");
    flut_suite_register_test(vector_pop, "fl_vector_pop function");
    flut_suite_register_test(vector_get, "fl_vector_get function");
    flut_suite_register_test(vector_value_writer, "fl_container_write function in vectors");
    flut_suite_register_test(vector_resize, "Vector resize operation");
    flut_suite_register_test(vector_max_capacity, "Vector max. capacity");
}

flut_define_test(vector_new) {
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

flut_define_test(vector_add) {
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
}

flut_define_test(vector_insert) {
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

    flut_describe("Insert of primitive types in position 0 must result in an ordedered vector") {
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
        flut_assert_is_true(flm_vector_insert(vector, size_t, 4, 0));
        flut_assert_is_true(flm_vector_insert(vector, size_t, 3, 0));
        flut_assert_is_true(flm_vector_insert(vector, size_t, 2, 0));
        flut_assert_is_true(flm_vector_insert(vector, size_t, 1, 0));
        flut_assert_is_true(flm_vector_insert(vector, size_t, 0, 0));
        flut_assert_size_t_is_equals(5, fl_vector_length(vector));
        flut_assert_is_false(flm_vector_insert(vector, size_t, 0, 0));
        flut_assert_is_false(flm_vector_insert(vector, size_t, 0, 1));
        flut_assert_is_false(flm_vector_insert(vector, size_t, 0, 2));
        flut_assert_is_false(flm_vector_insert(vector, size_t, 0, 3));
        fl_vector_free(vector);
    }
}

flut_define_test(vector_shift) {
    // Use pointers
    int *numbers[10] = { 0 };
    size_t size = 10;

    for (size_t i=0; i < size; i++)
    {
        numbers[i] = fl_malloc(sizeof(int));
        *(numbers[i]) = i;
    }

    FlVector *vector = flm_vector_new_with(.capacity = 10);

    for (size_t i=0; i < size; i++)
    {
        // Store a pointer
        fl_vector_add(vector, numbers + i);
    }

    flut_expect_compat("Vector must have length == 10", fl_vector_length(vector) == 10);

    for (size_t i=0; i < size; i++)
    {
        int *number = NULL;
        fl_vector_shift(vector, &number);
        flut_vexpect_compat(number && *number == *(numbers[i]), "Shifted element must be equals to %d", *(numbers[i]));
    }

    flut_expect_compat("Vector after shifting all the elements must be empty", fl_vector_length(vector) == 0);

    for (size_t i=0; i < size; i++)
        fl_free(numbers[i]);

    fl_vector_free(vector);

    vector = flm_vector_new_with(.element_size = sizeof(int), .capacity = 10);

    for (int i=0; i < 10; i++)
    {
        flut_vexpect_compat(fl_vector_add(vector, &i), "Element at position %d must be added successfully", i);
    }

    flut_expect_compat("Vector must have length == 10", fl_vector_length(vector) == 10);

    for (int i=0; i < 10; i++)
    {
        int number = -1;
        fl_vector_shift(vector, &number);
        flut_vexpect_compat(number == i, "Shifted element must be equals to %d", i);
    }

    flut_expect_compat("Vector after shifting all the elements must be empty", fl_vector_length(vector) == 0);

    fl_vector_free(vector);
}

flut_define_test(vector_pop) {
    // Use pointers
    int *numbers[10] = { 0 };
    size_t size = 10;

    for (size_t i=0; i < size; i++)
    {
        numbers[i] = fl_malloc(sizeof(int));
        *(numbers[i]) = i;
    }

    FlVector *vector = flm_vector_new_with(.capacity = 10);

    for (size_t i=0; i < size; i++)
        fl_vector_add(vector, numbers+i);

    flut_expect_compat("Vector must have length == 10", fl_vector_length(vector) == 10);

    for (size_t i=0; i < size; i++)
    {
        int *number = NULL;
        fl_vector_pop(vector, &number);
        flut_vexpect_compat(number && *number == *(numbers[size-1-i]), "Shifted element must be equals to %d", *(numbers[size-1-i]));
    }

    flut_expect_compat("Vector after popping all the elements must be empty", fl_vector_length(vector) == 0);

    for (size_t i=0; i < size; i++)
        fl_free(numbers[i]);

    fl_vector_free(vector);

    vector = flm_vector_new_with(.element_size = sizeof(int), .capacity = 10);

    for (int i=0; i < 10; i++)
    {
        flut_vexpect_compat(fl_vector_add(vector, &i), "Add (at position %zu) must success", i);
    }

    flut_expect_compat("Vector must have length == 10", fl_vector_length(vector) == 10);

    for (int i=0; i < 10; i++)
    {
        int number = -1;
        fl_vector_pop(vector, &number);
        flut_vexpect_compat(number == 9-i, "Shifted element must be equals to %d", 9-i);
    }

    flut_expect_compat("Vector after popping all the elements must be empty", fl_vector_length(vector) == 0);

    fl_vector_free(vector);
}

flut_define_test(vector_get) {
    flut_println("fl_vector_ref_get should return \"pointers to\" int for a vector that stores integers") {
        FlVector *vector_of_ints = flm_vector_new_with(.element_size = sizeof(int));

        int x = 10, y = 20;

        fl_vector_add(vector_of_ints, &x);              // Accepts "pointer to" integer
        fl_vector_insert(vector_of_ints, &y, 0);        // Accepts "pointer to" integer

        int *py = fl_vector_ref_get(vector_of_ints, 0);     // Returns "pointer to" integer
        int *px = fl_vector_ref_get(vector_of_ints, 1);     // Returns "pointer to" integer

        flut_vexpect_compat(*py == y, "Element at index 0 must be equals to variable y (%d)", y);
        flut_vexpect_compat(*px == x, "Element at index 1 must be equals to variable x (%d)", x);

        fl_vector_free(vector_of_ints);
    }

    flut_println("fl_vector_ref_get should return \"pointers to\" pointers to int for a vector that stores pointers to integer") {
        FlVector *vector_of_int_ptrs = flm_vector_new_with(.capacity = 2);

        int x = 10, y = 20;
        int *px = &x, *py = &y;

        fl_vector_add(vector_of_int_ptrs, &px);             // Accepts "pointer to" pointer to integer
        fl_vector_insert(vector_of_int_ptrs, &py, 0);       // Accepts "pointer to" pointer to integer

        int **ppy = fl_vector_ref_get(vector_of_int_ptrs, 0);   // Returns "pointer to" pointer to integer
        int **ppx = fl_vector_ref_get(vector_of_int_ptrs, 1);   // Returns "pointer to" pointer to integer

        flut_vexpect_compat(*ppy == py, "Element at index 0 must be equals to pointer py (0x%x)", py);
        flut_vexpect_compat(**ppy == y, "Dereferencing element at index 0 must be equals to variable y (%d)", y);
        flut_vexpect_compat(*ppx == px, "Element at index 1 must be equals to pointer px (0x%x)", px);
        flut_vexpect_compat(**ppx == x, "Dereferencing element at index 1 must be equals to variable x (%d)", x);

        fl_vector_free(vector_of_int_ptrs);
    }
}

flut_define_test(vector_value_writer) {
    flut_describe("A container writer should copy the inserted value instead of saving pointers") {
        size_t numbers[3] = { 0, 1, 2 };
        FlVector *vector = flm_vector_new_with(.capacity = 10, .writer = fl_container_writer);

        for (size_t i=0; i < flm_array_length(numbers); i++) {
            flm_vector_add(vector, size_t, numbers[i]);
        }

        size_t *ptr0 = (size_t*) fl_vector_ref_get(vector, 0);
        *ptr0 += 10;
        size_t *ptr1 = (size_t*) fl_vector_ref_get(vector, 1);
        *ptr1 += 10;
        size_t *ptr2 = (size_t*) fl_vector_ref_get(vector, 2);
        *ptr2 += 10;
        flut_assert_size_t_is_not_equals(numbers[0], flm_vector_get(vector, size_t, 0));
        flut_assert_size_t_is_not_equals(numbers[1], flm_vector_get(vector, size_t, 1));
        flut_assert_size_t_is_not_equals(numbers[2], flm_vector_get(vector, size_t, 2));

        fl_vector_free(vector);
    }
}

flut_define_test(vector_resize) {
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

        size_t current_capacity = fl_vector_capacity(vector);
        flut_assert_is_true(flm_vector_insert(vector, size_t, 0, 5));
        flut_assert_size_t_is_equals(10, fl_vector_capacity(vector));

        current_capacity = fl_vector_capacity(vector);
        flut_assert_is_true(flm_vector_insert(vector, size_t, 0, 10));
        flut_assert_size_t_is_equals(20, fl_vector_capacity(vector));

        current_capacity = fl_vector_capacity(vector);
        flut_assert_is_true(flm_vector_insert(vector, size_t, 0, 20));
        flut_assert_size_t_is_equals(40, fl_vector_capacity(vector));

        flut_assert_is_true(flm_vector_insert(vector, size_t, 0, 41));
        flut_assert_size_t_is_equals(45, fl_vector_capacity(vector));

        fl_vector_free(vector);
    }    
}

flut_define_test(vector_max_capacity) {
    flut_describe("Once the vector has reached its maximum capacity, inserts on positions greater than max_capacity-1 should not thrive") {
        FlVector *vector = flm_vector_new_with(.capacity = 5, .max_capacity = 5, .growth_factor = 1.5);

        flut_assert_is_true(flm_vector_insert(vector, void*, NULL, 0));
        flut_assert_is_true(flm_vector_insert(vector, void*, NULL, 4));
        flut_assert_is_false(flm_vector_insert(vector, void*, NULL, 10));

        fl_vector_free(vector);
    }
}
