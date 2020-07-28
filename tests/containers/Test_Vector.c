#include <fllib.h>
#include <math.h>
#include <flut/flut.h>
#include "Test_Vector.h"

void test_fl_vector_new(FlutContext *ctx, FlutAssertUtils *assert)
{
    flut_describe(ctx, "Passing a list of designated initializers to flm_vector_new_with must return a valid vector (.capacity = 1)")
    {
        FlVector *vector = flm_vector_new_with(.capacity = 1);

        flut_expect(ctx, assert->not_null(vector));
        flut_expect(ctx, assert->size_t.equals(0, fl_vector_length(vector)));
        flut_expect(ctx, assert->size_t.equals(1, fl_vector_capacity(vector)));
        flut_expect(ctx, assert->size_t.equals(SIZE_MAX / sizeof(void*), fl_vector_max_capacity(vector)));
        flut_expect(ctx, assert->is_true(fl_vector_growth_factor(vector) == 2.0));
        flut_expect(ctx, assert->size_t.equals(sizeof(void*), fl_vector_element_size(vector)));

        fl_vector_free(vector);
    }

    flut_describe(ctx, "Passing a list of positional initializers to flm_vector_new_with must return a valid vector (fl_container_writer, fl_container_cleaner_pointer, 1.5, 16, 15, 30)")
    {
        FlVector *vector = flm_vector_new_with(fl_container_writer, fl_container_cleaner_pointer, 1.5, 16, 15, 30);

        flut_expect(ctx, assert->not_null(vector));
        flut_expect(ctx, assert->size_t.equals(0, fl_vector_length(vector)));
        flut_expect(ctx, assert->size_t.equals(15, fl_vector_capacity(vector)));
        flut_expect(ctx, assert->size_t.equals(30, fl_vector_max_capacity(vector)));
        flut_expect(ctx, assert->is_true(fl_vector_growth_factor(vector) == 1.5));
        flut_expect(ctx, assert->size_t.equals(16, fl_vector_element_size(vector)));

        fl_vector_free(vector);
    }

    flut_describe(ctx, "flm_vector_new_with(.capacity = 10) should create a valid vector")
    {
        FlVector *vector = flm_vector_new_with(.capacity = 10);

        flut_expect(ctx, assert->not_null(vector));
        flut_expect(ctx, assert->size_t.equals(0, fl_vector_length(vector)));
        flut_expect(ctx, assert->size_t.equals(10, fl_vector_capacity(vector)));
        flut_expect(ctx, assert->size_t.equals(SIZE_MAX / sizeof(void*), fl_vector_max_capacity(vector)));
        flut_expect(ctx, assert->is_true(fl_vector_growth_factor(vector) == 2.0));
        flut_expect(ctx, assert->size_t.equals(sizeof(void*), fl_vector_element_size(vector)));

        fl_vector_free(vector);
    }

    flut_describe(ctx, "flm_vector_new_with(.capacity = 100) should create a valid vector")
    {
        FlVector *vector = flm_vector_new_with(.capacity = 100);

        flut_expect(ctx, assert->not_null(vector));
        flut_expect(ctx, assert->size_t.equals(0, fl_vector_length(vector)));
        flut_expect(ctx, assert->size_t.equals(100, fl_vector_capacity(vector)));
        flut_expect(ctx, assert->size_t.equals(SIZE_MAX / sizeof(void*), fl_vector_max_capacity(vector)));
        flut_expect(ctx, assert->is_true(fl_vector_growth_factor(vector) == 2.0));
        flut_expect(ctx, assert->size_t.equals(sizeof(void*), fl_vector_element_size(vector)));

        fl_vector_free(vector);
    }

    flut_describe(ctx, "flm_vector_new_with(.capacity = 0) should create a valid vector")
    {
        FlVector *vector = flm_vector_new_with(.capacity = 0);

        flut_expect(ctx, assert->not_null(vector));
        flut_expect(ctx, assert->size_t.equals(0, fl_vector_length(vector)));
        flut_expect(ctx, assert->size_t.equals(1, fl_vector_capacity(vector)));
        flut_expect(ctx, assert->size_t.equals(SIZE_MAX / sizeof(void*), fl_vector_max_capacity(vector)));
        flut_expect(ctx, assert->is_true(fl_vector_growth_factor(vector) == 2.0));
        flut_expect(ctx, assert->size_t.equals(sizeof(void*), fl_vector_element_size(vector)));

        fl_vector_free(vector);
    }

    flut_describe(ctx, "flm_vector_new_with(.element_size = sizeof(int*), .capacity = 20, .max_capacity = 40, .growth_factor = 1.6) should create a valid vector")
    {
        FlVector *vector = flm_vector_new_with(.element_size = sizeof(int*), .capacity = 20, .max_capacity = 40, .growth_factor = 1.6);

        flut_expect(ctx, assert->not_null(vector));
        flut_expect(ctx, assert->size_t.equals(0, fl_vector_length(vector)));
        flut_expect(ctx, assert->size_t.equals(20, fl_vector_capacity(vector)));
        flut_expect(ctx, assert->size_t.equals(40, fl_vector_max_capacity(vector)));
        flut_expect(ctx, assert->is_true(fl_vector_growth_factor(vector) == 1.6));
        flut_expect(ctx, assert->size_t.equals(sizeof(int*), fl_vector_element_size(vector)));

        fl_vector_free(vector);
    }

    flut_describe(ctx, "flm_vector_new_with(.element_size = sizeof(double), .capacity = 20, .growth_factor = 1.6) should create a valid vector")
    {
        FlVector *vector = flm_vector_new_with(.element_size = sizeof(double), .capacity = 20, .growth_factor = 1.6);

        flut_expect(ctx, assert->not_null(vector));
        flut_expect(ctx, assert->size_t.equals(0, fl_vector_length(vector)));
        flut_expect(ctx, assert->size_t.equals(20, fl_vector_capacity(vector)));
        flut_expect(ctx, assert->size_t.equals(SIZE_MAX / sizeof(double), fl_vector_max_capacity(vector)));
        flut_expect(ctx, assert->is_true(fl_vector_growth_factor(vector) == 1.6));
        flut_expect(ctx, assert->size_t.equals(sizeof(double), fl_vector_element_size(vector)));

        fl_vector_free(vector);
    }

    flut_describe(ctx, "flm_vector_new_with(.capacity = SIZE_MAX, .element_size = sizeof(void*)) must return NULL (cannot be allocated)")
    {
        FlVector *vector = flm_vector_new_with(.capacity = SIZE_MAX, .element_size = sizeof(void*));
        flut_expect(ctx, assert->null(vector));
    }

    flut_describe(ctx, "flm_vector_new_with(.element_size = SIZE_MAX, .max_capacity = 10) must return NULL (cannot be allocated")
    {
        FlVector *vector = flm_vector_new_with(.element_size = SIZE_MAX, .max_capacity = 10);
        flut_expect(ctx, assert->null(vector));
    }

    flut_describe(ctx, "flm_vector_new_with(.capacity = 10, .max_capacity = 5) must return NULL")
    {
        FlVector *vector = flm_vector_new_with(.capacity = 10, .max_capacity = 5);
        flut_expect(ctx, assert->null(vector));
    }
}

