#include <stdlib.h>
#include <stdio.h>


typedef struct _Playable_Node {
    int x, y;
    int valid;
    struct _Playable_Node *horizontal_next;
    struct _Playable_Node *vertical_next;
}PlayableNode;

typedef struct _TreeNode {
    int x, y;
    int hasTent;
    int num_playables;
    struct _Node *North;
    struct _Node *South;
    struct _Node *West;
    struct _Node *East;
}TreeNode;

typedef struct {
    int puzzleTents;
    int tentsNeeded;
    PlayableNode *first;
}HeadNode;


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
}board;


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
