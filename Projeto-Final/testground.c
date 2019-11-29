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


/* TODO: create the TreeNode ***treesInfo */



void MergeSort(TreeNode**);

/* checks if P position is alone
*  returns 0 when not alone, 1 when not alone
*  TODO: create 3 more functions so it reduces number of comparisons (doesn't have to compare with tree removing it)
*/
int checkIfPAlone(char *tabuleiro, int indexAUX, int x, int y, int colunas, int linhas) {
    if (x != 0) {

        if (tabuleiro[indexAUX-1] == 'A') {
            return 0;
        }
    }

    if (x != colunas-1) {

        if (tabuleiro[indexAUX+1] == 'A') {
            return 0;
        }
    }

    if (y != 0) {

        if (tabuleiro[indexAUX-colunas] == 'A') {
            return 0;
        }
    }
    if (y != linhas-1) {

        if (tabuleiro[indexAUX+colunas] == 'A') {
            return 0;

        }
    }
    return 1;
}

/*
*  assigns a tree to a tent, does all necessary modifications like checking if any P is left alone
*
*/

void assignTreeToTent(TreeNode ***treesInfo, int x, int y, char *tabuleiro, int index, int colunas, int linhas, HeadNode **horizontals, HeadNode **verticals) {


    treesInfo[y][x]->hasTentAssigned = 1;
    tabuleiro[index] = 'K';
    /* TODO: save changes */

    if (x != 0) {
        if (tabuleiro[index-1] == 'P') {
            if (checkIfPAlone(tabuleiro, index-1, x-1, y, colunas, linhas)) {
                tabuleiro[index-1] = '.';
                --(*horizontals[x-1]).availablePositions;
                --(*verticals[y]).availablePositions;
                /* TODO: save changes */
            }
        }
    }

    if (x != colunas-1) {
        if (tabuleiro[index+1] == 'P') {
            if (checkIfPAlone(tabuleiro, index+1, x+1, y, colunas, linhas)) {
                tabuleiro[index+1] = '.';
                --(*horizontals[x+1]).availablePositions;
                --(*verticals[y]).availablePositions;
                /* TODO: save changes */
            }
        }
    }


    if (y != 0) {
        if (tabuleiro[index-colunas] == 'P') {
            if (checkIfPAlone(tabuleiro, index-colunas, x, y-1, colunas, linhas)) {
                tabuleiro[index-colunas] = '.';
                --(*horizontals[x]).availablePositions;
                --(*verticals[y-1]).availablePositions;
                /* TODO: save changes */
            }
        }
    }
    if (y != linhas-1) {
        if (tabuleiro[index+colunas] == 'P') {
            if (checkIfPAlone(tabuleiro, index+colunas, x, y+1, colunas, linhas)) {
                tabuleiro[index+colunas] = '.';
                --(*horizontals[x]).availablePositions;
                --(*verticals[y+1]).availablePositions;
                /* TODO: save changes */
            }

        }
    }
}


