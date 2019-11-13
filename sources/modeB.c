#include "../headers/modeA.h"
#include "../headers/game.h"
#include "../headers/files.h"

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

            if(c == '.') {
                continue;
            }

            //element being verified
            if(i == row && j == column) {
                if(c == 'A') {
                    tree = 1;
                }
                continue;
            }


            if(c == 'T') {
                total_tents++;
                if(j == column) {
                    tents_column++;
                }
                if(i == row){
                    tents_row++;
                }
            }
            else if(c == 'A') {
                total_trees ++;
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
