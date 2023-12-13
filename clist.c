/*
 * clist.c
 *
 * Linked list implementation for ISSE Assignment 12
 *
 * Author: Ngirimana Schadrack <schadran@andrew.cmu.edu>
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "clist.h"

#define DEBUG
#define INVALID_RETURN ((CListElementType){TOK_END})

/*
 * Create (malloc) a new _cl_node and populate it with the supplied values
 *
 * Parameters:
 *   element, next  The values for the node to be created
 *
 * Returns: The newly-malloc'd node, or NULL in case of error
 */
static struct _cl_node *_CL_new_node(CListElementType element, struct _cl_node *next)
{
    struct _cl_node *new = (struct _cl_node *)malloc(sizeof(struct _cl_node));
    assert(new);
    new->element = element;
    new->next = next;
    return new;
}

// Documented in .h file
CList CL_new()
{
    CList list = (CList)malloc(sizeof(struct _clist));
    assert(list);
    list->head = NULL;
    list->length = 0;
    return list;
}

// Documented in .h file
void CL_free(CList list)
{
    if (!list)
        return;

    struct _cl_node *current = list->head;

    while (current != NULL)
    {
        struct _cl_node *currentItem = current->next;
        free(current);
        current = currentItem;
    }
    free(list);
}

// Documented in .h file
int CL_length(CList list)
{
    if (list == NULL)
        return 0;

        // traverse the list, counting the number of nodes
#ifdef DEBUG
    int len = 0;

    for (struct _cl_node *node = list->head; node != NULL; node = node->next)
        len++;

    assert(len == list->length);
#endif // DEBUG

    return list->length;
}

// Documented in .h file
void CL_push(CList list, CListElementType element)
{
    if (list == NULL || element.type)
        return;

    list->head = _CL_new_node(element, list->head);
    list->length++;
}

// Documented in .h file
CListElementType CL_pop(CList list)
{
    if (list == NULL)
        return INVALID_RETURN;

    struct _cl_node *popped_node = list->head;

    if (popped_node == NULL)
        return INVALID_RETURN;

    CListElementType ret = popped_node->element;

    // unlink previous head node, then free it
    list->head = popped_node->next;
    free(popped_node);

    list->length--;

    return ret;
}

// Documented in .h file
void CL_append(CList list, CListElementType element)
{

    if (list == NULL)
        return;

    struct _cl_node *newNode = _CL_new_node(element, NULL);

    if (list->head == NULL)
    {
        list->head = newNode;
    }
    else
    {
        struct _cl_node *currentNode = list->head;
        while (currentNode->next != NULL)
        {
            currentNode = currentNode->next;
        }

        currentNode->next = newNode;
    }
    list->length++;
}

// Documented in .h file
CListElementType CL_nth(CList list, int pos)
{
    assert(list);
    // Handle the case of an invalid position
    if (list->head == NULL || pos < -CL_length(list) || pos >= CL_length(list))
    {
        return INVALID_RETURN;
    }
    if (pos < 0)
    {
        pos = list->length + pos; // convert negative pos to positive
    }

    struct _cl_node *currentNode = list->head;

    // traverse list to find nth element which is at last position  - 1
    for (int i = 0; i < pos; i++)
    {
        currentNode = currentNode->next;
    }
    if (currentNode == NULL)
        return INVALID_RETURN;

    return currentNode->element;
}

// Documented in .h file
bool CL_insert(CList list, CListElementType element, int pos)
{
    assert(list);

    if (element.type == TOK_END)
        return false;

    // Handle the case of an invalid position
    if (pos < -CL_length(list) - 1 || pos > CL_length(list))
    {
        return false;
    }
    // Handle negative positions by converting them to positive
    if (pos < 0)
        pos = list->length + pos + 1;

    if (pos == 0)
    {
        list->head = _CL_new_node(element, list->head);
    }

    else
    {
        // Traverse the list to find the node before the desired position
        struct _cl_node *current = list->head;
        for (int i = 1; i < pos; i++)
        {
            current = current->next;
        }
        // Insert the new node after the found node
        current->next = _CL_new_node(element, current->next);
    }
    // Increment the length of the list
    list->length++;
    return true;
}

// Documented in .h file
CListElementType CL_remove(CList list, int pos)
{
    assert(list);

    if (list->head == NULL || pos < -CL_length(list) || pos >= CL_length(list))
    {
        return INVALID_RETURN;
    }
    if (pos < 0)
    {
        pos = CL_length(list) + pos;
    }
    struct _cl_node *temp = list->head;
    if (pos == 0)
    {
        list->head = temp->next;
        CListElementType element_to_return = temp->element;
        free(temp);
        list->length--;
        return element_to_return;
    }
    else
    {
        struct _cl_node *currentNode = list->head;
        for (int i = 1; i < pos; i++)
        {
            currentNode = currentNode->next;
        }

        struct _cl_node *remove_item = currentNode->next;
        currentNode->next = remove_item->next;
        CListElementType element_to_return = remove_item->element;
        free(remove_item);
        list->length--;

        return element_to_return;
    }
}

// Documented in .h file
CList CL_copy(CList list)
{
    assert(list);
    if (list == NULL)
        return NULL;

    // create a new list for the copy
    CList copy_list = CL_new();

    // Traverse the original list and copy elements to the new list
    for (struct _cl_node *original_node = list->head; original_node != NULL; original_node = original_node->next)
    {
        CL_append(copy_list, original_node->element);
    }

    return copy_list;
}

// Documented in .h file
void CL_join(CList list1, CList list2)
{
    assert(list1);
    assert(list2);
    if (list2->head == NULL)
    {
        return;
    }
    if (list1->head == NULL)
    {
        list1->head = list2->head;
    }
    else
    {
        struct _cl_node *last_node_list1 = list1->head;
        // Traverse list1 to find the last node
        for (; last_node_list1->next != NULL;)
        {
            last_node_list1 = last_node_list1->next;
        }

        // Update the next pointer of the last node of list1 to point to the head of list2
        last_node_list1->next = list2->head;
    }
    // Update the length of list1 to include the length of list2
    list1->length += list2->length;

    // Clear list2 (destroy its contents)
    list2->head = NULL;
    list2->length = 0;
}

// Documented in .h file
void CL_reverse(CList list)
{
    assert(list);

    struct _cl_node *prev_node = NULL;
    struct _cl_node *current_node = list->head;
    struct _cl_node *next_node = NULL;

    // Traverse the list and reverse the next pointers of each node
    while (current_node != NULL)
    {
        next_node = current_node->next;
        current_node->next = prev_node;
        prev_node = current_node;
        current_node = next_node;
    }

    // Update the head of the list to the last node (after reversal)
    list->head = prev_node;
}

// Documented in .h file
void CL_foreach(CList list, CL_foreach_callback callback, void *cb_data)
{
    assert(list);

    struct _cl_node *current_node = list->head;
    int num = 0;
    while (current_node != NULL)
    {
        callback(num, current_node->element, cb_data);
        current_node = current_node->next;
        num++;
    }
}
