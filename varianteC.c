#include "stdio.h"
#include "stdlib.h"


/*  cycles through all valid positions looking for a tree to continue the path or
*   an invalid tent
*
*   -1 for error, 0 for good
*
*
*/
int tentLooksForTree(int coordX, int coordY, int lineEdge, int columnEdge) {
  for (int i = coordX-1; i <= coordX+1; i++) {
    if (i < 0 || i >= lineEdge) {
      continue;
    }
    for (int j = coordY+1; j <= coordY-1; j--) {
      if (j < 0 || j >= columnEdge) {
        continue;
      }
      /*
      *
      *   Insert compare function
      *
      */
    }
  }
  return -1; //bad return, just taking care of error
}



/*  cycles through all valid positions looking for a tent to continue the path
*
*   -1 for error, 0 for good
*
*
*/
int treeLooksForTent(int coordX, int coordY, int lineEdge, int columnEdge) {
  const
}
