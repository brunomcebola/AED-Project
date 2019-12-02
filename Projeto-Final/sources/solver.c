#include <stdlib.h>
#include <stdio.h>
#include "../headers/bundle.h"
#include "../headers/game.h"
#include "../headers/solver.h"
#include "../headers/sort.h"
#include "../headers/stack.h"
#include "../headers/files.h" /* TODO: remove this include after debugging */


/* TODO: eliminate all PlayableNode stuff it won't be used */


//TODO: perguntar ao stor

struct _PlayableNode {
    int x, y;
    int valid;
    PlayableNode *horizontal_next;
    PlayableNode *vertical_next;
};


void removesP(TreeNode ***, char *, int, int, int, int, int, int, changeStore **);



HeadNode *row_vector = NULL, *column_vector = NULL;

HeadNode *getSolverVectorRow(void) {
    return row_vector;
}

HeadNode *getSolverVectorColumn(void) {
    return column_vector;
}

void setSolverVectors(HeadNode* row, HeadNode* column) {
    row_vector = row;
    column_vector = column;
}



//solver functions
int findPossibleLocations(char *tabuleiro, int linhas, int colunas) {

	int i, j, index = 0, numOfTrees = 0, numOfAskedTents = getBoardSum(), retVal;

	for (i = 0; i < linhas; ++i) {

		for (j = 0; j < colunas; ++j, ++index) {
			if (tabuleiro[index] == 'A') {

                ++numOfTrees;

                if (j != 0) {
                    if (tabuleiro[index-1] == '.' && column_vector[j-1].puzzleTents != 0 && row_vector[i].puzzleTents != 0) {
    					tabuleiro[index-1] = 'P';
                        ++(column_vector[j-1].availablePositions);
                        ++(row_vector[i].availablePositions);
    				}
                }

                if (j != colunas-1) {
                    if (tabuleiro[index+1] == '.' && column_vector[j+1].puzzleTents != 0 && row_vector[i].puzzleTents != 0) {
    					tabuleiro[index+1] = 'P';
                        ++(column_vector[j+1].availablePositions);
                        ++(row_vector[i].availablePositions);
    				}
                }

                if (i != 0) {
                    if (tabuleiro[index-colunas] == '.' && column_vector[j].puzzleTents != 0 && row_vector[i-1].puzzleTents != 0) {
    					tabuleiro[index-colunas] = 'P';
                        ++(column_vector[j].availablePositions);
                        ++(row_vector[i-1].availablePositions);
    				}
                }

                if (i != linhas-1) {
                    if (tabuleiro[index+colunas] == '.' && column_vector[j].puzzleTents != 0 && row_vector[i+1].puzzleTents != 0) {
    					tabuleiro[index+colunas] = 'P';
                        ++(column_vector[j].availablePositions);
                        ++(row_vector[i+1].availablePositions);
    				}
                }
			}

		}
	}

    if (numOfTrees == numOfAskedTents) {
        setBoardSeason(1);
        retVal = 1;
    } else if (numOfTrees > numOfAskedTents) {
        setBoardSeason(2);
        retVal = 2;
    } else {
        return 0;
    }

    return retVal;
}


TreeNode ***createTreeInfo(int colunas, int linhas) {
    TreeNode ***TreeInfo = NULL;
    TreeInfo = (TreeNode ***) malloc(linhas * sizeof(TreeNode **));
    checkNull(1, TreeInfo);
    for (int i = 0; i < linhas; ++i) {
        TreeInfo[i] = (TreeNode **) malloc(colunas * sizeof(TreeNode *));
        checkNull(1, TreeInfo[i]);
        for (int j = 0; j < colunas; j++) {
            TreeInfo[i][j] = NULL;
        }
    }
    return TreeInfo;
}

TreeNode *createTreeList(char *tabuleiro, int linhas, int colunas, TreeNode ***treesInfo) {
    int j = 0, i = 0, index = 0;
    TreeNode *list = NULL;

    for (i = 0; i < linhas; ++i) {

        for (j = 0; j < colunas; ++j, ++index) {

            if (tabuleiro[index] == 'A') {
                treesInfo[i][j] = (TreeNode *) malloc(sizeof(TreeNode));
                checkNull(1, treesInfo[i][j]);
                treesInfo[i][j]->x = j;
                treesInfo[i][j]->y = i;
                treesInfo[i][j]->hasTentAssigned = 0;
                treesInfo[i][j]->num_playables = 0;
                if (j != 0) {
                    if (tabuleiro[index-1] == 'P') {
    					++(treesInfo[i][j]->num_playables);
    				}
                }

                if (j != colunas-1) {
                    if (tabuleiro[index+1] == 'P') {
    					++(treesInfo[i][j]->num_playables);
    				}
                }


                if (i != 0) {
                    if (tabuleiro[index-colunas] == 'P') {
    					++(treesInfo[i][j]->num_playables);
    				}
                }
                if (i != linhas-1) {
                    if (tabuleiro[index+colunas] == 'P') {
    					++(treesInfo[i][j]->num_playables);

    				}
                }
                treesInfo[i][j]->next = list;
                list = treesInfo[i][j];
            }
        }
    }
    return list;
}


