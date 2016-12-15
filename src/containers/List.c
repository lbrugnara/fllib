#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "List.h"
#include "../Mem.h"
#include "../Cstr.h"

struct FlListNode
{
	FlByte *data;
	FlListNode prev;
	FlListNode next;
};

/**
 * {@type} Container type
 * {@dtisze} Data type size of each element to be almacenated
 * {@length} Number of current saved elements
 * {@head} Pointer to the first element of the list
 * {@tail} Pointer to the last element of the list
 */
struct FlList
{
    size_t dtsize;
    size_t length;
    FlListNode head;
    FlListNode tail;
};

/**
 * Creates a new {FlListNode} copying {elem} into {FlListNode->data} member
 */
FlListNode
new_list_node (size_t dtsize, const FlPointer elem)
{
	FlListNode node = fl_calloc(1, sizeof(struct FlListNode));
	node->data = fl_calloc(1, dtsize);
	node->prev = NULL;
	node->next = NULL;
	memcpy(node->data, elem, dtsize);
	return node;
}

/**
 * Free the memory reserved for a {FlListNode} and its member data
 */
void 
delete_list_node(FlListNode node)
{
	fl_free(node->data);
	fl_free(node);
}

/**
 * Returns a copy of {src}
 */
FlList
clone_list(FlList src)
{
	FlList dest = fl_list_new(src->dtsize);
	if (src->length == 0)
		return dest;
	dest->head = NULL;
	FlListNode nodecpy = NULL;
	FlListNode node = src->head;
	while (node != NULL)
	{
		fl_list_add(dest, node->data);
		node = node->next;
	}
	return dest;
}

FlListNode 
fl_list_node_next(FlListNode node)
{
	if (node == NULL)
		return NULL;
	return node->next;
}

FlListNode
fl_list_node_prev(FlListNode node)
{
	return node->prev;
}

FlByte*
fl_list_node_data(FlListNode node)
{
	return node->data;
}

/**
 * {@dtsize} DataType size for the new list
 *
 * Creates an empty list for elements of {dtsize} size
 */
FlList 
fl_list_new(size_t dtsize)
{
    FlList list = fl_calloc(1, sizeof(struct FlList));
    list->head = NULL;
    list->tail = NULL;
    list->dtsize = dtsize;
    list->length = 0;
	return list;
}

size_t
fl_list_length(FlList list)
{
	return list->length;
}

size_t
fl_list_dtsize(FlList list)
{
	return list->dtsize;
}

/**
 * {@list} Target list to add a new value
 * {@elem} Element to add to the list. (will be copied {list->dtsize} bytes of elem)
 * Return a pointer to the inserted element
 */
FlPointer 
fl_list_add(FlList list, const FlPointer elem)
{
	FlListNode node = new_list_node(list->dtsize, elem);
	if (list->length == 0)
	{
		node->next = NULL;
		node->prev = NULL;
		list->head = list->tail = node;
	}
	else
	{
		list->tail->next = node;
		node->prev = list->tail;
		node->next = NULL;
		list->tail = node;
	}
	list->length++;
	return node->data;
}

/**
 * {@list} Target list to insert a new value
 * {@elem} Element to add to the list. (will be copied {list->dtsize} bytes of elem)
 * Insert {elem} in the position {pos} of {list}
 * Return a pointer to the inserted element
 */
FlPointer 
fl_list_insert(FlList list, const FlPointer elem, size_t pos)
{
	if (pos > list->length)	
	{
		char msg[50];
		sprintf(msg, "Index %zd out of bound in ["__FILE__":"FL_LINE":fl_list_insert] (length %zd)\n", pos, list->length);
		fputs(msg, stderr);
		exit(-1);
	}
	FlListNode node = new_list_node(list->dtsize, elem);
	if (list->length == 0)
	{
		node->next = NULL;
		node->prev = NULL;
		list->head = list->tail = node;
	}
	else if (pos == list->length)
	{
		list->tail->next = node;
		node->prev = list->tail;
		list->tail = node;
	}
	else
	{
		FlListNode next = list->head;
		while(pos-- > 0)
			next = next->next;
		FlListNode prev = next->prev;
		prev->next = node;
		next->prev = node;
		node->prev = prev;
		node->next = next;
	}
	list->length++;
	return node->data;
}

/**
 * Returns a pointer to the element in the {index} of {list->data}
 */
FlPointer
fl_list_get(FlList list, size_t index)
{
	if (index >= list->length)	
	{
		char msg[50];
		sprintf(msg, "Index %zd out of bound in ["__FILE__":"FL_LINE":fl_list_get] (length %zd)\n", index, list->length);
		fputs(msg, stderr);
		exit(-1);
	}
	FlListNode targetIndex = list->head;
	while(index-- > 0)
		targetIndex = targetIndex->next;
	return targetIndex != NULL ? targetIndex->data : NULL;
}

/**
 * Returns a pointer to the element in the {index} of {list->data}
 */
