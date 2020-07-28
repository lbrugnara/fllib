#include <fllib.h>
#include <math.h>
#include <limits.h>
#include <time.h>

#include <flut/flut.h>

#include "Test_BinaryHeap.h"

void test_fl_binheap_new(void)
{
    flut_println("Calling fl_binheap_new(_args) function with valid parameters should return valid binary heaps")
    {
        FlBinaryHeap *heap = fl_binheap_new(fl_container_compare_pointer, NULL);
        flut_expect_compat("Heap must have length == 0", fl_binheap_length(heap) == 0);
        flut_expect_compat("Heap must have capacity for 15 elements", fl_binheap_capacity(heap) == 15);
        flut_vexpect_compat(fl_binheap_max_capacity(heap) == SIZE_MAX / sizeof(void*), "Heap must have a maximum capacity of %zu elements (default)", SIZE_MAX / sizeof(void*));
        flut_expect_compat("Heap must have a growth factor of 2.0 (default)", fl_binheap_growth_factor(heap) == 2.0);
        flut_vexpect_compat(fl_binheap_element_size(heap) == sizeof(void*), "Heap must have an element_size of %zu (default)", sizeof(void*));
        fl_binheap_free(heap);

        heap = fl_binheap_new_args((struct FlBinaryHeapArgs) {
            .element_size = sizeof(int*),
            .capacity = 20,
            .max_capacity = 40,
            .growth_factor = 1.6
        });
        flut_expect_compat("Heap must have length == 0", fl_binheap_length(heap) == 0);
        flut_expect_compat("Heap must have capacity for 20 element", fl_binheap_capacity(heap) == 20);
        flut_expect_compat("Heap must have a maximum capacity of 40 elements", fl_binheap_max_capacity(heap) == 40);
        flut_expect_compat("Heap must have a growth factor of 1.6", fl_binheap_growth_factor(heap) == 1.6);
        flut_vexpect_compat(fl_binheap_element_size(heap) == sizeof(int*), "Heap must have an element_size of %zu", sizeof(int*));
        fl_binheap_free(heap);

        heap = fl_binheap_new_args((struct FlBinaryHeapArgs) {
            .element_size = sizeof(double),
            .capacity = 20,
            .growth_factor = 1.6
        });
        flut_expect_compat("Heap must have length == 0", fl_binheap_length(heap) == 0);
        flut_expect_compat("Heap must have capacity for 20 element", fl_binheap_capacity(heap) == 20);
        flut_vexpect_compat(fl_binheap_max_capacity(heap) == SIZE_MAX / sizeof(double), "Heap must have a maximum capacity of %zu elements", SIZE_MAX / sizeof(double));
        flut_expect_compat("Heap must have a growth factor of 1.6", fl_binheap_growth_factor(heap) == 1.6);
        flut_vexpect_compat(fl_binheap_element_size(heap) == sizeof(double), "Heap must have an element_size of %zu", sizeof(double));
        fl_binheap_free(heap);
    }

    flut_println("Calling fl_binheap_new(_args) function with invalid configuration must return NULL")
    {
        FlBinaryHeap *heap = fl_binheap_new_args((struct FlBinaryHeapArgs) {
            .capacity = 10,
            .max_capacity = 5
        });
        flut_expect_compat("Heap initialization with a capacity greater than the maximum capacity returns NULL;", heap == NULL);

        heap = fl_binheap_new_args((struct FlBinaryHeapArgs) {
            .element_size = SIZE_MAX,
            .max_capacity = 10
        });
        flut_expect_compat("Heap initialization with element_size == SIZE_MAX and capacity greater than 1 returns NULL (results in overflow)", heap == NULL);
    }
}

