#include <fllib.h>
#include <math.h>
#include "../Test.h"
#include "Test_Vector.h"

void test_fl_vector_new()
{
    fl_test_description("New vectors with valid parameters should return valid vectors")
    {
        FlVector *vector = fl_vector_new(10, NULL);
        fl_expect("Vector must have length == 0", fl_vector_length(vector) == 0);
        fl_expect("Vector must have capacity for 10 elements", fl_vector_capacity(vector) == 10);
        fl_vexpect(fl_vector_max_capacity(vector) == SIZE_MAX / sizeof(void*), "Vector must have a maximum capacity of %zu elements (default)", SIZE_MAX / sizeof(void*));
        fl_expect("Vector must have a growth factor of 2.0 (default)", fl_vector_growth_factor(vector) == 2.0);
        fl_vexpect(fl_vector_element_size(vector) == sizeof(void*), "Vector must have an element_size of %zu (default)", sizeof(void*));
        fl_vector_free(vector);

        vector = fl_vector_new(100, NULL);
        fl_expect("Vector must have length == 0", fl_vector_length(vector) == 0);
        fl_expect("Vector must have capacity for 100 elements", fl_vector_capacity(vector) == 100);
        fl_vexpect(fl_vector_max_capacity(vector) == SIZE_MAX / sizeof(void*), "Vector must have a maximum capacity of %zu elements (default)", SIZE_MAX / sizeof(void*));
        fl_expect("Vector must have a growth factor of 2.0 (default)", fl_vector_growth_factor(vector) == 2.0);
        fl_vexpect(fl_vector_element_size(vector) == sizeof(void*), "Vector must have an element_size of %zu (default)", sizeof(void*));
        fl_vector_free(vector);

        vector = fl_vector_new(0, NULL);
        fl_expect("Vector must have length == 0", fl_vector_length(vector) == 0);
        fl_expect("Vector must have capacity for 1 element", fl_vector_capacity(vector) == 1);
        fl_vexpect(fl_vector_max_capacity(vector) == SIZE_MAX / sizeof(void*), "Vector must have a maximum capacity of %zu elements (default)", SIZE_MAX / sizeof(void*));
        fl_expect("Vector must have a growth factor of 2.0 (default)", fl_vector_growth_factor(vector) == 2.0);
        fl_vexpect(fl_vector_element_size(vector) == sizeof(void*), "Vector must have an element_size of %zu (default)", sizeof(void*));
        fl_vector_free(vector);

        vector = fl_vector_new_args((struct FlVectorArgs) {
            .element_size = sizeof(int*),
            .capacity = 20,
            .max_capacity = 40,
            .growth_factor = 1.6
        });
        fl_expect("Vector must have length == 0", fl_vector_length(vector) == 0);
        fl_expect("Vector must have capacity for 20 element", fl_vector_capacity(vector) == 20);
        fl_expect("Vector must have a maximum capacity of 40 elements", fl_vector_max_capacity(vector) == 40);
        fl_expect("Vector must have a growth factor of 1.6", fl_vector_growth_factor(vector) == 1.6);
        fl_vexpect(fl_vector_element_size(vector) == sizeof(int*), "Vector must have an element_size of %zu", sizeof(int*));
        fl_vector_free(vector);

        vector = fl_vector_new_args((struct FlVectorArgs) {
            .element_size = sizeof(double),
            .capacity = 20,
            .growth_factor = 1.6
        });
        fl_expect("Vector must have length == 0", fl_vector_length(vector) == 0);
        fl_expect("Vector must have capacity for 20 element", fl_vector_capacity(vector) == 20);
        fl_vexpect(fl_vector_max_capacity(vector) == SIZE_MAX / sizeof(double), "Vector must have a maximum capacity of %zu elements", SIZE_MAX / sizeof(double));
        fl_expect("Vector must have a growth factor of 1.6", fl_vector_growth_factor(vector) == 1.6);
        fl_vexpect(fl_vector_element_size(vector) == sizeof(double), "Vector must have an element_size of %zu", sizeof(double));
        fl_vector_free(vector);
    }

    fl_test_description("New vectors with invalid configuration must return NULL")
    {
        FlVector *vector = fl_vector_new(SIZE_MAX, NULL);
        fl_expect("Vector initialization with capacity == SIZE_MAX returns NULL (default element_size is sizeof(void*) which results in overflow)", vector == NULL);

        vector = fl_vector_new_args((struct FlVectorArgs) {
            .capacity = 10,
            .max_capacity = 5
        });
        fl_expect("Vector initialization with a capacity greater than the maximum capacity returns NULL;", vector == NULL);

        vector = fl_vector_new_args((struct FlVectorArgs) {
            .element_size = SIZE_MAX,
            .max_capacity = 10
        });
        fl_expect("Vector initialization with element_size == SIZE_MAX and capacity greater than 1 returns NULL (results in overflow)", vector == NULL);
    }
}