/* checks if P position is left alone
*  returns 0 when not alone, 1 when not alone
*  TODO: create 3 more functions so it reduces number of comparisons (doesn't have to compare with tree removing it)
*/
void checkIfPAlone(TreeNode*** treeInfo, char *tabuleiro, int index, int x, int y, int colunas, int linhas, changeStore **changeStorePtr) {
    if (x != 0) {

        if (tabuleiro[index-1] == 'A') {
            return;
        }
    }

    if (x != colunas-1) {

        if (tabuleiro[index+1] == 'A') {
            return;
        }
    }

    if (y != 0) {

        if (tabuleiro[index-colunas] == 'A') {
            return;
        }
    }
    if (y != linhas-1) {

        if (tabuleiro[index+colunas] == 'A') {
            return;

        }
    }
    tabuleiro[index] = '.';
    pushChange(changeStorePtr, NULL, x, y, 1, 0, 'P', 1);
    removesP(treeInfo, tabuleiro, linhas, colunas, index, x, y, 0, changeStorePtr);
}


/*
* returns: no return value
*
*
*/
void invalidateTreePPositions(TreeNode*** treeInfo, char * tabuleiro, int linhas, int colunas, int index, int x, int y, changeStore **changeStorePtr) {
    if (x != 0) {

        if (tabuleiro[index-1] == 'P') {
        checkIfPAlone(treeInfo, tabuleiro, index-1, x-1, y, colunas, linhas, changeStorePtr);
        }
    }

    if (x != colunas-1) {

        if (tabuleiro[index+1] == 'P') {
            checkIfPAlone(treeInfo, tabuleiro, index+1, x+1, y, colunas, linhas, changeStorePtr);
        }
    }

    if (y != 0) {

        if (tabuleiro[index-colunas] == 'P') {
            checkIfPAlone(treeInfo, tabuleiro, index-colunas, x, y-1, colunas, linhas, changeStorePtr);
        }
    }
    if (y != linhas-1) {

        if (tabuleiro[index+colunas] == 'P') {
            checkIfPAlone(treeInfo, tabuleiro, index+colunas, x, y+1, colunas, linhas, changeStorePtr);

        }
    }
}


/*
* returns: no return value
*
*
*/
void assignsTentToATree(TreeNode*** treeInfo, char * tabuleiro, int linhas, int colunas, int index, int x, int y, int test, changeStore **changeStorePtr) {
    int numOfTrees = 0;

    if (x != 0) {
        if (tabuleiro[index-1] == 'A') {
            if (test) {
                ++numOfTrees;
            } else {
                tabuleiro[index-1] = 'K';
                pushChange(changeStorePtr, NULL, x-1, y, 1, 0, 'A', 1);
                (treeInfo[y][x-1]->hasTentAssigned) = 1;
                pushChange(changeStorePtr, treeInfo[y][x-1], 0, 0, 2, 0, '\0', 1);
                invalidateTreePPositions(treeInfo, tabuleiro, linhas, colunas, index-1, x-1, y, changeStorePtr);
            }
        }
    }

    if (x != colunas-1) {
        if (tabuleiro[index+1] == 'A') {
            if (test) {
                ++numOfTrees;
            } else {
                tabuleiro[index+1] = 'K';
                pushChange(changeStorePtr, NULL, x+1, y, 1, 0, 'A', 1);
                (treeInfo[y][x+1]->hasTentAssigned) = 1;
                pushChange(changeStorePtr, treeInfo[y][x+1], 0, 0, 2, 0, '\0', 1);
                invalidateTreePPositions(treeInfo, tabuleiro, linhas, colunas, index+1, x+1, y, changeStorePtr);
            }
        }
    }

    if (y != 0) {
        if (tabuleiro[index-colunas] == 'A') {
            if (test) {
                ++numOfTrees;
            } else {
                tabuleiro[index-colunas] = 'K';
                pushChange(changeStorePtr, NULL, x, y-1, 1, 0, 'A', 1);
                (treeInfo[y-1][x]->hasTentAssigned) = 1;
                pushChange(changeStorePtr, treeInfo[y-1][x], 0, 0, 2, 0, '\0', 1);
                invalidateTreePPositions(treeInfo, tabuleiro, linhas, colunas, index-colunas, x, y-1, changeStorePtr);
            }
        }
    }

    if (y != linhas-1) {
        if (tabuleiro[index+colunas] == 'A') {
            if (test) {
                ++numOfTrees;
            } else {
                tabuleiro[index+colunas] = 'K';
                pushChange(changeStorePtr, NULL, x, y+1, 1, 0, 'A', 1);
                (treeInfo[y+1][x]->hasTentAssigned) = 1;
                pushChange(changeStorePtr, treeInfo[y+1][x], 0, 0, 2, 0, '\0', 1);
                invalidateTreePPositions(treeInfo, tabuleiro, linhas, colunas, index+colunas, x, y+1, changeStorePtr);
            }
        }
    }
    if (numOfTrees == 1) {
        assignsTentToATree(treeInfo, tabuleiro, linhas, colunas, index, x, y, 0, changeStorePtr);
    } else if (test) {
        tabuleiro[index] = 'V';
    }
}





