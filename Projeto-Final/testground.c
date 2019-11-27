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

void findPossibleLocations(char *tabuleiro, int linhas, int colunas) {

	int i, j;

	for (i = 0; i < linhas; i += colunas) {

		for (j = 0; j < colunas; j++) {

			if (tabuleiro[i+j] == 'A') {
				if (tabuleiro[(i+j)-1] == '.') {
					tabuleiro[(i+j)-1] = 'P';
				}

				if (tabuleiro[(i+j)+1] == '.') {
					tabuleiro[(i+j)+1] = 'P';
				}

				if (tabuleiro[(i+j)-colunas] == '.') {
					tabuleiro[(i+j)-colunas] = 'P';
				}

				if (tabuleiro[(i+j)+colunas] == '.') {
					tabuleiro[(i+j)+colunas] = 'P';
				}
			}

		}
	}
}



/*
*
* direction: 1 for horizontal, 0 for vertical
*
*/
void addAtEnd(HeadNode *head, PlayableNode *toInsert, int direction) {
    PlayableNode *aux = head->first;

    if (head->first == NULL) {
        head->first = toInsert;
        return;
    }

    if (direction) {
        while (aux->horizontal_next != NULL) {
            aux = aux->horizontal_next;
    }
        aux->horizontal_next = toInsert;
    } else {
        while (aux->vertical_next != NULL) {
            aux = aux->vertical_next;
        }
        aux->vertical_next = toInsert;
    }
}


void createGraph(char *tabuleiro, int linhas, int colunas, HeadNode *horizontals, HeadNode *verticals) {
    int j = 0, i = 0, linha_atual = 0;
    PlayableNode *newNode = NULL;

    for (i = 0; i < linhas; i++, linha_atual += colunas) {

        for (j = 0; j < colunas; j++) {

            if (tabuleiro[linha_atual+j] == 'P') {
            newNode = (PlayableNode *) malloc(sizeof(PlayableNode));
            newNode->x = j;
            newNode->y = i;
            newNode->valid = 0;
            newNode->horizontal_next = NULL;
            newNode->vertical_next = NULL;
            addAtEnd(&(horizontals[i]), newNode, 1);
            addAtEnd(&(verticals[j]), newNode, 0);
            }
        }
    }
}
