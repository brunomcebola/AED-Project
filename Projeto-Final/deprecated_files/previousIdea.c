#include "headers/defs.h"


void makeSpotATent(char *tabuleiro, PlayableNode *node, HeadNode **verticals, HeadNode **horizontals, int colunas, int linhas);


/*
*
*     return values: 1 - ok but no changes
*                    2 - ok and changes
*                    0 - not ok
*
* sorts tree list and checks if any trees are either lonely or only have one valid position
*/
int checkLonelyTree(TreeNode** list, HeadNode **verticals, HeadNode **horizontals, int linhas, int colunas, char *tabuleiro) {
    TreeNode *aux;
    PlayableNode *node;
    MergeSort(list);
    aux = *list;
    while ((aux)->num_playables == 0) {
        if ((aux)->hasTent == 0) {
            return 0;
        }
        aux = aux->next;
        if (aux == NULL) {
            break;
        }
    }

    if (aux == NULL) {
        return 1;
    }

    if ((aux)->num_playables == 1) {

        if ((aux)->North == 1) {
            node = (*horizontals[(aux->y)-1]).first;
            while (node->x != aux->x) {
                node = node->horizontal_next;
            }
            makeSpotATent(tabuleiro, node, verticals, horizontals, colunas, linhas);
        } else if ((aux)->South == 1) {
            node = (*horizontals[(aux->y)+1]).first;
            while (node->x != aux->x) {
                node = node->horizontal_next;
            }
            makeSpotATent(tabuleiro, node, verticals, horizontals, colunas, linhas);
        } else if ((aux)->West == 1) {
            node = (*horizontals[(aux->y)]).first;
            while (node->x != aux->x-1) {
                node = node->horizontal_next;
            }
            makeSpotATent(tabuleiro, node, verticals, horizontals, colunas, linhas);
        } else {
            node = (*horizontals[(aux->y)]).first;
            while (node->x != aux->x+1) {
                node = node->horizontal_next;
            }
            makeSpotATent(tabuleiro, node, verticals, horizontals, colunas, linhas);
        }

        aux->hasTentAssigned = 1;
        /* kkkk: insert save change func */

        return 2;
    }
    return 1;
}


/*
*   return 1 for removed, 0 for not removed
*   checks if a P position is valid, if it is invalidate it and save change
*
*/

int removeFromValidPositions(HeadNode* horizontals, int x, int y) {
    PlayableNode *aux = horizontals[x].first;
    while (aux->x != x) {
        aux = aux->horizontal_next;
    }
    if (aux->valid != 0) {
        aux->valid = 0;
        /* kkkk: insert save change func */
        return 1;
    }
    return 0;
}


/* marks a P position as a tent and invalidates any other 7 positions
* kkkk: modify it as needed
*/
void makeSpotATent(char *tabuleiro, PlayableNode *node, HeadNode **verticals, HeadNode **horizontals, int colunas, int linhas) {
    int verticalChange = 1, horizontalChange = 1, index = (node->y*colunas) + node->x;
    int x = node->x, y = node->y;
    node->isTent = 1;
    if (node->connectedForwardVertical) {
        if (node->vertical_next->valid != 0) {
            node->vertical_next->valid = 0;
            ++verticalChange;
            /* kkkk: insert save change func */
        }
    }
    if (node->connectedBackwardVertical) {
        if (node->vertical_prev->valid != 0) {
            node->vertical_prev->valid = 0;
            ++verticalChange;
            /* kkkk: insert save change func */
        }
    }
    if (node->connectedForwardHorizontal) {
        if (node->horizontal_next->valid != 0) {
            node->horizontal_next->valid = 0;
            ++horizontalChange;
            /* kkkk: insert save change func */
        }
    }
    if (node->connectedBackwardHorizontal) {
        if (node->horizontal_prev->valid != 0) {
            node->horizontal_prev->valid = 0;
            ++horizontalChange;
            /* kkkk: insert save change func */
        }
    }
    if (y != 0) {
        if (x != 0) {
            if (tabuleiro[index-colunas-1] == 'P') {
                if(removeFromValidPositions(*horizontals, x-1, y-1)) {
                    --(*horizontals[x-1]).availablePositions;
                    --(*verticals[y-1]).availablePositions;
                }
                /* kkkk: insert save change func */
            }
        }
        if (x != colunas-1) {
            if (tabuleiro[index-colunas+1] == 'P') {
                if(removeFromValidPositions(*horizontals, x+1, y-1)) {
                    --(*horizontals[x+1]).availablePositions;
                    --(*verticals[y-1]).availablePositions;
                }
                /* kkkk: insert save change func */
            }
        }
    }
    if (y != linhas-1) {
        if (x != 0) {
            if (tabuleiro[index+colunas-1] == 'P') {
                if(removeFromValidPositions(*horizontals, x-1, y+1)) {
                    --(*horizontals[x-1]).availablePositions;
                    --(*verticals[y+1]).availablePositions;
                }
                /* kkkk: insert save change func */
            }
        }
        if (x != colunas-1) {
            if (tabuleiro[index+colunas+1] == 'P') {
                if(removeFromValidPositions(*horizontals, x+1, y+1)) {
                    --(*horizontals[x+1]).availablePositions;
                    --(*verticals[y+1]).availablePositions;
                }
                /* kkkk: insert save change func */
            }
        }
    }

    (*verticals[x]).availablePositions -= verticalChange;
    --(*verticals[x]).tentsNeeded;
    --(*horizontals[y]).tentsNeeded;
    (*horizontals[y]).availablePositions -= horizontalChange;
}


/* frees graph of possible locations */
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

/* frees list of freeTreeList
* kkkk: modify it as needed */
void freeTreeList(TreeNode *list) {
    TreeNode *aux1 = NULL, *aux2 = NULL;

    aux1 = list;
    while(aux1 != NULL) {
        aux2 = aux1;
        aux1 = aux1->next;
        free(aux2);
    }
}




/*
*
* direction: 1 for horizontal, 0 for vertical
* kkkk: modificar a funcao dependendo do que for preciso na funcao de baixo
*/
/*void addAtEnd(HeadNode *headVertical, HeadNode *headHorizontal, PlayableNode *toInsert, int colunas) {
	PlayableNode *auxVertical = headVertical->first, *auxHorizontal = headHorizontal->first;

    if (headVertical->first == NULL) {
        headVertical->first = toInsert;
        headVertical->availablePositions = 1;
    } else {
        while (auxVertical->vertical_next != NULL) {
            auxVertical = auxVertical->vertical_next;
        }
        auxVertical->vertical_next = toInsert;
        toInsert->vertical_prev = auxVertical;
        if (auxVertical->y == (toInsert->y) -colunas) {
            auxVertical->connectedForwardVertical = 1;
            toInsert->connectedBackwardVertical = 1;
        }
        ++(headVertical->availablePositions);
    }

    if (headHorizontal->first == NULL) {
        headHorizontal->first = toInsert;
        headHorizontal->availablePositions = 1;
    } else {
        while (auxHorizontal->horizontal_next != NULL) {
            auxHorizontal = auxHorizontal->horizontal_next;
        }
        auxHorizontal->horizontal_next = toInsert;
        toInsert->horizontal_prev = auxHorizontal;
        if (auxVertical->x == (toInsert->x) -1) {
            auxVertical->connectedForwardHorizontal = 1;
            toInsert->connectedBackwardHorizontal = 1;
        }
        ++(headHorizontal->availablePositions);
    }
}*/
