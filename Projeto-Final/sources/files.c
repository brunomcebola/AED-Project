#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <ctype.h>

#include "../headers/files.h"
#include "../headers/game.h"
#include "../headers/bundle.h"

#define MAX(a,b) (a > b ? a : b)

FILE *in_file = NULL;
FILE *out_file = NULL;


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
    strncpy(file_name, basename((char *)file), size_out-6);

    out_file = fopen(strcat(file_name,".tents"), "w");
    checkNull(out_file);

    free(file_name);
}

/*

    TODO: chamar finishlayout when readbio da merda

*/

int readBio() {
    int rows = 0, columns = 0, tents = 0, sum_tents_row = 0, trash = 0, i = 0,
        sum_tents_column = 0, answer = 0, *el_linha = getBoardAllElRow(),
        *el_coluna = getBoardAllElColumn();

    //get number of rows and columns
    if(fscanf(in_file, "%d %d", &rows , &columns) != 2) {
        return 0;
    }

    for(i = 0; i < rows; i++) {
        trash = fscanf(in_file, " %d", &tents);
        el_linha[i] = tents;
        sum_tents_row += tents;
    }

    for(i = 0; i < columns; i++) {
        trash = fscanf(in_file, " %d", &tents);
        el_coluna[i] = tents;
        sum_tents_column += tents;
    }

    if(sum_tents_row != sum_tents_column) {
        answer = -1;
    }

    //set data to board
    setBoardBio(rows, columns, sum_tents_row, answer);
    return 1;
}

void finishLayout() {
    int rows, i;
    rows = getBoardRows();

    for(i = 0; i < rows; i++){
        fscanf(in_file, " %*s");
    }
}


/*
*               TODO: check if board is valid with preliminary checks before allocating memory
*
*/


void maxSize() {
    int max = 0, linhas = 0, colunas = 0, mux = 0, max_string = 0,
    aux = 0, max_row = 0, max_column = 0, i = 0,
    *el_linha = NULL, *el_coluna = NULL;
    char *tabuleiro = NULL, *buffer = NULL;


    while(checkEOF()) {
        aux = fscanf(in_file, " %d %d", &linhas , &colunas);
        if(aux != 2){
            break;
        }

        max_string = MAX(max_string, colunas);
        mux = linhas * colunas;
        max = MAX(max, mux);

        for(i = 0; i < linhas; i++) {
            aux = fscanf(in_file, " %*d");
        }

        for(i = 0; i < colunas; i++) {
            aux = fscanf(in_file, " %*d");
        }

        for(i = 0; i < linhas; i++){
            aux = fscanf(in_file, " %*s");
        }

        max_row = MAX(max_row, linhas);
        max_column = MAX(max_column, colunas);
    }


    tabuleiro = (char *) malloc((max+1) * sizeof(char));
    checkNull(tabuleiro);

    buffer = (char *) malloc(max_string * sizeof(char));
    checkNull(buffer);

    el_linha = (int *) malloc(max_row * sizeof(int));
    checkNull(el_linha);

    el_coluna = (int *) malloc(max_column * sizeof(int));
    checkNull(el_coluna);

    setBoardArrays(tabuleiro, el_linha, el_coluna, buffer);

}


/*
*           0 for error, 1 for good
*           TODO: put flag for high or low season
*
*/
int readLayout() {
    char *tabuleiro = getBoardLayout(), c = '\0', *buffer = getBoardBuffer();
    int trees = 0, i = 0, rows = getBoardRows(), columns = getBoardColumns(), j, num_asked_tents = getBoardSum();

    tabuleiro = "";

    for (i = 0; i < rows; i++) {

        fscanf(in_file, " %s", buffer);

        for (j = 0; i < columns; i++) {
            if (buffer[i] == 'A') {
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


void writeFile () {
    int answer = getBoardAnswer(), rows = getBoardRows(),
        columns = getBoardColumns(), i = 0, j = 0;
    char *layout = getBoardLayout();

    if(fprintf(out_file, "%d %d %d\n", rows, columns, answer) < 0) {
        exit(0);
    }

    if(answer == 1){
        for(i = 0; i < rows; i++){
            for(j = 0; j < columns; j++)
        }
    }
}

void begining(){
    fseek(in_file, 0, SEEK_SET) ;
}

int checkEOF(){
    static char aux;
    static int end;
    end = 0, aux = '\0';

    while((aux = fgetc(in_file))){
        end = feof(in_file);
        if(end){
            return !end;
        }

        if(isdigit(aux)) {
            fseek(in_file, -1, SEEK_CUR);
            return 1;
        }
    }
    return 0;
}

void terminateFile() {
    if(fclose(in_file) != 0) {
        exit(0);
    }

    if(fclose(out_file) != 0) {
        exit(0);
    }

    freeBoard();
}
