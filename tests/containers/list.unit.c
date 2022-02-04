#include <fllib.h>
#include <flut/flut.h>

flut_suite(list) {
    flut_suite_description("List functions");
    flut_suite_register_test(list_append, "fl_list_append function");
    flut_suite_register_test(list_prepend, "fl_list_prepend function");
    flut_suite_register_test(list_head, "fl_list_head function");
    flut_suite_register_test(list_tail, "fl_list_tail function");
    flut_suite_register_test(list_insert_after, "fl_list_insert_after function");
    flut_suite_register_test(list_insert_before, "fl_list_insert_before function");
}

flut_test(list_append) {
    FlList *list = fl_list_new_args((struct FlListArgs) {
        .value_allocator = fl_container_allocator_sizet,
        .value_cleaner = fl_container_cleaner_pointer
    });

    for (size_t i=0; i < 100; i++) {
        if (fl_list_append(list, &i) == NULL) {
            flut_unexpectedv("Could not append element %zu", i);
        }
    }

    flut_pass("100 elements appended to the list");

    size_t number = 0;
    struct FlListNode *node = fl_list_head(list);
    while (node != NULL) {
        if (number != *(size_t*) node->value) {
            flut_unexpectedv("Element at position %zu within the list with value %zu is not equals to the expected value %zu", number, *(size_t*) node->value, number);
        }
        number++;
        node = node->next;
    }

    flut_pass("100 elements exist within the list and are valid");

    fl_list_free(list);
}

flut_test(list_prepend) {
    FlList *list = fl_list_new_args((struct FlListArgs) {
        .value_allocator = fl_container_allocator_sizet,
        .value_cleaner = fl_container_cleaner_pointer
    });

    for (size_t i=0; i < 100; i++) {
        if (fl_list_prepend(list, &i) == NULL) {
            flut_unexpectedv("Could not prepend element %zu", i);
        }
    }

    flut_pass("100 elements prepended to the list");

    size_t number = 0;
    struct FlListNode *node = fl_list_tail(list);
    while (node != NULL) {
        if (number != *(size_t*) node->value) {
            flut_unexpectedv("Element at position %zu within the list with value %zu is not equals to the expected value %zu", number, *(size_t*) node->value, number);
        }
        number++;
        node = node->prev;
    }

    flut_pass("100 elements exist within the list and are valid");

    fl_list_free(list);
}

flut_test(list_head) {
    FlList *list = fl_list_new();

    flut_explain(flut_assert_is_null(fl_list_head(list)), "The head of an empty list must be NULL");

    struct FlListNode *first_node = NULL;
    flut_assert_is_not_null((first_node = fl_list_append(list, NULL)));
    flut_assert_pointer_is_equals(first_node, fl_list_head(list));
    flut_assert_is_null(first_node->prev);
    flut_assert_is_null(first_node->next);

    struct FlListNode *second_node = NULL;
    flut_assert_is_not_null((second_node = fl_list_append(list, NULL)));
    flut_assert_pointer_is_equals(first_node, fl_list_head(list));
    flut_assert_is_null(first_node->prev);
    flut_assert_pointer_is_equals(first_node->next, second_node);
    flut_assert_pointer_is_equals(second_node->prev, first_node);
    flut_assert_is_null(second_node->next);

    struct FlListNode *third_node = NULL;
    flut_assert_is_not_null((third_node = fl_list_prepend(list, NULL)));
    flut_assert_pointer_is_equals(third_node, fl_list_head(list));
    flut_assert_is_null(third_node->prev);
    flut_assert_pointer_is_equals(third_node->next, first_node);
    flut_assert_pointer_is_equals(first_node->prev, third_node);
    flut_assert_pointer_is_equals(first_node->next, second_node);
    flut_assert_pointer_is_equals(second_node->prev, first_node);
    flut_assert_is_null(second_node->next);

    fl_list_free(list);
}

