#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "bundle.h"


void readFile(FILE *fp) {
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





int main(int argc, char const *argv[]) {
    FILE *fp = NULL;

    init();

    //check if two arguments are passed
    if (argc != 2) {
        exit(0);
    }

    if(strcmp(".camp0", argv[1]+strlen(argv[1])-6) != 0) {
        exit(0);
    }

    fp = fopen(argv[1], "r");

    checkNull(fp);

    readFile(fp);

    fclose(fp);

    printLayout();

    return 0;
}
