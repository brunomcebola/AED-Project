#include "modeA.h"
#include "game.h"
#include "files.h"

void modeA() {
    int rows = getBoardRows(), columns = getBoardColumns(),
        tents = getBoardSum(), trees = 0;

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
