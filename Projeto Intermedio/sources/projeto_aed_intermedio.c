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

    maxSize();
    begining();

    while(checkEOF()) {
        if(readFile()) {
            selMode();
            writeFile();
        }
        setBoardAnswer(0);
    }
    terminateFile();

    return 0;
}
