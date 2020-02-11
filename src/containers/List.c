#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "Container.h"
#include "List.h"

struct FlList {
	struct FlListNode *head;
	struct FlListNode *tail;
	FlContainerAllocatorFunction allocator;
	FlContainerCleanupFunction cleaner;
};

FlList* fl_list_new(void) {
	return fl_list_new_args((struct FlListArgs){ 0 });
}

FlList* fl_list_new_args(struct FlListArgs args)
{
	struct FlList *list = fl_malloc(sizeof(struct FlList));
	
	list->head = NULL;
	list->tail = NULL;
	list->allocator = args.value_allocator;
	list->cleaner = args.value_cleaner;

	return list;
}

struct FlListNode* fl_list_head(FlList *list)
{
	return list->head;	
}

struct FlListNode* fl_list_tail(FlList *list)
{
	return list->tail;
}

struct FlListNode* fl_list_append(FlList *list, const void *value)
{
	struct FlListNode *node = fl_malloc(sizeof(struct FlListNode));
	node->prev = NULL;
	node->next = NULL;

	if (!list->allocator)
		node->value = (FlByte*)value;
	else
		list->allocator(&node->value, value);

	if (list->head == NULL)
		return list->head = list->tail = node;

	// Create the link between the last node and the new one
	node->prev = list->tail;
	list->tail->next = node;

	// Set the new node as the tail
	list->tail = node;

	return node;
}

struct FlListNode* fl_list_prepend(FlList *list, const void *value)
{
	struct FlListNode *node = fl_malloc(sizeof(struct FlListNode));
	node->prev = NULL;
	node->next = NULL;

	if (!list->allocator)
		node->value = (FlByte*)value;
	else
		list->allocator(&node->value, value);

	if (list->head == NULL)
		return list->head = list->tail = node;

	// Create the link between the last node and the new one
	node->next = list->head;
	list->head->prev = node;

	// Set the new node as the head
	list->head = node;

	return node;
}

void fl_list_remove(FlList *list, struct FlListNode *node)
{
	bool isHead = node == list->head;
	bool isTail = node == list->tail;

	if (node->prev != NULL)
		node->prev->next = node->next;

	if (node->next != NULL)
		node->next->prev = node->prev;

	if (isHead)
		list->head = node->next;
	else if (isTail)
		list->tail = node->prev;

	if (list->cleaner)
		list->cleaner(node->value);
	
	fl_free(node);
}

struct FlListNode* fl_list_insert_after(FlList *list, struct FlListNode *target, const void *value)
{
	struct FlListNode *node = fl_malloc(sizeof(struct FlListNode));
	node->prev = NULL;
	node->next = NULL;

	if (!list->allocator)
		node->value = (FlByte*)value;
	else
		list->allocator(&node->value, value);

	bool isTail = target == list->tail;

	// Link to the target's next node (could be NULL)
	node->next = target->next;
	// If there is a target's next node, update the "prev" link
	if (!isTail)
		node->next->prev = node;

	// Link the new node with the target node
	node->prev = target;
	target->next = node;

	// If the new value is the tail, update the list
	if (isTail)
		list->tail = node;

	return node;
}

struct FlListNode* fl_list_insert_before(FlList *list, struct FlListNode *target, const void *value)
{
	struct FlListNode *node = fl_malloc(sizeof(struct FlListNode));
	node->prev = NULL;
	node->next = NULL;

	if (!list->allocator)
		node->value = (FlByte*)value;
	else
		list->allocator(&node->value, value);

	bool isHead = target == list->head;

	// Link to the target's previous node (could be NULL)
	node->prev = target->prev;
	
	// If there is a target's previous node, update the "next" link
	if (!isHead)
		node->prev->next = node;

	// Link the new node with the target node
	node->next = target;
	target->prev = node;

	// If the new value is the head, update the list
	if (isHead)
		list->head = node;

	return node;
}

size_t fl_list_length(FlList *list)
{
	size_t length = 0;
	struct FlListNode *tmp = list->head;

	while (tmp)
		(length++, tmp = tmp->next);

	return length;
}

void fl_list_free(FlList *list)
{
	if (list->head != NULL)
	{
		struct FlListNode *tmp = list->head;
		while (tmp)
		{
			struct FlListNode *toFree = tmp;
			tmp = tmp->next;

			if (list->cleaner)
				list->cleaner(toFree->value);
			
			fl_free(toFree);
		}
	}
	fl_free(list);
}
