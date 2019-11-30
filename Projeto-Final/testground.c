#include <stdlib.h>
#include <stdio.h>

#include "headers/solver.h"
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







/*
*   Propagates consequences of a given play, if it makes puzzle unsolvable undo changes
*
*/
















/* checks if P position is alone
*  returns 0 when not alone, 1 when not alone
*  TODO: create 3 more functions so it reduces number of comparisons (doesn't have to compare with tree removing it)
*/
/*int checkIfPAlone(char *tabuleiro, int indexAUX, int x, int y, int colunas, int linhas) {
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
}*/

/*
*  assigns a tree to a tent, does all necessary modifications like checking if any P is left alone
*
*/

/*void assignTreeToTent(TreeNode ***treesInfo, int x, int y, char *tabuleiro, int index, int colunas, int linhas, HeadNode **horizontals, HeadNode **verticals) {


    treesInfo[y][x]->hasTentAssigned = 1;
    tabuleiro[index] = 'K';
    /* TODO: save changes

    if (x != 0) {
        if (tabuleiro[index-1] == 'P') {
            if (checkIfPAlone(tabuleiro, index-1, x-1, y, colunas, linhas)) {
                tabuleiro[index-1] = '.';
                --(*horizontals[x-1]).availablePositions;
                --(*verticals[y]).availablePositions;
                /* TODO: save changes
            }
        }
    }

    if (x != colunas-1) {
        if (tabuleiro[index+1] == 'P') {
            if (checkIfPAlone(tabuleiro, index+1, x+1, y, colunas, linhas)) {
                tabuleiro[index+1] = '.';
                --(*horizontals[x+1]).availablePositions;
                --(*verticals[y]).availablePositions;
                /* TODO: save changes
            }
        }
    }


    if (y != 0) {
        if (tabuleiro[index-colunas] == 'P') {
            if (checkIfPAlone(tabuleiro, index-colunas, x, y-1, colunas, linhas)) {
                tabuleiro[index-colunas] = '.';
                --(*horizontals[x]).availablePositions;
                --(*verticals[y-1]).availablePositions;
                /* TODO: save changes
            }
        }
    }
    if (y != linhas-1) {
        if (tabuleiro[index+colunas] == 'P') {
            if (checkIfPAlone(tabuleiro, index+colunas, x, y+1, colunas, linhas)) {
                tabuleiro[index+colunas] = '.';
                --(*horizontals[x]).availablePositions;
                --(*verticals[y+1]).availablePositions;
                /* TODO: save changes
            }

        }
    }
}*/


/*return values: 1 - ok
*                0 - not ok
*/

/* TODO: modify function so it changes trees adjacent to P's numOfPlayables to -=1 */
/*int mark_P_as_T_for_random_A(char *tabuleiro, int linhas, int colunas, HeadNode **horizontals, HeadNode **verticals, int x, int y, int index, TreeNode ***treesInfo) {

    tabuleiro[index] = 'T';
    /* TODO: save change



    //removes all diogonal P positions
    if (y != 0) {

        if (x != 0) {

            if (tabuleiro[index-colunas-1] == 'P') {
                removeFromValidPositions(); /* TODO: implement this function
                --(*horizontals[x-1]).availablePositions;
                --(*verticals[y-1]).availablePositions;
                /* TODO: insert save change func
            }
        }

        if (x != colunas-1) {

            if (tabuleiro[index-colunas+1] == 'P') {
                removeFromValidPositions(); /* TODO: implement this function
                --(*horizontals[x+1]).availablePositions;
                --(*verticals[y-1]).availablePositions;
                /* TODO: insert save change func
            }
        }
    }

    if (y != linhas-1) {

        if (x != 0) {
            if (tabuleiro[index+colunas-1] == 'P') {
                removeFromValidPositions(); /* TODO: implement this function
                --(*horizontals[x-1]).availablePositions;
                --(*verticals[y+1]).availablePositions;
                /* TODO: insert save change func
            }
        }

        if (x != colunas-1) {
            if (tabuleiro[index+colunas+1] == 'P') {
                removeFromValidPositions(); /* TODO: implement this function
                --(*horizontals[x+1]).availablePositions;
                --(*verticals[y+1]).availablePositions;
                /* TODO: insert save change func
            }
        }
    }



    //tries to assign tent to each available tree, it either finds solution or it is not part of solution

    if (x != 0) {

        if (tabuleiro[index-1] == 'A') {
            assignTreeToTent(treesInfo, x-1, y, tabuleiro, index-1, colunas, linhas, horizontals, verticals);
            /* TODO: make code to keep cycle going
            if (/* TODO complete if with solver return value ) {
                return 1;
            }
        }
    }

    if (x != colunas-1) {

        if (tabuleiro[index+1] == 'A') {
            assignTreeToTent(treesInfo, x+1, y, tabuleiro, index+1, colunas, linhas, horizontals, verticals);
            /* TODO: make code to keep cycle going
            if (/* TODO complete if with solver return value ) {
                return 1;
            }
        }
    }

    if (y != 0) {

        if (tabuleiro[index-colunas] == 'A') {
            assignTreeToTent(treesInfo, x, y-1, tabuleiro, index-colunas, colunas, linhas, horizontals, verticals);
            /* TODO: make code to keep cycle going
            if (/* TODO complete if with solver return value ) {
                return 1;
            }
        }
    }

    if (y != linhas-1) {

        if (tabuleiro[index+colunas] == 'A') {
            assignTreeToTent(treesInfo, x, y+1, tabuleiro, index+colunas, colunas, linhas, horizontals, verticals);
            /* TODO: make code to keep cycle going
            if (/* TODO complete if with solver return value ) {
                return 1;
            }

        }
    }

    return 0;
}
*/