void test_fl_vector_add()
{
    // Use pointers
    size_t numbers[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    FlVector *vector = fl_vector_new(10, NULL);

    for (size_t i=0; i < (size_t)(sizeof(numbers) / sizeof(numbers[0])); i++)
    {
        size_t *ptr = fl_vector_add(vector, numbers+i);
        fl_vexpect(*ptr == i+1, "Element at position %d must have value %d", i, i+1);
    }

    fl_expect("Vector must have length == 10", fl_vector_length(vector) == 10);

    fl_vector_free(vector);

    // Use a container writer
    vector = fl_vector_new_args((struct FlVectorArgs){
        .writer = fl_container_writer,
        .element_size = sizeof(size_t),
        .capacity = 10
    });

    for (size_t i=0; i < 10; i++)
    {
        size_t *ptr = fl_vector_add(vector, &i);
        fl_vexpect(*ptr == i, "Element at position %d must have value %d", i, i);
    }

    fl_expect("Vector must have length == 10", fl_vector_length(vector) == 10);

    fl_vector_free(vector);

    // Use malloc'd pointers
    vector = fl_vector_new_args((struct FlVectorArgs){
        .element_size = sizeof(char*),
        .cleaner = fl_container_cleaner_pointer,
        .capacity = 10
    });

    for (int i=0; i < 10; i++)
    {
        char *str = fl_cstring_vdup("Test%d", i);
        fl_vector_add(vector, str);
    }

    fl_expect("Vector must have length == 10", fl_vector_length(vector) == 10);

    for (int i=0; i < 10; i++)
    {
        char str[6];
        snprintf(str, 6, "Test%d", i);
        char *ptr = fl_vector_get(vector, i);
        fl_vexpect(flm_cstring_equals(ptr, str), "Element at position %d must have value %s", i, str);
    }

    fl_vector_free(vector);
}

void test_fl_vector_insert()
{
    // Use pointers
    size_t numbers[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    size_t array_length = sizeof(numbers) / sizeof(numbers[0]);

    fl_test_description("Insert on sequential positions must result in an ordedered vector")
    {
        FlVector *vector = fl_vector_new(10, NULL);
        
        for (size_t i=0; i < array_length; i++)
        {
            size_t *ptr = fl_vector_insert(vector, numbers+i, i);
            fl_vexpect(ptr != NULL, "Insert at position %zu must return a valid pointer", i);
        }

        fl_expect("Vector must have length == 10", fl_vector_length(vector) == 10);

        for (size_t i=0; i < array_length; i++)
        {
            size_t *ptr = fl_vector_get(vector, i);
            fl_vexpect(ptr != NULL && *ptr == numbers[i], "Inserted value at position %zu must be equals to numbers[%zu] (%zu)", i, i, numbers[i]);
        }

        fl_vector_free(vector);
    }

    fl_test_description("A container writer should copy the inserted value instead of saving pointers")
    {
        FlVector *vector = fl_vector_new_args((struct FlVectorArgs) {
            // Use a container writer
            .writer = fl_container_writer,
            .element_size = sizeof(size_t),
            .capacity = 10
        });

        size_t *lnumbers[10] = { 0 };
        for (size_t i=0; i < 10; i++)
        {
            lnumbers[i] = fl_vector_insert(vector, &i, i);
            
            fl_vexpect(lnumbers[i] != NULL, "Element at position %zu must return a valid pointer", i);

            *lnumbers[i] = *lnumbers[i] + 10;

            fl_vexpect(*lnumbers[i] != numbers[i], "Vector element at position %zu, modified to be equals to %zu should not affect original numbers array (%zu)", i, *lnumbers[i], numbers[i]);
        }

        fl_expect("Vector must have length == 10", fl_vector_length(vector) == 10);

        fl_vector_free(vector);
    }

    fl_test_description("On resize, the capacity must be round to the next higher integer after applying the growth factor")
    {
        FlVector *vector = fl_vector_new_args((struct FlVectorArgs) {
            .capacity = 10,
            .growth_factor = 1.5
        });

        fl_vector_insert(vector, numbers + 0, 16);

        fl_expect("Vector should have expanded its capacity to 20 elements", fl_vector_capacity(vector) == 23);

        fl_vector_insert(vector, numbers + 0, 26);

        fl_expect("Vector should have expanded its capacity to 40 elements", fl_vector_capacity(vector) == 35);

        fl_vector_free(vector);

        // Check growth factor not exceeds max. capacity
        size_t capacity = 5;
        size_t max_capacity = 10;
        double growth_factor = 1.2;

        vector = fl_vector_new_args((struct FlVectorArgs) {
            .capacity = capacity,
            .max_capacity = max_capacity,
            .growth_factor = growth_factor,
        });

        fl_vexpect(fl_vector_capacity(vector) == capacity, "Vector maximum capacity should be %zu", capacity);
        fl_vexpect(fl_vector_max_capacity(vector) == max_capacity, "Vector maximum capacity should be %zu", max_capacity);
        fl_vexpect(fl_vector_growth_factor(vector) == growth_factor, "Vector maximum capacity should be %f", growth_factor);

        size_t expected_capacity = ceil(capacity * growth_factor);
        int *n = fl_vector_insert(vector, numbers+0, 5);
        fl_expect("Insert must succeed at position 5", n != NULL);
        fl_vexpect(fl_vector_capacity(vector) == expected_capacity, "Vector capacity must be %zu", expected_capacity);

        capacity = expected_capacity;
        expected_capacity = ceil(capacity * growth_factor);
        n = fl_vector_insert(vector, numbers+0, 6);
        fl_expect("Insert must succeed at position 6", n != NULL);
        fl_vexpect(fl_vector_capacity(vector) == expected_capacity, "Vector capacity must be %zu", expected_capacity);

        capacity = expected_capacity;
        expected_capacity = ceil(capacity * growth_factor);
        n = fl_vector_insert(vector, numbers+0, 8);
        fl_expect("Insert must succeed at position 8", n != NULL);
        fl_vexpect(fl_vector_capacity(vector) == expected_capacity, "Vector capacity must be %zu", expected_capacity);


        n = fl_vector_insert(vector, numbers+0, 9);
        fl_expect("Insert must succeed at position 9", n != NULL);
        fl_vexpect(fl_vector_capacity(vector) == max_capacity, "Vector capacity must be %zu", max_capacity);

        fl_vector_free(vector);
    }

    fl_test_description("Once the vector has reached its maximum capacity, inserts on positions greater than max_capacity-1 should not thrive")
    {
        // Check insert failures if the max. capacity is reached
        FlVector *vector = fl_vector_new_args((struct FlVectorArgs) {
            .capacity = 1,
            .max_capacity = 5,
            .growth_factor = 1.5,
        });

        fl_expect("Vector maximum capacity should be 5", fl_vector_max_capacity(vector) == 5);

        for (size_t i=0; i < 10; i++)
        {
            size_t *ptr = fl_vector_insert(vector, &i, i);
            if (i < fl_vector_max_capacity(vector))
                fl_vexpect(*ptr == i, "Element at position %zu must have value %zu", i, i);
            else
                fl_vexpect(ptr == NULL, "Element at position %zu cannot be inserted as the max. capacity has been reached", i);
        }

        fl_vector_free(vector);
    }
}

void test_fl_vector_shift()
{
    // Use pointers
    int numbers[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int size = (int)(sizeof(numbers) / sizeof(numbers[0]));

    FlVector *vector = fl_vector_new(10, NULL);

    for (int i=0; i < size; i++)
        fl_vector_add(vector, numbers+i);

    fl_expect("Vector must have length == 10", fl_vector_length(vector) == 10);

    for (int i=0; i < size; i++)
    {
        int *number = NULL;
        fl_vector_shift(vector, &number);
        fl_vexpect(number && *number == numbers[i], "Shifted element must be equals to %d", numbers[i]);
    }

    fl_expect("Vector after shifting all the elements must be empty", fl_vector_length(vector) == 0);

    fl_vector_free(vector);

    // Use a container writer
    vector = fl_vector_new_args((struct FlVectorArgs){
        .writer = fl_container_writer,
        .element_size = sizeof(int),
        .capacity = 10
    });

    for (int i=0; i < 10; i++)
    {
        int *ptr = fl_vector_add(vector, &i);
        fl_vexpect(*ptr == i, "Element at position %d must have value %d", i, i);
    }

    fl_expect("Vector must have length == 10", fl_vector_length(vector) == 10);

    for (int i=0; i < 10; i++)
    {
        int number = -1;
        fl_vector_shift(vector, &number);
        fl_vexpect(number == i, "Shifted element must be equals to %d", i);
    }

    fl_expect("Vector after shifting all the elements must be empty", fl_vector_length(vector) == 0);

    fl_vector_free(vector);
}

void test_fl_vector_pop()
{
    // Use pointers
    int numbers[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int size = (int)(sizeof(numbers) / sizeof(numbers[0]));

    FlVector *vector = fl_vector_new(10, NULL);

    for (int i=0; i < size; i++)
        fl_vector_add(vector, numbers+i);

    fl_expect("Vector must have length == 10", fl_vector_length(vector) == 10);

    for (int i=0; i < size; i++)
    {
        int *number = NULL;
        fl_vector_pop(vector, &number);
        fl_vexpect(number && *number == numbers[size-1-i], "Shifted element must be equals to %d", numbers[size-1-i]);
    }

    fl_expect("Vector after popping all the elements must be empty", fl_vector_length(vector) == 0);

    fl_vector_free(vector);

    // Use a container writer
    vector = fl_vector_new_args((struct FlVectorArgs){
        .writer = fl_container_writer,
        .element_size = sizeof(int),
        .capacity = 10
    });

    for (int i=0; i < 10; i++)
    {
        int *ptr = fl_vector_add(vector, &i);
        fl_vexpect(*ptr == i, "Element at position %d must have value %d", i, i);
    }

    fl_expect("Vector must have length == 10", fl_vector_length(vector) == 10);

    for (int i=0; i < 10; i++)
    {
        int number = -1;
        fl_vector_pop(vector, &number);
        fl_vexpect(number == 9-i, "Shifted element must be equals to %d", 9-i);
    }

    fl_expect("Vector after popping all the elements must be empty", fl_vector_length(vector) == 0);

    fl_vector_free(vector);
}

void test_fl_vector_get()
{
    // TODO
}
