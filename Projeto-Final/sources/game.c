#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/game.h"
#include "../headers/files.h"

typedef struct {
    int n_rows, n_columns;
    int *n_el_row, *n_el_column;
    char *buffer;
    char *layout;
    int answer;
    int sum;
    int season;
} board;

board jogo;

//implementation of functions to manage 'jogo'

//INITITIALIZATIONS
void initBoard(void) {
    jogo.n_rows = 0;
    jogo.n_columns = 0;
    jogo.answer = 0;
    jogo.sum = 0;
    jogo.season = 0;
}


//SET FUNCTIONS
void setBoardBio(int rows, int columns, int sum, int answer) {
    jogo.n_rows = rows;
    jogo.n_columns = columns;
    jogo.sum = sum;
    jogo.answer = answer;
}

void setBoardArrays(char *layout, int *n_el_row, int *n_el_column, char *buffer) {
    jogo.layout = layout;
    jogo.n_el_row = n_el_row;
    jogo.n_el_column = n_el_column;
    jogo.buffer = buffer;
}


void setBoardSeason(int season) {
    jogo.season = season;
}


//GET FUNCTIONS
char *getBoardBuffer(void) {
    return jogo.buffer;
}

int getBoardSum(void) {
    return jogo.sum;
}

int getBoardRows(void) {
    return jogo.n_rows;
}

int getBoardColumns(void) {
    return jogo.n_columns;
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

//FREE
void freeBoard(void) {
    free(jogo.n_el_row);
    free(jogo.n_el_column);
    free(jogo.layout);
    free(jogo.buffer);
}
