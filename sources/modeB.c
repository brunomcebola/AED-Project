#include "../headers/modeA.h"
#include "../headers/game.h"

#include <stdio.h>

void modeB() {
    int row = getBoardCoordinateX(), column = getBoardCoordinateY(),
        rows = getBoardRows(), columns = getBoardColumns(), tents = 0, exists = 0;
    const int auxTrees[4][2] = {{-1, 0}, {0, -1}, {0, 1}, {1, 0}};
    const int auxTents[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
    char **layout = getBoardAllLayout();

    for(int i = 0; i < 4; i++) {
        int aux1 = row + auxTrees[i][0];
        int aux2 = column + auxTrees[i][1];
        if(aux1 < 0 || aux1 == rows || aux2 < 0 || aux2 == columns) {
            continue;
        }
        if(layout[aux1][aux2] == 'A') {
            exists++;
        }
    }

    if(!exists || layout[row][column] == 'A') {
        setBoardAnswer(1);
        return;
    }

    for(int i = 0; i < 8; i++) {
        int aux1 = row + auxTents[i][0];
        int aux2 = column + auxTents[i][1];
        if(aux1 < 0 || aux1 == rows || aux2 < 0 || aux2 == columns) {
            continue;
        }
        if(layout[aux1][aux2] == 'T') {
            setBoardAnswer(1);
            return;
        }
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
