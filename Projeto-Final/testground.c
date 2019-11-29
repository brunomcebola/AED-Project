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

/*
changeNodeSelector:
    1 - PlayableNode
    2 - TreeNode
    3 - HeadNode

valueID:
    PlayableNode:
        1 - valid
        2 - isTent
    TreeNode:
        1 - hasTent
        2 - num_playables
    HeadNode:
        1 - tentsNeeded
        2 - availablePositions

*/

typedef union {
    PlayableNode *P;
    TreeNode *T;
    HeadNode *H;
} changedNode;

typedef struct _changeStore {
    struct _changeStore *prevChange;
    changedNode changedNode;
    int changeNodeSelector;
    int previousValue;
    int valueID;
} changeStore;

void pushChange(changeStore *changeStorePtr, void *ptr, int changeNodeSelector, int previousValue, int valueID) {
    changeStore *new = (changeStore *) malloc(sizeof(changeStore));
    new -> prevChange = changeStorePtr;
    new -> changeNodeSelector = changeNodeSelector;
    switch(changeNodeSelector) {
        case 1:
            new -> changedNode.P = (PlayableNode*) ptr;
            break;
        case 2:
            new -> changedNode.T = (TreeNode*) ptr;
            break;
        case 3:
            new -> changedNode.H = (HeadNode*) ptr;
            break;
    }
    new -> previousValue = previousValue;
    new -> valueID = valueID;
    changeStorePtr = new;
}

void deleteChanges(changeStore *changeStorePtr) {
    PlayableNode *playAux = NULL;
    TreeNode *treeAux = NULL;
    HeadNode *headAux = NULL;

    while(changeStorePtr != NULL) {
        changeStore *current = changeStorePtr;
        changeStorePtr = changeStorePtr -> prevChange;
        switch (current -> changeNodeSelector) {
            case 1:
                playAux = current -> changedNode.P;
                switch (current -> valueID) {
                    case 1:
                        playAux -> valid = current -> previousValue;
                        break;
                    case 2:
                        playAux -> isTent = current -> previousValue;
                        break;
                }
                break;
            case 2:
                treeAux = current -> changedNode.T;
                switch (current -> valueID) {
                    case 1:
                        treeAux -> hasTent = current -> previousValue;
                        break;
                    case 2:
                        treeAux -> num_playables = current -> previousValue;
                        break;
                }
                break;
            case 3:
                headAux = current -> changedNode.H;
                switch (current -> valueID) {
                    case 1:
                        headAux -> tentsNeeded = current -> previousValue;
                        break;
                    case 2:
                        headAux -> availablePositions = current -> previousValue;
                        break;
                }
                break;
        }
        free(current);
    }
}







void MergeSort(TreeNode**);


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

        aux->hasTent = 1;
        /* TODO: insert save change func */

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
        /* TODO: insert save change func */
        return 1;
    }
    return 0;
}


