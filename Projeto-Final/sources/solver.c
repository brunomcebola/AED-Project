#include <stdlib.h>
#include "../headers/game.h"
#include "../headers/solver.h"

//perguntar ao stor

struct _PlayableNode {
    int x, y;
    int valid;
    PlayableNode *horizontal_next;
    PlayableNode *vertical_next;
};

typedef struct _TreeNode {
    int x, y;
    int hasTent;
    int num_playables;
    PlayableNode *North;
    PlayableNode *South;
    PlayableNode *West;
    PlayableNode *East;
} TreeNode;


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
    for (i = 0; i < colunas; ++i) {
        if (verticals[i].puzzleTents == 0) {
            index = i;
            for (j = 0; j < linhas; ++j, index += colunas) {
                if (tabuleiro[index] != 'A') {
                    tabuleiro[index] = '.';
                }
            }
        }
    }
}


/* Fids all P postions that are not 0 */
int findPossibleLocations(char *tabuleiro, int linhas, int colunas) {

	int i, j, index = 0, numOfTrees = 0, numOfAskedTents = getBoardSum();

	for (i = 0; i < linhas; ++i) {

		for (j = 0; j < colunas; ++j, ++index) {

			if (tabuleiro[index] == 'A') {

                ++numOfTrees;

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

    if (numOfTrees == numOfAskedTents) {
        setBoardSeason(1);
    } else if (numOfTrees > numOfAskedTents) {
        setBoardSeason(2);
    } else {
        return 0;
    }

    return 1;
}


void solver(void) {
    char *layout = getBoardLayout();
    int rows = getBoardRows(), columns = getBoardColumns();

    if(!findPossibleLocations(layout, rows, columns)) {
        setBoardAnswer(-1);
        return;
    }
    eliminateInvalidRowsANdColumns(layout, rows, columns, row_vector, column_vector);

}





void freeSolver(void) {
    free(row_vector);
    free(column_vector);
}
