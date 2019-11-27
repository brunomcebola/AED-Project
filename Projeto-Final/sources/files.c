#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <ctype.h>

#include "../headers/files.h"
#include "../headers/game.h"
#include "../headers/bundle.h"

#define MAX(a,b) (a > b ? a : b)

typedef struct _Board {
    int valid, sum;
    struct _Board *next;
} Board;

Board *board = NULL;
Board *board_ptr = NULL;

FILE *in_file = NULL;
FILE *out_file = NULL;

int reachedEOF(void) {
    if (feof(in_file)) {
        return 1;
    } else {
        return 0;
    }
}

void initFile(const char *file) {
    char *file_name;
    int size_in = strlen(file), size_out = strlen(basename((char *)file));


    if(strcmp(".camp", file+size_in-5) != 0) {
        exit(0);
    }
    in_file = fopen(file, "r");
    checkNull(in_file);

    file_name = (char *) calloc((size_out+2), sizeof(char)) ;
    checkNull(file_name);
    strncpy(file_name, basename((char *)file), size_out-5);

    out_file = fopen(strcat(file_name,".tents"), "w");
    checkNull(out_file);

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
    *el_linha = NULL, *el_coluna = NULL, valid = 1, sum_tents_column = 0;
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
        checkNull(file_aux);
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

    if(max == 0){
        tabuleiro = (char *) malloc((max+1) * sizeof(char));
        checkNull(tabuleiro);

        buffer = (char *) malloc((max_column+1) * sizeof(char));
        checkNull(buffer);

        el_linha = (int *) malloc(max_row * sizeof(int));
        checkNull(el_linha);

        el_coluna = (int *) malloc(max_column * sizeof(int));
        checkNull(el_coluna);

        setBoardArrays(tabuleiro, el_linha, el_coluna, buffer);
    }

    board_ptr = board;

}

int readBio(void) {
    int rows = 0, columns = 0, tents = 0, trash = 0, i = 0,
        *el_linha = getBoardAllElRow(), *el_coluna = getBoardAllElColumn();

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
            el_linha[i] = tents;
        }

        for(i = 0; i < columns; i++) {
            trash = fscanf(in_file, " %d", &tents);
            el_coluna[i] = tents;
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