/*return values: 1 - ok
*                0 - not ok
*/
int mark_P_as_T(char *tabuleiro, int linhas, int colunas, HeadNode **horizontals, HeadNode **verticals, int x, int y, int index, TreeNode ***treesInfo) {
    tabuleiro[index] = 'T';
    /* TODO: save change */



    //removes all diogonal P positions
    if (y != 0) {

        if (x != 0) {

            if (tabuleiro[index-colunas-1] == 'P') {
                removeFromValidPositions(); /* TODO: implement this function */
                --(*horizontals[x-1]).availablePositions;
                --(*verticals[y-1]).availablePositions;
                /* TODO: insert save change func */
            }
        }

        if (x != colunas-1) {

            if (tabuleiro[index-colunas+1] == 'P') {
                removeFromValidPositions(); /* TODO: implement this function */
                --(*horizontals[x+1]).availablePositions;
                --(*verticals[y-1]).availablePositions;
                /* TODO: insert save change func */
            }
        }
    }

    if (y != linhas-1) {

        if (x != 0) {
            if (tabuleiro[index+colunas-1] == 'P') {
                removeFromValidPositions(); /* TODO: implement this function */
                --(*horizontals[x-1]).availablePositions;
                --(*verticals[y+1]).availablePositions;
                /* TODO: insert save change func */
            }
        }

        if (x != colunas-1) {
            if (tabuleiro[index+colunas+1] == 'P') {
                removeFromValidPositions(); /* TODO: implement this function */
                --(*horizontals[x+1]).availablePositions;
                --(*verticals[y+1]).availablePositions;
                /* TODO: insert save change func */
            }
        }
    }



    //tries to assign tent to each available tree, it either finds solution or it is not part of solution

    if (x != 0) {

        if (tabuleiro[index-1] == 'A') {
            assignTreeToTent(treesInfo, x-1, y, tabuleiro, index-1, colunas, linhas, horizontals, verticals);
            /* TODO: make code to keep cycle going */
            if (/* TODO complete if with solver return value */) {
                return 1;
            }
        }
    }

    if (x != colunas-1) {

        if (tabuleiro[index+1] == 'A') {
            assignTreeToTent(treesInfo, x+1, y, tabuleiro, index+1, colunas, linhas, horizontals, verticals);
            /* TODO: make code to keep cycle going */
            if (/* TODO complete if with solver return value */) {
                return 1;
            }
        }
    }

    if (y != 0) {

        if (tabuleiro[index-colunas] == 'A') {
            assignTreeToTent(treesInfo, x, y-1, tabuleiro, index-colunas, colunas, linhas, horizontals, verticals);
            /* TODO: make code to keep cycle going */
            if (/* TODO complete if with solver return value */) {
                return 1;
            }
        }
    }

    if (y != linhas-1) {

        if (tabuleiro[index+colunas] == 'A') {
            assignTreeToTent(treesInfo, x, y+1, tabuleiro, index+colunas, colunas, linhas, horizontals, verticals);
            /* TODO: make code to keep cycle going */
            if (/* TODO complete if with solver return value */) {
                return 1;
            }

        }
    }
    
    return 0;
}



/* If a row or column has 0 tents, it fills that row/column with . so no P positions are wrongly marked
*  must call it after find possible locations is called
*/
void eliminateInvalidRowsANdColumns(char *tabuleiro, int linhas, int colunas, HeadNode *horizontals, HeadNode *verticals) {
    int i, j, index = 0;
    for (i = 0; i < linhas; ++i) {
        if (horizontals[i].puzzleTents == 0) {
            index = i*colunas;
            for (j = 0; j < colunas; ++j, ++index) {
                if (tabuleiro[index] != 'A') {
                    tabuleiro[index] = '.';
                }
            }
        }
    }
    for (i = 0; i < linhas; ++i) {
        if (horizontals[i].puzzleTents == 0) {
            index = i;
            for (j = 0; j < colunas; ++j, index += colunas) {
                if (tabuleiro[index] != 'A') {
                    tabuleiro[index] = '.';
                }
            }
        }
    }
}


/* Fids all P postions that are not 0 */
void findPossibleLocations(char *tabuleiro, int linhas, int colunas, HeadNode *horizontals, HeadNode *verticals) {

	int i, j, index = 0, numOfTrees = 0;

	for (i = 0; i < linhas; ++i) {

		for (j = 0; j < colunas; ++j, ++index) {

			if (tabuleiro[index] == 'A') {

                ++numOfTrees; /* TODO: create comparison to check season and insert it in puzzle parameters or exit earlier if num trees < numOfTents */

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




//cria o grapho tanto com as tendas como com as arvores
TreeNode * createTreeList(char *tabuleiro, int linhas, int colunas, HeadNode *horizontals, HeadNode *verticals, TreeNode ***treesInfo) {
    int j = 0, i = 0, index = 0;
    TreeNode *list = NULL;

    for (i = 0; i < linhas; i++) {

        for (j = 0; j < colunas; j++, ++index) {

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