void removesP(TreeNode*** treeInfo, char * tabuleiro, int linhas, int colunas, int index, int x, int y, int isTent, changeStore **changeStorePtr) {
    int previousValue = 0;

    if (isTent) {
        previousValue = (row_vector[y].tentsNeeded);
        (row_vector[y].tentsNeeded)--;
        pushChange(changeStorePtr, &(row_vector[y]), 0, 0, 3, previousValue, '\0', 1);
        previousValue = (column_vector[x].tentsNeeded);
        (column_vector[x].tentsNeeded)--;
        pushChange(changeStorePtr, &(column_vector[x]), 0, 0, 3, previousValue, '\0', 1);
    }

    previousValue = (row_vector[y].availablePositions);
    (row_vector[y].availablePositions)--;
    pushChange(changeStorePtr, &(row_vector[y]), 0, 0, 3, previousValue, '\0', 2);
    previousValue = (column_vector[x].availablePositions);
    (column_vector[x].availablePositions)--;
    pushChange(changeStorePtr, &(column_vector[x]), 0, 0, 3, previousValue, '\0', 2);

    if (x != 0) {
        if (tabuleiro[index-1] == 'A') {
            previousValue = (treeInfo[y][x-1]->num_playables);
            (treeInfo[y][x-1]->num_playables)--;
            pushChange(changeStorePtr, treeInfo[y][x-1], 0, 0, 2, previousValue, '\0', 2);
        } else if (tabuleiro[index-1] == 'P' && isTent) {
            tabuleiro[index-1] = '.';
            pushChange(changeStorePtr, NULL, x-1, y, 1, 0, 'P', 1);
            removesP(treeInfo, tabuleiro, linhas, colunas, index-1, x-1, y, 0, changeStorePtr);
        }
    }

    if (x != colunas-1) {
        if (tabuleiro[index+1] == 'A') {
            previousValue = (treeInfo[y][x+1]->num_playables);
            (treeInfo[y][x+1]->num_playables)--;
            pushChange(changeStorePtr, treeInfo[y][x+1], 0, 0, 2, previousValue, '\0', 2);
        } else if (tabuleiro[index+1] == 'P' && isTent) {
            tabuleiro[index+1] = '.';
            pushChange(changeStorePtr, NULL, x+1, y, 1, 0, 'P', 1);
            removesP(treeInfo, tabuleiro, linhas, colunas, index+1, x+1, y, 0, changeStorePtr);
        }
    }

    if (y != 0) {
        if (tabuleiro[index-colunas] == 'A') {
            previousValue = (treeInfo[y-1][x]->num_playables);
            (treeInfo[y-1][x]->num_playables)--;
            pushChange(changeStorePtr, treeInfo[y-1][x], 0, 0, 2, previousValue, '\0', 2);
        } else if (tabuleiro[index-colunas] == 'P' && isTent) {
            tabuleiro[index-colunas] = '.';
            pushChange(changeStorePtr, NULL, x, y-1, 1, 0, 'P', 1);
            removesP(treeInfo, tabuleiro, linhas, colunas, index-colunas, x, y-1, 0, changeStorePtr);
        }

        if (isTent) {
            if (x != 0) {
                if (tabuleiro[index-colunas-1] == 'P') {
                    tabuleiro[index-colunas-1] = '.';
                    pushChange(changeStorePtr, NULL, x-1, y-1, 1, 0, 'P', 1);
                    removesP(treeInfo, tabuleiro, linhas, colunas, index-colunas-1, x-1, y-1, 0, changeStorePtr);
                }
            }

            if (x != colunas-1) {
                if (tabuleiro[index-colunas+1] == 'P') {
                    tabuleiro[index-colunas+1] = '.';
                    pushChange(changeStorePtr, NULL, x+1, y-1, 1, 0, 'P', 1);
                    removesP(treeInfo, tabuleiro, linhas, colunas, index-colunas+1, x+1, y-1, 0, changeStorePtr);
                }
            }
        }

    }

    if (y != linhas-1) {
        if (tabuleiro[index+colunas] == 'A') {
            previousValue = (treeInfo[y+1][x]->num_playables);
            (treeInfo[y+1][x]->num_playables)--;
            pushChange(changeStorePtr, treeInfo[y+1][x], 0, 0, 2, previousValue, '\0', 2);
        } else if (tabuleiro[index+colunas] == 'P' && isTent) {
            tabuleiro[index+colunas] = '.';
            pushChange(changeStorePtr, NULL, x, y+1, 1, 0, 'P', 1);
            removesP(treeInfo, tabuleiro, linhas, colunas, index+colunas, x, y+1, 0, changeStorePtr);
        }

        if (isTent) {
            if (x != 0 ) {
                if (tabuleiro[index+colunas-1] == 'P') {
                    tabuleiro[index+colunas-1] = '.';
                    pushChange(changeStorePtr, NULL, x-1, y+1, 1, 0, 'P', 1);
                    removesP(treeInfo, tabuleiro, linhas, colunas, index+colunas-1, x-1, y+1, 0, changeStorePtr);
                }
            }

            if (x != colunas-1) {
                if (tabuleiro[index+colunas+1] == 'P') {
                    tabuleiro[index+colunas+1] = '.';
                    pushChange(changeStorePtr, NULL, x+1, y+1, 1, 0, 'P', 1);
                    removesP(treeInfo, tabuleiro, linhas, colunas, index+colunas+1, x+1, y+1, 0, changeStorePtr);
                }
            }
        }
    }
}


