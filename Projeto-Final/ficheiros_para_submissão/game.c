/*******************************************************************************
 * 2019-2020 AED - grupo 65
 * Last modified: 2019-12-09
 *
 * NAME
 *      game.c
 *
 * DESCRIPTION
 *      Implementation of the functions used to manage the current layout being solved
 *
 * COMMENTS
 *      none
 *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "files.h"

typedef struct {
    int rows, columns;
    char *buffer;
    char *layout;
    int answer;
    int sum;
    int season;
} board;

board jogo;


/*******************************************************************************
* Function name: initBoard()                                                   *
*                                                                              *
* Arguments: none                                                              *
*                                                                              *
* Return: none                                                                 *
*                                                                              *
* Side-effects: none                                                           *
*                                                                              *
* Description: Sets initial values to 'jogo'                                   *
*                                                                              *
*******************************************************************************/
void initBoard(void) {
    jogo.rows = 0;
    jogo.columns = 0;
    jogo.answer = 0;
    jogo.sum = 0;
    jogo.season = 0;
}


/*******************************************************************************
* Function name: setBoardBio()                                                 *
*                                                                              *
* Arguments: rows - number os rows in the layout                               *
*            columns - number of columns in the layout                         *
*            sum - total of tents to be placed                                 *
*                                                                              *
* Return: none                                                                 *
*                                                                              *
* Side-effects: none                                                           *
*                                                                              *
* Description: Sets rows, columns and sum members of struct 'jogo'             *
*                                                                              *
*******************************************************************************/
void setBoardBio(int rows, int columns, int sum) {
    jogo.rows = rows;
    jogo.columns = columns;
    jogo.sum = sum;
}

/*******************************************************************************
* Function name: setBoardArrays()                                              *
*                                                                              *
* Arguments: *layout - game layout represented in a uni-dimensional array      *
*            *buffer - array that enables reading layout row by row            *
*                                                                              *
* Return: none                                                                 *
*                                                                              *
* Side-effects: none                                                           *
*                                                                              *
* Description: Sets layout and buffer members of struct 'jogo'                 *
*                                                                              *
*******************************************************************************/
void setBoardArrays(char *layout, char *buffer) {
    jogo.layout = layout;
    jogo.buffer = buffer;
}

/*******************************************************************************
* Function name: setBoardSeason()                                              *
*                                                                              *
* Arguments: season - determines tree-tents rate (                             *
*                         1 - high season (trees == tents)                     *
*                         2 - low season (trees > tents)                       *
*                     )                                                        *
* Return: none                                                                 *
*                                                                              *
* Side-effects: none                                                           *
*                                                                              *
* Description: Sets season member of struct 'jogo'                             *
*                                                                              *
*******************************************************************************/
void setBoardSeason(int season) {
    jogo.season = season;
}

/*******************************************************************************
* Function name: setBoardSeason()                                              *
*                                                                              *
* Arguments: answer - determines game resolubility (                           *
*                         -1 - has no answer / impossible                      *
*                          0 - has a possible answer                           *
*                     )                                                        *
* Return: none                                                                 *
*                                                                              *
* Side-effects: none                                                           *
*                                                                              *
* Description: Sets season member of struct 'jogo'                             *
*                                                                              *
*******************************************************************************/
void setBoardAnswer(int answer) {
    jogo.answer = answer;
}


/*******************************************************************************
* Function name: *getBoardBuffer()                                             *
*                                                                              *
* Arguments: none                                                              *
*                                                                              *
* Return: jogo.buffer (char *)                                                 *
*                                                                              *
* Side-effects: none                                                           *
*                                                                              *
* Description: Gets the buffer member of struct 'jogo'                         *
*                                                                              *
*******************************************************************************/
char *getBoardBuffer(void) {
    return jogo.buffer;
}

/*******************************************************************************
* Function name: getBoardSum()                                                 *
*                                                                              *
* Arguments: none                                                              *
*                                                                              *
* Return: jogo.sum (int) - number of tents to be placed in the layout          *
*                                                                              *
* Side-effects: none                                                           *
*                                                                              *
* Description: Gets the sum member of struct 'jogo'                            *
*                                                                              *
*******************************************************************************/
int getBoardSum(void) {
    return jogo.sum;
}

/*******************************************************************************
* Function name: getBoardRows()                                                *
*                                                                              *
* Arguments: none                                                              *
*                                                                              *
* Return: jogo.rows (int) - number of rows in the layout                       *
*                                                                              *
* Side-effects: none                                                           *
*                                                                              *
* Description: Gets the rows member of struct 'jogo'                           *
*                                                                              *
*******************************************************************************/
int getBoardRows(void) {
    return jogo.rows;
}

/*******************************************************************************
* Function name: getBoardColumnss()                                            *
*                                                                              *
* Arguments: none                                                              *
*                                                                              *
* Return: jogo.columns (int) - number of columns in the layout                 *
*                                                                              *
* Side-effects: none                                                           *
*                                                                              *
* Description: Gets the colums member of struct 'jogo'                         *
*                                                                              *
*******************************************************************************/
int getBoardColumns(void) {
    return jogo.columns;
}

/*******************************************************************************
* Function name: getBoardLayoutElement()                                       *
*                                                                              *
* Arguments: none                                                              *
*                                                                              *
* Return: jogo.layout[index] (char) - element in position 'index'              *
*                                                                              *
* Side-effects: none                                                           *
*                                                                              *
* Description: Gets an element of the layout member of struct 'jogo'           *
*                                                                              *
*******************************************************************************/
char getBoardLayoutElement(int i, int j) {
    return jogo.layout[i*jogo.columns + j];
}

/*******************************************************************************
* Function name: *getBoardLayout()                                             *
*                                                                              *
* Arguments: none                                                              *
*                                                                              *
* Return: jogo.layout (char *) - game layout as an uni-dimensional array       *
*                                                                              *
* Side-effects: none                                                           *
*                                                                              *
* Description: Gets the layout member of struct 'jogo'                         *
*                                                                              *
*******************************************************************************/
char *getBoardLayout(void) {
    return jogo.layout;
}

/*******************************************************************************
* Function name: getBoardAnswer()                                              *
*                                                                              *
* Arguments: none                                                              *
*                                                                              *
* Return: jogo.answer (int) - current game answer                              *
*                                                                              *
* Side-effects: none                                                           *
*                                                                              *
* Description: Gets the answer member of struct 'jogo'                         *
*                                                                              *
*******************************************************************************/
int getBoardAnswer(void) {
    return jogo.answer;
}


/*******************************************************************************
* Function name: freeBoard()                                                   *
*                                                                              *
* Arguments: none                                                              *
*                                                                              *
* Return: none                                                                 *
*                                                                              *
* Side-effects: none                                                           *
*                                                                              *
* Description: Frees memory allocated for the layout and buffer members        *
*                                                                              *
*******************************************************************************/
void freeBoard(void) {
    free(jogo.layout);
    free(jogo.buffer);
}
