#include "stdio.h"
#include "stdlib.h"
#include "../headers/modeC.h"
#include "../headers/game.h"



int tentLooksForTree(int coordX, int coordY, int lineEdge, int columnEdge, char ***board, int *numOfTents, int *numOfTrees);
int treeLooksForTent(int coordX, int coordY, int lineEdge, int columnEdge, char ***board, int *numOfTents, int *numOfTrees);
int validateBoard(int lineEdge, int columnEdge, char ***board);
int initialChecks(int lineEdge, int columnEdge, char **board);



/*
*   kickstarts modeC checking functions and sets answer for the problem
*
*    no return value
*
*/
void modeC(int lineEdge, int columnEdge, char ***board, FILE **fptr) {
  int retVal = 0;
  retVal = initialChecks(lineEdge, columnEdge, *board);
  if (retVal == -1) {
    setBoardAnswer(0);
    return;
  }
  retVal = validateBoard(lineEdge, columnEdge, board);
  setBoardAnswer(retVal);
}



/*
*   checks if more tents are in a given row or column than allowed
*
*    -1 for error, 0 for good
*
*/

int initialChecks(int lineEdge, int columnEdge, char **board) {
  int *totalOfTentsLines = (int *) malloc(lineEdge*sizeof(int));
  int *totalOfTentsColumns = (int *) malloc(columnEdge*sizeof(int));

  for (int i = 0; i < lineEdge; i++) {
    for (int j = 0; j < columnEdge; j++) {
      if (board[i][j] == 'T') {
        totalOfTentsLines[i]++;
        totalOfTentsColumns[j]++;
      }
    }
    if (getBoardElRow(i) != totalOfTentsLines[i]) {
      free(totalOfTentsColumns);
      free(totalOfTentsLines);
      return -1;
    }
  }

  for (int i = 0; i < columnEdge; i++) {
    if (getBoardElColumn(i) != totalOfTentsColumns[i]) {
      free(totalOfTentsColumns);
      free(totalOfTentsLines);
      return -1;
    }
  }


  free(totalOfTentsColumns);
  free(totalOfTentsLines);
  return 0;
}




/*  cycles through all positions of the board
*   starts path finding operations
*
*   0 for error, 1 for good -- it is different from others
*
*
*/
int validateBoard(int lineEdge, int columnEdge, char ***board) {
  int numOfTents = 0, numOfTrees = 0, retVal = 0;


  for (int i = 0; i < columnEdge; i++) {
    for (int j = 0; j < lineEdge; j++) {

      if (*(board[i][j]) == 'T') {
        retVal = tentLooksForTree(i, j, lineEdge, columnEdge, board, &numOfTents, &numOfTrees);
        if ((numOfTrees < numOfTents) || (retVal == -1)) {
          return 0;
        }
        numOfTents = 0, numOfTrees = 0; //reinicializes count
      } else if (*(board[i][j]) == 'A') {
        retVal = treeLooksForTent(i, j, lineEdge, columnEdge, board, &numOfTents, &numOfTrees);
        if ((numOfTrees < numOfTents) || (retVal == -1)) {
          return 0;
        }
        numOfTents = 0, numOfTrees = 0; //reinicializes count
      }
    }
  }
  return 1;
}


/*  cycles through all valid positions looking for a tree to continue the path or
*   an invalid tent
*
*   -1 for error, 0 for good
*
*
*/
int tentLooksForTree(int coordX, int coordY, int lineEdge, int columnEdge, char ***board, int *numOfTents, int *numOfTrees) {
  const int auxJumps[8][2] = {{0, 1}, {-1, 0}, {1, 0}, {0, -1}, {-1, 1}, {1, 1}, {-1, -1}, {1, -1}}; //used to check positions
  int auxX = 0, auxY = 0, retVal = 0;

  *(board[coordX][coordY]) = 'K'; //prevents reading same position multiple times
  (*numOfTents) += 1; //increases number of found tents

  for (int i = 4; i < 8; i++) { //checks if tents are on NW, SW, NE or SE positions
    auxX = coordX + auxJumps[i][0];
    auxY = coordY + auxJumps[i][1];
    if (auxX < 0 || auxX >= columnEdge || auxY < 0 || auxY >= columnEdge) {//if out of board jump over this cycle
      continue;
    }
    if (*(board[auxX][auxY]) == 'T') { //if tent is found, it is in invalid place
      return -1;
    }
  }

  for (int i = 0; i < 4; i++) { //cycles through N, E, S and W positions for tree
    auxX = coordX + auxJumps[i][0];
    auxY = coordY + auxJumps[i][1];
    if ((auxX < 0 || auxX >= columnEdge) || (auxY < 0 || auxY >= columnEdge)) { //if out of board jump over this cycle
      continue;
    }
    if (*(board[auxX][auxY]) == 'T') { //if tent is found, it is in invalid place
      return -1;
    } else if (*(board[auxX][auxY]) == 'A') { //else looks for tree
      retVal = treeLooksForTent(auxX, auxY, lineEdge, columnEdge, board, numOfTents, numOfTrees);
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
int treeLooksForTent(int coordX, int coordY, int lineEdge, int columnEdge, char ***board, int *numOfTents, int *numOfTrees) {
  const int auxJumps[4][2] = {{0, 1}, {-1, 0}, {1, 0}, {0, -1}}; //used to check only 4 valid positions for a tent
  int auxX = 0, auxY = 0, retVal = 0;

  *(board[coordX][coordY]) = 'K'; //prevents reading same position multiple times
  (*numOfTrees) += 1; //increases number of found trees

  for (int i = 0; i < 4; i++) { //cycles through N, E, S and W positions for tree
    auxX = coordX + auxJumps[i][0];
    auxY = coordY + auxJumps[i][1];
    if (auxX < 0 || auxX >= columnEdge || auxY < 0 || auxY >= columnEdge) { //if out of board jump over this cycle
      continue;
    }
    if (*(board[auxX][auxY]) == 'T') { //looks for tent
      retVal = tentLooksForTree(auxX, auxY, lineEdge, columnEdge, board, numOfTents, numOfTrees);
      if (retVal == -1) {
        return -1;
      }
    }
  }
  return 0; //end of path without errors
}