/*
* returns: 1 for change, 0 for no change
*
*
*
*/
int checkNeededTents(char * tabuleiro, int linhas, int colunas, TreeNode ***treeInfo, changeStore **changeStorePtr) {
    int i = 0, j = 0, index = 0, retVal = 0, modified = 1;

    for (index = 0, i = 0; i < linhas; ++i, index += colunas) {
        if ((row_vector[i].tentsNeeded == row_vector[i].availablePositions) && row_vector[i].tentsNeeded > 0) {
            retVal = 1;
            for (j = 0; j < colunas; ++j) {
                if (tabuleiro[index+j] == 'P') {
                    tabuleiro[index+j] = 'T';
                    pushChange(changeStorePtr, NULL, j, i, 1, 0, 'P', 1);
                    removesP(treeInfo, tabuleiro, linhas, colunas, index+j, j, i, 1, changeStorePtr);
                    assignsTentToATree(treeInfo, tabuleiro, linhas, colunas, index+j, j, i, 1, changeStorePtr);
                }
            }
            while (modified) {
                modified = 0;
                for (j = 0; j < colunas; ++j) {
                    if (tabuleiro[index+j] == 'V') {
                        tabuleiro[index] = 'T';
                        assignsTentToATree(treeInfo, tabuleiro, linhas, colunas, index+j, j, i, 1, changeStorePtr);
                        if (tabuleiro[index+j] != 'V') {
                            modified = 1;
                        }
                    }
                }
            }
        } else if (row_vector[i].tentsNeeded > row_vector[i].availablePositions) {
            return 404;
        } else if (row_vector[i].availablePositions > 0 && row_vector[i].tentsNeeded == 0) {
            retVal = 1;
            for (j = 0; j < colunas; ++j) {
                if (tabuleiro[index+j] == 'P') {
                    tabuleiro[index+j] = '.';
                    pushChange(changeStorePtr, NULL, j, i, 1, 0, 'P', 1);
                    removesP(treeInfo, tabuleiro, linhas, colunas, index+j, j, i, 0, changeStorePtr);
                }
            }
        }
    }
    modified = 1;
    for (i = 0; i < colunas; ++i) {
        if ((column_vector[i].tentsNeeded == column_vector[i].availablePositions) && column_vector[i].tentsNeeded > 0) {
            retVal = 1;
            for (index = i, j = 0; j < linhas; ++j, index += colunas) {
                if (tabuleiro[index] == 'P') {
                    tabuleiro[index] = 'T';
                    pushChange(changeStorePtr, NULL, i, j, 1, 0, 'P', 1);
                    removesP(treeInfo, tabuleiro, linhas, colunas, index, i, j, 1, changeStorePtr);
                    assignsTentToATree(treeInfo, tabuleiro, linhas, colunas, index, i, j, 1, changeStorePtr);
                }
            }
            while (modified) {
                modified = 0;
                for (index = i, j = 0; j < linhas; ++j, index += colunas) {
                    if (tabuleiro[index] == 'V') {
                        tabuleiro[index] = 'T';
                        assignsTentToATree(treeInfo, tabuleiro, linhas, colunas, index, i, j, 1, changeStorePtr);
                        if (tabuleiro[index] != 'V') {
                            modified = 1;
                        }
                    }
                }
            }
        } else if (column_vector[i].tentsNeeded > column_vector[i].availablePositions) {
            return 404;
        } else if (column_vector[i].availablePositions > 0 && column_vector[i].tentsNeeded == 0) {
            retVal = 1;
            for (index = i, j = 0; j < linhas; ++j, index += colunas) {
                if (tabuleiro[index] == 'P') {
                    tabuleiro[index] = '.';
                    pushChange(changeStorePtr, NULL, i, j, 1, 0, 'P', 1);
                    removesP(treeInfo, tabuleiro, linhas, colunas, index, i, j, 0, changeStorePtr);
                }
            }
        }
    }
    return retVal;
}


