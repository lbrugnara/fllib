#include <fllib.h>
#include <flut/flut.h>
#include "Test_List.h"

void test_fl_list_head(void)
{
    FlList *list = fl_list_new();

    struct FlListNode *head = fl_list_head(list);
    flut_expect_compat("The head of a newly created list must be NULL", head == NULL);

    struct FlListNode *node = fl_list_append(list, NULL);
    head = fl_list_head(list);
    flut_expect_compat("After append, the head must be the same node as the newly appended node", head == node);

    struct FlListNode *node2 = fl_list_append(list, NULL);
    flut_expect_compat("After another append, the head must still be the first appended node", head == node);
    flut_expect_compat("Head's next node must be the newly inserted node, and node's previous node must be the head of the list", head->next == node2 && head == node2->prev);

    struct FlListNode *node3 = fl_list_prepend(list, NULL);
    struct FlListNode *oldhead = head;
    head = fl_list_head(list);
    flut_expect_compat("After a prepend, the head must change", head != oldhead);
    flut_expect_compat("After a prepend, the new head must be the prepended node", node3 == head);

    fl_list_free(list);
}

void test_fl_list_tail(void)
{
    FlList *list = fl_list_new();

    struct FlListNode *tail = fl_list_tail(list);
    flut_expect_compat("The tail of a newly created list must be NULL", tail == NULL);

    struct FlListNode *node = fl_list_append(list, NULL);
    tail = fl_list_tail(list);
    flut_expect_compat("The new tail must be the same node as the newly appended node", tail == node);

    struct FlListNode *node2 = fl_list_append(list, NULL);
    struct FlListNode *oldtail = tail;
    tail = fl_list_tail(list);
    flut_expect_compat("After append, the tail must change", tail != oldtail);
    flut_expect_compat("After append, the new tail must be the appended node", tail == node2);
    flut_expect_compat("Tail's prev node must be the first inserted node, and first node's next node must be the tail of the list", tail->prev == node && tail == node->next);

    fl_list_prepend(list, NULL);
    flut_expect_compat("After prepend, the tail must not change", tail == node2);

    fl_list_free(list);
}

void test_fl_list_append(void)
{
    FlList *list = fl_list_new_args((struct FlListArgs) {
        .value_allocator = fl_container_allocator_sizet,
        .value_cleaner = fl_container_cleaner_pointer
    });

    for (size_t i=0; i < 100; i++)
        fl_list_append(list, &i);

    struct FlListNode *tmp, *head = fl_list_head(list);
    tmp = head;
    for (size_t i=0; tmp; i++, tmp = tmp->next)
    {
        size_t el = *(size_t*)tmp->value;
        flut_vexpect_compat(el == i, "List element at %zu must be equals to %zu", i, el);
    }

    fl_list_free(list);
}

void test_fl_list_prepend(void)
{
    FlList *list = fl_list_new_args((struct FlListArgs) {
        .value_allocator = fl_container_allocator_sizet,
        .value_cleaner = fl_container_cleaner_pointer
    });

    for (size_t i=0; i < 100; i++)
        fl_list_prepend(list, &i);
    
    struct FlListNode *tmp, *tail = fl_list_tail(list);
    tmp = tail;
    for (size_t i=0; tmp; i++, tmp = tmp->prev)
    {
        size_t el = *(size_t*)tmp->value;
        flut_vexpect_compat(el == i, "List element at %zu must be equals to %zu", i, el);
    }

    fl_list_free(list);
}

void test_fl_list_insert_after(void)
{
    FlList *list = fl_list_new_args((struct FlListArgs) {
        .value_allocator = fl_container_allocator_sizet,
        .value_cleaner = fl_container_cleaner_pointer
    });

    #define N 100
    struct FlListNode *nodes[N];

    for (size_t i=0; i < N; i+=2)
        nodes[i] = fl_list_append(list, &i);

    for (size_t i=1; i < N; i+=2)
        nodes[i] = fl_list_insert_after(list, nodes[i-1], &i);

    for (size_t i=0; i < N; i++)
    {
        size_t el = *(size_t*)nodes[i]->value;
        flut_vexpect_compat(el == i, "List element at %zu must be equals to %zu", i, el);
    }

    flut_vexpect_compat(nodes[N-1] == fl_list_tail(list), "Expect node at position %zu be the tail of the list", N-1);

    #undef N

    fl_list_free(list);
}

void test_fl_list_insert_before(void)
{
    FlList *list = fl_list_new_args((struct FlListArgs) {
        .value_allocator = fl_container_allocator_sizet,
        .value_cleaner = fl_container_cleaner_pointer
    });

    #define N 100
    struct FlListNode *nodes[N];

    for (size_t i=1; i < N; i+=2)
        nodes[i] = fl_list_append(list, &i);

    for (size_t i=0; i < N; i+=2)
        nodes[i] = fl_list_insert_before(list, nodes[i+1], &i);

    for (size_t i=0; i < N; i++)
    {
        size_t el = *(size_t*)nodes[i]->value;
        flut_vexpect_compat(el == i, "List element at %zu must be equals to %zu", i, el);
    }

    flut_expect_compat("Expect node at position 0 be the head of the list", nodes[0] == fl_list_head(list));

    #undef N

    fl_list_free(list);
}
