#include <time.h>
#include <stdbool.h>
#include <fllib.h>

#include <flut/flut.h>

#define FL_TEST_ALPHABET_LENGTH (size_t) ('Z' - 'A') + 1

flut_define_suite(hashtable) {
    flut_suite_description("Hashtable functions");
    flut_suite_register_test(hashtable_add, "fl_hashtable_add function");
    flut_suite_register_test(hashtable_add_fhash, "fl_hashtable_add with a bad hash function");
    flut_suite_register_test(hashtable_get, "fl_hashtable_get function");
    flut_suite_register_test(hashtable_set, "fl_hashtable_set function");
    flut_suite_register_test(hashtable_clear, "fl_hashtable_clear function");
    flut_suite_register_test(hashtable_keys_and_values, "fl_hashtable_keys and fl_hashtable_values functions");
    flut_suite_register_test(hashtable_remove, "fl_hashtable_remove function");
    flut_suite_register_test(hashtable_resize, "fl_hashtable_resize function");
    flut_suite_register_test(hashtable_values, "fl_hashtable_values function");
}

flut_define_test(hashtable_add) {
    flut_describe("fl_hashtable_add for a <char, int> hashtable") {
        FlHashtable *hashtable = NULL;

        flut_assert_is_not_null((hashtable = fl_hashtable_new_args((struct FlHashtableArgs) {
            .hash_function = fl_hashtable_hash_char,
            .key_comparer = fl_container_equals_char,
            .key_cleaner = fl_container_cleaner_pointer,
            .value_cleaner = fl_container_cleaner_pointer,
            .key_allocator = fl_container_allocator_char,
            .value_allocator = fl_container_allocator_int
        })));

        #define MAKE_CHAR_PTR(c) (char[]) { c }
        #define MAKE_INT_PTR(i) (int[]) { i }

        flut_assert_is_not_null(fl_hashtable_add(hashtable, MAKE_CHAR_PTR('a'), MAKE_INT_PTR(97)));
        flut_assert_int_is_equals(97, *(int*) fl_hashtable_get(hashtable, MAKE_CHAR_PTR('a')));
        flut_assert_is_null(fl_hashtable_add(hashtable, MAKE_CHAR_PTR('a'), MAKE_INT_PTR(97)));
        flut_assert_is_null(fl_hashtable_add(hashtable, MAKE_CHAR_PTR('a'), MAKE_INT_PTR(65)));

        flut_assert_is_not_null(fl_hashtable_add(hashtable, MAKE_CHAR_PTR('b'), MAKE_INT_PTR(98)));
        flut_assert_int_is_equals(98, *(int*) fl_hashtable_get(hashtable, MAKE_CHAR_PTR('b')));
        flut_assert_is_null(fl_hashtable_add(hashtable, MAKE_CHAR_PTR('b'), MAKE_INT_PTR(98)));
        flut_assert_is_null(fl_hashtable_add(hashtable, MAKE_CHAR_PTR('b'), MAKE_INT_PTR(66)));

        flut_assert_is_not_null(fl_hashtable_add(hashtable, MAKE_CHAR_PTR('c'), MAKE_INT_PTR(99)));
        flut_assert_int_is_equals(99, *(int*) fl_hashtable_get(hashtable, MAKE_CHAR_PTR('c')));
        flut_assert_is_null(fl_hashtable_add(hashtable, MAKE_CHAR_PTR('c'), MAKE_INT_PTR(99)));
        flut_assert_is_null(fl_hashtable_add(hashtable, MAKE_CHAR_PTR('c'), MAKE_INT_PTR(67)));
        
        flut_assert_is_not_null(fl_hashtable_add(hashtable, MAKE_CHAR_PTR('d'), MAKE_INT_PTR(100)));
        flut_assert_int_is_equals(100, *(int*) fl_hashtable_get(hashtable, MAKE_CHAR_PTR('d')));
        flut_assert_is_null(fl_hashtable_add(hashtable, MAKE_CHAR_PTR('d'), MAKE_INT_PTR(100)));
        flut_assert_is_null(fl_hashtable_add(hashtable, MAKE_CHAR_PTR('d'), MAKE_INT_PTR(68)));

        flut_assert_is_not_null(fl_hashtable_add(hashtable, MAKE_CHAR_PTR('e'), MAKE_INT_PTR(101)));
        flut_assert_int_is_equals(101, *(int*) fl_hashtable_get(hashtable, MAKE_CHAR_PTR('e')));
        flut_assert_is_null(fl_hashtable_add(hashtable, MAKE_CHAR_PTR('e'), MAKE_INT_PTR(101)));
        flut_assert_is_null(fl_hashtable_add(hashtable, MAKE_CHAR_PTR('e'), MAKE_INT_PTR(69)));

        flut_assert_is_not_null(fl_hashtable_add(hashtable, MAKE_CHAR_PTR('f'), MAKE_INT_PTR(102)));
        flut_assert_int_is_equals(102, *(int*) fl_hashtable_get(hashtable, MAKE_CHAR_PTR('f')));
        flut_assert_is_null(fl_hashtable_add(hashtable, MAKE_CHAR_PTR('f'), MAKE_INT_PTR(102)));
        flut_assert_is_null(fl_hashtable_add(hashtable, MAKE_CHAR_PTR('f'), MAKE_INT_PTR(70)));

        fl_hashtable_free(hashtable);
        #undef MAKE_CHAR_PTR
        #undef MAKE_INT_PTR
    }
}

