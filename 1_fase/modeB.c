#include "modeA.h"
#include "game.h"
#include "files.h"

void modeB() {
    int row = getBoardCoordinateX(), column = getBoardCoordinateY(),
        rows = getBoardRows(), columns = getBoardColumns(),
        el_row = getBoardElRow(row), el_column = getBoardElColumn(column),
        tents_column = 0, exists = 0, tree = 0, tent = 0, tents_row = 0,
        total_trees = 0, total_tents = 0;

    char c = '\0';

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            c = readChar();

            //element being verified
            if(i == row && j == column) {
                if(c == 'A') {
                    tree = 1;
                }
                continue;
            }

            if(c == 'T') {
                total_tents++;
            }
            else if(c == 'A') {
                total_trees ++;
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

    if(exists == 0 || tree >= 1 || tent >= 1 || total_tents >= total_trees) {
        setBoardAnswer(1);
        return;
    }

    if(tents_column >= el_column || tents_row >= el_row  || el_column == 0 || el_row == 0){
        setBoardAnswer(1);
        return;
    }
}
