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


void MergeSort(TreeNode**);





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
TreeNode * createTreeList(char *tabuleiro, int linhas, int colunas, HeadNode *horizontals, HeadNode *verticals) {
    int j = 0, i = 0, index = 0;
    TreeNode *list = NULL, *newTree = NULL;

    for (i = 0; i < linhas; i++) {

        for (j = 0; j < colunas; j++, ++index) {

            if (tabuleiro[index] == 'A') {
                newTree = (TreeNode *) malloc(sizeof(TreeNode));
                newTree->x = j;
                newTree->y = i;
                newTree->hasTentAssigned = 0;
                newTree->num_playables = 0;
                if (j != 0) {
                    if (tabuleiro[index-1] == 'P') {
    					++(newTree->num_playables);
    				}
                }

                if (j != colunas-1) {
                    if (tabuleiro[index+1] == 'P') {
    					++(newTree->num_playables);
    				}
                }


                if (i != 0) {
                    if (tabuleiro[index-colunas] == 'P') {
    					++(newTree->num_playables);
    				}
                }
                if (i != linhas-1) {
                    if (tabuleiro[index+colunas] == 'P') {
    					++(newTree->num_playables);

    				}
                }
                newTree->next = list;
                list = newTree;
            }
        }
    }
}
