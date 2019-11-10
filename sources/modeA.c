#include "../headers/modeA.h"
#include "../headers/game.h"

void modeA(void) {
    int tents_row = 0, tents_column = 0, trees = 0;

    for(int i=0; i<getBoardRows(); i++) {
        tents_row += getBoardElRow(i);
    }
    for(int j=0; j<getBoardColumns(); j++) {
        tents_column += getBoardElColumn(j);
    }

    if(tents_row != tents_column) {
        setBoardAnswer(0);
        return;
    }


    for(int i=0; i<getBoardRows(); i++) {
        for(int j=0; j<getBoardColumns(); j++) {
            if(getBoardLayoutElement(i,j) == 'A') {
                trees++;
            }
        }
    }

    if(tents_row > trees) {
        setBoardAnswer(0);
        return;
    }

    setBoardAnswer(1);
}
