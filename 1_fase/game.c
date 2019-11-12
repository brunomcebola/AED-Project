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
    char **layout;
    int answer;
} board;

board jogo;

//implementation of functions to manage 'jogo'

//INITITIALIZATIONS
void initBoard(void) {
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

int getBoardElColumn(int j) {
    return jogo.n_el_column[j];
}

char getBoardLayoutElement(int i, int j) {
    return jogo.layout[i][j];
}

char **getBoardAllLayout(void) {
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
            modeA();
            break;
        case 'B':
            modeB();
            break;
        case 'C':
            if(jogo.answer == 2){
                jogo.answer = 0;
            }
            else {
                modeC();
            }
            break;
        default:
            setBoardAnswer(-1);
    }
}

//FREE
void freeBoard(void) {
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
