#include <fllib.h>
#include <math.h>
#include <limits.h>
#include <time.h>

#include <flut/flut.h>

flut_define_suite(binheap) {
    flut_suite_description("Binary heap functions");
    flut_suite_register_test(binheap_new, "fl_binheap_new function");
    flut_suite_register_test(binheap_insert, "fl_binheap_insert function");
}

flut_define_test(binheap_new) {
    flut_describe("Function fl_binheap_new should return a valid binary heap object") {
        FlBinaryHeap *heap = NULL;
        flut_assert_is_not_null((heap = fl_binheap_new(fl_container_compare_pointer, NULL)));
        flut_assert_size_t_is_equals(0, fl_binheap_length(heap));
        flut_assert_size_t_is_equals(15, fl_binheap_capacity(heap));
        flut_assert_size_t_is_equals(SIZE_MAX / sizeof(void*), fl_binheap_max_capacity(heap));
        flut_assert_double_is_equals(2.0, fl_binheap_growth_factor(heap), 0.01);
        flut_assert_size_t_is_equals(sizeof(void*), fl_binheap_element_size(heap));
        fl_binheap_free(heap);
    }

    flut_describe("Function fl_binheap_new_args should return a valid binary heap object") {
        FlBinaryHeap *heap = NULL;
        flut_assert_is_not_null((heap = fl_binheap_new_args((struct FlBinaryHeapArgs) {
            .element_size = sizeof(int*),
            .capacity = 20,
            .max_capacity = 40,
            .growth_factor = 1.6
        })));
        flut_assert_size_t_is_equals(0, fl_binheap_length(heap));
        flut_assert_size_t_is_equals(20, fl_binheap_capacity(heap));
        flut_assert_size_t_is_equals(40, fl_binheap_max_capacity(heap));
        flut_assert_double_is_equals(1.6, fl_binheap_growth_factor(heap), 0.01);
        flut_assert_size_t_is_equals(sizeof(int*), fl_binheap_element_size(heap));
        fl_binheap_free(heap);
    }

    flut_describe("Function fl_binheap_new_args should return a valid binary heap object") {
        FlBinaryHeap *heap = NULL;
        flut_assert_is_not_null((heap = fl_binheap_new_args((struct FlBinaryHeapArgs) {
            .element_size = sizeof(double),
            .capacity = 20,
            .growth_factor = 1.6
        })));
        flut_assert_size_t_is_equals(0, fl_binheap_length(heap));
        flut_assert_size_t_is_equals(20, fl_binheap_capacity(heap));
        flut_assert_size_t_is_equals(SIZE_MAX / sizeof(double), fl_binheap_max_capacity(heap));
        flut_assert_double_is_equals(1.6, fl_binheap_growth_factor(heap), 0.01);
        flut_assert_size_t_is_equals(sizeof(double), fl_binheap_element_size(heap));
        fl_binheap_free(heap);
    }

    flut_describe("Calling fl_binheap_new_args function with invalid configuration must return NULL") {
        flut_explain(flut_assert_is_null(fl_binheap_new_args((struct FlBinaryHeapArgs) {
            .capacity = 10,
            .max_capacity = 5
        })), "Binary heap initialization with a capacity greater than the maximum capacity returns NULL;");

        flut_explain(flut_assert_is_null(fl_binheap_new_args((struct FlBinaryHeapArgs) {
            .element_size = SIZE_MAX,
            .max_capacity = 10
        })), "Heap initialization with element_size == SIZE_MAX and capacity greater than 1 returns NULL (results in overflow)");
    }
}

static void binheap_insert_and_remove(FLUT_TEST_CONTEXT_PARAM, FlBinaryHeap *binary_heap, size_t length, int *numbers, int *expected_order) {
    // Randomize input
    for (size_t i = 0; i < length - 1; i++) 
    {
        size_t j = i + rand() / (RAND_MAX / (length - i) + 1);
        int t = numbers[j];
        numbers[j] = numbers[i];
        numbers[i] = t;
    }

    char *numbers_array_str = fl_cstring_new(0);
    for (size_t i=0; i < length; i++) {
        if (fl_binheap_insert(binary_heap, numbers + i) == NULL) {
            flut_unexpectedv("Could not insert element %zu in the binary heap", numbers[i]);
        }

        if (i != length - 1) {
            fl_cstring_vappend(&numbers_array_str, "%d, ", numbers[i]);
        } else {
            fl_cstring_vappend(&numbers_array_str, "%d", numbers[i]);
        }
    }
    flut_passv("Randomized elements were inserted in the binary heap: %s", numbers_array_str);
    fl_cstring_free(numbers_array_str);

    for (size_t i=0; i < length; i++) {
        int *current = fl_binheap_get_root(binary_heap);
        if (current == NULL) {
            flut_unexpectedv("Root element must be equals to %d, actually the pointer to it is NULL", expected_order[i]);
        }

        if (*current != expected_order[i]) {
            flut_unexpectedv("Root element must be equals to %d, actually %d", expected_order[i], *current);
        }

        if (!fl_binheap_remove_root(binary_heap, false)) {
            flut_unexpectedv("Removing current root element (%d) must return true", expected_order[i]);
        }
    }
}

