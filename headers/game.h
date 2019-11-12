#ifndef game_included
#define game_included

void initBoard(void);

void setBoardRows(int);
void setBoardColumns(int);
void setBoardMode(char);
void setBoardCoordinates(int, int);
void setBoardElRows(int *);
void setBoardElColumns(int *);
void setBoardLayout(char **);
void setBoardAnswer(int);

int getBoardRows(void);
int getBoardColumns(void);
char getBoardMode(void);
int getBoardElRow(int);
int getBoardElColumn(int);
char getBoardLayoutElement(int, int);
char **getBoardAllLayout(void);
int getBoardAnswer(void);
int getBoardCoordinateX(void);
int getBoardCoordinateY(void);

void selMode(void);

void freeBoard(void);

#endif
