#include "../headers/modeA.h"
#include "../headers/game.h"
#include "../headers/files.h"

void modeA() {
    static int rows, columns, tents, trees, i, j;
    rows = getBoardRows(), columns = getBoardColumns(), tents = getBoardSum(), trees = 0;

    for(i = 0; i < rows; i++){
        for(j = 0; j < columns; j++){
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
