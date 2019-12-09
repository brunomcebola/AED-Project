/*******************************************************************************
 * 2019-2020 AED - grupo 65
 * Last modified: 2019-12-09
 *
 * NAME
 *      sort.c
 *
 * DESCRIPTION
 *      Implementation of the functions used to sort the linked list
 *      used during the program execution according to a specif order
 *
 * COMMENTS
 *      none
 *
 ******************************************************************************/

#include <stdlib.h>
#include "solver.h"

TreeNode* SortedMerge(TreeNode* a, TreeNode* b);
void FrontBackSplit(TreeNode* source,
                    TreeNode** frontRef, TreeNode** backRef);

/* sorts the linked list by changing next pointers (not data) */
void MergeSort(TreeNode** headRef)
{
    TreeNode* head = *headRef;
    TreeNode* a;
    TreeNode* b;

    /* Base case -- length 0 or 1 */
    if ((head == NULL) || (head->next == NULL)) {
        return;
    }

    /* Split head into 'a' and 'b' sublists */
    FrontBackSplit(head, &a, &b);

    /* Recursively sort the sublists */
    MergeSort(&a);
    MergeSort(&b);

    /* answer = merge the two sorted lists together */
    *headRef = SortedMerge(a, b);
}

/* See https:// www.geeksforgeeks.org/?p=3622 for details of this
function */
TreeNode* SortedMerge(TreeNode* a, TreeNode* b)
{
    TreeNode* result = NULL;

    /* Base cases */
    if (a == NULL)
        return (b);
    else if (b == NULL)
        return (a);

    /* Pick either a or b, and recur */
    if (a->num_playables <= b->num_playables) {
        result = a;
        result->next = SortedMerge(a->next, b);
    }
    else {
        result = b;
        result->next = SortedMerge(a, b->next);
    }
    return (result);
}

/* UTILITY FUNCTIONS */
/* Split the nodes of the given list into front and back halves,
    and return the two lists using the reference parameters.
    If the length is odd, the extra node should go in the front list.
    Uses the fast/slow pointer strategy. */
void FrontBackSplit(TreeNode* source,
                    TreeNode** frontRef, TreeNode** backRef)
{
    TreeNode* fast;
    TreeNode* slow;
    slow = source;
    fast = source->next;

    /* Advance 'fast' two nodes, and advance 'slow' one node */
    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    /* 'slow' is before the midpoint in the list, so split it in two
    at that point. */
    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
}
