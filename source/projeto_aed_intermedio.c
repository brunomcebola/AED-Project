#include <stdlib.h>

#include "files.h"
#include "game.h"


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

    return 0;
}
