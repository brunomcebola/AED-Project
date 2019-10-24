#include <stdlib.h>
#include <stdio.h>

#include "../headers/files.h"
#include "../headers/game.h"


int main(int argc, char const *argv[]) {
    //check if two arguments are passed
    if (argc != 2) {
        exit(0);
    }

    initFile(argv[1]);
    initBoard();

    while(checkEOF()) {
        if(readFile() == 0) {
            break;
        };
        printLayout();
        freeBoard();
    }

    terminateFile();

    printf("aaa\n");

    return 0;
}