unsigned long bad_hash_function(const FlByte *key) {
    return 1;
}

flut_define_test(hashtable_add_fhash) {
    flut_describe("fl_hashtable_add with a bad hash function should still work") {
        FlHashtable *hashtable = NULL;

        flut_assert_is_not_null((hashtable = fl_hashtable_new_args((struct FlHashtableArgs) {
            .hash_function = bad_hash_function,
            .key_comparer = fl_container_equals_char,
            .key_cleaner = fl_container_cleaner_pointer,
            .value_cleaner = fl_container_cleaner_pointer,
            .key_allocator = fl_container_allocator_char,
            .value_allocator = fl_container_allocator_int
        })));

        #define MAKE_CHAR_PTR(c) (char[]) { c }
        #define MAKE_INT_PTR(i) (int[]) { i }

        flut_assert_is_not_null(fl_hashtable_add(hashtable, MAKE_CHAR_PTR('a'), MAKE_INT_PTR(97)));
        flut_assert_int_is_equals(97, *(int*) fl_hashtable_get(hashtable, MAKE_CHAR_PTR('a')));
        flut_assert_is_null(fl_hashtable_add(hashtable, MAKE_CHAR_PTR('a'), MAKE_INT_PTR(97)));
        flut_assert_is_null(fl_hashtable_add(hashtable, MAKE_CHAR_PTR('a'), MAKE_INT_PTR(65)));

        flut_assert_is_not_null(fl_hashtable_add(hashtable, MAKE_CHAR_PTR('b'), MAKE_INT_PTR(98)));
        flut_assert_int_is_equals(98, *(int*) fl_hashtable_get(hashtable, MAKE_CHAR_PTR('b')));
        flut_assert_is_null(fl_hashtable_add(hashtable, MAKE_CHAR_PTR('b'), MAKE_INT_PTR(98)));
        flut_assert_is_null(fl_hashtable_add(hashtable, MAKE_CHAR_PTR('b'), MAKE_INT_PTR(66)));

        flut_assert_is_not_null(fl_hashtable_add(hashtable, MAKE_CHAR_PTR('c'), MAKE_INT_PTR(99)));
        flut_assert_int_is_equals(99, *(int*) fl_hashtable_get(hashtable, MAKE_CHAR_PTR('c')));
        flut_assert_is_null(fl_hashtable_add(hashtable, MAKE_CHAR_PTR('c'), MAKE_INT_PTR(99)));
        flut_assert_is_null(fl_hashtable_add(hashtable, MAKE_CHAR_PTR('c'), MAKE_INT_PTR(67)));
        
        flut_assert_is_not_null(fl_hashtable_add(hashtable, MAKE_CHAR_PTR('d'), MAKE_INT_PTR(100)));
        flut_assert_int_is_equals(100, *(int*) fl_hashtable_get(hashtable, MAKE_CHAR_PTR('d')));
        flut_assert_is_null(fl_hashtable_add(hashtable, MAKE_CHAR_PTR('d'), MAKE_INT_PTR(100)));
        flut_assert_is_null(fl_hashtable_add(hashtable, MAKE_CHAR_PTR('d'), MAKE_INT_PTR(68)));

        flut_assert_is_not_null(fl_hashtable_add(hashtable, MAKE_CHAR_PTR('e'), MAKE_INT_PTR(101)));
        flut_assert_int_is_equals(101, *(int*) fl_hashtable_get(hashtable, MAKE_CHAR_PTR('e')));
        flut_assert_is_null(fl_hashtable_add(hashtable, MAKE_CHAR_PTR('e'), MAKE_INT_PTR(101)));
        flut_assert_is_null(fl_hashtable_add(hashtable, MAKE_CHAR_PTR('e'), MAKE_INT_PTR(69)));

        flut_assert_is_not_null(fl_hashtable_add(hashtable, MAKE_CHAR_PTR('f'), MAKE_INT_PTR(102)));
        flut_assert_int_is_equals(102, *(int*) fl_hashtable_get(hashtable, MAKE_CHAR_PTR('f')));
        flut_assert_is_null(fl_hashtable_add(hashtable, MAKE_CHAR_PTR('f'), MAKE_INT_PTR(102)));
        flut_assert_is_null(fl_hashtable_add(hashtable, MAKE_CHAR_PTR('f'), MAKE_INT_PTR(70)));

        fl_hashtable_free(hashtable);
        #undef MAKE_CHAR_PTR
        #undef MAKE_INT_PTR
    }
}

