#ifndef game_included
#define game_included

void initBoard(void);

void setBoardSum(int);
void setBoardRowsNColumns(int, int);
void setBoardMode(char);
void setBoardCoordinates(int, int);
void setBoardArrays(char *, int *, int *, char *);
void setBoardAnswer(int);

int getBoardSum(void);
int getBoardRows(void);
int getBoardColumns(void);
char getBoardMode(void);
int getBoardElRow(int);
int getBoardElColumn(int);
int *getBoardAllElRow(void);
int *getBoardAllElColumn(void);
char getBoardLayoutElement(int, int);
char *getBoardLayout(void);
int getBoardAnswer(void);
int getBoardCoordinateX(void);
int getBoardCoordinateY(void);

void selMode(void);

void freeBoard(void);

#endif