/*
* returns: 1 for changes, 0 for no changes, 404 for invalid map
*
*
*/
int checkForLonelyTrees(TreeNode*** treeInfo, TreeNode** list, char * tabuleiro, int linhas, int colunas, changeStore **changeStorePtr) {
    TreeNode *aux;
    int index;
    MergeSort(list);
    aux = *list;

    if (aux == NULL) {
        return 0;
    }

    while ((aux)->num_playables == 0) {
        if (aux->hasTentAssigned == 0) {
            index = (aux->y)*colunas + aux->x;
            if (aux->x != 0) {
                if (tabuleiro[index-1] == 'V') {
                    aux->hasTentAssigned = 1;
                    pushChange(changeStorePtr, aux, 0, 0, 2, 0, '\0', 1);
                    tabuleiro[index] = 'K';
                    pushChange(changeStorePtr, NULL, aux->x, aux->y, 1, 0, 'A', 1);
                    tabuleiro[index-1] = 'T';
                    /* NO SAVES BECAUSE CHANGE HAS ALREADY BEEN SAVED */
                    return 1;
                }
            }

            if (aux->x != colunas-1) {
                if (tabuleiro[index+1] == 'V') {
                    aux->hasTentAssigned = 1;
                    pushChange(changeStorePtr, aux, 0, 0, 2, 0, '\0', 1);
                    tabuleiro[index] = 'K';
                    pushChange(changeStorePtr, NULL, aux->x, aux->y, 1, 0, 'A', 1);
                    tabuleiro[index+1] = 'T';
                    /* NO SAVES BECAUSE CHANGE HAS ALREADY BEEN SAVED */
                    return 1;
                }
            }

            if (aux->y != 0) {
                if (tabuleiro[index-colunas] == 'V') {
                    aux->hasTentAssigned = 1;
                    pushChange(changeStorePtr, aux, 0, 0, 2, 0, '\0', 1);
                    tabuleiro[index] = 'K';
                    pushChange(changeStorePtr, NULL, aux->x, aux->y, 1, 0, 'A', 1);
                    tabuleiro[index-colunas] = 'T';
                    /* NO SAVES BECAUSE CHANGE HAS ALREADY BEEN SAVED */
                    return 1;
                }
            }

            if (aux->y != linhas-1) {
                if (tabuleiro[index+colunas] == 'V') {
                    aux->hasTentAssigned = 1;
                    pushChange(changeStorePtr, aux, 0, 0, 2, 0, '\0', 1);
                    tabuleiro[index] = 'K';
                    pushChange(changeStorePtr, NULL, aux->x, aux->y, 1, 0, 'A', 1);
                    tabuleiro[index+colunas] = 'T';
                    /* NO SAVES BECAUSE CHANGE HAS ALREADY BEEN SAVED */
                    return 1;
                }
            }
            return 404;
        }
        aux = aux->next;
    }

    while (aux->num_playables <= 1 && aux->hasTentAssigned == 1) {
        if (aux->next == NULL) {
            return 0;
        }
        aux = aux->next;
    }

    while (aux != NULL) {
        if ((aux)->num_playables == 1) {

            index = (aux->y)*colunas + aux->x;

            if (aux->x != 0) {
                if (tabuleiro[index-1] == 'V') {
                    aux = aux->next;
                    continue;
                }
            }
            if (aux->x != colunas-1) {
                if (tabuleiro[index+1] == 'V') {
                    aux = aux->next;
                    continue;
                }
            }
            if (aux->y != 0) {
                if (tabuleiro[index-colunas] == 'V') {
                    aux = aux->next;
                    continue;
                }
            }
            if (aux->y != linhas-1) {
                if (tabuleiro[index+colunas] == 'V') {
                    aux = aux->next;
                    continue;
                }
            }


            if (aux->x != 0) {
                if (tabuleiro[index-1] == 'P') {
                    aux->hasTentAssigned = 1;
                    pushChange(changeStorePtr, aux, 0, 0, 2, 0, '\0', 1);
                    tabuleiro[index] = 'K';
                    pushChange(changeStorePtr, NULL, aux->x, aux->y, 1, 0, 'A', 1);
                    tabuleiro[index-1] = 'T';
                    pushChange(changeStorePtr, NULL, aux->x -1, aux->y, 1, 0, 'P', 1);
                    removesP(treeInfo, tabuleiro, linhas, colunas, index-1, aux->x -1, aux->y, 1, changeStorePtr);
                    return 1;
                }
            }

            if (aux->x != colunas-1) {
                if (tabuleiro[index+1] == 'P') {
                    aux->hasTentAssigned = 1;
                    pushChange(changeStorePtr, aux, 0, 0, 2, 0, '\0', 1);
                    tabuleiro[index] = 'K';
                    pushChange(changeStorePtr, NULL, aux->x, aux->y, 1, 0, 'A', 1);
                    tabuleiro[index+1] = 'T';
                    pushChange(changeStorePtr, NULL, aux->x +1, aux->y, 1, 0, 'P', 1);
                    removesP(treeInfo, tabuleiro, linhas, colunas, index+1, aux->x +1, aux->y, 1, changeStorePtr);
                    return 1;
                }
            }

            if (aux->y != 0) {
                if (tabuleiro[index-colunas] == 'P') {
                    aux->hasTentAssigned = 1;
                    pushChange(changeStorePtr, aux, 0, 0, 2, 0, '\0', 1);
                    tabuleiro[index] = 'K';
                    pushChange(changeStorePtr, NULL, aux->x, aux->y, 1, 0, 'A', 1);
                    tabuleiro[index-colunas] = 'T';
                    pushChange(changeStorePtr, NULL, aux->x, aux->y -1, 1, 0, 'P', 1);
                    removesP(treeInfo, tabuleiro, linhas, colunas, index-colunas, aux->x, aux->y -1, 1, changeStorePtr);
                    return 1;
                }
            }

            if (aux->y != linhas-1) {
                if (tabuleiro[index+colunas] == 'P') {
                    aux->hasTentAssigned = 1;
                    pushChange(changeStorePtr, aux, 0, 0, 2, 0, '\0', 1);
                    tabuleiro[index] = 'K';
                    pushChange(changeStorePtr, NULL, aux->x, aux->y, 1, 0, 'A', 1);
                    tabuleiro[index+colunas] = 'T';
                    pushChange(changeStorePtr, NULL, aux->x, aux->y +1, 1, 0, 'P', 1);
                    removesP(treeInfo, tabuleiro, linhas, colunas, index+colunas, aux->x, aux->y +1, 1, changeStorePtr);
                    return 1;
                }
            }

        } else {
            break;
        }
        aux = aux->next;
    }

    return 0;
}

