#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <ctype.h>

#include "../headers/game.h"
#include "../headers/files.h"
#include "../headers/bundle.h"
#include "../headers/solver.h"

#ifndef _MAX_
#define _MAX_
#define MAX(a,b) (a > b ? a : b)
#endif


typedef struct _Board {
    int valid, sum;
    struct _Board *next;
} Board;

Board *board = NULL;
Board *board_ptr = NULL;

FILE *in_file = NULL;
FILE *out_file = NULL;

/*******************************************************************************
* Function name: initFile()                                                    *
*                                                                              *
* Arguments: const char *file - input file passed through the command line     *
*                                                                              *
* Return: none                                                                 *
*                                                                              *
* Side-effects: Forces the program to terminate (with exit(0)) if there is an  *
*               error with the memory managment (opening/creating files or     *
*               allocating memory)                                             *
*                                                                              *
* Description: Checks the input file's extension (.camp) and creates an output *
*              file with the same name and extension .tents                    *
*                                                                              *
*******************************************************************************/
void initFile(const char *file) {
    char *file_name;
    int size_in = strlen(file), size_out = strlen(basename((char *)file));

    //checks the extension
    if(strcmp(".camp", file+size_in-5) != 0) {
        exit(0);
    }
    //opens input file in reading mode
    in_file = fopen(file, "r");

    //gets file name
    file_name = (char *) calloc((size_out+2), sizeof(char));
    checkNull(1, file_name);
    strncpy(file_name, basename((char *)file), size_out-5);

    //creates output file in writing mode
    out_file = fopen(strcat(file_name,".check"), "w"); /* TODO: change extension after debugging */

    //checks if both files (input and output) are opened correctly
    checkNull(2, in_file, out_file);

    free(file_name);
}

/*******************************************************************************
* Function name: maxSize()                                                     *
*                                                                              *
* Arguments: none                                                              *
*                                                                              *
* Return: none                                                                 *
*                                                                              *
* Side-effects: Forces the program to terminate (with exit(0)) if there is an  *
*               error in any of the allocations                                *
*               Generates a linked list with board properties (valid and sum)  *
*                                                                              *
* Description: Reads entire input file to find the max row and max column and  *
*              allocates memory (for the game layout and the arrays used to    *
*              solve the layout - one for the rows and other for the colmuns). *
*              Performs initial checks in order to determine the layout        *
*              admissibility                                                   *
*                                                                              *
*                                                                              *
*******************************************************************************/
void maxSize() {
    int max = 0, rows = 0, columns = 0, mux = 0, tents = 0,
    aux = 0, max_row = 0, max_column = 0, i = 0, sum_tents_row = 0,
    valid = 1, sum_tents_column = 0;

    char *tabuleiro = NULL, *buffer = NULL;
    Board *file_aux = NULL;
    HeadNode *row_vector = NULL, *column_vector = NULL;

    //loop through the entire file
    while(!feof(in_file)) {
        valid = 1;
        sum_tents_row = 0;
        sum_tents_column = 0;

        //gets number of rows and nuber of columns
        aux = fscanf(in_file, " %d %d", &rows , &columns);
        //error in this scanf => error in the following => stops the loop
        if(aux != 2){
            break;
        }

        //gets sum of tents relatively to the rows
        for(i = 0; i < rows; i++) {
            aux = fscanf(in_file, " %d", &tents);
            sum_tents_row += tents;
        }

        //gets sum of tents relatively to the columns
        for(i = 0; i < columns; i++) {
            aux = fscanf(in_file, " %d", &tents);
            sum_tents_column += tents;
        }

        //reads through the layout without saving it
        for(i = 0; i < rows; i++){
            aux = fscanf(in_file, " %*s");
        }

        //tents relatively to the rows must be equal to the tents relatively
        //to the columns. Otherwise the layout is not admissible
        if(sum_tents_row != sum_tents_column) {
            valid = 0;
        }
        else {
            //gets the maximum layout size, the maximum row size and the
            //maximum column size in the input file
            mux = rows * columns;

            max = MAX(max, mux);
            max_row = MAX(max_row, rows);
            max_column = MAX(max_column, columns);
        }

        //saves info relative to each layout in a linked list
        file_aux = malloc(sizeof(Board));
        checkNull(1, file_aux);
        file_aux -> valid = valid;
        file_aux -> sum = sum_tents_row;
        file_aux -> next = NULL;

        if(board == NULL) {
            board = file_aux;
        }
        else {
            board_ptr->next = file_aux;
        }
        board_ptr = file_aux;

    }

    //if any of the layouts present in the input file is admissible, then
    //is allocated memory to save the layout and the  arrays used to solve it
    //(row_vector and row_column)
    if(max != 0){
        tabuleiro = (char *) malloc((max+1) * sizeof(char));

        buffer = (char *) malloc((max_column+1) * sizeof(char));

        row_vector = (HeadNode *) malloc(max_row * sizeof(HeadNode));

        column_vector = (HeadNode *) malloc(max_column * sizeof(HeadNode));

        checkNull(4, tabuleiro, buffer, row_vector, column_vector);

        //saves the data in a struct inside game.c
        setBoardArrays(tabuleiro, buffer);
        //saves the data in two arrays declared inside solver.c
        setSolverVectors(row_vector, column_vector);
    }

    //resets the board_ptr to first layout analyzed
    board_ptr = board;

}