flut_define_test(hashtable_get) {
    FlHashtable *hashtable = NULL;
    
    flut_assert_is_not_null((hashtable = fl_hashtable_new_args((struct FlHashtableArgs) {
        .hash_function = fl_hashtable_hash_char,
        .key_comparer = fl_container_equals_char,
        .key_cleaner = fl_container_cleaner_pointer,
        .value_cleaner = fl_container_cleaner_pointer,
        .key_allocator = fl_container_allocator_char,
        .value_allocator = fl_container_allocator_int
    })));

    #define MAKE_CHAR_PTR(c) (char[]) { c }
    #define MAKE_INT_PTR(i) (int[]) { i }
    
    for (char c = 'A'; c <= 'Z'; c++) {
        if (!fl_hashtable_add(hashtable, MAKE_CHAR_PTR(c), MAKE_INT_PTR((int) c))) {
            flut_unexpectedv("Could not add element in the hashtable with key %c and value %d", c, (int) c);
        }
    }
    flut_pass("All letters from 'A' to 'Z' where added to the hashtable");
    
    for (char c = 'a'; c <= 'z'; c++) {
        if (!fl_hashtable_add(hashtable, MAKE_CHAR_PTR(c), MAKE_INT_PTR((int) c))) {
            flut_unexpectedv("Could not add element in the hashtable with key %c and value %d", c, (int) c);
        }
    }
    flut_pass("All letters from 'a' to 'z' where added to the hashtable");

    for (char c = 'A'; c <= 'Z'; c++) {
        int *number_ptr = fl_hashtable_get(hashtable, MAKE_CHAR_PTR(c));
        if (number_ptr == NULL) {
            flut_unexpectedv("fl_hashtable_get should have returned value %d under key %c, but it returned NULL", (int) c, c);
        } else if (*number_ptr != (int) c) {
            flut_unexpectedv("fl_hashtable_get should have returned value %d under key %c, but it returned %d", (int) c, c, *number_ptr);
        }
    }
    flut_pass("All letters from 'A' to 'Z' where correctly retrieved from the hashtable using fl_hashtable_get");

    for (char c = 'a'; c <= 'z'; c++) {
        int *number_ptr = fl_hashtable_get(hashtable, MAKE_CHAR_PTR(c));
        if (number_ptr == NULL) {
            flut_unexpectedv("fl_hashtable_get should have returned value %d under key %c, but it returned NULL", (int) c, c);
        } else if (*number_ptr != (int) c) {
            flut_unexpectedv("fl_hashtable_get should have returned value %d under key %c, but it returned %d", (int) c, c, *number_ptr);
        }
    }
    flut_pass("All letters from 'a' to 'z' where correctly retrieved from the hashtable using fl_hashtable_get");

    fl_hashtable_free(hashtable);
    #undef MAKE_CHAR_PTR
    #undef MAKE_INT_PTR
}