void test_fl_vector_add()
{
    /*// Use pointers
    size_t numbers[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    FlVector *vector = flm_vector_new_with(.capacity = 10);

    for (size_t i=0; i < (size_t)(sizeof(numbers) / sizeof(numbers[0])); i++)
    {
        flut_vexpect_compat(fl_vector_add(vector, numbers+i), "Add (at position %zu) must success", i);
    }

    flut_expect_compat("Vector must have length == 10", fl_vector_length(vector) == 10);

    fl_vector_free(vector);*/

    FlVector *vector = flm_vector_new_with(.element_size = sizeof(size_t), .capacity = 10);

    for (size_t i=0; i < 10; i++)
    {
        flut_vexpect_compat(fl_vector_add(vector, &i), "Add (at position %zu) must success", i);
    }

    flut_expect_compat("Vector must have length == 10", fl_vector_length(vector) == 10);

    fl_vector_free(vector);

    // Use malloc'd pointers
    vector = flm_vector_new_with(.element_size = sizeof(char*), .cleaner = fl_container_cleaner_pointer, .capacity = 10);

    for (int i=0; i < 10; i++)
    {
        char *str = fl_cstring_vdup("Test%d", i);
        fl_vector_add(vector, &str);
    }

    flut_expect_compat("Vector must have length == 10", fl_vector_length(vector) == 10);

    for (int i=0; i < 10; i++)
    {
        char str[7];
        snprintf(str, 7, "Test%d", i);
        char **ptr = fl_vector_ref_get(vector, i);
        flut_vexpect_compat(flm_cstring_equals(*ptr, str), "Element at position %d must have value %s", i, str);
    }

    fl_vector_free(vector);
}

