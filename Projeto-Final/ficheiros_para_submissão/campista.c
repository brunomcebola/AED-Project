#include <stdlib.h>
#include <stdio.h>

#include "files.h"
#include "game.h"
#include "solver.h"

int main(int argc, char const *argv[]) {
    //check if two arguments are passed
    if (argc != 2) {
        exit(0);
    }

    initFile(argv[1]);
    initBoard();

    maxSize();
    begining();

    while(!reachedEOF()) {
        if(readFile()) {
            if(getBoardAnswer() != -1) {
                solver();
            }
            writeFile();
        }
        setBoardAnswer(0);
    }
    terminateFile();

    return 0;
}
