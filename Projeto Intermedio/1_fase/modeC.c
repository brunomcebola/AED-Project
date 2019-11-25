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

  puzzle.lineEdge = getBoardRows();
  puzzle.columnEdge = getBoardColumns();
  puzzle.board = getBoardLayout();

  setBoardAnswer(validateBoard());
}



/*  cycles through all positions of the board
*   starts path finding operations
*
*   1 for error, 0 for good -- it is different from others
*
*
*/
int validateBoard(void) {
  static int i = 0, j = 0;
  static long int position = 0;
  puzzle.numOfTents = 0, puzzle.numOfTrees = 0;

  i = puzzle.lineEdge - 1;

  position = (puzzle.lineEdge * puzzle.columnEdge) - 1;
  do {
    j = puzzle.columnEdge - 1;

    do {
      if ((puzzle.board[position]) != '.') {
        if ((puzzle.board[position]) == 'T') {
          if (((tentLooksForTree(j, i)) || puzzle.numOfTrees < puzzle.numOfTents)) {
            return 1;
          }
          puzzle.numOfTents = 0, puzzle.numOfTrees = 0; //reinicializes count
        } else {
          if ((treeLooksForTent(j, i) || (puzzle.numOfTrees < puzzle.numOfTents) )) {
            return 1;
          }
          puzzle.numOfTents = 0, puzzle.numOfTrees = 0; //reinicializes count
        }
      }
      --position;
    } while(j--);
  } while(i--);

  return 0;
}


/*  cycles through all valid positions looking for a tree to continue the path or
*   an invalid tent
*
*   1 for error, 0 for good
*
*
*/
int tentLooksForTree(int coordX, int coordY) {
  static const int auxJumps1[5][2] = {{0, 0}, {-1, 1}, {1, 1}, {-1, -1}, {1, -1}}; //used to diagonal check positions
  static const int auxJumps2[5][2] = {{0, 0}, {0, 1}, {-1, 0}, {1, 0}, {0, -1}}; //used to check normal positions
  int auxX = 0, auxY = 0, i = 4;

  puzzle.board[coordY*puzzle.columnEdge+coordX] = '.'; //prevents reading same position multiple times
  puzzle.numOfTents++; //increases number of found tents

  do {
    auxX = coordX + auxJumps1[i][0];
    auxY = coordY + auxJumps1[i][1];
    if (auxX < 0 || auxX >= puzzle.columnEdge || auxY < 0 || auxY >= puzzle.lineEdge) {//if out of board jump over this cycle
      continue;
    }
    if ((puzzle.board[auxY*puzzle.columnEdge+auxX]) == 'T') { //if tent is found, it is in invalid place
      return 1;
    }
  } while(--i);

  i = 4;

  do {
    auxX = coordX + auxJumps2[i][0];
    auxY = coordY + auxJumps2[i][1];
    if (auxX < 0 || auxX >= puzzle.columnEdge || auxY < 0 || auxY >= puzzle.lineEdge) { //if out of board jump over this cycle
      continue;
    }
    if ((puzzle.board[auxY*puzzle.columnEdge+auxX]) == 'T') { //if tent is found, it is in invalid place
      return 1;
    } else if ((puzzle.board[auxY*puzzle.columnEdge+auxX]) == 'A') { //else looks for tree
      if (treeLooksForTent(auxX, auxY)) {
        return 1;
      }
    }
  } while(--i);

  return 0; //end of path without errors
}


/*  cycles through all valid positions looking for a tent to continue the path
*
*   1 for error, 0 for good
*
*
*/
int treeLooksForTent(int coordX, int coordY) {
  static const int auxJumps[5][2] = {{0, 0}, {0, 1}, {-1, 0}, {1, 0}, {0, -1}}; //used to check only 4 valid positions for a tent
  int auxX, auxY, i = 4;

  puzzle.board[coordY*puzzle.columnEdge+coordX] = '.'; //prevents reading same position multiple times
  puzzle.numOfTrees++; //increases number of found trees

  do {
    auxX = coordX + auxJumps[i][0];
    auxY = coordY + auxJumps[i][1];
    if (auxX < 0 || auxX >= puzzle.columnEdge || auxY < 0 || auxY >= puzzle.lineEdge) { //if out of board jump over this cycle
      continue;
    }
    if ((puzzle.board[auxY*puzzle.columnEdge+auxX]) == 'T') { //looks for tent
      if (tentLooksForTree(auxX, auxY)) {
        return 1;
      }
    }
  } while(--i);

  return 0; //end of path without errors
}
