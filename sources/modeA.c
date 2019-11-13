#include "../headers/modeA.h"
#include "../headers/game.h"
#include "../headers/files.h"

void modeA() {
    int rows = getBoardRows(), columns = getBoardColumns(),
        tents = 0, trees = 0;

    //get summation of tents in rows
    for(int i = 0; i < rows; i++) {
        tents += getBoardElRow(i);
    }

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            if(readChar() == 'A') {
                trees++;
            }
        }
    }
    //number of trees must be greater or equal
    //to summation of tents in rows/columns
    if(tents > trees) {
        return;
    }

    //if all correct then answer is 1
    setBoardAnswer(1);
}
