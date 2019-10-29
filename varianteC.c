#include "stdio.h"
#include "stdlib.h"


int tentLooksForTree(int coordX, int coordY, int lineEdge, int columnEdge, char ***board, int *numOfTents, int *numOfTrees);
int treeLooksForTent(int coordX, int coordY, int lineEdge, int columnEdge, char ***board, int *numOfTents, int *numOfTrees);
int validateBoard(int lineEdge, int columnEdge, char ***board);
int initSearch(char ***board, int lineEdge, int columnEdge);





int analyzeBoard() {
  /*
  *
  *
  *
  *
  *
  */



}




/*  initializes searh parameters
*   decides to save board on external file or not
*
*   0 for good & no extra file
*   1 for good & file
*   -1 for not good & file
*   -2 for not good & no extra file
*
*/
int initSearch(char ***board, int lineEdge, int columnEdge) {
  int retVal = 0;
  FILE *fptr = NULL;

  if ((long long int)(lineEdge*columnEdge) > 43000000) {
    fptr = fopen("tempfile.txt", "w");
    for (int i = 0; i < columnEdge; i++) {
      for (int j = 0; j < lineEdge; j++) {
        fputc(*(board[i][j]), fptr);
      }
    }
    fclose(fptr);
    retVal = validateBoard(lineEdge, columnEdge, board);
  } else {
    char **boardcopy = (char **) malloc(lineEdge*sizeof(char *));
    for (int i = 0; i < lineEdge; i++) {
      boardcopy[i] = (char *) malloc(lineEdge*sizeof(char));
    }
    retVal = validateBoard(lineEdge, columnEdge, &boardcopy);
    for (int i = 0; i < lineEdge; i++) {
      free(boardcopy[i]);
    }
    free(boardcopy);
  }
  return retVal;
}



/*  cycles through all positions of the board
*   starts path finding operations
*
*   -1 for error, 0 for good
*
*
*/
int validateBoard(int lineEdge, int columnEdge, char ***board) {
  int numOfTents = 0, numOfTrees = 0;

  for (int i = 0; i < columnEdge; i++) {
    for (int j = 0; j < lineEdge; j++) {
      if (*(board[i][j]) == 'T') {
        tentLooksForTree(i, j, lineEdge, columnEdge, board, &numOfTents, &numOfTrees);
        if (numOfTrees != numOfTents) {
          return -1;
        }
      } else if (*(board[i][j]) == 'A') {
        treeLooksForTent(i, j, lineEdge, columnEdge, board, &numOfTents, &numOfTrees);
        if (numOfTrees != numOfTents) {
          return -1;
        }
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
int tentLooksForTree(int coordX, int coordY, int lineEdge, int columnEdge, char ***board, int *numOfTents, int *numOfTrees) {
  const int auxJumps[8][2] = {{0, 1}, {-1, 0}, {1, 0}, {0, -1}, {-1, 1}, {1, 1}, {-1, -1}, {1, -1}}; //used to check only 4 valid positions
  int auxX = 0, auxY = 0, retVal = 0;

  *(board[coordX][coordY]) = 'K'; //prevents reading same position multiple times
  (*numOfTents) += 1; //increases number of found tents

  for (int i = 4; i < 8; i++) {
    auxX = coordX + auxJumps[i][0];
    auxY = coordY + auxJumps[i][1];
    if ((auxX < 0 || auxX >= columnEdge) || (auxY < 0 || auxY >= columnEdge)) {//if out of board jump over this cycle
      continue;
    }
    if (*(board[auxX][auxY]) == 'T') { //if tent is found, it is in invalid place
      return -1;
    }
  }

  for (int i = 0; i < 4; i++) { //cycles through all 4 valid positions for tree
    auxX = coordX + auxJumps[i][0];
    auxY = coordY + auxJumps[i][1];
    if ((auxX < 0 || auxX >= columnEdge) || (auxY < 0 || auxY >= columnEdge)) {//if out of board jump over this cycle
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
  return 0; //bad return?, just taking care of error
}



/*  cycles through all valid positions looking for a tent to continue the path
*
*   -1 for error, 0 for good
*
*
*/
int treeLooksForTent(int coordX, int coordY, int lineEdge, int columnEdge, char ***board, int *numOfTents, int *numOfTrees) {
  const int auxJumps[4][2] = {{0, 1}, {-1, 0}, {1, 0}, {0, -1}}; //used to check only 4 valid positions
  int auxX = 0, auxY = 0, retVal = 0;

  *(board[coordX][coordY]) = 'K'; //prevents reading same position multiple times
  (*numOfTrees) += 1; //increases number of found trees

  for (int i = 0; i < 4; i++) { //cycles through all 4 valid positions for tree
    auxX = coordX + auxJumps[i][0];
    auxY = coordY + auxJumps[i][1];
    if ((auxX < 0 || auxX >= columnEdge) && (auxY < 0 || auxY >= columnEdge)) {//if out of board jump over this cycle
      continue;
    }
    if (*(board[auxX][auxY]) == 'T') { //looks for tent
      retVal = tentLooksForTree(auxX, auxY, lineEdge, columnEdge, board, numOfTents, numOfTrees);
      if (retVal == -1) {
        return -1;
      }
    }
  }
  return 0; //bad return?, just taking care of error
}