/*******************************************************************************
* Function name: begining()                                                    *
*                                                                              *
* Arguments: none                                                              *
*                                                                              *
* Return: none                                                                 *
*                                                                              *
* Side-effects: none                                                           *
*                                                                              *
* Description: Rewinds the input file to begining                              *
*                                                                              *
*******************************************************************************/
void begining(){
    fseek(in_file, 0, SEEK_SET) ;
}

/*******************************************************************************
* Function name: reachEOF()                                                    *
*                                                                              *
* Arguments: none                                                              *
*                                                                              *
* Return: 1 - reached end of file                                              *
*         0 - file not over                                                    *
*                                                                              *
* Side-effects: none                                                           *
*                                                                              *
* Description: Checks if input file has reached its end                        *
*                                                                              *
*******************************************************************************/
int reachedEOF(void) {
    return feof(in_file);
}

/*******************************************************************************
* Function name: finishLayout()                                                *
*                                                                              *
* Arguments: flag - determines wich part of the board to skip (                *
*                       0 - layout;                                            *
*                       1 - layout Description                                 *
*                   )                                                          *
*                                                                              *
* Return: none                                                                 *
*                                                                              *
* Side-effects: If flag equals 1 sets board rows, columns and answer (-1)      *
*                                                                              *
* Description: Skips unnecessary information during input file reading         *
*                                                                              *
*******************************************************************************/
void finishLayout(int flag) {
    int rows = -1, columns, i;

    //skips layout description
    if(flag) {
        //gets board rows an columns
        fscanf(in_file, " %d %d", &rows, &columns);

        for(i = 0; i < rows; i++) {
            fscanf(in_file, " %*d");
        }

        for(i = 0; i < columns; i++) {
            fscanf(in_file, " %*d");
        }
        //sets essential information to print an answer to the output file
        setBoardBio(rows, columns, 0);
        setBoardAnswer(-1);
    }
    //skips layout
    else {
        rows = getBoardRows();
        for(i = 0; i < rows; i++){
            fscanf(in_file, " %*s");
        }
    }

}

/*******************************************************************************
* Function name: readBio()                                                     *
*                                                                              *
* Arguments: none                                                              *
*                                                                              *
* Return: 1 - everything was read correctly                                    *
*         0 - something went wrong during file reading                         *
*                                                                              *
* Side-effects: If layout is marked as inadmissible then it is skipped         *
*                                                                              *
* Description: Reads and saves layout description                              *
*              Saves tents per row and tents per column in the array of        *
*              structures declared in solver.c                                 *
*                                                                              *
*******************************************************************************/

int readBio(void) {
    int rows = 0, columns = 0, tents = 0, trash = 0, i = 0;
    HeadNode *row_vector = getSolverVectorRow(),
             *column_vector = getSolverVectorColumn();

    if(board_ptr == NULL) {
        fscanf(in_file, " ");
        return 0;
    }

    //if layout is valid reads its description
    if(board_ptr -> valid){

        //gets layout rows and columns
        trash = fscanf(in_file, " %d %d", &rows , &columns);
        //error in this scanf => error in the following => return 0
        if(trash != 2) {
            return 0;
        }

        //gets number of tents per row and initializes the structs in row array
        for(i = 0; i < rows; i++) {
            trash = fscanf(in_file, " %d", &tents);
            if (tents > (columns/2) || tents < 0) {
                setBoardAnswer(-1);
            }
            row_vector[i].puzzleTents = tents;
            row_vector[i].tentsNeeded = tents;
            row_vector[i].availablePositions = 0;
            row_vector[i].first = NULL;
        }

        //gets number of tents per column and initializes the structs in
        //columns array
        for(i = 0; i < columns; i++) {
            trash = fscanf(in_file, " %d", &tents);
            if (tents > (rows/2) || tents < 0) {
                setBoardAnswer(-1);
            }
            column_vector[i].puzzleTents = tents;
            column_vector[i].tentsNeeded = tents;
            column_vector[i].availablePositions = 0;
            column_vector[i].first = NULL;
        }

        //set data to board
        setBoardBio(rows, columns, board_ptr -> sum);
    }
    //if the layout is inadmissible it skips the information
    else {
        finishLayout(1);
    }

    //sets the board_ptr to the next game
    board_ptr = board_ptr -> next;

    return 1;
}