flut_define_test(hashtable_set) {
    FlHashtable *hashtable = NULL;
    
    flut_assert_is_not_null((hashtable = fl_hashtable_new_args((struct FlHashtableArgs) {
        .hash_function = fl_hashtable_hash_int,
        .key_comparer = fl_container_equals_int,
        .key_cleaner = fl_container_cleaner_pointer,
        .value_cleaner = fl_container_cleaner_pointer,
        .key_allocator = fl_container_allocator_int,
        .value_allocator = fl_container_allocator_char
    })));

    #define MAKE_CHAR_PTR(c) (char[]) { c }
    #define MAKE_INT_FROM_UPPER_CHAR(c) ((int) (c) - 'A')
    #define MAKE_INT_PTR_FROM_UPPER_CHAR(c) (int[]) { MAKE_INT_FROM_UPPER_CHAR(c) }
    #define MAKE_INT_FROM_LOWER_CHAR(c) ((int) (c) - 'a')
    #define MAKE_INT_PTR_FROM_LOWER_CHAR(c) (int[]) { MAKE_INT_FROM_LOWER_CHAR(c) }
    
    for (char c = 'A'; c <= 'Z'; c++) {
        if (!fl_hashtable_add(hashtable, MAKE_INT_PTR_FROM_UPPER_CHAR(c), MAKE_CHAR_PTR(c))) {
            flut_unexpectedv("Could not add element in the hashtable with key %d and value %c", MAKE_INT_FROM_UPPER_CHAR(c), c);
        }
    }
    flut_pass("All letters from 'A' to 'Z' where added to the hashtable");
    
    for (char c = 'a'; c <= 'z'; c++) {
        if (!fl_hashtable_set(hashtable, MAKE_INT_PTR_FROM_LOWER_CHAR(c), MAKE_CHAR_PTR(c))) {
            flut_unexpectedv("Could not replace element in the hashtable with key %d by value %c", MAKE_INT_FROM_LOWER_CHAR(c), c);
        }
    }
    flut_pass("All letters from 'A' to 'Z' where replaced by 'a' to 'z' in the hashtable");

    fl_hashtable_free(hashtable);
    #undef MAKE_CHAR_PTR
    #undef MAKE_INT_FROM_UPPER_CHAR
    #undef MAKE_INT_PTR_FROM_UPPER_CHAR
    #undef MAKE_INT_FROM_LOWER_CHAR
    #undef MAKE_INT_PTR_FROM_LOWER_CHAR
}

flut_define_test(hashtable_clear) {
    FlHashtable *hashtable = NULL;
    
    flut_assert_is_not_null((hashtable = fl_hashtable_new_args((struct FlHashtableArgs) {
        .hash_function = fl_hashtable_hash_char,
        .key_comparer = fl_container_equals_char,
        .key_cleaner = fl_container_cleaner_pointer,
        .value_cleaner = fl_container_cleaner_pointer,
        .key_allocator = fl_container_allocator_char,
        .value_allocator = fl_container_allocator_int
    })));

    #define MAKE_CHAR_PTR(c) (char[]) { c }
    #define MAKE_INT_PTR(i) (int[]) { i }
    
    for (char c = 'A'; c <= 'Z'; c++) {
        if (!fl_hashtable_add(hashtable, MAKE_CHAR_PTR(c), MAKE_INT_PTR((int) c))) {
            flut_unexpectedv("Could not add element in the hashtable with key %c and value %d", c, (int) c);
        }
    }
    flut_pass("All letters from 'A' to 'Z' where added to the hashtable");

    flut_assert_size_t_is_equals(FL_TEST_ALPHABET_LENGTH, fl_hashtable_length(hashtable));
    fl_hashtable_clear(hashtable);
    flut_explain(flut_assert_size_t_is_equals(0, fl_hashtable_length(hashtable)), "After a call to the fl_hashtable_clear function all elements must be removed");

    fl_hashtable_free(hashtable);

    #undef MAKE_CHAR_PTR
    #undef MAKE_INT_PTR
}

static int compare_char_ptr(const void *a, const void *b) {
    char **char_a = (char**) a;
    char **char_b = (char**) b;
    return fl_container_compare_char((const FlByte*) *char_a, (const FlByte*) *char_b);
}

static int compare_int_ptr(const void *a, const void *b) {
    int **int_a = (int**) a;
    int **int_b = (int**) b;
    return fl_container_compare_int((const FlByte*) *int_a, (const FlByte*) *int_b);
}

