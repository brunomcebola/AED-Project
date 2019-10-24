#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "files.h"
#include "game.h"


int main(int argc, char const *argv[]) {
    //check if two arguments are passed
    if (argc != 2) {
        exit(0);
    }

    initFile(argv[1]);
    initBoard();

    readFile();

    printLayout();

    return 0;
}
