#include "modeA.h"
#include "game.h"
#include "files.h"

void modeB() {
    int row = getBoardCoordinateX(), column = getBoardCoordinateY(),
        tents_row = 0, tents_column = 0, exists = 0, tree = 0, tent = 0;

    char c = '\0';

    for(int i=0;i<getBoardRows();i++){
        for(int j=0;j<getBoardColumns();j++){
            c = readChar();

            //element being verified
            if(i == row && j == column) {
                if(c == 'A') {
                    tree = 1;
                }
                continue;
            }

            //get number of tents in column
            if(j == column && c == 'T'){
                tents_column++;
            }

            //get number of tents in row
            if(i == row && c == 'T'){
                tents_row++;
            }

            //checks up and down for trees
            if((j == column && i == row - 1) || (j == column && i == row + 1)) {
                if(c == 'A'){
                    exists++;
                }
            }

            //checks left and right for trees
            if((j == column - 1 && i == row) || (j == column + 1 && i == row)) {
                if(c == 'A'){
                    exists++;
                }
            }

            //checks for tents in the upper row
            if(i == row - 1 && (j == column - 1 || j == column || j == column + 1)) {
                if(c == 'T') {
                    tent++;
                }
            }

            //checks for tents in the actual row
            if(i == row && (j == column - 1 || j == column + 1)) {
                if(c == 'T') {
                    tent++;
                }
            }

            //checks for tents in the bottom row
            if(i == row + 1 && (j == column - 1 || j == column || j == column + 1)) {
                if(c == 'T') {
                    tent++;
                }
            }

        }
    }

    if(!exists || tree >= 1 || tent >= 1) {
        setBoardAnswer(1);
        return;
    }

    if(tents_column >= getBoardElColumn(column) || tents_row >= getBoardElRow(row)){
        setBoardAnswer(1);
        return;
    }
}
