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

	int i, j, index = 0, numOfTrees = 0, numOfAskedTents = getBoardSum();

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
    } else if (numOfTrees > numOfAskedTents) {
        setBoardSeason(2);
    } else {
        return 0;
    }

    return 1;
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

int checkNeededTents(char * tabuleiro, int linhas, int colunas) {
    int i = 0, j = 0, index = 0;

    for (index = 0, i = 0; i < linhas; ++i, index += colunas) {
        if (row_vector[i].tentsNeeded == row_vector[i].availablePositions) {
            row_vector[i].tentsNeeded = row_vector[i].availablePositions = 0;
            for (j = 0; j < colunas; ++j) {
                if (tabuleiro[index+j] == 'P') {
                    tabuleiro[index+j] = 'T';
                    /*TODO: falta dar assign a uma arvore*/
                }
            }
        } else if (row_vector[i].tentsNeeded > row_vector[i].availablePositions) {
            return 0;
        }
    }

    for (i = 0; i < colunas; ++i) {
        if (column_vector[i].tentsNeeded == column_vector[i].availablePositions) {
            column_vector[i].tentsNeeded = column_vector[i].availablePositions = 0;
            for (index = 0, j = 0; j < linhas; ++j, index += colunas) {
                if (tabuleiro[i+index] == 'P') {
                    tabuleiro[i+index] = 'T';
                    /*TODO: falta dar assign a uma arvore*/
                }
            }
        } else if (column_vector[i].tentsNeeded > column_vector[i].availablePositions) {
            return 0;
        }
    }
    return 1;
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
                /* TODO: insert take only available spot as tent function */
                return 1;
            }
        }

        if (aux->x != colunas-1) {
            if (tabuleiro[index+1] == 'P') {
                /* TODO: insert take only available spot as tent function */
                return 1;
            }
        }

        if (aux->y != 0) {
            if (tabuleiro[index-colunas] == 'P') {
                /* TODO: insert take only available spot as tent function */
                return 1;
            }
        }

        if (aux->y != linhas-1) {
            if (tabuleiro[index+colunas] == 'P') {
                /* TODO: insert take only available spot as tent function */
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

        if ((modified = checkNeededTents(tabuleiro, linhas, colunas))) {
            continue;
        }

        /*if ((modified = checkCosecutive())) {
            continue;
        }*/
    }
    return 1;
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
            }
        }
        free(treesInfo[i]);
    }
    free(treesInfo);
}

void solver(void) {
    char *layout = getBoardLayout();
    int rows = getBoardRows(), columns = getBoardColumns();
    TreeNode ***treesInfo = NULL, *treesList = NULL;

    if(!findPossibleLocations(layout, rows, columns)) {
        setBoardAnswer(-1);
        return;
    }
    treesInfo = createTreeInfo(rows, columns);
    treesList = createTreeList(layout, rows, columns, treesInfo);

    makeSureGuesses(1, treesInfo, &treesList, layout, rows, columns);

    freeTreeInfo(layout, rows, columns, treesInfo);
}





void freeSolver(void) {
    free(row_vector);
    free(column_vector);
}
