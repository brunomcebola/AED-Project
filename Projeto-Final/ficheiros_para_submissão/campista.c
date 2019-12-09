/*******************************************************************************
 * 2019-2020 AED - grupo 65
 * Last modified: 2019-12-09
 *
 * NAME
 *      campista.c
 *
 * DESCRIPTION
 *      Implementation of the main function of the program
 *
 * COMMENTS
 *      none
 *
 ******************************************************************************/

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

    initFile(argv[1]); //opens files
    initBoard();

    maxSize(); //checks size of biggest valid board
    begining(); //places file pointer to the beggining of the file

    while(!reachedEOF()) { //while there are puzzles to solve
        if(readFile()) { //reads map
            if(getBoardAnswer() != -1) { //if it didn't fail inicial checks
                solver(); //tries and solves it
            }
            writeFile(); //writes answer on output file
        }
        setBoardAnswer(0);
    }
    terminateFile(); //closes both files

    return 0;
}
