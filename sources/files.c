#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/files.h"
#include "../headers/game.h"
#include "../headers/bundle.h"

FILE *fp = NULL;
char *file_name = NULL;


//INNER FUNCTIONS
void initFile(const char *file) {
    int size = strlen(file);

    if(strcmp(".camp0", file+size-6) != 0) {
        exit(0);
    }
    fp = fopen(file, "r");
    checkNull(fp);
    file_name = (char *) malloc((size-5) * sizeof(char)) ;
    checkNull(file_name);
    strncpy(file_name, file, size-6);
}

int readRowsAndColumns() {
    int linhas = 0, colunas = 0;

    //get number of rows and columns
    if(fscanf(fp, "%d %d", &linhas , &colunas) != 2) {
        return 0;
    }
    setBoardRows(linhas);
    setBoardColumns(colunas);
    return 1;
}

int readMode() {
    char mode = '\0';
    int x = 0, y = 0;

    //get test mode
    while(mode != 'A' && mode != 'B' && mode != 'C'){
        if(fscanf(fp, "%c", &mode) != 1 ) {
            return 0;
        }
    }

    //get coordinates for test mode 'B'
    if(mode == 'B') {
        if(fscanf(fp, "%d %d", &x , &y) != 2) {
            return 0;
        }
    }

    setBoardMode(mode);
    setBoardCoordinates(x, y);

    return 1;
}

int readElRowsAndColumns() {
    int *el_linha = NULL, *el_coluna = NULL;

    //get number of elements in each row
    el_linha = (int *) malloc(getBoardRows() * sizeof(int));
    checkNull(el_linha);
    for(int i=0; i<getBoardRows(); i++) {
        if(fscanf(fp, "%d", &el_linha[i]) != 1 ) {
            free(el_linha);
            return 0;
        }
    }

    //get number of elemets in each column
    el_coluna = (int *) malloc(getBoardColumns() * sizeof(int));
    checkNull(el_coluna);
    for(int i=0; i<getBoardColumns(); i++) {
        if(fscanf(fp, "%d", &el_coluna[i]) != 1 ) {
            free(el_linha);
            free(el_coluna);
            return 0;
        }
    }

    setBoardElRows(el_linha);
    setBoardElColumns(el_coluna);

    return 1;
}

int readLayout() {
    char **tabuleiro = NULL;
    char c = '\0';
    int linha_atual = 0, coluna_atual = 0;

    tabuleiro = (char **) malloc(getBoardRows() * sizeof(char *));
    checkNull(tabuleiro);
    for(int i = 0; i < getBoardRows(); i++) {
        tabuleiro[i] = (char *) malloc(getBoardColumns() * sizeof(char));
        checkNull(tabuleiro[i]);
    }
    while(linha_atual!=getBoardRows()) {
        if(fscanf(fp, "%c", &c) != 1) {
            return 0;
        }
        if(c == 'T' || c == 'A' || c == '.'){
            tabuleiro[linha_atual][coluna_atual] = c;
            coluna_atual++;
            if(coluna_atual == getBoardColumns()) {
                linha_atual++;
                coluna_atual = 0;
            }
        }
    }

    setBoardLayout(tabuleiro);

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

int checkEOF(){
    return !feof(fp);
}

void terminateFile() {
    fclose(fp);
    free(file_name);
}
