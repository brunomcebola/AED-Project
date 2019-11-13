#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "modeA.h"
#include "modeB.h"
#include "modeC.h"
#include "files.h"

typedef struct {
    int n_rows, n_columns, x, y;
    int *n_el_row, *n_el_column;
    char mode;
    char *layout;
    int answer;
    int sum;
} board;

board jogo;

//implementation of functions to manage 'jogo'

//INITITIALIZATIONS
void initBoard(void) {
    jogo.n_rows = 0;
    jogo.n_columns = 0;
    jogo.mode = '\0';
    jogo.x = 0;
    jogo.y = 0;
    jogo.answer = 0;
    jogo.sum = 0;
}


//SET FUNCTIONS
void setBoardSum(int sum) {
    jogo.sum = sum;
}

void setBoardRowsNColumns(int lines, int columns) {
    jogo.n_rows = lines;
    jogo.n_columns = columns;
}

void setBoardMode(char mode) {
    jogo.mode = mode;
}

void setBoardCoordinates(int x, int y) {
    jogo.x = x;
    jogo.y = y;
}

void setBoardArrays(char *layout, int *n_el_row, int *n_el_column) {
    jogo.layout = layout;
    jogo.n_el_row = n_el_row;
    jogo.n_el_column = n_el_column;
}

void setBoardAnswer(int answer) {
    jogo.answer = answer;
}


//GET FUNCTIONS
int getBoardSum(void) {
    return jogo.sum;
}

int getBoardRows(void) {
    return jogo.n_rows;
}

int getBoardColumns(void) {
    return jogo.n_columns;
}

char getBoardMode(void) {
    return jogo.mode;
}

int getBoardElRow(int i) {
    return jogo.n_el_row[i];
}

int *getBoardAllElRow(void) {
    return jogo.n_el_row;
}

int *getBoardAllElColumn(void) {
    return jogo.n_el_column;
}


int getBoardElColumn(int j) {
    return jogo.n_el_column[j];
}

char getBoardLayoutElement(int i, int j) {
    return jogo.layout[i*jogo.n_columns + j];
}

char *getBoardLayout(void) {
    return jogo.layout;
}

int getBoardAnswer(void) {
    return jogo.answer;
}

int getBoardCoordinateX(void) {
    return jogo.x;
}

int getBoardCoordinateY(void) {
    return jogo.y;
}


void selMode(void) {
    if(jogo.answer == -1) {
        finishLayout();
        return;
    }
    switch (jogo.mode) {
        case 'A':
            if(jogo.answer == 2){
                jogo.answer = 0;
                finishLayout();
                break;
            }
            modeA();

            break;
        case 'B':
            modeB();
            break;
        case 'C':
            if(jogo.answer == 2){
                jogo.answer = 1;
                break;
            }
            modeC();
            break;
        default:
            setBoardAnswer(-1);
    }
}

//FREE
void freeBoard(void) {
    free(jogo.n_el_row);
    free(jogo.n_el_column);
    free(jogo.layout);
}
