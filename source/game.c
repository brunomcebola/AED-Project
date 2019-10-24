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
}

void setRows(int lines) {
    jogo.n_lines = lines;
}

void setColumns(int columns) {
    jogo.n_lines = columns;
}

void setLayout(char **layout) {
    jogo.layout = layout;
}

void printLayout() {
    for(int i=0;i<jogo.n_lines;i++) {
        for(int j=0;j<jogo.n_lines;j++) {
            printf("%c", jogo.layout[i][j]);
        }
        printf("\n");
    }
}
