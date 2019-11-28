#include <stdlib.h>
#include <stdio.h>

#include "headers/defs.h"

typedef struct {
    int n_rows, n_columns;
    int *n_el_row, *n_el_column;
    char *buffer;
    char *layout;
    HeadNode *horizontals;
    HeadNode *verticals;
    int answer;
    int sum;
    int season;
} board;



void freePossibleLocations(HeadNode *horizontal, int colunas) {
    int i = colunas+1;
    PlayableNode *aux1 = NULL, *aux2 = NULL;

    while (--i) {
        aux1 = horizontal[i].first;
        while(aux1 != NULL) {
            aux2 = aux1;
            aux1 = aux1->vertical_next;
            free(aux2);
        }
    }
}


void freeTreeList(TreeNode *list) {
    TreeNode *aux1 = NULL, *aux2 = NULL;

    aux1 = list;
    while(aux1 != NULL) {
        aux2 = aux1;
        aux1 = aux1->next;
        free(aux2);
    }
}



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



TreeNode * findPossibleLocations(char *tabuleiro, int linhas, int colunas, HeadNode *horizontals, HeadNode *verticals) {

	int i, j, linha_atual = 0, numPlayables;
    TreeNode *list = NULL, *newTree = NULL;

	for (i = 0; i < linhas; i++, linha_atual += colunas) {

		for (j = 0; j < colunas; j++) {

			if (tabuleiro[linha_atual+j] == 'A') {

                numPlayables = 0;
                newTree = (TreeNode *) malloc(sizeof(TreeNode));
                checkNull(1, newTree);
                newTree->West = 0;
                newTree->East = 0;
                newTree->North = 0;
                newTree->South = 0;

                if (horizontals[i].puzzleTents != 0) {
                    if (tabuleiro[(linha_atual+j)-1] != 'A') {
    					tabuleiro[(linha_atual+j)-1] = 'P';
                        numPlayables++;
                        newTree->West = 1;
    				}

    				if (tabuleiro[(linha_atual+j)+1] != 'A') {
    					tabuleiro[(linha_atual+j)+1] = 'P';
                        numPlayables++;
                        newTree->East = 1;
    				}
                }
				if (verticals[j].puzzleTents != 0) {
                    if (tabuleiro[(linha_atual+j)-colunas] != 'A') {
    					tabuleiro[(linha_atual+j)-colunas] = 'P';
                        numPlayables++;
                        newTree->North = 1;
    				}

    				if (tabuleiro[(linha_atual+j)+colunas] != 'A') {
    					tabuleiro[(linha_atual+j)+colunas] = 'P';
                        numPlayables++;
                        newTree->South = 1;
    				}
                }
                newTree->x = j;
                newTree->y = i;
                newTree->num_playables = numPlayables;
                newTree->next = list;
                list = newTree;
			}

		}
	}
    return list;
}



/*
*
* direction: 1 for horizontal, 0 for vertical
*
*/
void addAtEnd(HeadNode *headVertical, HeadNode *headHorizontal, PlayableNode *toInsert) {
	PlayableNode *auxVertical = headVertical->first, *auxHorizontal = headHorizontal->first;

    if (headVertical->first == NULL) {
        headVertical->first = toInsert;
    } else {
        while (auxVertical->vertical_next != NULL) {
            auxVertical = auxVertical->vertical_next;
        }
        auxVertical->vertical_next = toInsert;
        toInsert->vertical_prev = auxVertical;
    }

    if (headHorizontal->first == NULL) {
        headHorizontal->first = toInsert;
    } else {
        while (auxHorizontal->horizontal_next != NULL) {
            auxHorizontal = auxHorizontal->horizontal_next;
        }
        auxHorizontal->horizontal_next = toInsert;
        toInsert->horizontal_prev = auxHorizontal;
    }
}


void createGraph(char *tabuleiro, int linhas, int colunas, HeadNode *horizontals, HeadNode *verticals) {
    int j = 0, i = 0, linha_atual = 0;
    PlayableNode *newNode = NULL;

    for (i = 0; i < linhas; i++, linha_atual += colunas) {

        if (horizontals[i].puzzleTents == 0) {
            continue;
        }

        for (j = 0; j < colunas; j++) {

            if (verticals[j].puzzleTents == 0) {
                continue;
            }

            if (tabuleiro[linha_atual+j] == 'P') {
                newNode = (PlayableNode *) malloc(sizeof(PlayableNode));
                newNode->x = j;
                newNode->y = i;
                newNode->valid = 0;
                newNode->horizontal_next = NULL;
                newNode->vertical_next = NULL;
                newNode->horizontal_prev = NULL;
                newNode->vertical_prev = NULL;
                addAtEnd(&(horizontals[i]), &(verticals[j]),newNode);
            }
        }
    }
}
