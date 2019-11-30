#include <stdlib.h>
#include <stdio.h>
#include "../headers/game.h"
#include "../headers/solver.h"
#include "../headers/sort.h"

//perguntar ao stor

struct _PlayableNode {
    int x, y;
    int valid;
    PlayableNode *horizontal_next;
    PlayableNode *vertical_next;
};


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


/* TODO: change changedNode to include coordinates and not pointers */

/* TODO: eliminate all PlayableNode stuff it won't be used */
TreeNode ***createTreeInfo(int colunas, int linhas) {
    TreeNode ***TreeInfo = NULL;
    TreeInfo = (TreeNode ***) malloc(linhas * sizeof(TreeNode **));
    for (int i = 0; i < linhas; ++i) {
        TreeInfo[i] = (TreeNode **) malloc(colunas * sizeof(TreeNode *));
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
void checkIfPAlone(char *tabuleiro, int index, int x, int y, int colunas, int linhas) {
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
    /* TODO: insert save change func */
}



void invalidateTreePPositions(char * tabuleiro, int linhas, int colunas, int index, int x, int y) {
    if (x != 0) {

        if (tabuleiro[index-1] == 'P') {
        checkIfPAlone(tabuleiro, index-1, x-1, y, colunas, linhas);
        }
    }

    if (x != colunas-1) {

        if (tabuleiro[index+1] == 'P') {
            checkIfPAlone(tabuleiro, index+1, x+1, y, colunas, linhas);
        }
    }

    if (y != 0) {

        if (tabuleiro[index-colunas] == 'P') {
            checkIfPAlone(tabuleiro, index-colunas, x, y-1, colunas, linhas);
        }
    }
    if (y != linhas-1) {

        if (tabuleiro[index+colunas] == 'P') {
            checkIfPAlone(tabuleiro, index+colunas, x, y+1, colunas, linhas);

        }
    }
}



void assginsTentToATree(TreeNode*** treeInfo, char * tabuleiro, int linhas, int colunas, int index, int x, int y) {
    if (x != 0) {
        if (tabuleiro[index-1] == 'A') {
            tabuleiro[index-1] = 'K';
            (treeInfo[y][x-1]->hasTentAssigned) = 1;
            invalidateTreePPositions(tabuleiro, linhas, colunas, index-1, x-1, y);
            return;
        }
    }

    if (x != colunas-1) {
        if (tabuleiro[index+1] == 'A') {
            tabuleiro[index+1] = 'K';
            (treeInfo[y][x+1]->hasTentAssigned) = 1;
            invalidateTreePPositions(tabuleiro, linhas, colunas, index+1, x+1, y);
            return;
        }
    }

    if (y != 0) {
        if (tabuleiro[index-colunas] == 'A') {
            tabuleiro[index-colunas] = 'K';
            (treeInfo[y-1][x]->hasTentAssigned) = 1;
            invalidateTreePPositions(tabuleiro, linhas, colunas, index-colunas, x, y-1);
            return;
        }
    }

    if (y != linhas-1) {
        if (tabuleiro[index+colunas] == 'A') {
            tabuleiro[index+colunas] = 'K';
            (treeInfo[y+1][x]->hasTentAssigned) = 1;
            invalidateTreePPositions(tabuleiro, linhas, colunas, index+colunas, x, y+1);
            return;
        }
    }
}





void removesP(TreeNode*** treeInfo, char * tabuleiro, int linhas, int colunas, int index, int x, int y, int isTent) {
    --(row_vector[x].tentsNeeded);
    --(row_vector[x].availablePositions);
    --(column_vector[y].tentsNeeded);
    --(column_vector[y].availablePositions);

    if (x != 0) {
        if (tabuleiro[index-1] == 'A') {
            --(treeInfo[y][x-1]->num_playables);
            /* TODO: insert save changes function */
        } else if (tabuleiro[index-1] == 'P' && isTent) {
            tabuleiro[index-1] = '.';
            /* TODO: insert save change func */
        }
    }

    if (x != colunas-1) {
        if (tabuleiro[index+1] == 'A') {
            --(treeInfo[y][x+1]->num_playables);
            /* TODO: insert save changes function */
        } else if (tabuleiro[index+1] == 'P' && isTent) {
            tabuleiro[index+1] = '.';
            /* TODO: insert save change func */
        }
    }

    if (y != 0) {
        if (tabuleiro[index-colunas] == 'A') {
            --(treeInfo[y-1][x]->num_playables);
            /* TODO: insert save changes function */
        } else if (tabuleiro[index-colunas] == 'P' && isTent) {
            tabuleiro[index-colunas] = '.';
            /* TODO: insert save change func */
        }

        if (isTent) {
            if (x != 0) {
                if (tabuleiro[index-colunas-1] == 'P') {
                    removesP(treeInfo, tabuleiro, linhas, colunas, index-colunas-1, x-1, y-1, 0);
                    --(column_vector[x-1]).availablePositions;
                    --(row_vector[y-1]).availablePositions;
                    /* TODO: insert save change func */
                }
            }

            if (x != colunas-1) {
                if (tabuleiro[index-colunas+1] == 'P') {
                    removesP(treeInfo, tabuleiro, linhas, colunas, index-colunas+1, x+1, y-1, 0);
                    --(column_vector[x+1]).availablePositions;
                    --(row_vector[y-1]).availablePositions;
                    /* TODO: insert save change func */
                }
            }
        }

    }

    if (y != linhas-1) {
        if (tabuleiro[index+colunas] == 'A') {
            --(treeInfo[y+1][x]->num_playables);
            /* TODO: insert save changes function */
        } else if (tabuleiro[index+colunas] == 'P' && isTent) {
            tabuleiro[index+colunas] = '.';
            /* TODO: insert save change func */
        }

        if (isTent) {
            if (x != 0 && isTent) {
                if (tabuleiro[index+colunas-1] == 'P') {
                    removesP(treeInfo, tabuleiro, linhas, colunas, index+colunas-1, x-1, y+1, 0);
                    --(column_vector[x-1]).availablePositions;
                    --(row_vector[y+1]).availablePositions;
                    /* TODO: insert save change func */
                }
            }

            if (x != colunas-1 && isTent) {
                if (tabuleiro[index+colunas+1] == 'P') {
                    removesP(treeInfo, tabuleiro, linhas, colunas, index+colunas+1, x+1, y+1, 0);
                    --(column_vector[x+1]).availablePositions;
                    --(row_vector[y+1]).availablePositions;
                    /* TODO: insert save change func */
                }
            }
        }
    }
}




int checkNeededTents(char * tabuleiro, int linhas, int colunas, TreeNode ***treeInfo) {
    int i = 0, j = 0, index = 0, retVal = 0;

    for (index = 0, i = 0; i < linhas; ++i, index += colunas) {
        if ((row_vector[i].tentsNeeded == row_vector[i].availablePositions) && row_vector[i].tentsNeeded > 0) {
            retVal = 1;
            row_vector[i].tentsNeeded = row_vector[i].availablePositions = 0;
            for (j = 0; j < colunas; ++j) {
                if (tabuleiro[index+j] == 'P') {
                    tabuleiro[index+j] = 'T';
                    assginsTentToATree(treeInfo, tabuleiro, linhas, colunas, index+j, j, i);
                    removesP(treeInfo, tabuleiro, linhas, colunas, index+j, j, i, 1);
                    --(column_vector[j].availablePositions);
                    /*TODO: falta dar assign a uma arvore*/
                }
            }
        } else if (row_vector[i].tentsNeeded > row_vector[i].availablePositions) {
            return 404;
        } else if (row_vector[i].availablePositions > 0 && row_vector[i].tentsNeeded == 0) {
            for (j = 0; j < colunas; ++j) {
                if (tabuleiro[index+j] == 'P') {
                    tabuleiro[index+j] = '.';
                    removesP(treeInfo, tabuleiro, linhas, colunas, index+j, j, i, 0);
                    --(column_vector[j].availablePositions);
                    /*TODO: save changes */
                }
            }
        }
    }

    for (i = 0; i < colunas; ++i) {
        if ((column_vector[i].tentsNeeded == column_vector[i].availablePositions) && column_vector[i].tentsNeeded > 0) {
            retVal = 1;
            column_vector[i].tentsNeeded = column_vector[i].availablePositions = 0;
            for (index = i, j = 0; j < linhas; ++j, index += colunas) {
                if (tabuleiro[index] == 'P') {
                    tabuleiro[index] = 'T';
                    assginsTentToATree(treeInfo, tabuleiro, linhas, colunas, index, i, j);
                    removesP(treeInfo, tabuleiro, linhas, colunas, index, i, j, 1);
                    --(row_vector[j].availablePositions);
                    /*TODO: falta dar assign a uma arvore*/
                }
            }
        } else if (column_vector[i].tentsNeeded > column_vector[i].availablePositions) {
            return 404;
        } else if (column_vector[i].availablePositions > 0 && column_vector[i].tentsNeeded == 0) {
            for (index = i, j = 0; j < linhas; ++j, index += colunas) {
                if (tabuleiro[index] == 'P') {
                    tabuleiro[index] = '.';
                    removesP(treeInfo, tabuleiro, linhas, colunas, index, i, j, 0);
                    --(row_vector[j].availablePositions);
                    /*TODO: save changes */
                }
            }
        }
    }
    return retVal;
}





int checkForLonelyTrees(TreeNode*** treeInfo, TreeNode** list, char * tabuleiro, int linhas, int colunas) {
    TreeNode *aux;
    int index;
    MergeSort(list);
    aux = *list;

    if (aux == NULL) {
        return 2;
    }

    if ((aux)->num_playables == 0) {
        return 404;
    }

    while (aux->hasTentAssigned == 1) {
        if (aux->next == NULL || aux->num_playables > 1) {
            return 2;
        }
        aux = aux->next;
    }

    if ((aux)->num_playables == 1){

        index = (aux->y)*colunas + aux->x;


        if (aux->x != 0) {
            if (tabuleiro[index-1] == 'P') {
                aux->hasTentAssigned = 1;
                tabuleiro[index] = 'K';
                tabuleiro[index-1] = 'T';
                /* TODO: insert save changes function */
                removesP(treeInfo, tabuleiro, linhas, colunas, index-1, aux->x -1, aux->y, 1);
                return 1;
            }
        }

        if (aux->x != colunas-1) {
            if (tabuleiro[index+1] == 'P') {
                aux->hasTentAssigned = 1;
                tabuleiro[index] = 'K';
                tabuleiro[index+1] = 'T';
                /* TODO: insert save changes function */
                removesP(treeInfo, tabuleiro, linhas, colunas, index+1, aux->x +1, aux->y, 1);
                return 1;
            }
        }

        if (aux->y != 0) {
            if (tabuleiro[index-colunas] == 'P') {
                aux->hasTentAssigned = 1;
                tabuleiro[index] = 'K';
                tabuleiro[index-colunas] = 'T';
                /* TODO: insert save changes function */
                removesP(treeInfo, tabuleiro, linhas, colunas, index-colunas, aux->x, aux->y -1, 1);
                return 1;
            }
        }

        if (aux->y != linhas-1) {
            if (tabuleiro[index+colunas] == 'P') {
                aux->hasTentAssigned = 1;
                tabuleiro[index] = 'K';
                tabuleiro[index-colunas] = 'T';
                /* TODO: insert save changes function */
                removesP(treeInfo, tabuleiro, linhas, colunas, index+colunas, aux->x, aux->y +1, 1);
                return 1;
            }
        }

    }
    return 404;
}

int makeSureGuesses(int season, TreeNode*** treeInfo, TreeNode** list, char * tabuleiro, int linhas, int colunas) {
    int modified = 1;
    while (modified) {

        if (modified == 404) {
            return 0;
        }
        modified = 0;

        if (season == 1) {
            if ((modified = checkForLonelyTrees(treeInfo, list, tabuleiro, linhas, colunas)) == 1) {
                continue;
            }
        }

        modified = 0;

        if ((modified = checkNeededTents(tabuleiro, linhas, colunas, treeInfo))) {
            continue;
        }

        modified = 0;

        /*if ((modified = checkCosecutive())) {
            continue;
        }*/
    }
    return 1;
}



void freeTreeInfo(char *tabuleiro, int linhas, int colunas, TreeNode ***treesInfo) {
    int j = 0, i = 0, index = 0;

    for (i = 0; i < linhas; ++i) {
        printf("%d\n", i);

        for (j = 0; j < colunas; ++j, ++index) {

            if (tabuleiro[index] == 'A') {
                free(treesInfo[i][j]);
            } else if (tabuleiro[index] == 'K') {
                tabuleiro[index] = 'A';
                free(treesInfo[i][j]);
            }
        }
        free(treesInfo[i]);
    }
    free(treesInfo);
}

void solver(void) {
    char *layout = getBoardLayout();
    int rows = getBoardRows(), columns = getBoardColumns(), season;
    TreeNode ***treesInfo = NULL, *treesList = NULL;

    if(!(season = findPossibleLocations(layout, rows, columns))) {
        setBoardAnswer(-1);
        return;
    }
    treesInfo = createTreeInfo(rows, columns);
    treesList = createTreeList(layout, rows, columns, treesInfo);

    makeSureGuesses(season, treesInfo, &treesList, layout, rows, columns);

    freeTreeInfo(layout, rows, columns, treesInfo);
}





void freeSolver(void) {
    free(row_vector);
    free(column_vector);
}