void test_fl_binheap_insert(void)
{
    srand(time(NULL));

    flut_println("Root element on max. oriented heap must decrease after removal")
    {
        int expected_order_for_max[] = { 83, 74, 40, 35, 30, 27, 20, 15, 12, 10 };
        int numbers[] = { 10, 20, 30, 40, 15, 35, 83, 12, 74, 27 };
        size_t length = sizeof(numbers) / sizeof(numbers[0]);

        for (size_t i = 0; i < length - 1; i++) 
        {
            size_t j = i + rand() / (RAND_MAX / (length - i) + 1);
            int t = numbers[j];
            numbers[j] = numbers[i];
            numbers[i] = t;
        }

        FlBinaryHeap *maxheap = fl_binheap_new_args((struct FlBinaryHeapArgs) {
            .comparer = fl_container_compare_int,
            .writer = fl_container_writer,
            .cleaner =  NULL,
            .capacity = length,
            .max_capacity = length,
            .element_size = sizeof(int),
        });

        for (size_t i=0; i < length; i++)
        {
            flut_vexpect_compat(*(int*) fl_binheap_insert(maxheap, numbers + i) == numbers[i], "Returned pointer on insert must point to the same value in numbers[%zu]: %d", i, numbers[i]);
        }

        for (size_t i=0; i < length; i++)
        {
            flut_vexpect_compat(*(int*) fl_binheap_get_root(maxheap) == expected_order_for_max[i], "Root element must be equals to %d", expected_order_for_max[i]);
            flut_vexpect_compat(fl_binheap_remove_root(maxheap, false), "Removing current root element (%d) must return true", expected_order_for_max[i]);
        }

        fl_binheap_free(maxheap);
    }

    flut_println("Root element on min. oriented heap must increase after removal")
    {
        int expected_order_for_min[] = { 10, 12, 15, 20, 27, 30, 35, 40, 74, 83 };
        int numbers[] = { 10, 20, 30, 40, 15, 35, 83, 12, 74, 27 };
        size_t length = sizeof(numbers) / sizeof(numbers[0]);

        for (size_t i = 0; i < length - 1; i++) 
        {
            size_t j = i + rand() / (RAND_MAX / (length - i) + 1);
            int t = numbers[j];
            numbers[j] = numbers[i];
            numbers[i] = t;
        }

        FlBinaryHeap *minheap = fl_binheap_new_args((struct FlBinaryHeapArgs) {
            .comparer = fl_container_compare_int,
            .writer = fl_container_writer,
            .cleaner =  NULL,
            .capacity = length,
            .max_capacity = length,
            .element_size = sizeof(int),
            .type = FL_BINHEAP_MIN
        });

        for (size_t i=0; i < length; i++)
        {
            flut_vexpect_compat(*(int*) fl_binheap_insert(minheap, numbers + i) == numbers[i], "Returned pointer on insert must point to the same value in numbers[%zu]: %d", i, numbers[i]);
        }

        for (size_t i=0; i < length; i++)
        {
            flut_vexpect_compat(*(int*) fl_binheap_get_root(minheap) == expected_order_for_min[i], "Root element must be equals to %d", expected_order_for_min[i]);
            flut_vexpect_compat(fl_binheap_remove_root(minheap, false), "Removing current root element (%d) must return true", expected_order_for_min[i]);
        }

        fl_binheap_free(minheap);
    }

    flut_println("Max. oriented heap must play well with INT_MIN and INT_MAX")
    {
        int expected_order_for_max[] = { INT_MAX, 83, 74, 40, 35, 30, 27, 20, 15, 12, 10, INT_MIN };
        int numbers[] = { 10, 20, 30, INT_MAX, 40, 15, 35, INT_MIN, 83, 12, 74, 27 };
        size_t length = sizeof(numbers) / sizeof(numbers[0]);

        for (size_t i = 0; i < length - 1; i++) 
        {
            size_t j = i + rand() / (RAND_MAX / (length - i) + 1);
            int t = numbers[j];
            numbers[j] = numbers[i];
            numbers[i] = t;
        }

        FlBinaryHeap *maxheap = fl_binheap_new_args((struct FlBinaryHeapArgs) {
            .comparer = fl_container_compare_int,
            .writer = fl_container_writer,
            .cleaner =  NULL,
            .capacity = length,
            .max_capacity = length,
            .element_size = sizeof(int),
        });

        for (size_t i=0; i < length; i++)
        {
            flut_vexpect_compat(*(int*) fl_binheap_insert(maxheap, numbers + i) == numbers[i], "Returned pointer on insert must point to the same value in numbers[%zu]: %d", i, numbers[i]);
        }

        for (size_t i=0; i < length; i++)
        {
            flut_vexpect_compat(*(int*) fl_binheap_get_root(maxheap) == expected_order_for_max[i], "Root element must be equals to %d", expected_order_for_max[i]);
            flut_vexpect_compat(fl_binheap_remove_root(maxheap, false), "Removing current root element (%d) must return true", expected_order_for_max[i]);
        }

        fl_binheap_free(maxheap);
    }

    flut_println("Min. oriented heap must play well with INT_MIN and INT_MAX")
    {
        int expected_order_for_min[] = { INT_MIN, 10, 12, 15, 20, 27, 30, 35, 40, 74, 83, INT_MAX };
        int numbers[] = { 10, 20, 30, INT_MAX, 40, 15, 35, INT_MIN, 83, 12, 74, 27 };
        size_t length = sizeof(numbers) / sizeof(numbers[0]);

        for (size_t i = 0; i < length - 1; i++) 
        {
            size_t j = i + rand() / (RAND_MAX / (length - i) + 1);
            int t = numbers[j];
            numbers[j] = numbers[i];
            numbers[i] = t;
        }

        FlBinaryHeap *minheap = fl_binheap_new_args((struct FlBinaryHeapArgs) {
            .comparer = fl_container_compare_int,
            .writer = fl_container_writer,
            .cleaner =  NULL,
            .capacity = length,
            .max_capacity = length,
            .element_size = sizeof(int),
            .type = FL_BINHEAP_MIN
        });

        for (size_t i=0; i < length; i++)
        {
            flut_vexpect_compat(*(int*) fl_binheap_insert(minheap, numbers + i) == numbers[i], "Returned pointer on insert must point to the same value in numbers[%zu]: %d", i, numbers[i]);
        }

        for (size_t i=0; i < length; i++)
        {
            flut_vexpect_compat(*(int*) fl_binheap_get_root(minheap) == expected_order_for_min[i], "Root element must be equals to %d", expected_order_for_min[i]);
            flut_vexpect_compat(fl_binheap_remove_root(minheap, false), "Removing current root element (%d) must return true", expected_order_for_min[i]);
        }

        fl_binheap_free(minheap);
    }
}