flut_define_test(binheap_insert) {
    srand(time(NULL));

    flut_describe("Root element on max. oriented heap must decrease after removal") {
        int expected_order_for_max[] = { 83, 74, 40, 35, 30, 27, 20, 15, 12, 10 };
        int numbers[] = { 10, 20, 30, 40, 15, 35, 83, 12, 74, 27 };
        size_t length = sizeof(numbers) / sizeof(numbers[0]);

        FlBinaryHeap *max_oriented_heap = NULL;
        
        flut_assert_is_not_null((max_oriented_heap = fl_binheap_new_args((struct FlBinaryHeapArgs) {
            .comparer = fl_container_compare_int,
            .writer = fl_container_writer,
            .cleaner =  NULL,
            .capacity = length,
            .max_capacity = length,
            .element_size = sizeof(int),
            .type = FL_BINHEAP_MAX // Default
        })));

        binheap_insert_and_remove(FLUT_TEST_CONTEXT, max_oriented_heap, length, numbers, expected_order_for_max);

        flut_pass("Max. oriented binary heap correctly managed the order of the elements");

        fl_binheap_free(max_oriented_heap);
    }

    flut_describe("Root element on min. oriented heap must increase after removal") {
        int expected_order_for_min[] = { 10, 12, 15, 20, 27, 30, 35, 40, 74, 83 };
        int numbers[] = { 10, 20, 30, 40, 15, 35, 83, 12, 74, 27 };
        size_t length = sizeof(numbers) / sizeof(numbers[0]);

        FlBinaryHeap *min_oriented_heap = NULL;
        
        flut_assert_is_not_null((min_oriented_heap = fl_binheap_new_args((struct FlBinaryHeapArgs) {
            .comparer = fl_container_compare_int,
            .writer = fl_container_writer,
            .cleaner =  NULL,
            .capacity = length,
            .max_capacity = length,
            .element_size = sizeof(int),
            .type = FL_BINHEAP_MIN
        })));

        binheap_insert_and_remove(FLUT_TEST_CONTEXT, min_oriented_heap, length, numbers, expected_order_for_min);

        flut_pass("Min. oriented binary heap correctly managed the order of the elements");

        fl_binheap_free(min_oriented_heap);
    }

    flut_describe("Max. oriented heap must play well with INT_MIN and INT_MAX") {
        int expected_order_for_max[] = { INT_MAX, 83, 74, 40, 35, 30, 27, 20, 15, 12, 10, INT_MIN };
        int numbers[] = { 10, 20, 30, INT_MAX, 40, 15, 35, INT_MIN, 83, 12, 74, 27 };
        size_t length = sizeof(numbers) / sizeof(numbers[0]);

        FlBinaryHeap *max_oriented_heap = fl_binheap_new_args((struct FlBinaryHeapArgs) {
            .comparer = fl_container_compare_int,
            .writer = fl_container_writer,
            .cleaner =  NULL,
            .capacity = length,
            .max_capacity = length,
            .element_size = sizeof(int),
        });

        binheap_insert_and_remove(FLUT_TEST_CONTEXT, max_oriented_heap, length, numbers, expected_order_for_max);

        flut_pass("Max. oriented binary heap correctly managed the order of the elements");

        fl_binheap_free(max_oriented_heap);
    }

    flut_describe("Min. oriented heap must play well with INT_MIN and INT_MAX") {
        int expected_order_for_min[] = { INT_MIN, 10, 12, 15, 20, 27, 30, 35, 40, 74, 83, INT_MAX };
        int numbers[] = { 10, 20, 30, INT_MAX, 40, 15, 35, INT_MIN, 83, 12, 74, 27 };
        size_t length = sizeof(numbers) / sizeof(numbers[0]);

        FlBinaryHeap *min_oriented_heap = fl_binheap_new_args((struct FlBinaryHeapArgs) {
            .comparer = fl_container_compare_int,
            .writer = fl_container_writer,
            .cleaner =  NULL,
            .capacity = length,
            .max_capacity = length,
            .element_size = sizeof(int),
            .type = FL_BINHEAP_MIN
        });

        binheap_insert_and_remove(FLUT_TEST_CONTEXT, min_oriented_heap, length, numbers, expected_order_for_min);

        flut_pass("Min. oriented binary heap correctly managed the order of the elements");

        fl_binheap_free(min_oriented_heap);
    }
}
