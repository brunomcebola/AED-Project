#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <ctype.h>

#include "../headers/files.h"
#include "../headers/game.h"
#include "../headers/bundle.h"

#include "../headers/solver.h"

#define MAX(a,b) (a > b ? a : b)

typedef struct _Board {
    int valid, sum;
    struct _Board *next;
} Board;

Board *board = NULL;
Board *board_ptr = NULL;

FILE *in_file = NULL;
FILE *out_file = NULL;

HeadNode *row_vector = NULL, *column_vector = NULL;


void initFile(const char *file) {
    char *file_name;
    int size_in = strlen(file), size_out = strlen(basename((char *)file));

    if(strcmp(".camp", file+size_in-5) != 0) {
        exit(0);
    }
    in_file = fopen(file, "r");

    file_name = (char *) calloc((size_out+2), sizeof(char));
    strncpy(file_name, basename((char *)file), size_out-5);

    out_file = fopen(strcat(file_name,".tents"), "w");

    checkNull(2, in_file, file_name, out_file);

    free(file_name);
}

void finishLayout(int flag) {
    int rows = -1, columns, i;

    if(flag) {
        fscanf(in_file, " %d %d", &rows, &columns);

        for(i = 0; i < rows; i++) {
            fscanf(in_file, " %*d");
        }

        for(i = 0; i < columns; i++) {
            fscanf(in_file, " %*d");
        }
        setBoardBio(rows, columns, 0);
        setBoardAnswer(-1);
    }
    else {
        rows = getBoardRows();
        for(i = 0; i < rows; i++){
            fscanf(in_file, " %*s");
        }
    }

}

void maxSize() {
    int max = 0, linhas = 0, colunas = 0, mux = 0, tents = 0,
    aux = 0, max_row = 0, max_column = 0, i = 0, sum_tents_row = 0,
    valid = 1, sum_tents_column = 0;

    char *tabuleiro = NULL, *buffer = NULL;
    Board *file_aux = NULL;

    while(!feof(in_file)) {
        valid = 1;
        sum_tents_row = 0;
        sum_tents_column = 0;

        aux = fscanf(in_file, " %d %d", &linhas , &colunas);
        if(aux != 2){
            break;
        }

        for(i = 0; i < linhas; i++) {
            aux = fscanf(in_file, " %d", &tents);
            sum_tents_row += tents;
        }

        for(i = 0; i < colunas; i++) {
            aux = fscanf(in_file, " %d", &tents);
            sum_tents_column += tents;
        }

        for(i = 0; i < linhas; i++){
            aux = fscanf(in_file, " %*s");
        }

        if(sum_tents_row != sum_tents_column) {
            valid = 0;
        }

        mux = linhas * colunas;
        max = valid ? MAX(max, mux): max;

        max_row = valid ? MAX(max_row, linhas): max_row;
        max_column = valid ? MAX(max_column, colunas): max_column;

        file_aux = malloc(sizeof(Board));
        checkNull(1, file_aux);
        file_aux -> valid = valid;
        file_aux -> sum = sum_tents_row;
        file_aux -> next = NULL;

        if(board == NULL) {
            board = file_aux;
        }
        else {
            board_ptr->next = file_aux;
        }
        board_ptr = file_aux;

    }

    if(max != 0){
        tabuleiro = (char *) malloc((max+1) * sizeof(char));

        buffer = (char *) malloc((max_column+1) * sizeof(char));

        row_vector = (HeadNode *) malloc(max_row * sizeof(HeadNode));

        column_vector = (HeadNode *) malloc(max_row * sizeof(HeadNode));

        checkNull(4, tabuleiro, buffer, row_vector, column_vector);

        setBoardArrays(tabuleiro, buffer);
    }

    board_ptr = board;

}

int readBio(void) {
    int rows = 0, columns = 0, tents = 0, trash = 0, i = 0;

    //get number of rows and columns
    if(board_ptr == NULL) {
        fscanf(in_file, " ");
        return 0;
    }

    if(board_ptr -> valid){
        trash = fscanf(in_file, " %d %d", &rows , &columns);
        if(trash != 2) {
            return 0;
        }

        for(i = 0; i < rows; i++) {
            trash = fscanf(in_file, " %d", &tents);
            row_vector[i].puzzleTents = tents;
            row_vector[i].tentsNeeded = tents;
            row_vector[i].first = NULL;
        }

        for(i = 0; i < columns; i++) {
            trash = fscanf(in_file, " %d", &tents);
            column_vector[i].puzzleTents = tents;
            column_vector[i].tentsNeeded = tents;
            column_vector[i].first = NULL;
        }

        //set data to board
        setBoardBio(rows, columns, board_ptr -> sum);
    }
    else {
        finishLayout(1);
    }

    board_ptr = board_ptr -> next;

    return 1;
}

int readLayout(void) {
    char *tabuleiro = getBoardLayout(), *buffer = getBoardBuffer();
    int trees = 0, i = 0, rows = getBoardRows(), columns = getBoardColumns(), j, num_asked_tents = getBoardSum();

    tabuleiro[0] = '\0';
    buffer[0]= '\0';

    for (i = 0; i < rows; i++) {

        fscanf(in_file, " %s", buffer);

        for (j = 0; j < columns; j++) {
            if (buffer[j] == 'A') {
                trees++;
            }

        }

        strcat(tabuleiro, buffer);
    }

    if (trees == num_asked_tents) {
        setBoardSeason(1);
    } else if (trees > num_asked_tents) {
        setBoardSeason(2);
    } else {
        return 0;
    }

    return 1;
}

int readFile(void){
    if(!readBio()){
        return 0;
    }
    if(getBoardAnswer() == -1){
        finishLayout(0);
    }
    else {
        if(!readLayout()){
            return 0;
        }
    }
    return 1;
}

void writeFile () {
    int answer = getBoardAnswer(), rows = getBoardRows(),
        columns = getBoardColumns(), i = 0;
    char *layout = getBoardLayout();

    if(fprintf(out_file, "%d %d %d\n", rows, columns, answer) < 0) {
        exit(0);
    }

    if(answer == 1){
        for(i = 0; i < rows; i++){
            fprintf(out_file, "%.*s\n", columns, layout+(i*columns));
        }
    }
}

void begining(){
    fseek(in_file, 0, SEEK_SET) ;
}

int reachedEOF(void) {
    if (feof(in_file)) {
        return 1;
    } else {
        return 0;
    }
}

void terminateFile() {
    if(fclose(in_file) != 0) {
        exit(0);
    }

    if(fclose(out_file) != 0) {
        exit(0);
    }


    while(board != NULL){
        board_ptr = board;
        board = board->next;
        free(board_ptr);
    }


    freeBoard();
}