flut_test(list_tail) {
    FlList *list = fl_list_new();

    flut_explain(flut_assert_is_null(fl_list_tail(list)), "The tail of an empty list must be NULL");

    struct FlListNode *first_node = NULL;
    flut_assert_is_not_null((first_node = fl_list_append(list, NULL)));
    flut_assert_pointer_is_equals(first_node, fl_list_tail(list));
    flut_assert_is_null(first_node->prev);
    flut_assert_is_null(first_node->next);

    struct FlListNode *second_node = NULL;
    flut_assert_is_not_null((second_node = fl_list_append(list, NULL)));
    flut_assert_pointer_is_equals(second_node, fl_list_tail(list));
    flut_assert_is_null(first_node->prev);
    flut_assert_pointer_is_equals(first_node->next, second_node);
    flut_assert_pointer_is_equals(second_node->prev, first_node);
    flut_assert_is_null(second_node->next);

    struct FlListNode *third_node = NULL;
    flut_assert_is_not_null((third_node = fl_list_prepend(list, NULL)));
    flut_assert_pointer_is_equals(second_node, fl_list_tail(list));
    flut_assert_is_null(third_node->prev);
    flut_assert_pointer_is_equals(third_node->next, first_node);
    flut_assert_pointer_is_equals(first_node->prev, third_node);
    flut_assert_pointer_is_equals(first_node->next, second_node);
    flut_assert_pointer_is_equals(second_node->prev, first_node);
    flut_assert_is_null(second_node->next);

    fl_list_free(list);
}

flut_test(list_insert_after) {
    flut_describe("Using fl_list_insert_after to insert odd items after inserting all the even items should give an ordered list") {
        FlList *list = fl_list_new_args((struct FlListArgs) {
            .value_allocator = fl_container_allocator_sizet,
            .value_cleaner = fl_container_cleaner_pointer
        });

        #define ITEMS_COUNT 100
        struct FlListNode *nodes[ITEMS_COUNT];

        for (size_t i=0; i < ITEMS_COUNT; i+=2) {
            nodes[i] = fl_list_append(list, &i);
            if (nodes[i] == NULL) {
                flut_unexpectedv("Could not append even item %zu", i);
            }
        }

        flut_pass("All even items have been added to the list");

        for (size_t i=1; i < ITEMS_COUNT; i+=2) {
            nodes[i] = fl_list_insert_after(list, nodes[i-1], &i);
            if (nodes[i] == NULL) {
                flut_unexpectedv("Could not append odd item %zu", i);
            }
        }

        flut_pass("All odd items have been added to the list");

        flut_assert_pointer_is_equals(nodes[0], fl_list_head(list));
        flut_assert_pointer_is_equals(nodes[ITEMS_COUNT - 1], fl_list_tail(list));

        struct FlListNode *node = fl_list_head(list);
        for (size_t i=0; i < ITEMS_COUNT; i++) {
            if (i != *(size_t*) node->value) {
                flut_unexpectedv("Element at position %zu within the list with value %zu is not equals to the expected value %zu", i, *(size_t*) node->value, i);
            }
            node = node->next;
        }

        flut_pass("All items are ordered within the list");
        
        #undef ITEMS_COUNT

        fl_list_free(list);
    }
}

flut_test(list_insert_before) {
    flut_describe("Using fl_list_insert_before to insert even items after inserting all the odd items should give an ordered list") {
        FlList *list = fl_list_new_args((struct FlListArgs) {
            .value_allocator = fl_container_allocator_sizet,
            .value_cleaner = fl_container_cleaner_pointer
        });

        #define ITEMS_COUNT 100
        struct FlListNode *nodes[ITEMS_COUNT];

        for (size_t i=1; i < ITEMS_COUNT; i+=2) {
            nodes[i] = fl_list_append(list, &i);
            if (nodes[i] == NULL) {
                flut_unexpectedv("Could not append odd item %zu", i);
            }
        }

        flut_pass("All odd items have been added to the list");

        for (size_t i=0; i < ITEMS_COUNT; i+=2) {
            nodes[i] = fl_list_insert_before(list, nodes[i+1], &i);
            if (nodes[i] == NULL) {
                flut_unexpectedv("Could not append even item %zu", i);
            }
        }

        flut_pass("All even items have been added to the list");

        flut_assert_pointer_is_equals(nodes[0], fl_list_head(list));
        flut_assert_pointer_is_equals(nodes[ITEMS_COUNT - 1], fl_list_tail(list));

        struct FlListNode *node = fl_list_head(list);
        for (size_t i=0; i < ITEMS_COUNT; i++) {
            if (i != *(size_t*) node->value) {
                flut_unexpectedv("Element at position %zu within the list with value %zu is not equals to the expected value %zu", i, *(size_t*) node->value, i);
            }
            node = node->next;
        }

        flut_pass("All items are ordered within the list");
        
        #undef ITEMS_COUNT

        fl_list_free(list);
    }
}
