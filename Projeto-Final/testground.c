#include <stdlib.h>
#include <stdio.h>

#include "headers/defs.h"
#include "headers/sort.h"

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


/* TODO: general solver function */

/*return values: 1 - ok and changes
*                2 - ok but no changes
*                404 - not ok
*/

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


int checkNeededTents(char * tabuleiro, int linhas, int colunas, HeadNode** horizontals, HeadNode** verticals) {
    int i = 0, j = 0, index = 0;
    for (i = 0; i < linhas; i++, index += colunas) {
        if (horizontals[i]->tentsNeeded == horizontals[i]->availablePositions) {
            horizontals[i]->tentsNeeded = horizontals[i]->availablePositions = 0;
            for (j = 0; j < colunas; j++) {
                if (tabuleiro[index+j] == 'P') {
                    /* TODO: implement function to put tent there and assign one tree */
                }
            }
        } else if (horizontals[i]->tentsNeeded > horizontals[i]->availablePositions) {
            return 0;
        }
    }

    for (i = 0; i < colunas; i++) {
        if (verticals[i]->tentsNeeded == verticals[i]->availablePositions) {
            verticals[i]->tentsNeeded = verticals[i]->availablePositions = 0;
            for (j = 0; j < colunas; j += colunas) {
                if (tabuleiro[i+j] == 'P') {
                    /* TODO: implement function to put tent there and assign one tree */
                }
            }
        } else if (verticals[i]->tentsNeeded > verticals[i]->availablePositions) {
            return 0;
        }
    }
    return 1;
}



/*
*   Propagates consequences of a given play, if it makes puzzle unsolvable undo changes
*
*/

int makeSureGuesses(int season, TreeNode*** treeInfo, TreeNode** list, char * tabuleiro, int linhas, int colunas, HeadNode **horizontals, HeadNode **verticals) {
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

        if ((modified = checkNeededTents(tabuleiro, linhas, colunas, horizontals, verticals))) {
            continue;
        }

        if ((modified = checkCosecutive())) {
            continue;
        }
    }
    return 1;
}


/* TODO: change changedNode to include coordinates and not pointers */

/* TODO: eliminate all PlayableNode stuff it won't be used */


TreeNode *** createTreeInfo(int colunas, int linhas) {
    TreeNode ***TreeInfo = NULL;
    TreeInfo = (TreeNode ***) malloc(linhas * sizeof(TreeNode **));
    for (int i = 0; i < colunas; ++i) {
        TreeInfo[i] = (TreeNode **) malloc(linhas * sizeof(TreeNode *));
    }
    return TreeInfo;
}



/* Frees treeInfo data structure and replaces all K (assigned trees) to A (trees) */

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

/* TODO: modify function so it changes trees adjacent to P's numOfPlayables to -=1 */
int mark_P_as_T_for_random_A(char *tabuleiro, int linhas, int colunas, HeadNode **horizontals, HeadNode **verticals, int x, int y, int index, TreeNode ***treesInfo) {

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