FlListNode
fl_list_get_node(FlList list, size_t index)
{
	if (index >= list->length)	
	{
		char msg[50];
		sprintf(msg, "Index %zd out of bound in ["__FILE__":"FL_LINE":fl_list_get] (length %zd)\n", index, list->length);
		fputs(msg, stderr);
		exit(-1);
	}
	FlListNode targetIndex = list->head;
	while(index-- > 0)
		targetIndex = targetIndex->next;
	return targetIndex != NULL ? targetIndex : NULL;
}

/**
 * Return true if {list} contains {elem}
 */
FlPointer
fl_list_find(FlList list, const FlPointer elem)
{
	if (list->length == 0)
		return NULL;
	FlListNode node = list->head;
	while (node != NULL)
	{
		if (memcmp(node->data, elem, list->dtsize) == 0)
			return node->data;
		node = node->next;
	}
	return NULL;
}

FlPointer
fl_list_find_h(FlList list, bool (*handler)(const FlPointer celem, const FlPointer elem, size_t dtsize), const FlPointer elem)
{
	if (list->length == 0)
		return NULL;
	FlListNode node = list->head;
	while (node != NULL)
	{
		if (handler(node->data, elem, list->dtsize))
			return node->data;
		node = node->next;
	}
	return NULL;	
}

/**
 * Remove from {list}, the first element in {list->data} copying it into {dest}
 */
bool 
fl_list_shift(FlList list, FlPointer dest)
{
	if (list->length == 0)
	{
		char msg[50];
		sprintf(msg, "List is empty in ["__FILE__":"FL_LINE"] (length %zd)\n", list->length);
		fputs(msg, stderr);
		exit(-1);
	}

	if (dest != NULL)
		memcpy(dest, list->head->data, list->dtsize);
	FlListNode shifted = list->head;
	list->head = list->head->next;
	list->head->prev = NULL;
	list->length--;
	delete_list_node(shifted);
	return true;
}

/**
 * Insert {elem} in the first position of {list}
 */
FlPointer
fl_list_unshift(FlList list, const FlPointer elem)
{
	FlListNode node = new_list_node(list->dtsize, elem);
	if (list->length == 0)
	{
		node->next = NULL;
		node->prev = NULL;
		list->head = list->tail = node;
	}
	else
	{
		node->next = list->head;
		list->head->prev = node;
		list->head = node;
	}
	list->length++;
	return node->data;
}

/**
 * Remove from {list}, the last element in {list->data} copying it into {dest}
 */
bool 
fl_list_pop(FlList list, FlPointer dest)
{
	if (list->length == 0)
	{
		dest = NULL;
		return false;
	}
	FlListNode tail = list->tail;
	if (dest != NULL)
	{
		memcpy(dest, tail->data, list->dtsize);
	}
	memset(tail->data, 0, list->dtsize);
	if (list->length > 1)
	{
		FlListNode prev = tail->prev;
		if (prev != NULL)
		{
			list->tail = prev;
			prev->next = NULL;
		}
	} 
	else
	{
		list->head = list->tail = NULL;
	}
	delete_list_node(tail);
	list->length--;
	return true;
}

/**
 * Return true if {list} contains {elem}
 */
bool
fl_list_contains(FlList list, const FlPointer elem)
{
	if (list->length == 0)
		return false;
	FlListNode node = list->head;
	while (node != NULL)
	{
		if (memcmp(node->data, elem, list->dtsize) == 0)
			return true;
		node = node->next;
	}
	return false;
}

/**
 * Append {list2} elements at the end of {list}
 */
void
fl_list_concat(FlList list, FlList list2)
{
	FlList tmplist = clone_list(list2);
	if (list->length == 0)
	{
		list->head = tmplist->head;
		list->tail = tmplist->tail;
	}
	else
	{
		FlListNode oldtail = list->tail;
		oldtail->next = tmplist->head;
		tmplist->head->prev = oldtail;
		list->tail = tmplist->tail;
	}
	fl_free(tmplist);
	list->length += list2->length;
}

/**
 * Creates a new FlList that will contain all the elements of
 * {list} and {list2}
 */
FlList
fl_list_merge(FlList list, FlList list2)
{
	FlList newlist1 = clone_list(list);
	FlList newlist2 = clone_list(list2);
	fl_list_concat(newlist1, newlist2);
	fl_list_delete(newlist2);
	return newlist1;
}

/**
 * Remove from {list}, the {pos}-th element in {list->data}.
 * The removed element will be copied in {dest}
 */
bool
fl_list_remove(FlList list, size_t pos, FlPointer dest)
{
	if (pos >= list->length)
	{
		char msg[50];
		sprintf(msg, "Index %zd out of bound in ["__FILE__":"FL_LINE":fl_list_remove] (length %zd)\n", pos, list->length);
		fputs(msg, stderr);
		exit(-1);
	}
	FlListNode target = list->head;
	while(pos-- > 0)
		target = target->next;
	FlListNode prev = target->prev;
	FlListNode next = target->next;
	if (prev != NULL)
		prev->next = next;
	if (next != NULL)
		next->prev = prev;

	if (list->head == target)
		list->head = next;
	if (list->tail == target)
		list->tail = prev;

	if (dest != NULL)
		memcpy(dest, target->data, list->dtsize);
	delete_list_node(target);
	list->length--;
	return true;
}