/*******************************************************************************
* Function name: readLyout()                                                   *
*                                                                              *
* Arguments: none                                                              *
*                                                                              *
* Return: 1 - everything was read correctly                                    *
*         0 - something went wrong during file reading or layout               *
*             is unadmissable                                                  *
*                                                                              *
* Side-effects: Determines and saves which season is being evaluated           *
*                                                                              *
* Description: Reads game layout                                               *
*                                                                              *
*******************************************************************************/
int readLayout(void) {
    char *tabuleiro = getBoardLayout(), *buffer = getBoardBuffer();
    int i = 0, rows = getBoardRows(), columns = getBoardColumns(), index = 0;

    tabuleiro[0] = '\0';
    buffer[0]= '\0';

    //reads layoutrow by row
    for (i = 0; i < rows; i++, index += columns) {
        fscanf(in_file, " %s", buffer);

        //fills the array containing the layout
        strcpy(&tabuleiro[index], buffer);
    }

    return 1;
}

/*******************************************************************************
* Function name: readFile()                                                    *
*                                                                              *
* Arguments: none                                                              *
*                                                                              *
* Return: 1 - everything was read correctly                                    *
*         0 - something went wrong during file reading                         *
*                                                                              *
* Side-effects: none                                                           *
*                                                                              *
* Description: Main function to choose what needs to be done to current game   *
*                                                                              *
*******************************************************************************/
int readFile(void) {
        if(!readBio()){
        return 0;
    }
    if(getBoardAnswer() == -1){
        finishLayout(0);
    }
    else {
        if(!readLayout()){
            return 0;
        }
    }
    return 1;
}

/*******************************************************************************
* Function name: writeFile()                                                   *
*                                                                              *
* Arguments: none                                                              *
*                                                                              *
* Return: 1 - everything was read correctly                                    *
*         0 - something went wrong during file reading                         *
*                                                                              *
* Side-effects: Forces the program to terminate (with exit(0)) if there is an  *
*               error while writting to the output file                        *
*                                                                              *
* Description: Writes board answer to the output file                          *
*                                                                              *
*******************************************************************************/
void writeFile (void) {
    int answer = getBoardAnswer(), rows = getBoardRows(),
        columns = getBoardColumns(), i = 0;
    char *layout = getBoardLayout();

    //prints layout description and answer
    if(fprintf(out_file, "%d %d %d\n", rows, columns, answer) < 0) {
        exit(0);
    }

    //if the is a correct answer then the solved layout is printed
//    if(answer == 1){
//        TODO: uncomment this
        for(i = 0; i < rows; i++){
            fprintf(out_file, "%.*s\n", columns, layout+(i*columns));
        }
//    }
    fprintf(out_file, "\n");
}

/*******************************************************************************
* Function name: terminateFile()                                               *
*                                                                              *
* Arguments: none                                                              *
*                                                                              *
* Return: none                                                                 *
*                                                                              *
* Side-effects: Forces the program to terminate (with exit(0)) if there is an  *
*               error while closing the input/output files                     *
*                                                                              *
* Description: Frees allocated memory and closes input and output files        *
*                                                                              *
*******************************************************************************/
void terminateFile(void) {
    //closes input and output file
    if(fclose(in_file) != 0) {
        exit(0);
    }

    if(fclose(out_file) != 0) {
        exit(0);
    }

    //frees game linked list
    while(board != NULL){
        board_ptr = board;
        board = board->next;
        free(board_ptr);
    }

    //frees memory allocated relatively to game.c
    freeBoard();
    //frees memory allocated relatively to solver.c
    freeSolver();
}