/*
*  returns: 0 for no changes, 1 for changes
*
*
*/
int checkConsecutive(char * tabuleiro, int linhas, int colunas, TreeNode ***treeInfo, changeStore **changeStorePtr) {
    int totalSimultaneousSpots = 0, numOfConsecutives = 0, i, j, index = 0, retVal = 0, flag1 = 0, flag2 = 0, temp = 0;

    for (i = 0; i < linhas; ++i, index += colunas) {
        totalSimultaneousSpots = 0, numOfConsecutives = 0;
        for (j = 0; j < colunas; ++j) {
            if (numOfConsecutives && (tabuleiro[index+j] != 'P')) {
                totalSimultaneousSpots += ((numOfConsecutives+1)>>1);
                numOfConsecutives = 0;
            } else if (tabuleiro[index+j] == 'P') {
                ++numOfConsecutives;
            }
        }

        totalSimultaneousSpots += ((numOfConsecutives+1)>>1);

        if ((totalSimultaneousSpots == row_vector[i].tentsNeeded) && row_vector[i].tentsNeeded != 0) {

            for (j = 0; j < colunas; ++j) {
                if (tabuleiro[index+j] == 'P') {
                    flag1 = 0;
                    temp = j;
                    while (!(tabuleiro[index+j] != 'P')) {
                        ++flag1;
                        if ((++j) == colunas) break;
                    }

                    --j;

                    if (flag1 == 1) {
                        retVal = 1;
                        tabuleiro[index+j] = 'T';
                        pushChange(changeStorePtr, NULL, j, i, 1, 0, 'P', 1);
                        removesP(treeInfo, tabuleiro, linhas, colunas, index+j, j, i, 1, changeStorePtr);
                        assignsTentToATree(treeInfo, tabuleiro, linhas, colunas, index+j, j, i, 1, changeStorePtr);
                    } else if (flag1%2 == 1) {
                        retVal = 1;
                        flag2 = 1;
                        do {
                            if (flag2) {
                                flag2 = -1;
                                tabuleiro[index+j] = 'T';
                                pushChange(changeStorePtr, NULL, j, i, 1, 0, 'P', 1);
                                removesP(treeInfo, tabuleiro, linhas, colunas, index+j, j, i, 1, changeStorePtr);
                                assignsTentToATree(treeInfo, tabuleiro, linhas, colunas, index+j, j, i, 1, changeStorePtr);
                            }
                            --j;
                            ++flag2;
                        } while(--flag1);
                        j = temp;
                    }
                }
            }
        }
    }
    for (i = 0; i < colunas; ++i) {
        totalSimultaneousSpots = 0, numOfConsecutives = 0;
        for (index = i, j = 0; j < linhas; ++j, index += colunas) {
            if (numOfConsecutives && (tabuleiro[index] != 'P')) {
                totalSimultaneousSpots += ((numOfConsecutives+1)>>1);
                numOfConsecutives = 0;
            } else if (tabuleiro[index] == 'P') {
                ++numOfConsecutives;
            }
        }

        totalSimultaneousSpots += ((numOfConsecutives+1)>>1);

        if ((totalSimultaneousSpots == column_vector[i].tentsNeeded) && column_vector[i].tentsNeeded != 0) {
            for (index = i, j = 0; j < linhas; ++j, index += colunas) {
                if (tabuleiro[index] == 'P') {
                    flag1 = 0;
                    while (!(tabuleiro[index] != 'P')) {
                        ++flag1;
                        index += colunas;
                        if ( (++j) == linhas) break;
                    }

                    --j;
                    index -= colunas;

                    if (flag1 == 1) {
                        retVal = 1;
                        tabuleiro[index] = 'T';
                        pushChange(changeStorePtr, NULL, i, j, 1, 0, 'P', 1);
                        removesP(treeInfo, tabuleiro, linhas, colunas, index, i, j, 1, changeStorePtr);
                        assignsTentToATree(treeInfo, tabuleiro, linhas, colunas, index, i, j, 1, changeStorePtr);
                    } else if (flag1%2 == 1) {
                        retVal = 1;
                        flag2 = 1;
                        do {
                            if (flag2) {
                                flag2 = -1;
                                tabuleiro[index] = 'T';
                                pushChange(changeStorePtr, NULL, i, j, 1, 0, 'P', 1);
                                removesP(treeInfo, tabuleiro, linhas, colunas, index, i, j, 1, changeStorePtr);
                                assignsTentToATree(treeInfo, tabuleiro, linhas, colunas, index, i, j, 1, changeStorePtr);
                            }
                            --j;
                            index -= colunas;
                            ++flag2;
                        } while(--flag1);
                    }
                }
            }
        }
    }
    return retVal;
}



