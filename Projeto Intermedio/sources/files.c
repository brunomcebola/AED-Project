#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <ctype.h>

#include "../headers/files.h"
#include "../headers/bundle.h"

#define MAX(a,b) (a > b ? a : b)

FILE *in_file = NULL;
FILE *out_file = NULL;

//INNER FUNCTIONS
void initFile(const char *file) {
    char *file_name;
    int size_in = strlen(file), size_out = strlen(basename((char *)file));


    if(strcmp(".camp0", file+size_in-6) != 0) {
        exit(0);
    }
    in_file = fopen(file, "r");
    checkNull(in_file);

    file_name = (char *) calloc((size_out+2), sizeof(char)) ;
    checkNull(file_name);
    strncpy(file_name, basename((char *)file), size_out-6);

    out_file = fopen(strcat(file_name,".tents0"), "w");
    checkNull(out_file);

    free(file_name);
}



char readChar() {
    static char c;
    c = '\0';

    while(c != 'A' && c != 'T' && c != '.'){
        if((c = fgetc(in_file)) == EOF) {
            return '\0';
        }
    }

    return c;
}

void finishLayout() {
    static int rows, columns, i, j;
    rows = getBoardRows(), columns = getBoardColumns();

    for(i = 0; i < rows; i++){
        for(j = 0; j < columns; j++){
            readChar();
        }
    }
}

void maxSize() {
    static int max, linhas, colunas, mux, c, x, aux, max_row,
               max_column, *el_linha, *el_coluna, i, j;
    static char mode, *tabuleiro;

    static char el[100];

    max = 0, linhas = 0, colunas = 0, mux = 0, c = 0,
    x = 0, aux = 0, max_row = 0, max_column = 0,
    el_linha = NULL, el_coluna = NULL;
    mode = '\0', tabuleiro = NULL;

    while(checkEOF()) {
        aux = fscanf(in_file, "%d %d", &linhas , &colunas);
        if(aux != 2){
            break;
        }

<<<<<<< HEAD
        while(mode < 'A' || mode > 'Z'){
=======
        while(!isalpha(mode)){
>>>>>>> fe28e4b50a913fedf6845a1065736460fbf8cb5e
            mode = fgetc(in_file);
        }

        if(mode == 'B') {
            aux = fscanf(in_file, "%d %d", &x, &x);
        }
        else if(mode == 'C') {
            c = 1;
            mux = linhas * colunas;
            max = MAX(max, mux);
        }

        for(i = 0; i < linhas; i++) {
            el[0] = '\0';
            j = 0;
            el[j] = fgetc(in_file);
            while (isdigit(el[j])) {
                j++;
                el[j] = fgetc(in_file);
            }

        }

        for(i = 0; i < colunas; i++) {
            el[0] = '\0';
            j = 0;
            el[j] = fgetc(in_file);
            while (isdigit(el[j])) {
                j++;
                el[j] = fgetc(in_file);
            }
        }

        for(i = 0; i < linhas; i++){
            for(j = 0; j < colunas; j++){
                readChar();
            }
        }

        max_row = MAX(max_row, linhas);
        max_column = MAX(max_column, colunas);
        mode = '\0';
    }

    if(c == 1){
        tabuleiro = (char *) malloc(max * sizeof(char));
        checkNull(tabuleiro);
    }

    el_linha = (int *) malloc(max_row * sizeof(int));
    checkNull(el_linha);
    el_coluna = (int *) malloc(max_column * sizeof(int));
    checkNull(el_coluna);
    setBoardArrays(tabuleiro, el_linha, el_coluna);

}

int readLayout() {
    static char *tabuleiro, c;
    static int sum_tents, trees, linha_atual, coluna_atual,
               rows, columns, tents_row, *tents_column, j;

    sum_tents = 0, trees = 0, linha_atual = 0, coluna_atual = 0,
    rows = getBoardRows(), columns = getBoardColumns(), tents_row = 0,
    tents_column = NULL, tabuleiro = getBoardLayout(), c = '\0';

    if(getBoardMode() == 'C') {
        //get summation of tents in rows
        if(getBoardAnswer() == 2){
            return 1;
        }

        sum_tents = getBoardSum();

        //save the actual number of tents in each column
        tents_column = (int *) calloc((unsigned int)columns , sizeof(int));
        checkNull(tents_column);

        while(linha_atual != rows) {

            if((c = fgetc(in_file)) == EOF) {
                return 0;
            }

            if(c == 'T' || c == 'A' || c == '.'){
                if (c == 'A') {
                    trees++;
                }
                else if(c == 'T') {
                    tents_column[coluna_atual]++;
                    tents_row++;
                }

                tabuleiro[linha_atual*columns+coluna_atual] = c;

                coluna_atual++;
                if(coluna_atual == columns) {
                    if(tents_row > getBoardElRow(linha_atual)) {
                        setBoardAnswer(2);
                    }
                    tents_row = 0;
                    linha_atual++;
                    coluna_atual = 0;
                }
            }
        }

        j = columns;
        while(j--) {
            if(tents_column[j] > getBoardElColumn(j)) {
                setBoardAnswer(2);
                break;
            }
        }

        if(sum_tents > trees) {
            setBoardAnswer(2);
            free(tents_column);
            return 1;
        }
        free(tents_column);
    }

    return 1;
}



//OUTER FUNCTIONS
int readFile() {
    if(!readRowsAndColumns()) {
        return 0;
    }

    if(!readMode()) {
        return 0;
    }

    if(!readElRowsAndColumns()) {
        return 0;
    }

    if(!readLayout()){
        return 0;
    }

    return 1;
}

void writeFile () {
    static char mode;
    mode = getBoardMode();

    if(fprintf(out_file, "%d %d %c ", getBoardRows(), getBoardColumns(), mode) < 0) {
        exit(0);
    }
    if(mode == 'B') {
        if(fprintf(out_file, "%d %d ", getBoardCoordinateX(), getBoardCoordinateY()) < 0) {
            exit(0);
        }
    }
    if(fprintf(out_file, "%d\n\n", getBoardAnswer()) < 0) {
        exit(0);
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