/* marks a P position as a tent and invalidates any other 7 positions
* TODO: modify it as needed
*/
void makeSpotATent(char *tabuleiro, PlayableNode *node, HeadNode **verticals, HeadNode **horizontals, int colunas, int linhas) {
    int verticalChange = 1, horizontalChange = 1, index = (node->y*colunas) + node->x;
    int x = node->x, y = node->y;
    node->isTent = 1;
    if (node->connectedForwardVertical) {
        if (node->vertical_next->valid != 0) {
            node->vertical_next->valid = 0;
            ++verticalChange;
            /* TODO: insert save change func */
        }
    }
    if (node->connectedBackwardVertical) {
        if (node->vertical_prev->valid != 0) {
            node->vertical_prev->valid = 0;
            ++verticalChange;
            /* TODO: insert save change func */
        }
    }
    if (node->connectedForwardHorizontal) {
        if (node->horizontal_next->valid != 0) {
            node->horizontal_next->valid = 0;
            ++horizontalChange;
            /* TODO: insert save change func */
        }
    }
    if (node->connectedBackwardHorizontal) {
        if (node->horizontal_prev->valid != 0) {
            node->horizontal_prev->valid = 0;
            ++horizontalChange;
            /* TODO: insert save change func */
        }
    }
    if (y != 0) {
        if (x != 0) {
            if (tabuleiro[index-colunas-1] == 'P') {
                if(removeFromValidPositions(*horizontals, x-1, y-1)) {
                    --(*horizontals[x-1]).availablePositions;
                    --(*verticals[y-1]).availablePositions;
                }
                /* TODO: insert save change func */
            }
        }
        if (x != colunas-1) {
            if (tabuleiro[index-colunas+1] == 'P') {
                if(removeFromValidPositions(*horizontals, x+1, y-1)) {
                    --(*horizontals[x+1]).availablePositions;
                    --(*verticals[y-1]).availablePositions;
                }
                /* TODO: insert save change func */
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
                /* TODO: insert save change func */
            }
        }
        if (x != colunas-1) {
            if (tabuleiro[index+colunas+1] == 'P') {
                if(removeFromValidPositions(*horizontals, x+1, y+1)) {
                    --(*horizontals[x+1]).availablePositions;
                    --(*verticals[y+1]).availablePositions;
                }
                /* TODO: insert save change func */
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
* TODO: modify it as needed */
void freeTreeList(TreeNode *list) {
    TreeNode *aux1 = NULL, *aux2 = NULL;

    aux1 = list;
    while(aux1 != NULL) {
        aux2 = aux1;
        aux1 = aux1->next;
        free(aux2);
    }
}


/* If a row or column has 0 tents, it fills that row/column with 0 so no P positions are wrongly marked */
void eliminateInvalidRowsANdColumns(char *tabuleiro, int linhas, int colunas, HeadNode *horizontals, HeadNode *verticals) {
    int i, j, index = 0;
    for (i = 0; i < linhas; ++i) {
        if (horizontals[i].puzzleTents == 0) {
            index = i*colunas;
            for (j = 0; j < colunas; ++j, ++index) {
                if (tabuleiro[index] != 'A') {
                    tabuleiro[index] = '0';
                }
            }
        }
    }
    for (i = 0; i < linhas; ++i) {
        if (horizontals[i].puzzleTents == 0) {
            index = i;
            for (j = 0; j < colunas; ++j, index += colunas) {
                if (tabuleiro[index] != 'A') {
                    tabuleiro[index] = '0';
                }
            }
        }
    }
}

/* Restores all 0 values to . as they should, call it only befor printing */
void restore0ValuesToDots(char *tabuleiro, int linhas, int colunas, HeadNode *horizontals, HeadNode *verticals) {
    int i, j, index = 0;
    for (i = 0; i < linhas; ++i) {
        if (horizontals[i].puzzleTents == 0) {
            index = i*colunas;
            for (j = 0; j < colunas; ++j, ++index) {
                if (tabuleiro[index] == '0') {
                    tabuleiro[index] = '.';
                }
            }
        }
    }
    for (i = 0; i < linhas; ++i) {
        if (horizontals[i].puzzleTents == 0) {
            index = i;
            for (j = 0; j < colunas; ++j, index += colunas) {
                if (tabuleiro[index] == '0') {
                    tabuleiro[index] = '.';
                }
            }
        }
    }
}


/* Fids all P postions that are not 0 */
void findPossibleLocations(char *tabuleiro, int linhas, int colunas, HeadNode *horizontals, HeadNode *verticals) {

	int i, j, index = 0;

	for (i = 0; i < linhas; ++i) {

		for (j = 0; j < colunas; ++j, ++index) {

			if (tabuleiro[index] == 'A') {

                if (j != 0) {
                    if (tabuleiro[index-1] == '.') {
    					tabuleiro[index-1] = 'P';
    				}
                }

                if (j != colunas-1) {
                    if (tabuleiro[index+1] == '.') {
    					tabuleiro[index+1] = 'P';
    				}
                }


                if (i != 0) {
                    if (tabuleiro[index-colunas] == '.') {
    					tabuleiro[index-colunas] = 'P';
    				}
                }
                if (i != linhas-1) {
                    if (tabuleiro[index+colunas] == '.') {
    					tabuleiro[index+colunas] = 'P';

    				}
                }
			}

		}
	}
}



/*
*
* direction: 1 for horizontal, 0 for vertical
* TODO: modificar a funcao dependendo do que for preciso na funcao de baixo
*/
void addAtEnd(HeadNode *headVertical, HeadNode *headHorizontal, PlayableNode *toInsert, int colunas) {
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
}


//cria o grapho tanto com as tendas como com as arvores
void createGraph(char *tabuleiro, int linhas, int colunas, HeadNode *horizontals, HeadNode *verticals) {
    int j = 0, i = 0, linha_atual = 0;
    PlayableNode *newNode = NULL;

    for (i = 0; i < linhas; i++, linha_atual += colunas) {

        /* MODIFIED: Nao podemos saltar colunas ou linhas pq podem la estar arvores */
        for (j = 0; j < colunas; j++) {

            if (tabuleiro[linha_atual+j] == 'P') {
                newNode = (PlayableNode *) malloc(sizeof(PlayableNode));
                newNode->x = j;
                newNode->y = i;
                newNode->valid = 1;
                newNode->horizontal_next = NULL;
                newNode->vertical_next = NULL;
                newNode->horizontal_prev = NULL;
                newNode->vertical_prev = NULL;
                newNode->isTent = 0;
                addAtEnd(&(horizontals[i]), &(verticals[j]),newNode, colunas);
            } else if (tabuleiro[linha_atual+j] == 'A') {

            }
        }
    }
}
