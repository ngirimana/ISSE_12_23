/*
 * clist.h
 *
 * Linked list implementation for ISSE Assignment 5
 *
 * Author: Ngirimana Schadrack <schadran@andrew.cmu.edu>
 *
 */
#ifndef _CLIST_H_
#define _CLIST_H_

#include <stdbool.h>
#include "token.h"

    // The element type for this list
    typedef Token CListElementType;

struct _cl_node
{
    CListElementType element;
    struct _cl_node *next;
};

struct _clist
{
    struct _cl_node *head;
    int length;
};

// struct _clist to be used in the .c as CList
typedef struct _clist *CList;

// Indicates an error on some functions
#define INVALID_RETURN ((CListElementType){TOK_END})

/*
 * Create a new CList
 *
 * Parameters: None
 *
 * Returns: The new list
 */
CList CL_new();

/*
 * Destroy a list, calling free() on all malloc'd memory.
 *
 * Parameters:
 *   list   The list
 *
 * Returns: None
 */
void CL_free(CList list);

/*
 * Compute the length of a list
 *
 * Parameters:
 *   list   The list
 *
 * Returns: The length of the list, or 0 if list is empty
 */
int CL_length(CList list);

/*
 * Insert the specified element onto the head of the list.
 *
 * Parameters:
 *   list     The list
 *   element  The element to insert
 *
 * Returns: None
 */
void CL_push(CList list, CListElementType element);

/*
 * Remove the element from the head of the list and return
 * it. If the list is empty, return INVALID_RETURN.
 *
 * Parameters:
 *   list     The list
 *
 * Returns: The popped item
 */
CListElementType CL_pop(CList list);

/*
 * Append the specfied element to the tail of the list
 *
 * Parameters:
 *   list     The list
 *   element  The element to append
 *
 * Returns: None
 */
void CL_append(CList list, CListElementType element);

/*
 * Return the Nth element, without modifying the list
 *
 * Parameters:
 *   list     The list
 *   pos      Position to return
 *
 * If pos >= 0, the corresponding element will be returned, counting 0
 * as the head element.  So pos == 0 will return the head element, and
 * pos == 1 will return the second element on the list.
 *
 * If pos <= -1, the corresponding element counting from the end of
 * the list will be returned, so for instance pos == -1 will return the
 * tail element and pos == -2 will return the element before the tail
 * element.
 *
 * pos must be in the range [-length, length-1] inclusive. If pos is
 * outside this range, returns INVALID_RETURN.
 *
 * Returns: The requested element, or INVALID_RETURN if no element was found.
 */
CListElementType CL_nth(CList list, int pos);

/*
 * Insert the specified element onto the list at a given position.
 *
 * Parameters:
 *   list     The list
 *   element  The element to insert
 *   pos      Position to perform the insert
 *
 * If pos >= 0, the element is inserted before pos, such that a
 * subsequent call to CL_nth with the same position will return
 * element. So, if pos == 0, element will be inserted at the beginning
 * of the list.
 *
 * If pos < -1, the element will be inserted at the corresponding
 * position counting from the end of the list, so pos == -1 will
 * append element to the tail of the list, while pos == -2 will insert
 * element at the pentultimate position in list.
 *
 * pos must be in the range [-length-1, length] inclusive. If pos is
 * outside this range, returns false.
 *
 * Returns: true if the operation was successful, false otherwise
 */
bool CL_insert(CList list, CListElementType element, int pos);

/*
 * Remove an element from the specified position and return it.
 *
 * Parameters:
 *   list     The list
 *   pos      Position to perform the removal
 *
 * If pos >= 0, the corresponding element will be removed, counting 0
 * as the head element.  So pos == 0 will remove and return the head
 * element, and pos == 1 will remove the second element on the list.
 *
 * If pos <= -1, the corresponding element counting from the end of
 * the list will be removed and returned, so for instance pos == -1
 * will remove the tail element and pos == -2 will remove the element
 * before the tail element.
 *
 * pos must be in the range [-length, length-1] inclusive. If pos is
 * outside this range, returns INVALID_RETURN.
 *
 * Returns: The element that was removed, or INVALID_RETURN if no
 *   element was removed.
 */
CListElementType CL_remove(CList list, int pos);

/*
 * Copy the list.
 *
 * A new list is allocated and must be destroyed by the caller. To be
 * clear, this is a true copy: Changes to the copy will not affect the
 * original, and vice versa.
 *
 * Parameters:
 *   list     The list to copy
 *
 * Returns:  A new list, which is a copy of the argument.
 */
CList CL_copy(CList list);

/*
 * Join (concatenate) two lists. The contents of list2 are appended
 * to list1. After this operation, list2 will still exist, but it will
 * be empty (length == 0).
 *
 * Example: If list1 = A B C D and list2 = X Y Z, after CL_join
 * returns, list1 will contain A B C D X Y Z and list2 will be empty.
 *
 * Parameters:
 *   list1     First list, which will grow in size
 *   list2     Second list, which will be destroyed.
 *
 * Returns: None
 */
void CL_join(CList list1, CList list2);

/*
 * Reverse a list.  Specifically, if the original list contained
 * A B C D (in that order), after a call to CL_reverse, the list
 * will contain D C B A.
 *
 * Parameters:
 *   list     The list
 *
 * Returns: None
 */
void CL_reverse(CList list);

typedef void (*CL_foreach_callback)(int pos, CListElementType element, void *cb_data);
/*
 * Iterate through the list; call the user-specified callback function
 * for each element.  Each call to callback will be of the form
 *
 *   callback( <element's position>, <element>, <cb_data> )
 *
 * Parameters:
 *   list       The list
 *   callback   The function to call
 *   cb_data    Caller data to pass to the function
 *
 * Returns: None
 */
void CL_foreach(CList list, CL_foreach_callback callback, void *cb_data);

#endif /* _CLIST_H_ */