#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/game.h"

typedef struct {
    int n_rows, n_columns, x, y;
    int *n_el_row, *n_el_column;
    char mode;
    char **layout;
} board;

board jogo;


void initBoard() {
    jogo.n_rows = 0;
    jogo.n_columns = 0;
    jogo.n_el_row = NULL;
    jogo.n_el_column = NULL;
    jogo.mode = '\0';
    jogo.x = 0;
    jogo.y = 0;
    jogo.layout = NULL;

}

void setBoardRows(int lines) {
    jogo.n_rows = lines;
}

void setBoardColumns(int columns) {
    jogo.n_columns = columns;
}

void setBoardMode(char mode) {
    jogo.mode = mode;
}

void setBoardCoordinates(int x, int y) {
    jogo.x = x;
    jogo.y = y;
}

void setBoardElRows(int *n_el_row) {
    jogo.n_el_row = n_el_row;
}

void setBoardElColumns(int *n_el_column) {
    jogo.n_el_column = n_el_column;
}

void setBoardLayout(char **layout) {
    jogo.layout = layout;
}

void printLayout() {
    printf("%d %d %c\n", jogo.n_rows, jogo.n_columns, jogo.mode);
    for(int i=0;i<jogo.n_rows;i++) {
        printf("%d ", jogo.n_el_row[i]);
    }
    printf("\n");
    for(int i=0;i<jogo.n_columns;i++) {
        printf("%d ", jogo.n_el_column[i]);
    }
    printf("\n");
    for(int i=0;i<jogo.n_rows;i++) {
        for(int j=0;j<jogo.n_columns;j++) {
            printf("%c", jogo.layout[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
void freeBoard() {
    free(jogo.n_el_row);
    free(jogo.n_el_column);
    for(int i=0;i<jogo.n_rows;i++) {
        free(jogo.layout[i]);
    }
    free(jogo.layout);
}
