#include "stdio.h"
#include "stdlib.h"


int tentLooksForTree(int coordX, int coordY, int lineEdge, int columnEdge, char **board, int *numOfTents, int *numOfTrees);
int treeLooksForTent(int coordX, int coordY, int lineEdge, int columnEdge, char **board, int *numOfTents, int *numOfTrees);



/*  cycles through all valid positions looking for a tree to continue the path or
*   an invalid tent
*
*   -1 for error, 0 for good
*
*
*/
int tentLooksForTree(int coordX, int coordY, int lineEdge, int columnEdge, char **board, int *numOfTents, int *numOfTrees) {
  int retVal = 0;

  board[coordX][coordY] = 'K'; //prevents reading same position multiple times
  (*numOfTents) += 1; //increases number of found tents

  for (int i = coordX-1; i <= coordX+1; i++) {//cycles through X coordinates
    if (i < 0 || i >= lineEdge) { //if out of board jump over this cycle
      continue;
    }
    for (int j = coordY+1; j <= coordY-1; j--) {//cycles through Y coordinates
      if (j < 0 || j >= columnEdge) {//if out of board jump over this cycle
        continue;
      }
      if (board[i][j] == 'T') { //if tent is found, it is in invalid place
        return -1;
      } else if (board[i][j] == 'A') { //else looks for tree
        retVal = treeLooksForTent(i, j, lineEdge, columnEdge, board, &(*numOfTents), &(*numOfTrees));
        if (retVal == -1) {
          return -1;
        }
      }
    }
  }
  return 0; //bad return?, just taking care of error
}



/*  cycles through all valid positions looking for a tent to continue the path
*
*   -1 for error, 0 for good
*
*
*/
int treeLooksForTent(int coordX, int coordY, int lineEdge, int columnEdge, char **board, int *numOfTents, int *numOfTrees) {
  const int auxJumps[4][2] = {{0, 1}, {-1, 0}, {1, 0}, {0, -1}}; //used to check only 4 valid positions
  int auxX = 0, auxY = 0, retVal = 0;

  board[coordX][coordY] = 'K'; //prevents reading same position multiple times
  (*numOfTrees) += 1; //increases number of found trees

  for (int i = 0; i < 4; i++) { //cycles through all 4 valid positions for tree
    auxX = coordX + auxJumps[i][0];
    auxY = coordY + auxJumps[i][1];
    if ((auxX < 0 || auxX >= columnEdge) && (auxY < 0 || auxY >= columnEdge)) {//if out of board jump over this cycle
      continue;
    }
    if (board[auxX][auxY] == 'T') { //looks for tent
      retVal = tentLooksForTree(auxX, auxY, lineEdge, columnEdge, board, &(*numOfTents), &(*numOfTrees));
      if (retVal == -1) {
        return -1;
      }
    }
  }
  return 0; //bad return?, just taking care of error
}
