#include "../headers/modeA.h"
#include "../headers/game.h"

void modeB() {
    //1 -> n há tenda de certeza
    int row = getBoardCoordinateX(), column = getBoardCoordinateY(),
        rows = getBoardRows(), columns = getBoardColumns(), tents = 0;
    char **layout = getBoardAllLayout();

    if(layout[row-1 < 0 ? row+1 : row-1][column] != 'T' && layout[row+1 == rows ? row-1 : row+1][column] != 'T' &&
        layout[row][column-1 < 0 ? column+1 : column-1] != 'T' && layout[row][column+1 == columns ? columns-1 : columns+1] != 'T') {
            setBoardAnswer(1);
            return;
    }

    if(layout[row-1 < 0 ? row+1 : row-1][column] != 'T' && layout[row+1 == rows ? row-1 : row+1][column] != 'T' &&
        layout[row][column-1 < 0 ? column+1 : column-1] != 'T' && layout[row][column+1 == columns ? columns-1 : columns+1] != 'T') {
            setBoardAnswer(1);
            return;
    }

    for(int i = 0; i < rows; i++) {
        if(i == row) {
            continue;
        }
        if(layout[i][column] == 'T') {
            tents++;
        }
    }

    if(tents == getBoardElColumn(column)) {
        setBoardAnswer(1);
        return;
    }

    tents = 0;

    for(int i = 0; i < columns; i++) {
        if(i == column) {
            continue;
        }
        if(layout[row][i] == 'T') {
            tents++;
        }
    }

    if(tents == getBoardElRow(row)) {
        setBoardAnswer(1);
        return;
    }
}
