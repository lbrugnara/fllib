#ifndef FL_LIST_H
#define FL_LIST_H

#include "../Types.h"
#include "Container.h"
#include "Iterator.h"

/*
 * Type: FlList
 *  
 *  A typedef to an opaque struct that represents a doubly linked list instance.
 *
 * ===== C =====
 *  typedef struct FlList FlList;
 * =============
 */
typedef struct FlList FlList;

/*
 * Type: struct FlListNode
 *  Represents a list node
 * 
 * ===== C ===== 
 * struct FlListNode {
 *    FlByte *value;
 *    struct FlListNode *prev;
 *    struct FlListNode *next;
 * };
 */
struct FlListNode {
    FlByte *value;
    struct FlListNode *prev;
    struct FlListNode *next;
};

/*
 * Type: struct FlListArgs
 *  Arguments to create a new doubly linked list using
 *  the provided configuration
 * 
 * ===== C ===== 
 * struct FlListArgs {
 *     FlContainerAllocatorFunction value_allocator;
 *     FlContainerCleanupFunction value_cleaner;
 * };
 */
struct FlListArgs {
    FlContainerAllocatorFunction value_allocator;
    FlContainerCleanupFunction value_cleaner;
};

/*
 * Function: fl_list_new
 *  Creates a new doubly linked list that simply saves pointer references
 *  into its nodes' value pointer, and at delete it does not
 *  perform nodes' value cleanup
 * 
 * Parameters:
 *  None.
 *
 * Returns:
 *  FlList - an opaque pointer to a list
 *
 */
FlList* fl_list_new(void);

/*
 * Function: fl_list_new_args
 *  Creates a new list using the provided <FlContainerAllocatorFunction> and
 *  <FlContainerCleanupFunction> functions to save and clean nodes' values
 *
 * Parameters:
 *  args - The arguments to configure the list instance (<struct FlListArgs>)
 *
 * Returns:
 *  FlList - an opaque pointer to a list
 *
 */
FlList* fl_list_new_args(struct FlListArgs args);

/*
 * Function: fl_list_head
 *  Returns the head of the doubly linked list
 *
 * Parameters:
 *  list - List to get its head
 *
 * Returns:
 * struct FlListNode* - The list's head
 *
 */
struct FlListNode* fl_list_head(FlList *list);

/*
 * Function: fl_list_tail
 *  Returns the tail of the doubly linked list
 *
 * Parameters:
 *  list - List to get its tail
 *
 * Returns:
 * struct FlListNode* - The list's tail
 *
 */
struct FlListNode* fl_list_tail(FlList *list);

/*
 * Function: fl_list_append
 *  Adds a new <struct FlListNode> to the end of the list and returns
 *  a pointer to it (which is the new list's tail)
 *
 * Parameters:
 *  list - Target list to add the new node
 *  value - Value to be assigned/copied to the node's value
 *
 * Returns:
 *  struct FlListNode* - Pointer to the newly inserted node
 *
 */
struct FlListNode* fl_list_append(FlList *list, const void *value);

/*
 * Function: fl_list_prepend
 *  Adds a new <struct FlListNode> to the start of the list and returns
 *  a pointer to it (which is the new list's head)
 *
 * Parameters:
 *  list - Target list to add the new node
 *  value - Value to be assigned/copied to the node's value
 *
 * Returns:
 *  struct FlListNode* - Pointer to the newly inserted node
 *
 */
struct FlListNode* fl_list_prepend(FlList *list, const void *value);

/*
 * Function: fl_list_insert_after
 *  Inserts a new <struct FlListNode> after the *target* node and returns a pointer
 *  to the newly inserted node. If *target* is the list's tail, the inserted node
 *  will be the new tail
 * 
 * Parameters:
 *  list - Target list to insert the new node
 *  target - Target node to insert the new node after it
 *  value - Value to be assigned/copied to the node's value
 *
 * Returns:
 *  struct FlListNode* - Pointer to the newly inserted node
 *
 */
struct FlListNode* fl_list_insert_after(FlList *list, struct FlListNode *target, const void *value);

/*
 * Function: fl_list_insert_before
 *  Inserts a new <struct FlListNode> before the *target* node and returns a pointer
 *  to the newly inserted node. If *target* is the list's head, the inserted node
 *  will be the new head
 * 
 * Parameters:
 *  list - Target list to insert the new node
 *  target - Target node to insert the new node before it
 *  value - Value to be assigned/copied to the node's value
 *
 * Returns:
 *  struct FlListNode* - Pointer to the newly inserted node
 *
 */
struct FlListNode* fl_list_insert_before(FlList *list, struct FlListNode *target, const void *value);

void fl_list_remove(FlList *list, struct FlListNode *node);

/*
 * Function: fl_list_free
 *  Frees the resources allocated by the list, including the memory allocated 
 *  for the <struct FlListNode>s. If the list has a <FlContainerCleanupFunction>
 *  the nodes' value will be cleaned using it.
 *
 * Parameters:
 *  list - List to be freed
 *
 * Returns:
 *  void - This function does not return a value
 */
void fl_list_free(FlList *list);

size_t fl_list_length(FlList *list);

#endif /* FL_LIST_H */
