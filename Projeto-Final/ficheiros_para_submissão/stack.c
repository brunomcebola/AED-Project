/*******************************************************************************
 * 2019-2020 AED - grupo 65
 * Last modified: 2019-12-09
 *
 * NAME
 *      stack.c
 *
 * DESCRIPTION
 *      Implementation of the explicit stack used to save the changes
 *      throughout the code execution
 *
 * COMMENTS
 *      none
 *
 ******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "solver.h"
#include "stack.h"


/*
changeNodeSelector:
    1 - Board update
    2 - TreeNode
    3 - HeadNode

valueID:
    TreeNode:
        1 - hasTentAssigned
        2 - num_playables
    HeadNode:
        1 - tentsNeeded
        2 - availablePositions

*/

/*******************************************************************************
* Function name: pushChange()
*
* Arguments: **changeStorePtr - stack head pointer
*            *ptr - changed struct (with new value) pointer
*            x - line where the change was made
*            y - column where the change was made
*            changeNodeSelector - indicates the type of struct that has been changed
*            previousValue - previous integer value to the change
*            previousChar - previous char value to the change
*
* Return: none
*
* Side-effects: none
*
* Description: adds a new modification to the stack, moves the pointer to the stack
*              pointing to new head
*
*******************************************************************************/
void pushChange(changeStore **changeStorePtr, void *ptr, int x, int y, int changeNodeSelector, int previousValue, char previousChar, int valueID) {
    changeStore *new = (changeStore *) malloc(sizeof(changeStore));
    new -> prevChange = *changeStorePtr;
    new -> changeNodeSelector = changeNodeSelector;
    switch(changeNodeSelector) {
        case 1:
            new -> changedNode.coord.x = x;
            new -> changedNode.coord.y = y;
            new -> previousChar = previousChar;
            break;
        case 2:
            new -> changedNode.T = (TreeNode*) ptr;
            new -> previousValue = previousValue;
            break;
        case 3:
            new -> changedNode.H = (HeadNode*) ptr;
            new -> previousValue = previousValue;
            break;
    }
    new -> valueID = valueID;
    *changeStorePtr = new;
}

/*******************************************************************************
* Function name: freeChangeList()
*
* Arguments: **changeStorePtr - stack head pointer
*
* Return: none
*
* Side-effects: none
*
* Description: Frees the memory allocated during the stack creation and management
*
*******************************************************************************/
void freeChangeList(changeStore **changeStorePtr) {
    changeStore *current;
    while(*changeStorePtr != NULL) {
        current = *changeStorePtr;
        *changeStorePtr = (*changeStorePtr) -> prevChange;
        free(current);
    }
    *changeStorePtr = NULL;
}

/*******************************************************************************
* Function name: deleteChanges()
*
* Arguments: **changeStorePtr - stack head pointer
*            *tabuleiro - game layout vector
*            colunas - number of columns in the layout
*
* Return: none
*
* Side-effects: none
*
* Description: Revertes the modifications made during the program execution
*
*******************************************************************************/
void deleteChanges(changeStore **changeStorePtr, char *tabuleiro, int colunas) {
    TreeNode *treeAux = NULL;
    HeadNode *headAux = NULL;
    changeStore *current;

    while(*changeStorePtr != NULL) {
        current = *changeStorePtr;
        *changeStorePtr = (*changeStorePtr) -> prevChange;
        switch (current -> changeNodeSelector) {
            case 1:
                tabuleiro[(current->changedNode.coord.y*colunas) + current->changedNode.coord.x]  = current -> previousChar;
                break;
            case 2:
                treeAux = current -> changedNode.T;
                switch (current -> valueID) {
                    case 1:
                        treeAux -> hasTentAssigned = current -> previousValue;
                        break;
                    case 2:
                        treeAux -> num_playables = current -> previousValue;
                        break;
                }
                break;
            case 3:
                headAux = current -> changedNode.H;
                switch (current -> valueID) {
                    case 1:
                        headAux -> tentsNeeded = current -> previousValue;
                        break;
                    case 2:
                        headAux -> availablePositions = current -> previousValue;
                        break;
                }
                break;
        }
        free(current);
    }
    *changeStorePtr = NULL;
}
