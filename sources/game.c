#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/game.h"

typedef struct {
    int n_rows, n_columns, x, y;
    int *n_el_row, *n_el_column;
    char mode;
    char **layout;
    int answer;
} board;

board jogo;

//implementation of functions to manage 'jogo'

//INITITIALIZATIONS
void initBoard() {
    jogo.n_rows = 0;
    jogo.n_columns = 0;
    jogo.n_el_row = NULL;
    jogo.n_el_column = NULL;
    jogo.mode = '\0';
    jogo.x = 0;
    jogo.y = 0;
    jogo.layout = NULL;
    jogo.answer = 0;
}


//SET FUNCTIONS
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

void setBoardAnswer(int answer) {
    jogo.answer = answer;
}


//GET FUNCTIONS
int getBoardRows() {
    return jogo.n_rows;
}

int getBoardColumns() {
    return jogo.n_columns;
}

char getBoardMode() {
    return jogo.mode;
}

int getBoardElRow(int i) {
    return jogo.n_el_row[i];
}

int getBoardElColumn(int j) {
    return jogo.n_el_column[j];
}

char getBardLayoutElement(int i, int j) {
    return jogo.layout[i][j];
}

int getBoardAnswer() {
    return jogo.answer;
}

int getBoardCoordinateX() {
    return jogo.x;
}

int getBoardCoordinateY() {
    return jogo.y;
}


//FREE
void freeBoard() {
    if(jogo.n_el_row != NULL) {
        free(jogo.n_el_row);
    }
    if(jogo.n_el_column != NULL ) {
        free(jogo.n_el_column);
    }

    if(jogo.layout != NULL) {
        for(int i=0;i<jogo.n_rows;i++) {
            if(jogo.layout[i] != NULL) {
                free(jogo.layout[i]);
            }
        }
    }
    if(jogo.layout != NULL) {
        free(jogo.layout);
    }
    initBoard();
}