bool
fl_list_remove_h(FlList list, bool (*comparer)(const FlPointer celem, const FlPointer elem, size_t dtsize), const FlPointer elem, FlPointer dest)
{
	FlListNode node = list->head;
	while (node != NULL)
	{
		if (comparer(node->data, elem, list->dtsize))
			break;
		node = node->next;
	}
	if (node == NULL)
		return false;
	FlListNode prev = node->prev;
	FlListNode next = node->next;
	if (prev != NULL)
		prev->next = next;
	if (next != NULL)
		next->prev = prev;

	if (list->head == node)
		list->head = next;
	if (list->tail == node)
		list->tail = prev;

	if (dest != NULL)
		memcpy(dest, node->data, list->dtsize);
	delete_list_node(node);
	list->length--;
	return true;
}

/**
 * Remove all elements from the list
 */
void
fl_list_clear(FlList list)
{
	if (list->length == 0)
		return;
	FlListNode node = list->head;
	FlListNode tmp = NULL;
	while (node != NULL)
	{
		tmp = node->next;
		memset(node->data, 0, list->dtsize);
		delete_list_node(node);
		node = tmp;
	}
	list->head = list->tail = NULL;
	list->length = 0;
}

void
fl_list_clear_h(FlList list, void (*clear_handler)(FlByte*))
{
	if (list->length == 0)
		return;
	FlListNode node = list->head;
	FlListNode tmp = NULL;
	while (node != NULL)
	{
		tmp = node->next;
		if (clear_handler != NULL)
			clear_handler(node->data);
		fl_free(node);
		node = tmp;
	}
	list->head = list->tail = NULL;
	list->length = 0;
}


/**
 * Free the memory reserved for list 
 */
void 
fl_list_delete(FlList list)
{
	FlListNode tmp = NULL;
	FlListNode node = list->head;
	while (node != NULL)
	{
		tmp = node->next;
		delete_list_node(node);
		node = tmp;
	}
	fl_free(list);
}

/**
 * Free the memory reserved for list using {delete_handler} for each element.
 * The handler MUST free the memory used by each element
 */
void
fl_list_delete_h(FlList list, void (*delete_handler)(FlByte*))
{
	FlListNode tmp = NULL;
	FlListNode node = list->head;
	while (node != NULL)
	{
		tmp = node->next;
		if (delete_handler != NULL)
			delete_handler((FlByte*)node->data);
		fl_free(node);
		node = tmp;
	}
	fl_free(list);
}

/* -------------------------------------------------------------
 * FlIterator support
 * -------------------------------------------------------------
 */
typedef struct FlListIterator {
    FlIteratorType type;
    FlListNode current;
    FlListNode next;
    FlListNode prev;
} FlListIterator;

static void it_next(FlPointer it)
{
    FlListIterator *lit = (FlListIterator*)it;
    lit->prev = lit->current;
    lit->current = lit->next;
    lit->next = lit->current != NULL ? lit->current->next : NULL;
}

static void it_prev(FlPointer it)
{
    FlListIterator *lit = (FlListIterator*)it;
    lit->next = lit->current;
    lit->current = lit->prev;
    lit->prev = lit->current != NULL ? lit->current->prev : NULL;
}

static FlPointer it_value(FlPointer it)
{
    FlListIterator *lit = (FlListIterator*)it;
    return lit->current->data;
}

static bool it_equals(FlPointer it1, FlPointer it2)
{
    FlListIterator *lit1 = (FlListIterator*)it1;
    FlListIterator *lit2 = (FlListIterator*)it2;
    return lit1->current == lit2->current;
}

static bool it_start(FlPointer it, FlPointer container)
{
    FlListIterator *lit = it;
    FlList list = container;
    return list->head != NULL && lit->current == list->head;
}

static bool it_end(FlPointer it, FlPointer container)
{
    FlListIterator *lit = it;
    FlList list = container;
    return list->tail != NULL && lit->prev == list->tail;
}

static void it_delete(FlPointer it)
{
    fl_free(it);
}

FlIterator fl_list_start(const FlList list)
{
    FlListIterator *list_it = fl_calloc(1, sizeof(struct FlListIterator));
    list_it->prev = NULL;
    list_it->current = list->head;
    list_it->next = list->head != NULL ? list->head->next : NULL;
    return fl_iterator_new(IT_LIST, list_it, &it_next, &it_prev, &it_value, &it_equals, &it_start, &it_end, &it_delete);
}

FlIterator fl_list_end(const FlList list)
{
    FlListIterator *list_it = fl_calloc(1, sizeof(struct FlListIterator));
    list_it->prev = list->tail;
    list_it->current = NULL;
    list_it->next = NULL;
    return fl_iterator_new(IT_LIST, list_it, &it_next, &it_prev, &it_value, &it_equals, &it_start, &it_end, &it_delete);
}