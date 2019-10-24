#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"

typedef struct {
    int n_lines, n_columns;
    int *n_el_line, *n_el_column;
    char mode;
    char **layout;
} board;

board jogo;


void initBoard() {
    jogo.n_lines = 0;
    jogo.n_columns = 0;
    jogo.n_el_line = NULL;
    jogo.n_el_column = NULL;
    jogo.layout = NULL;
    jogo.mode = '\0';
}

void setBoardRows(int lines) {
    jogo.n_lines = lines;
}

void setBoardColumns(int columns) {
    jogo.n_columns = columns;
}

void setBoardMode(char mode) {
    jogo.mode = mode;
}

void setBoardLayout(char **layout) {
    jogo.layout = layout;
}

void printLayout() {
    printf("%d %d %c\n", jogo.n_lines, jogo.n_columns, jogo.mode);
    for(int i=0;i<jogo.n_lines;i++) {
        for(int j=0;j<jogo.n_columns;j++) {
            printf("%c", jogo.layout[i][j]);
        }
        printf("\n");
    }
}
void freeBoard() {
    for(int i=0;i<jogo.n_lines;i++) {
        free(jogo.layout[i]);
    }
    free(jogo.layout);
}