/*
* returns: 1 is solved, 0 if not solved
*
*
*/
int checkIfPuzzleSolved(int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        if (row_vector[i].tentsNeeded != 0) {
            return 0;
        }
    }
    for (int i = 0; i < colunas; i++) {
        if (column_vector[i].tentsNeeded != 0) {
            return 0;
        }
    }
    setBoardAnswer(1);
    return 1;
}



/*
* returns: 1 for error, 0 for no errors
*
*
*/
int makeSureMoves(int season, TreeNode*** treeInfo, TreeNode** list, char * tabuleiro, int linhas, int colunas, changeStore **changeStorePtr) {
    int modified = 1;

    if (season == 1) {
        while (modified) {
            if (modified == 404) {
                return 1;
            }
            modified = 0;
            printf("AAAAAAAAA\n");
            fflush(stdout);
            if ((modified = checkForLonelyTrees(treeInfo, list, tabuleiro, linhas, colunas, changeStorePtr))) {
                continue;
            }
            printf("BBBBBBBBB\n");
            fflush(stdout);
            if ((modified = checkNeededTents(tabuleiro, linhas, colunas, treeInfo, changeStorePtr))) {
                continue;
            }
            printf("CCCCCCCCCCC\n");
            fflush(stdout);
            if ((modified = checkConsecutive(tabuleiro, linhas, colunas, treeInfo, changeStorePtr))) {
                continue;
            }
        }
    } else {
        while (modified) {
            if (modified == 404) {
                return 1;
            }
            modified = 0;

            if ((modified = checkNeededTents(tabuleiro, linhas, colunas, treeInfo, changeStorePtr))) {
                continue;
            }

            if ((modified = checkConsecutive(tabuleiro, linhas, colunas, treeInfo, changeStorePtr))) {
                continue;
            }
            modified = 0;
        }
    }

    return 0;
}



void freeTreeInfo(char *tabuleiro, int linhas, int colunas, TreeNode ***treesInfo) {
    int j = 0, i = 0, index = 0;

    for (i = 0; i < linhas; ++i) {

        for (j = 0; j < colunas; ++j, ++index) {

            if (tabuleiro[index] == 'A') {
                free(treesInfo[i][j]);
            } else if (tabuleiro[index] == 'K') {
                tabuleiro[index] = 'A';
                free(treesInfo[i][j]);
            } else if (tabuleiro[index] == 'P') {
                tabuleiro[index] = '.';
            } else if (tabuleiro[index] == 'V') {
                tabuleiro[index] = 'T';
            }
        }
        free(treesInfo[i]);
    }
    free(treesInfo);
}


int randomPlay(TreeNode ***, TreeNode **, char *, int, int, int, int, int, int, int);


