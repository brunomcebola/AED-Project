#include <stdlib.h>
#include <stdio.h>

#include "../headers/files.h"
#include "../headers/game.h"
#include "../headers/modeA.h"


int main(int argc, char const *argv[]) {
    int valid = 0;

    //check if two arguments are passed
    if (argc != 2) {
        exit(0);
    }

    initFile(argv[1]);
    initBoard();

    while(checkEOF()) {
        valid = readFile();
        if(valid) {
            modeA();
        }
        freeBoard();
    }

    terminateFile();

    return 0;
}