flut_define_test(hashtable_keys_and_values) {
    FlHashtable *hashtable = NULL;
    
    flut_assert_is_not_null((hashtable = fl_hashtable_new_args((struct FlHashtableArgs) {
        .hash_function = fl_hashtable_hash_char,
        .key_comparer = fl_container_equals_char,
        .key_cleaner = fl_container_cleaner_pointer,
        .value_cleaner = fl_container_cleaner_pointer,
        .key_allocator = fl_container_allocator_char,
        .value_allocator = fl_container_allocator_int
    })));

    #define MAKE_CHAR_PTR(c) (char[]) { c }
    #define MAKE_INT_FROM_UPPER_CHAR(c) ((int) (c) - 'A')
    #define MAKE_INT_PTR(i) (int[]) { i }
    
    for (char c = 'A'; c <= 'Z'; c++) {
        if (!fl_hashtable_add(hashtable, MAKE_CHAR_PTR(c), MAKE_INT_PTR((int) c))) {
            flut_unexpectedv("Could not add element in the hashtable with key %c and value %d", c, (int) c);
        }
    }
    flut_pass("All letters from 'A' to 'Z' where added to the hashtable");

    flut_assert_size_t_is_equals(FL_TEST_ALPHABET_LENGTH, fl_hashtable_length(hashtable));

    char **keys = NULL;
    flut_assert_is_not_null((keys = fl_hashtable_keys(hashtable)));
    flut_assert_size_t_is_equals(FL_TEST_ALPHABET_LENGTH, fl_array_length(keys));
    qsort(keys, FL_TEST_ALPHABET_LENGTH, sizeof(char*), compare_char_ptr);
    for (char c = 'A'; c <= 'Z'; c++) {
        if (c != *keys[MAKE_INT_FROM_UPPER_CHAR(c)]) {
            flut_unexpectedv("Char '%c' in the keys array should be '%c' instead of '%c'", c, c, *keys[MAKE_INT_FROM_UPPER_CHAR(c)]);
        }
    }
    flut_pass("All characters are present in the keys array");

    int **values = NULL;
    flut_assert_is_not_null((values = fl_hashtable_values(hashtable)));
    flut_assert_size_t_is_equals(FL_TEST_ALPHABET_LENGTH, fl_array_length(values));
    qsort(values, FL_TEST_ALPHABET_LENGTH, sizeof(int*), compare_int_ptr);
    for (char c = 'A'; c <= 'Z'; c++) {
        if ((int) c != *values[MAKE_INT_FROM_UPPER_CHAR(c)]) {
            flut_unexpectedv("Int '%d' in the values array should be '%d' instead of '%d'",
                MAKE_INT_FROM_UPPER_CHAR(c),
                (int) c,
                *values[MAKE_INT_FROM_UPPER_CHAR(c)]
            );
        }
    }
    flut_pass("All characters are present in the values array");

    fl_array_free(keys);
    fl_array_free(values);
    fl_hashtable_free(hashtable);

    #undef MAKE_CHAR_PTR
    #undef MAKE_INT_FROM_UPPER_CHAR
    #undef MAKE_INT_PTR
}

flut_define_test(hashtable_remove) {
    FlHashtable *hashtable = NULL;
    
    flut_assert_is_not_null((hashtable = fl_hashtable_new_args((struct FlHashtableArgs) {
        .hash_function = fl_hashtable_hash_char,
        .key_comparer = fl_container_equals_char,
        .key_cleaner = fl_container_cleaner_pointer,
        .value_cleaner = fl_container_cleaner_pointer,
        .key_allocator = fl_container_allocator_char,
        .value_allocator = fl_container_allocator_int
    })));

    #define MAKE_CHAR_PTR(c) (char[]) { c }
    #define MAKE_INT_PTR(i) (int[]) { i }
    
    for (char c = 'A'; c <= 'Z'; c++) {
        if (!fl_hashtable_add(hashtable, MAKE_CHAR_PTR(c), MAKE_INT_PTR((int) c))) {
            flut_unexpectedv("Could not add element in the hashtable with key %c and value %d", c, (int) c);
        }
    }
    flut_pass("All letters from 'A' to 'Z' where added to the hashtable");

    flut_assert_size_t_is_equals(FL_TEST_ALPHABET_LENGTH, fl_hashtable_length(hashtable));

    for (char c = 'A'; c <= 'Z'; c++) {
        if (!fl_hashtable_remove(hashtable, MAKE_CHAR_PTR(c), true, true)) {
            flut_unexpectedv("Could not remove element with key '%c'", c);
        }
    }
    
    flut_pass("All characters where removed from the hashtable");
    flut_assert_size_t_is_equals(0, fl_hashtable_length(hashtable));

    fl_hashtable_free(hashtable);

    #undef MAKE_CHAR_PTR
    #undef MAKE_INT_PTR
}

static unsigned long hash_size_t_by_its_value_fn(const FlByte *key)
{
    return *(size_t*)key;
}

