#ifndef game_included
#define game_included

void initBoard();
void setBoardRows(int);
void setBoardColumns(int);
void setBoardMode(char);
void setBoardCoordinates(int, int);
void setBoardElRows(int *);
void setBoardElColumns(int *);
void setBoardLayout(char **);
void printLayout();
void freeBoard();

#endif