void heuristicsForRandomPlay(TreeNode*** treeInfo, TreeNode** list, char * tabuleiro, int linhas, int colunas, int season) {
    int linhasOuColunas = 0, i = 0, index = 0, numOfMoves = 0;
    double max = 0, temp = 0;

    max = -1, temp = 0, numOfMoves = 0, index = 0, linhasOuColunas = -1;

    for (i = 0; i < colunas; i++) {
        if (column_vector[i].availablePositions > 0) {
            if (max < (temp = column_vector[i].tentsNeeded/column_vector[i].availablePositions)) {
                max = temp;
                numOfMoves = column_vector[i].tentsNeeded;
                index = i;
                linhasOuColunas = 0;

            }
        }

    }

    for (i = 0; i < linhas; i++) {
        if (row_vector[i].availablePositions > 0) {
            if (max < (temp = row_vector[i].tentsNeeded/row_vector[i].availablePositions)) {
                max = temp;
                numOfMoves = row_vector[i].tentsNeeded;
                index = i;
                linhasOuColunas = 1;
            }
        }

    }
    if (linhasOuColunas == 1) {
        randomPlay(treeInfo, list, tabuleiro, linhas, colunas, 0, index, numOfMoves, linhasOuColunas, season);
    } else if (linhasOuColunas == 0){
        randomPlay(treeInfo, list, tabuleiro, linhas, colunas, index, 0, numOfMoves, linhasOuColunas, season);
    }
    checkIfPuzzleSolved(linhas, colunas);
}


/*
* returns: 1 if puzzle solved, returns 0 if puzzle not solved
*
*
*/
int randomPlay(TreeNode*** treeInfo, TreeNode** list, char * tabuleiro, int linhas, int colunas, int x, int y, int numOfMoves, int linhasOuColunas, int season) {
    int i = 0, index = 0, edge;
    changeStore *changes = NULL;

    if (linhasOuColunas) {
        edge = colunas;
        i = x;
    } else {
        edge = linhas;
        i = y;
    }

    for (index = y*colunas +x; i < edge; ++i) {
        if (tabuleiro[index] == 'P') {
            tabuleiro[index] = 'T';
            if (linhasOuColunas) {
                pushChange(&changes, NULL, i, y, 1, 0, 'P', 1);
                removesP(treeInfo, tabuleiro, linhas, colunas, index, i, y, 1, &changes);
                assignsTentToATree(treeInfo, tabuleiro, linhas, colunas, index, i, y, 1, &changes);
            } else {
                pushChange(&changes, NULL, x, i, 1, 0, 'P', 1);
                removesP(treeInfo, tabuleiro, linhas, colunas, index, x, i, 1, &changes);
                assignsTentToATree(treeInfo, tabuleiro, linhas, colunas, index, x, i, 1, &changes);
            }

            if (numOfMoves > 1) {
                if (linhasOuColunas) {
                    if (randomPlay(treeInfo, list, tabuleiro, linhas, colunas, i, y, numOfMoves-1, linhasOuColunas, season)) {
                        freeChangeList(&changes);
                        return 1;
                    } else {
                        deleteChanges(&changes, tabuleiro, colunas);
                    }
                } else {
                    if(randomPlay(treeInfo, list, tabuleiro, linhas, colunas, x, i, numOfMoves-1, linhasOuColunas, season)) {
                        freeChangeList(&changes);
                        return 1;
                    } else {
                        deleteChanges(&changes, tabuleiro, colunas);
                    }
                }
            } else {
                if (makeSureMoves(season, treeInfo, list, tabuleiro, linhas, colunas, &changes)) {
                    deleteChanges(&changes, tabuleiro, colunas);
                } else {
                    if(checkIfPuzzleSolved(linhas, colunas)) {
                        freeChangeList(&changes);
                        return 1;
                    } else {
                        heuristicsForRandomPlay(treeInfo, list, tabuleiro, linhas, colunas, season);
                        if(checkIfPuzzleSolved(linhas, colunas)) {
                            freeChangeList(&changes);
                            return 1;
                        } else {
                            deleteChanges(&changes, tabuleiro, colunas);
                        }
                    }
                }
            }

        }
        if (linhasOuColunas) {
            ++index;
        } else {
            index += colunas;
        }
    }
    deleteChanges(&changes, tabuleiro, colunas);
    return 0;
}



void solver(void) {
    char *layout = getBoardLayout();
    int rows = getBoardRows(), columns = getBoardColumns(), season;
    TreeNode ***treesInfo = NULL, *treesList = NULL;
    changeStore *changeStorePtr = NULL;

    if(!(season = findPossibleLocations(layout, rows, columns))) {
        setBoardAnswer(-1);
        return;
    }
    treesInfo = createTreeInfo(columns,rows);
    treesList = createTreeList(layout, rows, columns, treesInfo);

    /* TODO: maybe have a second version of makeSureMoves without saving stuff features for inicial analysis*/
    makeSureMoves(season, treesInfo, &treesList, layout, rows, columns, &changeStorePtr);

    if(!checkIfPuzzleSolved(rows, columns)) {
        heuristicsForRandomPlay(treesInfo, &treesList, layout, rows, columns, season);
    }

    if(!checkIfPuzzleSolved(rows, columns)) {
        setBoardAnswer(-1);
    }

    freeChangeList(&changeStorePtr);

    freeTreeInfo(layout, rows, columns, treesInfo);
}





void freeSolver(void) {
    free(row_vector);
    free(column_vector);
}
