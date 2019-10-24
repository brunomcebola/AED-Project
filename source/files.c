#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "files.h"
#include "game.h"
#include "bundle.h"

FILE *fp = NULL;
char *file_name = NULL;

void initFile(const char *file) {
    int size = strlen(file);

    if(strcmp(".camp0", file+size-6) != 0) {
        exit(0);
    }
    fp = fopen(file, "r");
    file_name = (char *) malloc((size-5) * sizeof(char)) ;
    checkNull(file_name);
    strncpy(file_name, file, size-6);
}



void readFile() {
    int line = 0, column = 0;
    int linhas = 0, colunas = 0;
    char c = '\0';
    char **tabuleiro;

    fscanf(fp, "%d %d", &linhas , &colunas);

    tabuleiro = (char **) malloc(linhas * sizeof(char *));
    checkNull(tabuleiro);
    for(int i = 0; i < linhas; i++) {
        tabuleiro[i] = (char *) malloc(colunas * sizeof(char));
        checkNull(tabuleiro[i]);
    }

    while(line!=linhas) {
        fscanf(fp, "%c", &c);
        if(c == 'T' || c == 'A' || c == '.'){
            tabuleiro[line][column] = c;
            column++;
            if(column == colunas) {
                line++;
                column = 0;
            }
        }
    }


    setRows(linhas);
    setColumns(colunas);
    setLayout(tabuleiro);

}
