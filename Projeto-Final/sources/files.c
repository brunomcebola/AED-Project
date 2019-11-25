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


char readChar() {

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