static int compare_size_t_ptr(const void *a, const void *b) {
    size_t **size_t_a = (size_t**) a;
    size_t **size_t_b = (size_t**) b;
    return fl_container_compare_sizet((const FlByte*) *size_t_a, (const FlByte*) *size_t_b);
}

flut_define_test(hashtable_resize) {
    FlHashtable *hashtable = NULL;
    
    flut_assert_is_not_null((hashtable = fl_hashtable_new_args((struct FlHashtableArgs){
        .hash_function = hash_size_t_by_its_value_fn,
        .key_comparer = fl_container_equals_sizet,
        .key_cleaner = fl_container_cleaner_pointer,
        .value_cleaner = fl_container_cleaner_pointer,
        .key_allocator = fl_container_allocator_sizet,
        .value_allocator = fl_container_allocator_sizet,
        .buckets_count = 52735,
        .load_factor = 1.0
    })));

    #define FL_RESIZE_COUNT 52736

    FlTimer *timer = fl_timer_create();
    fl_timer_start(timer);
    for (size_t i=0; i < 52736; i++) {
        fl_hashtable_add(hashtable, &i, &i);
    }
    fl_timer_end(timer);

    flut_explainv(flut_assert_size_t_is_equals(52736, fl_hashtable_length(hashtable)), "%d elements where added in %ldms", FL_RESIZE_COUNT, fl_timer_elapsed_ms(timer));

    size_t **keys = NULL;
    flut_assert_is_not_null((keys = fl_hashtable_keys(hashtable)));
    flut_assert_size_t_is_equals(FL_RESIZE_COUNT, fl_array_length(keys));
    qsort(keys, FL_RESIZE_COUNT, sizeof(size_t*), compare_size_t_ptr);
    for (size_t i=0; i < 52736; i++) {
        if (i != *keys[i]) {
            flut_unexpectedv("Element at '%zu' in the keys array should be '%zu' instead of '%zu'", i, i, *keys[i]);
        }
        size_t *value = fl_hashtable_get(hashtable, &i);
        if (i != *value) {
            flut_unexpectedv("Key '%zu' should map to value '%zu' instead of '%zu'", i, i, *keys[i]);
        }
    }
    flut_pass("All elements exist within the hashtable and are valid after a resize operation");

    fl_array_free(keys);
    fl_timer_free(timer);
    fl_hashtable_free(hashtable);
    #undef FL_RESIZE_COUNT
}

static void allocate_hashtable_args_struct(FlByte **dest, const FlByte *src)
{
    size_t size = sizeof( struct FlHashtableArgs);
    *dest = fl_malloc(size);
    memcpy(*dest, src, size);
}

flut_define_test(hashtable_values) {
    struct FlHashtableArgs args = { 0 };
    args.hash_function = fl_hashtable_hash_int;
    args.key_comparer = fl_container_equals_int;
    args.key_allocator = fl_container_allocator_int;
    args.key_cleaner = fl_container_cleaner_pointer;
    args.value_allocator = allocate_hashtable_args_struct;
    args.value_cleaner = fl_container_cleaner_pointer;
    args.buckets_count = 10;

    #define MAKE_INT_PTR(i) (int[]) { i }

    FlHashtable *hashtable = NULL;

    flut_assert_is_not_null((hashtable = fl_hashtable_new_args(args)));
    flut_assert_is_not_null(fl_hashtable_add(hashtable, MAKE_INT_PTR(0), &args));

    struct FlHashtableArgs **values = NULL;
    flut_assert_is_not_null((values = fl_hashtable_values(hashtable)));

    flut_assert_size_t_is_equals(1, fl_array_length(values));
    flut_assert_is_true(memcmp(&args, values[0], sizeof(struct FlHashtableArgs)) == 0);

    values[0]->buckets_count = 20;
    flut_explain(flut_assert_is_true(memcmp(&args, values[0], sizeof(struct FlHashtableArgs)) != 0), "values[0]->buckets_count changed to 20");

    struct FlHashtableArgs *value_ptr = NULL;
    flut_assert_is_not_null((value_ptr = fl_hashtable_get(hashtable, MAKE_INT_PTR(0))));
    flut_explain(flut_assert_is_true(memcmp(value_ptr, values[0], sizeof(struct FlHashtableArgs)) == 0), "A copy of the args variable was performed on fl_hashtable_add");

    fl_array_free(values);
    fl_hashtable_free(hashtable);
}
