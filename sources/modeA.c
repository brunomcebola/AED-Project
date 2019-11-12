#include "../headers/modeA.h"
#include "../headers/game.h"

void modeA() {
    int tents_row = 0, tents_column = 0, trees = 0;

    for(int i=0; i<getBoardRows(); i++) {
        tents_row += getBoardElRow(i);
    }
    for(int j=0; j<getBoardColumns(); j++) {
        tents_column += getBoardElColumn(j);
    }

    if(tents_row != tents_column) {
        return;
    }


    for(int i=0; i<getBoardRows(); i++) {
        for(int j=0; j<getBoardColumns(); j++) {
            if(getBardLayoutElement(i,j) == 'A') {
                trees++;
            }
        }
    }

    if(tents_row > trees) {
        return;
    }

    setBoardAnswer(1);
}
