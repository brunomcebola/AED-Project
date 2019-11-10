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
void setBoardAnswer(int);

int getBoardRows();
int getBoardColumns();
char getBoardMode();
int getBoardElRow(int);
int getBoardElColumn(int);
char getBoardLayoutElement(int, int);
char ***getBoardAllLayout();
int getBoardAnswer();
int getBoardCoordinateX();
int getBoardCoordinateY();

void selMode();

void freeBoard();

#endif
