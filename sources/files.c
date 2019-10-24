#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/files.h"
#include "../headers/game.h"
#include "../headers/bundle.h"

FILE *fp = NULL;
char *file_name = NULL;

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

int readFile() {
    int linhas = 0, colunas = 0, linha_atual = 0, coluna_atual = 0;
    int *el_linha = NULL, *el_coluna = NULL;
    char c = '\0', mode = '\0';
    char **tabuleiro = NULL;

    if(fscanf(fp, "%d %d", &linhas , &colunas) != 2) {
        return 0;
    }

    if(fscanf(fp, "%c", &mode) != 1 ) {
        return 0;
    }

    el_linha = (int *) malloc(linhas * sizeof(int));
    checkNull(el_linha);
    for(int i=0; i<linhas; i++) {
        if(fscanf(fp, "%d", &el_linha[i]) != 1 ) {
            return 0;
        }
    }

    el_coluna = (int *) malloc(colunas * sizeof(int));
    checkNull(el_coluna);
    for(int i=0; i<colunas; i++) {
        if(fscanf(fp, "%d", &el_coluna[i]) != 1 ) {
            return 0;
        }
    }

    tabuleiro = (char **) malloc(linhas * sizeof(char *));
    checkNull(tabuleiro);
    for(int i = 0; i < linhas; i++) {
        tabuleiro[i] = (char *) malloc(colunas * sizeof(char));
        checkNull(tabuleiro[i]);
    }

    while(linha_atual!=linhas) {
        if(fscanf(fp, "%c", &c) != 1) {
            return 0;
        }
        if(c == 'T' || c == 'A' || c == '.'){
            tabuleiro[linha_atual][coluna_atual] = c;
            coluna_atual++;
            if(coluna_atual == colunas) {
                linha_atual++;
                coluna_atual = 0;
            }
        }
    }

    setBoardRows(linhas);
    setBoardColumns(colunas);
    setBoardMode(mode);
    setBoardElRows(el_linha);
    setBoardElColumns(el_coluna);
    setBoardLayout(tabuleiro);

    return 1;
}

int checkEOF(){
    return !feof(fp);
}

void terminateFile() {
    fclose(fp);
    free(file_name);
}
