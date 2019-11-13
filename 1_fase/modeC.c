#include "stdio.h"
#include "stdlib.h"
#include "modeC.h"
#include "game.h"
#include "bundle.h"

int tentLooksForTree(int coordX, int coordY);
int treeLooksForTent(int coordX, int coordY);
int validateBoard(void);


typedef struct {
    int lineEdge, columnEdge;
    int numOfTents, numOfTrees;
    char *board;
} mode_c_board;

mode_c_board puzzle = {0, 0, 0, 0, NULL};

/*
*   kickstarts modeC checking functions and sets answer for the problem
*
*    no return value
*
*/
void modeC(void) {
  static int retVal = 8;

  puzzle.lineEdge = getBoardRows();
  puzzle.columnEdge = getBoardColumns();
  puzzle.board = getBoardLayout();

  retVal = validateBoard();
  setBoardAnswer(retVal);
}



/*  cycles through all positions of the board
*   starts path finding operations
*
*   1 for error, 0 for good -- it is different from others
*
*
*/
int validateBoard(void) {
  static int retVal = 0, i = 0, j = 0;
  puzzle.numOfTents = 0, puzzle.numOfTrees = 0;

  for (i = 0; i < puzzle.lineEdge; i++) {
    for (j = 0; j < puzzle.columnEdge; j++) {
      if ((puzzle.board[i*puzzle.columnEdge+j]) == 'T') {
        retVal = tentLooksForTree(j, i);
        if ((puzzle.numOfTrees < puzzle.numOfTents) || (retVal == -1)) {
          return 1;
        }
        puzzle.numOfTents = 0, puzzle.numOfTrees = 0; //reinicializes count
      } else if ((puzzle.board[i*puzzle.columnEdge+j]) == 'A') {
        retVal = treeLooksForTent(j, i);
        if ((puzzle.numOfTrees < puzzle.numOfTents) || (retVal == -1)) {
          return 1;
        }
        puzzle.numOfTents = 0, puzzle.numOfTrees = 0; //reinicializes count
      }
    }
  }
  return 0;
}


/*  cycles through all valid positions looking for a tree to continue the path or
*   an invalid tent
*
*   -1 for error, 0 for good
*
*
*/
int tentLooksForTree(int coordX, int coordY) {
  static const int auxJumps[8][2] = {{0, 1}, {-1, 0}, {1, 0}, {0, -1}, {-1, 1}, {1, 1}, {-1, -1}, {1, -1}}; //used to check positions
  int auxX = 0, auxY = 0, i = 0, retVal = 0;

  puzzle.board[coordY*puzzle.columnEdge+coordX] = 'K'; //prevents reading same position multiple times
  puzzle.numOfTents++; //increases number of found tents
  for (i = 4; i < 8; i++) { //checks if tents are on NW, SW, NE or SE positions
    auxX = coordX + auxJumps[i][0];
    auxY = coordY + auxJumps[i][1];
    if (auxX < 0 || auxX >= puzzle.columnEdge || auxY < 0 || auxY >= puzzle.lineEdge) {//if out of board jump over this cycle
      continue;
    }
    if ((puzzle.board[auxY*puzzle.columnEdge+auxX]) == 'T') { //if tent is found, it is in invalid place
      return -1;
    }
  }

  for (i = 0; i < 4; i++) { //cycles through N, E, S and W positions for tree
    auxX = coordX + auxJumps[i][0];
    auxY = coordY + auxJumps[i][1];
    if (auxX < 0 || auxX >= puzzle.columnEdge || auxY < 0 || auxY >= puzzle.lineEdge) { //if out of board jump over this cycle
      continue;
    }
    if ((puzzle.board[auxY*puzzle.columnEdge+auxX]) == 'T') { //if tent is found, it is in invalid place
      return -1;
    } else if ((puzzle.board[auxY*puzzle.columnEdge+auxX]) == 'A') { //else looks for tree
      retVal = treeLooksForTent(auxX, auxY);
      if (retVal == -1) {
        return -1;
      }
    }
  }
  return 0; //end of path without errors
}



/*  cycles through all valid positions looking for a tent to continue the path
*
*   -1 for error, 0 for good
*
*
*/
int treeLooksForTent(int coordX, int coordY) {
  static const int auxJumps[4][2] = {{0, 1}, {-1, 0}, {1, 0}, {0, -1}}; //used to check only 4 valid positions for a tent
  int auxX = 0, auxY = 0, i = 0, retVal = 0;

  puzzle.board[coordY*puzzle.columnEdge+coordX] = 'K'; //prevents reading same position multiple times
  puzzle.numOfTrees++; //increases number of found trees

  for (i = 0; i < 4; i++) { //cycles through N, E, S and W positions for tree
    auxX = coordX + auxJumps[i][0];
    auxY = coordY + auxJumps[i][1];
    if (auxX < 0 || auxX >= puzzle.columnEdge || auxY < 0 || auxY >= puzzle.lineEdge) { //if out of board jump over this cycle
      continue;
    }
    if ((puzzle.board[auxY*puzzle.columnEdge+auxX]) == 'T') { //looks for tent
      retVal = tentLooksForTree(auxX, auxY);
      if (retVal == -1) {
        return -1;
      }
    }
  }
  return 0; //end of path without errors
}
