#ifndef game_included
#define game_included

void initBoard(void);

void setBoardBio(int, int, int);
void setBoardArrays(char *, char *);
void setBoardSeason(int);
void setBoardAnswer(int);

int getBoardSum(void);
int getBoardRows(void);
int getBoardColumns(void);
int getBoardAnswer(void);
char getBoardLayoutElement(int, int);
char *getBoardBuffer(void);
char *getBoardLayout(void);


void freeBoard(void);

#endif