void test_fl_vector_insert()
{
    // Use pointers
    size_t numbers[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    size_t array_length = sizeof(numbers) / sizeof(numbers[0]);

    flut_println("Insert on sequential positions must result in an ordedered vector")
    {
        FlVector *vector = flm_vector_new_with(.capacity = 10, .element_size = sizeof(size_t));
        
        for (size_t i=0; i < array_length; i++)
        {
            flut_vexpect_compat(fl_vector_insert(vector, numbers + i, i), "Insert at position %zu must success", i);
        }

        flut_expect_compat("Vector must have length == 10", fl_vector_length(vector) == 10);

        for (size_t i=0; i < array_length; i++)
        {
            size_t *ptr = fl_vector_ref_get(vector, i);
            flut_vexpect_compat(ptr != NULL && *ptr == numbers[i], "Inserted value at position %zu must be equals to numbers[%zu] (%zu)", i, i, numbers[i]);
        }

        fl_vector_free(vector);
    }

    flut_println("A container writer should copy the inserted value instead of saving pointers")
    {
        FlVector *vector = flm_vector_new_with(.element_size = sizeof(size_t), .capacity = 10);

        size_t *vector_numbers[10] = { 0 };
        for (size_t i=0; i < 10; i++)
        {
            fl_vector_insert(vector, &i, i);
            vector_numbers[i] = fl_vector_ref_get(vector, i);
            
            flut_vexpect_compat(vector_numbers[i] != NULL, "Element at position %zu must return a valid pointer", i);

            *vector_numbers[i] += 10;

            flut_vexpect_compat(*vector_numbers[i] != numbers[i], 
                "Vector element at position %zu, modified to be equals to %zu should not affect original numbers array (%zu)", 
                i, *vector_numbers[i], numbers[i]);
        }

        flut_expect_compat("Vector must have length == 10", fl_vector_length(vector) == 10);

        fl_vector_free(vector);
    }

    flut_println("On resize, the capacity must be round to the next higher integer after applying the growth factor")
    {
        FlVector *vector = flm_vector_new_with(.capacity = 10, .growth_factor = 1.5);

        fl_vector_insert(vector, numbers + 0, 16);

        flut_expect_compat("Vector should have expanded its capacity to 20 elements", fl_vector_capacity(vector) == 23);

        fl_vector_insert(vector, numbers + 0, 26);

        flut_expect_compat("Vector should have expanded its capacity to 40 elements", fl_vector_capacity(vector) == 35);

        fl_vector_free(vector);

        // Check growth factor not exceeds max. capacity
        size_t capacity = 5;
        size_t max_capacity = 10;
        double growth_factor = 1.2;

        vector = flm_vector_new_with(.capacity = capacity, .max_capacity = max_capacity, .growth_factor = growth_factor);

        flut_vexpect_compat(fl_vector_capacity(vector) == capacity, "Vector maximum capacity should be %zu", capacity);
        flut_vexpect_compat(fl_vector_max_capacity(vector) == max_capacity, "Vector maximum capacity should be %zu", max_capacity);
        flut_vexpect_compat(fl_vector_growth_factor(vector) == growth_factor, "Vector maximum capacity should be %f", growth_factor);

        size_t expected_capacity = ceil(capacity * growth_factor);
        flut_expect_compat("Insert must succeed at position 5", fl_vector_insert(vector, numbers+0, 5));
        flut_vexpect_compat(fl_vector_capacity(vector) == expected_capacity, "Vector capacity must be %zu", expected_capacity);

        capacity = expected_capacity;
        expected_capacity = ceil(capacity * growth_factor);
        flut_expect_compat("Insert must succeed at position 6", fl_vector_insert(vector, numbers+0, 6));
        flut_vexpect_compat(fl_vector_capacity(vector) == expected_capacity, "Vector capacity must be %zu", expected_capacity);

        capacity = expected_capacity;
        expected_capacity = ceil(capacity * growth_factor);
        flut_expect_compat("Insert must succeed at position 8", fl_vector_insert(vector, numbers+0, 8));
        flut_vexpect_compat(fl_vector_capacity(vector) == expected_capacity, "Vector capacity must be %zu", expected_capacity);

        flut_expect_compat("Insert must succeed at position 9", fl_vector_insert(vector, numbers+0, 9));
        flut_vexpect_compat(fl_vector_capacity(vector) == max_capacity, "Vector capacity must be %zu", max_capacity);

        fl_vector_free(vector);
    }

    flut_println("Once the vector has reached its maximum capacity, inserts on positions greater than max_capacity-1 should not thrive")
    {
        // Check insert failures if the max. capacity is reached
        FlVector *vector = flm_vector_new_with(.capacity = 1, .max_capacity = 5, .growth_factor = 1.5);

        flut_expect_compat("Vector maximum capacity should be 5", fl_vector_max_capacity(vector) == 5);

        for (size_t i=0; i < 10; i++)
        {
            bool result = fl_vector_insert(vector, &i, i);
            if (i < fl_vector_max_capacity(vector))
                flut_vexpect_compat(result, "Element at position %zu must be correctly inserted", i, i);
            else
                flut_vexpect_compat(!result, "Element at position %zu cannot be inserted as the max. capacity has been reached", i);
        }

        fl_vector_free(vector);
    }
}

void test_fl_vector_shift()
{
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

void test_fl_vector_pop()
{
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

void test_fl_vector_get()
{
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
