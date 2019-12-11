/*******************************************************************************
 * 2019-2020 AED - grupo 65
 * Last modified: 2019-12-09
 *
 * NAME
 *      solver.c
 *
 * DESCRIPTION
 *      Implementation of the functions needed to solve a given puzzle
 *
 * COMMENTS
 *      none
 *
 ******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include "bundle.h"
#include "game.h"
#include "solver.h"
#include "sort.h"
#include "stack.h"


struct Puzzle {
    TreeNode*** treeInfo;
    TreeNode* treeList;
    char * tabuleiro;
    int linhas;
    int colunas;
    int season;
};

struct Puzzle puzzleInfo; //saves current puzzle information to be easilly accessed throughout all file


int randomPlay(int, int, int, int);
void removesP(long unsigned int, int, int, int, changeStore **);



HeadNode *row_vector = NULL, *column_vector = NULL; //save information about a given row or column

/*******************************************************************************
* Function name: getSolverVectorRow()
*
* Arguments: none
*
* Return: vector with information about the rows 
*
* Side-effects: none
*
* Description: return the global vector with information about the rows 
*              (used for external functions)
*
*******************************************************************************/
HeadNode *getSolverVectorRow(void) {
    return row_vector;
}

/*******************************************************************************
* Function name: getSolverVectorColumn()
*
* Arguments: none
*
* Return: vector with information about the columns 
*
* Side-effects: none
*
* Description: return the global vector with information about the columns 
*              (used for external functions)
*
*******************************************************************************/
HeadNode *getSolverVectorColumn(void) {
    return column_vector;
}

/*******************************************************************************
* Function name: setSolverVectors()
*
* Arguments: row - vector with information about the rows
*	     column - vector with information about the columns
*
* Return: none 
*
* Side-effects: none
*
* Description: sets information about rows and columns
*
*******************************************************************************/
void setSolverVectors(HeadNode* row, HeadNode* column) {
    row_vector = row;
    column_vector = column;
}



/*******************************************************************************
* Function name: findPossibleLocations()
*
* Arguments: none
*
* Return: 0 - number of trees is lower than asked tents
*         1 - the puzzle is of type "high season"
*         2 - the puzzle is of type "low season"
*
* Side-effects: if number of trees is lower than asked tents marks puzzle as impossible
*
* Description: finds locations where it is possible to place tents,
*              marks them with 'P' on the board;
*              also counts the number of trees and assigns season to current puzzle
*
*******************************************************************************/
int findPossibleLocations(void) {

	int i, j, numOfTrees = 0, numOfAskedTents = getBoardSum(), retVal;
    long unsigned int index = 0;

	for (i = 0; i < puzzleInfo.linhas; ++i) {

		for (j = 0; j < puzzleInfo.colunas; ++j, ++index) {
			if ((puzzleInfo.tabuleiro)[index] == 'A') {

                ++numOfTrees;

                if (j != 0) {
                    if ((puzzleInfo.tabuleiro)[index-1] == '.' && column_vector[j-1].puzzleTents != 0 && row_vector[i].puzzleTents != 0) {
    					(puzzleInfo.tabuleiro)[index-1] = 'P';
                        ++(column_vector[j-1].availablePositions);
                        ++(row_vector[i].availablePositions);
    				}
                }

                if (j != puzzleInfo.colunas-1) {
                    if ((puzzleInfo.tabuleiro)[index+1] == '.' && column_vector[j+1].puzzleTents != 0 && row_vector[i].puzzleTents != 0) {
    					(puzzleInfo.tabuleiro)[index+1] = 'P';
                        ++(column_vector[j+1].availablePositions);
                        ++(row_vector[i].availablePositions);
    				}
                }

                if (i != 0) {
                    if ((puzzleInfo.tabuleiro)[index-puzzleInfo.colunas] == '.' && column_vector[j].puzzleTents != 0 && row_vector[i-1].puzzleTents != 0) {
    					(puzzleInfo.tabuleiro)[index-puzzleInfo.colunas] = 'P';
                        ++(column_vector[j].availablePositions);
                        ++(row_vector[i-1].availablePositions);
    				}
                }

                if (i != puzzleInfo.linhas-1) {
                    if ((puzzleInfo.tabuleiro)[index+puzzleInfo.colunas] == '.' && column_vector[j].puzzleTents != 0 && row_vector[i+1].puzzleTents != 0) {
    					(puzzleInfo.tabuleiro)[index+puzzleInfo.colunas] = 'P';
                        ++(column_vector[j].availablePositions);
                        ++(row_vector[i+1].availablePositions);
    				}
                }
			}

		}
	}

    if (numOfTrees == numOfAskedTents) {
        setBoardSeason(1);
        retVal = 1;
    } else if (numOfTrees > numOfAskedTents) {
        setBoardSeason(2);
        retVal = 2;
    } else {
        return 0;
    }

    return retVal;
}



/*******************************************************************************
* Function name: createTreeInfo()
*
* Arguments: none
*
* Return: pointer to bi-dimensional array of pointers of type TreeNode
*
* Side-effects: Forces the program to terminate (with exit(0)) if there is an
*               error with the memory managment (opening/creating files or
*               allocating memory)
*
* Description: allocates memory for bi-demensional array of pointers,
*              it keep information about trees easilly accessible
*
*******************************************************************************/
TreeNode ***createTreeInfo(void) {
    TreeNode ***TreeInfo = NULL;
    TreeInfo = (TreeNode ***) malloc(puzzleInfo.linhas * sizeof(TreeNode **));
    checkNull(1, TreeInfo);
    for (int i = 0; i < puzzleInfo.linhas; ++i) {
        TreeInfo[i] = (TreeNode **) malloc(puzzleInfo.colunas * sizeof(TreeNode *));
        checkNull(1, TreeInfo[i]);
        for (int j = 0; j < puzzleInfo.colunas; j++) {
            TreeInfo[i][j] = NULL;
        }
    }
    return TreeInfo;
}


/*******************************************************************************
* Function name: createTreeInfo()
*
* Arguments: none
*
* Return: pointer to inicial head of the list os TreeNode structures
*
* Side-effects: if the puzzle is of type "high season" and a tree has no playable
*               squares around it, it marks the puzzle as impossible
*
* Description: allocates structures with information about a tree's surroundings
*              and status, also connects them through pointers to eachother
*              creating a list
*
*******************************************************************************/
TreeNode *createTreeList(void) {
    int j = 0, i = 0, numPlayables = 0;
    TreeNode *list = NULL;
    long unsigned int index = 0;


    if (puzzleInfo.season == 2) {
        for (i = 0; i < puzzleInfo.linhas; ++i) {

            for (j = 0; j < puzzleInfo.colunas; ++j, ++index) {

                if ((puzzleInfo.tabuleiro)[index] == 'A') {
                    numPlayables = 0;

                    if (j != 0) {
                        if ((puzzleInfo.tabuleiro)[index-1] == 'P') {
        					++numPlayables;
        				}
                    }

                    if (j != puzzleInfo.colunas-1) {
                        if ((puzzleInfo.tabuleiro)[index+1] == 'P') {
        					++numPlayables;
        				}
                    }


                    if (i != 0) {
                        if ((puzzleInfo.tabuleiro)[index-puzzleInfo.colunas] == 'P') {
        					++numPlayables;
        				}
                    }
                    if (i != puzzleInfo.linhas-1) {
                        if ((puzzleInfo.tabuleiro)[index+puzzleInfo.colunas] == 'P') {
        					++numPlayables;

        				}
                    }
                    if (numPlayables > 0) {
                        (puzzleInfo.treeInfo)[i][j] = (TreeNode *) malloc(sizeof(TreeNode));
                        checkNull(1, (puzzleInfo.treeInfo)[i][j]);
                        (puzzleInfo.treeInfo)[i][j]->x = j;
                        (puzzleInfo.treeInfo)[i][j]->y = i;
                        (puzzleInfo.treeInfo)[i][j]->hasTentAssigned = 0;
                        (puzzleInfo.treeInfo)[i][j]->num_playables = numPlayables;
                        (puzzleInfo.treeInfo)[i][j]->next = list;
                        list = (puzzleInfo.treeInfo)[i][j];
                    } else {
                        (puzzleInfo.tabuleiro)[index] = 'D';
                    }
                }
            }
        }
    } else {
        for (i = 0; i < puzzleInfo.linhas; ++i) {

            for (j = 0; j < puzzleInfo.colunas; ++j, ++index) {

                if ((puzzleInfo.tabuleiro)[index] == 'A') {
                    (puzzleInfo.treeInfo)[i][j] = (TreeNode *) malloc(sizeof(TreeNode));
                    checkNull(1, (puzzleInfo.treeInfo)[i][j]);
                    (puzzleInfo.treeInfo)[i][j]->x = j;
                    (puzzleInfo.treeInfo)[i][j]->y = i;
                    (puzzleInfo.treeInfo)[i][j]->hasTentAssigned = 0;
                    (puzzleInfo.treeInfo)[i][j]->num_playables = 0;
                    if (j != 0) {
                        if ((puzzleInfo.tabuleiro)[index-1] == 'P') {
        					++((puzzleInfo.treeInfo)[i][j]->num_playables);
        				}
                    }

                    if (j != puzzleInfo.colunas-1) {
                        if ((puzzleInfo.tabuleiro)[index+1] == 'P') {
        					++((puzzleInfo.treeInfo)[i][j]->num_playables);
        				}
                    }


                    if (i != 0) {
                        if ((puzzleInfo.tabuleiro)[index-puzzleInfo.colunas] == 'P') {
        					++((puzzleInfo.treeInfo)[i][j]->num_playables);
        				}
                    }
                    if (i != puzzleInfo.linhas-1) {
                        if ((puzzleInfo.tabuleiro)[index+puzzleInfo.colunas] == 'P') {
        					++((puzzleInfo.treeInfo)[i][j]->num_playables);

        				}
                    }
                    (puzzleInfo.treeInfo)[i][j]->next = list;
                    list = (puzzleInfo.treeInfo)[i][j];

                    if ((puzzleInfo.treeInfo)[i][j]->num_playables == 0) {
                        setBoardAnswer(-1);
                    }
                }
            }
        }
    }

    return list;
}


/*******************************************************************************
* Function name: checkIfPAlone()
*
* Arguments: index - position of square inside "puzzleInfo.tabuleiro" array
*            x - coordinate of the square
*            y - coordinate of the square
*            changeStorePtr - pointer to stack of changes
*
* Return: none
*
* Side-effects: saves changes on current "change stack"
*
* Description: checks if a previously possible playable square is left alone
*              after associating a tree with a tent (rendering that square unplayble)
*
*******************************************************************************/
void checkIfPAlone(long unsigned int index, int x, int y, changeStore **changeStorePtr) {
    if (x != 0) {

        if ((puzzleInfo.tabuleiro)[index-1] == 'A') {
            return;
        }
    }

    if (x != puzzleInfo.colunas-1) {

        if ((puzzleInfo.tabuleiro)[index+1] == 'A') {
            return;
        }
    }

    if (y != 0) {

        if ((puzzleInfo.tabuleiro)[index-puzzleInfo.colunas] == 'A') {
            return;
        }
    }
    if (y != puzzleInfo.linhas-1) {

        if ((puzzleInfo.tabuleiro)[index+puzzleInfo.colunas] == 'A') {
            return;

        }
    }
    (puzzleInfo.tabuleiro)[index] = '.';
    pushChange(changeStorePtr, NULL, x, y, 1, 0, 'P', 1);
    removesP(index, x, y, 0, changeStorePtr);
}


/*******************************************************************************
* Function name: invalidateTreePPositions()
*
* Arguments: index - position of square inside "puzzleInfo.tabuleiro" array
*            x - coordinate of the square
*            y - coordinate of the square
*            changeStorePtr - pointer to stack of changes
*
* Return: none
*
* Side-effects: none
*
* Description: checks the surroundings of a tree for a previously playble square
*              that has been rendered unplayble
*
*******************************************************************************/
void invalidateTreePPositions(long unsigned int index, int x, int y, changeStore **changeStorePtr) {
    if (x != 0) {

        if ((puzzleInfo.tabuleiro)[index-1] == 'P') {
        checkIfPAlone(index-1, x-1, y, changeStorePtr);
        }
    }

    if (x != puzzleInfo.colunas-1) {

        if ((puzzleInfo.tabuleiro)[index+1] == 'P') {
            checkIfPAlone(index+1, x+1, y, changeStorePtr);
        }
    }

    if (y != 0) {

        if ((puzzleInfo.tabuleiro)[index-puzzleInfo.colunas] == 'P') {
            checkIfPAlone(index-puzzleInfo.colunas, x, y-1, changeStorePtr);
        }
    }
    if (y != puzzleInfo.linhas-1) {

        if ((puzzleInfo.tabuleiro)[index+puzzleInfo.colunas] == 'P') {
            checkIfPAlone(index+puzzleInfo.colunas, x, y+1, changeStorePtr);

        }
    }
}


/*******************************************************************************
* Function name: assignsTentToATree()
*
* Arguments: index - position of square inside "puzzleInfo.tabuleiro" array
*            x - coordinate of the square
*            y - coordinate of the square
*            test - determines if function is just checking the surroundings
*                   of a tent square for trees or is assigning it to the only tree
*                   available
*            changeStorePtr - pointer to stack of changes
*
* Return: none
*
* Side-effects: updates status/information and saves changes on current "change stack"
*
* Description: tries to assign a tent to a tree; it succeeds if only one tree is
*              available to do such association
*
*******************************************************************************/
void assignsTentToATree(long unsigned int index, int x, int y, int test, changeStore **changeStorePtr) {
    int numOfTrees = 0;

    if (x != 0) {
        if ((puzzleInfo.tabuleiro)[index-1] == 'A') {
            if (test) {
                ++numOfTrees;
            } else {
                (puzzleInfo.tabuleiro)[index-1] = 'K';
                pushChange(changeStorePtr, NULL, x-1, y, 1, 0, 'A', 1);
                ((puzzleInfo.treeInfo)[y][x-1]->hasTentAssigned) = 1;
                pushChange(changeStorePtr, (puzzleInfo.treeInfo)[y][x-1], 0, 0, 2, 0, '\0', 1);
                invalidateTreePPositions(index-1, x-1, y, changeStorePtr);
            }
        }
    }

    if (x != puzzleInfo.colunas-1) {
        if ((puzzleInfo.tabuleiro)[index+1] == 'A') {
            if (test) {
                ++numOfTrees;
            } else {
                (puzzleInfo.tabuleiro)[index+1] = 'K';
                pushChange(changeStorePtr, NULL, x+1, y, 1, 0, 'A', 1);
                ((puzzleInfo.treeInfo)[y][x+1]->hasTentAssigned) = 1;
                pushChange(changeStorePtr, (puzzleInfo.treeInfo)[y][x+1], 0, 0, 2, 0, '\0', 1);
                invalidateTreePPositions(index+1, x+1, y, changeStorePtr);
            }
        }
    }

    if (y != 0) {
        if ((puzzleInfo.tabuleiro)[index-puzzleInfo.colunas] == 'A') {
            if (test) {
                ++numOfTrees;
            } else {
                (puzzleInfo.tabuleiro)[index-puzzleInfo.colunas] = 'K';
                pushChange(changeStorePtr, NULL, x, y-1, 1, 0, 'A', 1);
                ((puzzleInfo.treeInfo)[y-1][x]->hasTentAssigned) = 1;
                pushChange(changeStorePtr, (puzzleInfo.treeInfo)[y-1][x], 0, 0, 2, 0, '\0', 1);
                invalidateTreePPositions(index-puzzleInfo.colunas, x, y-1, changeStorePtr);
            }
        }
    }

    if (y != puzzleInfo.linhas-1) {
        if ((puzzleInfo.tabuleiro)[index+puzzleInfo.colunas] == 'A') {
            if (test) {
                ++numOfTrees;
            } else {
                (puzzleInfo.tabuleiro)[index+puzzleInfo.colunas] = 'K';
                pushChange(changeStorePtr, NULL, x, y+1, 1, 0, 'A', 1);
                ((puzzleInfo.treeInfo)[y+1][x]->hasTentAssigned) = 1;
                pushChange(changeStorePtr, (puzzleInfo.treeInfo)[y+1][x], 0, 0, 2, 0, '\0', 1);
                invalidateTreePPositions(index+puzzleInfo.colunas, x, y+1, changeStorePtr);
            }
        }
    }
    if (numOfTrees == 1) {
        assignsTentToATree(index, x, y, 0, changeStorePtr);
    } else if (test) {
        (puzzleInfo.tabuleiro)[index] = 'V';
    }
}




/*******************************************************************************
* Function name: removesP()
*
* Arguments: index - position of square inside "puzzleInfo.tabuleiro" array
*            x - coordinate of the square
*            y - coordinate of the square
*            isTent - determines if updating a playable square into a tent
*                     or an unplayble square
*            changeStorePtr - pointer to stack of changes
*
* Return: none
*
* Side-effects: updates status/information and saves changes on current "change stack"
*
* Description: updates information about the square's row and column and surrounding
*              trees, also marks diagonally adjacent squares as unplayble if
*              "isTent" is set to "true" (1)
*
*******************************************************************************/
void removesP(long unsigned int index, int x, int y, int isTent, changeStore **changeStorePtr) {
    int previousValue = 0;

    if (isTent) {
        previousValue = (row_vector[y].tentsNeeded);
        (row_vector[y].tentsNeeded)--;
        pushChange(changeStorePtr, &(row_vector[y]), 0, 0, 3, previousValue, '\0', 1);
        previousValue = (column_vector[x].tentsNeeded);
        (column_vector[x].tentsNeeded)--;
        pushChange(changeStorePtr, &(column_vector[x]), 0, 0, 3, previousValue, '\0', 1);
    }

    previousValue = (row_vector[y].availablePositions);
    (row_vector[y].availablePositions)--;
    pushChange(changeStorePtr, &(row_vector[y]), 0, 0, 3, previousValue, '\0', 2);
    previousValue = (column_vector[x].availablePositions);
    (column_vector[x].availablePositions)--;
    pushChange(changeStorePtr, &(column_vector[x]), 0, 0, 3, previousValue, '\0', 2);

    if (x != 0) {
        if ((puzzleInfo.tabuleiro)[index-1] == 'A') {
            previousValue = ((puzzleInfo.treeInfo)[y][x-1]->num_playables);
            ((puzzleInfo.treeInfo)[y][x-1]->num_playables)--;
            pushChange(changeStorePtr, (puzzleInfo.treeInfo)[y][x-1], 0, 0, 2, previousValue, '\0', 2);
        } else if ((puzzleInfo.tabuleiro)[index-1] == 'P' && isTent) {
            (puzzleInfo.tabuleiro)[index-1] = '.';
            pushChange(changeStorePtr, NULL, x-1, y, 1, 0, 'P', 1);
            removesP(index-1, x-1, y, 0, changeStorePtr);
        }
    }

    if (x != puzzleInfo.colunas-1) {
        if ((puzzleInfo.tabuleiro)[index+1] == 'A') {
            previousValue = ((puzzleInfo.treeInfo)[y][x+1]->num_playables);
            ((puzzleInfo.treeInfo)[y][x+1]->num_playables)--;
            pushChange(changeStorePtr, (puzzleInfo.treeInfo)[y][x+1], 0, 0, 2, previousValue, '\0', 2);
        } else if ((puzzleInfo.tabuleiro)[index+1] == 'P' && isTent) {
            (puzzleInfo.tabuleiro)[index+1] = '.';
            pushChange(changeStorePtr, NULL, x+1, y, 1, 0, 'P', 1);
            removesP(index+1, x+1, y, 0, changeStorePtr);
        }
    }

    if (y != 0) {
        if ((puzzleInfo.tabuleiro)[index-puzzleInfo.colunas] == 'A') {
            previousValue = ((puzzleInfo.treeInfo)[y-1][x]->num_playables);
            ((puzzleInfo.treeInfo)[y-1][x]->num_playables)--;
            pushChange(changeStorePtr, (puzzleInfo.treeInfo)[y-1][x], 0, 0, 2, previousValue, '\0', 2);
        } else if ((puzzleInfo.tabuleiro)[index-puzzleInfo.colunas] == 'P' && isTent) {
            (puzzleInfo.tabuleiro)[index-puzzleInfo.colunas] = '.';
            pushChange(changeStorePtr, NULL, x, y-1, 1, 0, 'P', 1);
            removesP(index-puzzleInfo.colunas, x, y-1, 0, changeStorePtr);
        }

        if (isTent) {
            if (x != 0) {
                if ((puzzleInfo.tabuleiro)[index-puzzleInfo.colunas-1] == 'P') {
                    (puzzleInfo.tabuleiro)[index-puzzleInfo.colunas-1] = '.';
                    pushChange(changeStorePtr, NULL, x-1, y-1, 1, 0, 'P', 1);
                    removesP(index-puzzleInfo.colunas-1, x-1, y-1, 0, changeStorePtr);
                }
            }

            if (x != puzzleInfo.colunas-1) {
                if ((puzzleInfo.tabuleiro)[index-puzzleInfo.colunas+1] == 'P') {
                    (puzzleInfo.tabuleiro)[index-puzzleInfo.colunas+1] = '.';
                    pushChange(changeStorePtr, NULL, x+1, y-1, 1, 0, 'P', 1);
                    removesP(index-puzzleInfo.colunas+1, x+1, y-1, 0, changeStorePtr);
                }
            }
        }

    }

    if (y != puzzleInfo.linhas-1) {
        if ((puzzleInfo.tabuleiro)[index+puzzleInfo.colunas] == 'A') {
            previousValue = ((puzzleInfo.treeInfo)[y+1][x]->num_playables);
            ((puzzleInfo.treeInfo)[y+1][x]->num_playables)--;
            pushChange(changeStorePtr, (puzzleInfo.treeInfo)[y+1][x], 0, 0, 2, previousValue, '\0', 2);
        } else if ((puzzleInfo.tabuleiro)[index+puzzleInfo.colunas] == 'P' && isTent) {
            (puzzleInfo.tabuleiro)[index+puzzleInfo.colunas] = '.';
            pushChange(changeStorePtr, NULL, x, y+1, 1, 0, 'P', 1);
            removesP(index+puzzleInfo.colunas, x, y+1, 0, changeStorePtr);
        }

        if (isTent) {
            if (x != 0 ) {
                if ((puzzleInfo.tabuleiro)[index+puzzleInfo.colunas-1] == 'P') {
                    (puzzleInfo.tabuleiro)[index+puzzleInfo.colunas-1] = '.';
                    pushChange(changeStorePtr, NULL, x-1, y+1, 1, 0, 'P', 1);
                    removesP(index+puzzleInfo.colunas-1, x-1, y+1, 0, changeStorePtr);
                }
            }

            if (x != puzzleInfo.colunas-1) {
                if ((puzzleInfo.tabuleiro)[index+puzzleInfo.colunas+1] == 'P') {
                    (puzzleInfo.tabuleiro)[index+puzzleInfo.colunas+1] = '.';
                    pushChange(changeStorePtr, NULL, x+1, y+1, 1, 0, 'P', 1);
                    removesP(index+puzzleInfo.colunas+1, x+1, y+1, 0, changeStorePtr);
                }
            }
        }
    }
}



/*******************************************************************************
* Function name: checkNeededTents()
*
* Arguments: changeStorePtr - pointer to stack of changes
*
* Return: 1 - changes have been made
*         0 - exited withou making any move
*         404 - the map has reached an impossible state
*
* Side-effects: updates status/information and saves changes on current "change stack"
*
* Description: checks if the remaining needed tents for every row/column is equal
*              to remaining playable square; updates those squares to tents
*
*******************************************************************************/
int checkNeededTents(changeStore **changeStorePtr) {
    int i = 0, j = 0, retVal = 0, modified = 1;
    long unsigned int index = 0;

    for (index = 0, i = 0; i < puzzleInfo.linhas; ++i, index += puzzleInfo.colunas) {
        if ((row_vector[i].tentsNeeded == row_vector[i].availablePositions) && row_vector[i].tentsNeeded > 0) {

            for (j = 0; j < puzzleInfo.colunas; ++j) {
                if ((puzzleInfo.tabuleiro)[index+j] == 'P') {
                    retVal = 1;
                    (puzzleInfo.tabuleiro)[index+j] = 'T';
                    pushChange(changeStorePtr, NULL, j, i, 1, 0, 'P', 1);
                    removesP(index+j, j, i, 1, changeStorePtr);
                    assignsTentToATree(index+j, j, i, 1, changeStorePtr);
                }
            }
            while (modified) {
                modified = 0;
                for (j = 0; j < puzzleInfo.colunas; ++j) {
                    if ((puzzleInfo.tabuleiro)[index+j] == 'V') {
                        (puzzleInfo.tabuleiro)[index+j] = 'T';
                        assignsTentToATree(index+j, j, i, 1, changeStorePtr);
                        if ((puzzleInfo.tabuleiro)[index+j] != 'V') {
                            pushChange(changeStorePtr, NULL, j, i, 1, 0, 'V', 1);
                            modified = 1;
                        }
                    }
                }
            }
        } else if (row_vector[i].tentsNeeded > row_vector[i].availablePositions) {
            return 404;
        } else if (row_vector[i].availablePositions > 0 && row_vector[i].tentsNeeded == 0) {

            for (j = 0; j < puzzleInfo.colunas; ++j) {
                if ((puzzleInfo.tabuleiro)[index+j] == 'P') {
                    retVal = 1;
                    (puzzleInfo.tabuleiro)[index+j] = '.';
                    pushChange(changeStorePtr, NULL, j, i, 1, 0, 'P', 1);
                    removesP(index+j, j, i, 0, changeStorePtr);
                }
            }
        }
    }
    modified = 1;
    for (i = 0; i < puzzleInfo.colunas; ++i) {
        if ((column_vector[i].tentsNeeded == column_vector[i].availablePositions) && column_vector[i].tentsNeeded > 0) {

            for (index = i, j = 0; j < puzzleInfo.linhas; ++j, index += puzzleInfo.colunas) {
                if ((puzzleInfo.tabuleiro)[index] == 'P') {
                    retVal = 1;
                    (puzzleInfo.tabuleiro)[index] = 'T';
                    pushChange(changeStorePtr, NULL, i, j, 1, 0, 'P', 1);
                    removesP(index, i, j, 1, changeStorePtr);
                    assignsTentToATree(index, i, j, 1, changeStorePtr);
                }
            }
            while (modified) {
                modified = 0;
                for (index = i, j = 0; j < puzzleInfo.linhas; ++j, index += puzzleInfo.colunas) {
                    if ((puzzleInfo.tabuleiro)[index] == 'V') {
                        (puzzleInfo.tabuleiro)[index] = 'T';
                        assignsTentToATree(index, i, j, 1, changeStorePtr);
                        if ((puzzleInfo.tabuleiro)[index] != 'V') {
                            pushChange(changeStorePtr, NULL, i, j, 1, 0, 'V', 1);
                            modified = 1;
                        }
                    }
                }
            }
        } else if (column_vector[i].tentsNeeded > column_vector[i].availablePositions) {
            return 404;
        } else if (column_vector[i].availablePositions > 0 && column_vector[i].tentsNeeded == 0) {

            for (index = i, j = 0; j < puzzleInfo.linhas; ++j, index += puzzleInfo.colunas) {
                if ((puzzleInfo.tabuleiro)[index] == 'P') {
                    retVal = 1;
                    (puzzleInfo.tabuleiro)[index] = '.';
                    pushChange(changeStorePtr, NULL, i, j, 1, 0, 'P', 1);
                    removesP(index, i, j, 0, changeStorePtr);
                }
            }
        }
    }
    return retVal;
}



/*******************************************************************************
* Function name: checkForLonelyTrees()
*
* Arguments: changeStorePtr - pointer to stack of changes
*
* Return: 1 - changes have been made
*         0 - exited withou making any move
*         404 - if a tree has no tent assigned and hasn't any adjacent playable
*               positions or unassigned tents
*
* Side-effects: updates status/information and saves changes on current "change stack"
*
* Description: checks if any tree has only one playable square or unassigned tent
*              next to it; updates that square into an assigned tent
*
*******************************************************************************/
int checkForLonelyTrees(changeStore **changeStorePtr) {
    TreeNode *aux;
    int numOfV = 0;
    long unsigned int index = 0;
    MergeSort(&(puzzleInfo.treeList));
    aux = puzzleInfo.treeList;

    if (aux == NULL) {
        return 404; //if there are no trees, there's an error
    }

    while ((aux)->num_playables == 0) {
        numOfV = 0;
        if (aux->hasTentAssigned == 0) {
            index = (aux->y)*puzzleInfo.colunas + aux->x;

            if (aux->x != 0) {
                if ((puzzleInfo.tabuleiro)[index-1] == 'V') {
                    ++numOfV;
                }
            }
            if (aux->x != puzzleInfo.colunas-1) {
                if ((puzzleInfo.tabuleiro)[index+1] == 'V') {
                    ++numOfV;
                }
            }
            if (aux->y != 0) {
                if ((puzzleInfo.tabuleiro)[index-puzzleInfo.colunas] == 'V') {
                    ++numOfV;
                }
            }
            if (aux->y != puzzleInfo.linhas-1) {
                if ((puzzleInfo.tabuleiro)[index+puzzleInfo.colunas] == 'V') {
                    ++numOfV;
                }
            }

            if (numOfV > 1) {
                aux = aux->next;
                if (aux == NULL) {
                    return 0;
                } else {
                    continue;
                }
            }

            if (aux->x != 0) {
                if ((puzzleInfo.tabuleiro)[index-1] == 'V') {
                    aux->hasTentAssigned = 1;
                    pushChange(changeStorePtr, aux, 0, 0, 2, 0, '\0', 1);
                    (puzzleInfo.tabuleiro)[index] = 'K';
                    pushChange(changeStorePtr, NULL, aux->x, aux->y, 1, 0, 'A', 1);
                    (puzzleInfo.tabuleiro)[index-1] = 'T';
                    pushChange(changeStorePtr, NULL, aux->x -1, aux->y, 1, 0, 'V', 1);
                    //doesn't use "removesP()" because those changes have already been made
                    return 1;
                }
            }

            if (aux->x != puzzleInfo.colunas-1) {
                if ((puzzleInfo.tabuleiro)[index+1] == 'V') {
                    aux->hasTentAssigned = 1;
                    pushChange(changeStorePtr, aux, 0, 0, 2, 0, '\0', 1);
                    (puzzleInfo.tabuleiro)[index] = 'K';
                    pushChange(changeStorePtr, NULL, aux->x, aux->y, 1, 0, 'A', 1);
                    (puzzleInfo.tabuleiro)[index+1] = 'T';
                    pushChange(changeStorePtr, NULL, aux->x +1, aux->y, 1, 0, 'V', 1);
                    //doesn't use "removesP()" because those changes have already been made
                    return 1;
                }
            }

            if (aux->y != 0) {
                if ((puzzleInfo.tabuleiro)[index-puzzleInfo.colunas] == 'V') {
                    aux->hasTentAssigned = 1;
                    pushChange(changeStorePtr, aux, 0, 0, 2, 0, '\0', 1);
                    (puzzleInfo.tabuleiro)[index] = 'K';
                    pushChange(changeStorePtr, NULL, aux->x, aux->y, 1, 0, 'A', 1);
                    (puzzleInfo.tabuleiro)[index-puzzleInfo.colunas] = 'T';
                    pushChange(changeStorePtr, NULL, aux->x , aux->y -1, 1, 0, 'V', 1);
                    //doesn't use "removesP()" because those changes have already been made
                    return 1;
                }
            }

            if (aux->y != puzzleInfo.linhas-1) {
                if ((puzzleInfo.tabuleiro)[index+puzzleInfo.colunas] == 'V') {
                    aux->hasTentAssigned = 1;
                    pushChange(changeStorePtr, aux, 0, 0, 2, 0, '\0', 1);
                    (puzzleInfo.tabuleiro)[index] = 'K';
                    pushChange(changeStorePtr, NULL, aux->x, aux->y, 1, 0, 'A', 1);
                    (puzzleInfo.tabuleiro)[index+puzzleInfo.colunas] = 'T';
                    pushChange(changeStorePtr, NULL, aux->x , aux->y +1, 1, 0, 'V', 1);
                    //doesn't use "removesP()" because those changes have already been made
                    return 1;
                }
            }
            return 404;
        }
        aux = aux->next;
    }

    while (aux->num_playables <= 1 && aux->hasTentAssigned == 1) {
        if (aux->next == NULL) {
            return 0;
        }
        aux = aux->next;
    }

    while (aux != NULL) {
        if ((aux)->num_playables == 1 && (aux)->hasTentAssigned == 0) {

            index = (aux->y)*puzzleInfo.colunas + aux->x;

            if (aux->x != 0) {
                if ((puzzleInfo.tabuleiro)[index-1] == 'V') {
                    aux = aux->next;
                    continue;
                }
            }
            if (aux->x != puzzleInfo.colunas-1) {
                if ((puzzleInfo.tabuleiro)[index+1] == 'V') {
                    aux = aux->next;
                    continue;
                }
            }
            if (aux->y != 0) {
                if ((puzzleInfo.tabuleiro)[index-puzzleInfo.colunas] == 'V') {
                    aux = aux->next;
                    continue;
                }
            }
            if (aux->y != puzzleInfo.linhas-1) {
                if ((puzzleInfo.tabuleiro)[index+puzzleInfo.colunas] == 'V') {
                    aux = aux->next;
                    continue;
                }
            }


            if (aux->x != 0) {
                if ((puzzleInfo.tabuleiro)[index-1] == 'P') {
                    aux->hasTentAssigned = 1;
                    pushChange(changeStorePtr, aux, 0, 0, 2, 0, '\0', 1);
                    (puzzleInfo.tabuleiro)[index] = 'K';
                    pushChange(changeStorePtr, NULL, aux->x, aux->y, 1, 0, 'A', 1);
                    (puzzleInfo.tabuleiro)[index-1] = 'T';
                    pushChange(changeStorePtr, NULL, aux->x -1, aux->y, 1, 0, 'P', 1);
                    removesP(index-1, aux->x -1, aux->y, 1, changeStorePtr);
                    return 1;
                }
            }

            if (aux->x != puzzleInfo.colunas-1) {
                if ((puzzleInfo.tabuleiro)[index+1] == 'P') {
                    aux->hasTentAssigned = 1;
                    pushChange(changeStorePtr, aux, 0, 0, 2, 0, '\0', 1);
                    (puzzleInfo.tabuleiro)[index] = 'K';
                    pushChange(changeStorePtr, NULL, aux->x, aux->y, 1, 0, 'A', 1);
                    (puzzleInfo.tabuleiro)[index+1] = 'T';
                    pushChange(changeStorePtr, NULL, aux->x +1, aux->y, 1, 0, 'P', 1);
                    removesP(index+1, aux->x +1, aux->y, 1, changeStorePtr);
                    return 1;
                }
            }

            if (aux->y != 0) {
                if ((puzzleInfo.tabuleiro)[index-puzzleInfo.colunas] == 'P') {
                    aux->hasTentAssigned = 1;
                    pushChange(changeStorePtr, aux, 0, 0, 2, 0, '\0', 1);
                    (puzzleInfo.tabuleiro)[index] = 'K';
                    pushChange(changeStorePtr, NULL, aux->x, aux->y, 1, 0, 'A', 1);
                    (puzzleInfo.tabuleiro)[index-puzzleInfo.colunas] = 'T';
                    pushChange(changeStorePtr, NULL, aux->x, aux->y -1, 1, 0, 'P', 1);
                    removesP(index-puzzleInfo.colunas, aux->x, aux->y -1, 1, changeStorePtr);
                    return 1;
                }
            }

            if (aux->y != puzzleInfo.linhas-1) {
                if ((puzzleInfo.tabuleiro)[index+puzzleInfo.colunas] == 'P') {
                    aux->hasTentAssigned = 1;
                    pushChange(changeStorePtr, aux, 0, 0, 2, 0, '\0', 1);
                    (puzzleInfo.tabuleiro)[index] = 'K';
                    pushChange(changeStorePtr, NULL, aux->x, aux->y, 1, 0, 'A', 1);
                    (puzzleInfo.tabuleiro)[index+puzzleInfo.colunas] = 'T';
                    pushChange(changeStorePtr, NULL, aux->x, aux->y +1, 1, 0, 'P', 1);
                    removesP(index+puzzleInfo.colunas, aux->x, aux->y +1, 1, changeStorePtr);
                    return 1;
                }
            }

        } else {
            return 0;
        }
        aux = aux->next;
    }

    return 0;
}


/*******************************************************************************
* Function name: checkConsecutive()
*
* Arguments: changeStorePtr - pointer to stack of changes
*
* Return:  1 - changes have been made
*          0 - exited withou making any move
*
* Side-effects: updates status/information and saves changes on current "change stack"
*
* Description: checks if maximum of playable squares that can be updated to tents
*              is equal to the the needed tents for that given row or column;
*              updates squares to tents if they are grouped in sets of
*              odd adjacent playble squares
*
*******************************************************************************/
int checkConsecutive(changeStore **changeStorePtr) {
    int totalSimultaneousSpots = 0, numOfConsecutives = 0, i, j, retVal = 0, flag1 = 0, flag2 = 0;
    long unsigned int  index = 0;

    for (i = 0; i < puzzleInfo.linhas; ++i, index += puzzleInfo.colunas) {
        totalSimultaneousSpots = 0, numOfConsecutives = 0;
        for (j = 0; j < puzzleInfo.colunas; ++j) {
            if (numOfConsecutives && ((puzzleInfo.tabuleiro)[index+j] != 'P')) {
                totalSimultaneousSpots += ((numOfConsecutives+1)>>1);
                numOfConsecutives = 0;
            } else if ((puzzleInfo.tabuleiro)[index+j] == 'P') {
                ++numOfConsecutives;
            }
        }

        totalSimultaneousSpots += ((numOfConsecutives+1)>>1);

        if ((totalSimultaneousSpots == row_vector[i].tentsNeeded) && row_vector[i].tentsNeeded != 0) {

            for (j = 0; j < puzzleInfo.colunas; ++j) {
                if ((puzzleInfo.tabuleiro)[index+j] == 'P') {
                    if (j != 0) {
                        if ((puzzleInfo.tabuleiro)[index+j-1] == 'V') {
                            continue;
                        }
                    }
                    flag1 = 0;
                    while (!((puzzleInfo.tabuleiro)[index+j] != 'P')) {
                        ++flag1;
                        if ((++j) == puzzleInfo.colunas) break;
                    }
                    if (j != puzzleInfo.colunas) {
                        if ((puzzleInfo.tabuleiro)[index+j] == 'V') {
                            continue;
                        }
                    }
                    --j;

                    if (flag1 == 1) {
                        retVal = 1;
                        (puzzleInfo.tabuleiro)[index+j] = 'T';
                        pushChange(changeStorePtr, NULL, j, i, 1, 0, 'P', 1);
                        removesP(index+j, j, i, 1, changeStorePtr);
                        assignsTentToATree(index+j, j, i, 1, changeStorePtr);
                    } else if (flag1%2 == 1) {
                        retVal = 1;
                        flag2 = 1;
                        do {
                            if (flag2) {
                                flag2 = -1;
                                (puzzleInfo.tabuleiro)[index+j] = 'T';
                                pushChange(changeStorePtr, NULL, j, i, 1, 0, 'P', 1);
                                removesP(index+j, j, i, 1, changeStorePtr);
                                assignsTentToATree(index+j, j, i, 1, changeStorePtr);
                            }
                            --j;
                            ++flag2;
                        } while(--flag1);
                    }
                }
            }
        }
    }
    for (i = 0; i < puzzleInfo.colunas; ++i) {
        totalSimultaneousSpots = 0, numOfConsecutives = 0;
        for (index = i, j = 0; j < puzzleInfo.linhas; ++j, index += puzzleInfo.colunas) {
            if (numOfConsecutives && ((puzzleInfo.tabuleiro)[index] != 'P')) {
                totalSimultaneousSpots += ((numOfConsecutives+1)>>1);
                numOfConsecutives = 0;
            } else if ((puzzleInfo.tabuleiro)[index] == 'P') {
                ++numOfConsecutives;
            }
        }

        totalSimultaneousSpots += ((numOfConsecutives+1)>>1);

        if ((totalSimultaneousSpots == column_vector[i].tentsNeeded) && column_vector[i].tentsNeeded != 0) {
            for (index = i, j = 0; j < puzzleInfo.linhas; ++j, index += puzzleInfo.colunas) {
                if ((puzzleInfo.tabuleiro)[index] == 'P') {
                    flag1 = 0;
                    if (j != 0) {
                        if ((puzzleInfo.tabuleiro)[index-puzzleInfo.colunas] == 'V') {
                            continue;
                        }
                    }

                    while (!((puzzleInfo.tabuleiro)[index] != 'P')) {
                        ++flag1;
                        index += puzzleInfo.colunas;
                        if ( (++j) == puzzleInfo.linhas) break;
                    }
                    if (j != puzzleInfo.linhas) {
                        if ((puzzleInfo.tabuleiro)[index] == 'V') {
                            continue;
                        }
                    }
                    --j;
                    index -= puzzleInfo.colunas;


                    if (flag1 == 1) {
                        retVal = 1;
                        (puzzleInfo.tabuleiro)[index] = 'T';
                        pushChange(changeStorePtr, NULL, i, j, 1, 0, 'P', 1);
                        removesP(index, i, j, 1, changeStorePtr);
                        assignsTentToATree(index, i, j, 1, changeStorePtr);
                    } else if (flag1%2 == 1) {
                        retVal = 1;
                        flag2 = 1;
                        do {
                            if (flag2) {
                                flag2 = -1;
                                (puzzleInfo.tabuleiro)[index] = 'T';
                                pushChange(changeStorePtr, NULL, i, j, 1, 0, 'P', 1);
                                removesP(index, i, j, 1, changeStorePtr);
                                assignsTentToATree(index, i, j, 1, changeStorePtr);
                            }
                            --j;
                            index -= puzzleInfo.colunas;
                            ++flag2;
                        } while(--flag1);
                    }
                }
            }
        }
    }
    return retVal;
}



/*******************************************************************************
* Function name: checkIfPuzzleSolved()
*
* Arguments: none
*
* Return: 1 - current puzzle is solved
*         0 - current puzzle is not yet solved
*
* Side-effects: sets board answer to 1 if puzzle is solved
*
* Description: checks if current game state has reached a solution to the puzzle
*
*******************************************************************************/
int checkIfPuzzleSolved(void) {
    for (int i = 0; i < puzzleInfo.linhas; i++) {
        if (row_vector[i].tentsNeeded != 0) {
            return 0;
        }
    }
    for (int i = 0; i < puzzleInfo.colunas; i++) {
        if (column_vector[i].tentsNeeded != 0) {
            return 0;
        }
    }
    setBoardAnswer(1);
    return 1;
}


/*******************************************************************************
* Function name: makeSureMoves()
*
* Arguments: changeStorePtr - pointer to stack of changes
*
* Return: 1 - the game has reached an impossible state
*         0 - no errors so far
*
* Side-effects: none
*
* Description: calls, while squares are being updated, the 3 (or 2 in "low season"
*              type of puzzles) functions that update squares that are, without any
*              doubt, a tent
*
*******************************************************************************/
int makeSureMoves(changeStore **changeStorePtr) {
    int modified = 1;

    if (puzzleInfo.season == 1) {
        while (modified) {

            if (modified == 404) {
                return 1;
            }

            modified = 0;

            if ((modified = checkForLonelyTrees(changeStorePtr))) {
                continue;
            }

            if ((modified = checkNeededTents(changeStorePtr))) {

                continue;
            }

            if ((modified = checkConsecutive(changeStorePtr))) {

                continue;
            }
        }
    } else {
        while (modified) {
            if (modified == 404) {
                return 1;
            }
            modified = 0;

            if ((modified = checkNeededTents(changeStorePtr))) {
                continue;
            }

            if ((modified = checkConsecutive(changeStorePtr))) {
                continue;
            }
            modified = 0;
        }
    }

    return 0;
}



/*******************************************************************************
* Function name: freeTreeInfo()
*
* Arguments: none
*
* Return: none
*
* Side-effects: changes any invalid symbol to it's intended state
*
* Description: frees treeNode type of structures
*
*******************************************************************************/
void freeTreeInfo(void) {
    int j = 0, i = 0;
    long unsigned int index = 0;

    for (i = 0; i < puzzleInfo.linhas; ++i) {

        for (j = 0; j < puzzleInfo.colunas; ++j, ++index) {

            switch ((puzzleInfo.tabuleiro)[index]) {
                case 'K':
                    (puzzleInfo.tabuleiro)[index] = 'A';
                    free((puzzleInfo.treeInfo)[i][j]);
                    break;
                case 'A':
                    free((puzzleInfo.treeInfo)[i][j]);
                    break;
                case 'P':
                    (puzzleInfo.tabuleiro)[index] = '.';
                    break;
                case 'V':
                    (puzzleInfo.tabuleiro)[index] = 'T';
                    break;
                case 'D':
                    (puzzleInfo.tabuleiro)[index] = 'A';
                    break;
            }
        }
        free((puzzleInfo.treeInfo)[i]);
    }
    free((puzzleInfo.treeInfo));
}



/*******************************************************************************
* Function name: heuristicsForRandomPlay()
*
* Arguments: none
*
* Return: none
*
* Side-effects: checks if puzzle is solved
*
* Description: finds column or row that has the lowest number of random combinations
*              and uses it to start making "pseudo-random" moves
*
*******************************************************************************/
void heuristicsForRandomPlay(void) {

    int linhasOuColunas = 0, i = 0, index = 0, numOfMoves = 0;
    double max = 0, temp = 0;

    max = -1, temp = 0, numOfMoves = 0, index = 0, linhasOuColunas = -1;

    for (i = 0; i < puzzleInfo.colunas; i++) {
        if (column_vector[i].availablePositions > 0) {
            if (max < (temp = (double)column_vector[i].tentsNeeded/(double)column_vector[i].availablePositions)) {
                max = temp;
                numOfMoves = column_vector[i].tentsNeeded;
                index = i;
                linhasOuColunas = 0;

            }
        }

    }

    for (i = 0; i < puzzleInfo.linhas; i++) {
        if (row_vector[i].availablePositions > 0) {
            if (max < (temp = (double)row_vector[i].tentsNeeded/(double)row_vector[i].availablePositions)) {
                max = temp;
                numOfMoves = row_vector[i].tentsNeeded;
                index = i;
                linhasOuColunas = 1;
            }
        }

    }
    if (linhasOuColunas == 1) {
        randomPlay( 0, index, numOfMoves, 1);
    } else if (linhasOuColunas == 0){
        randomPlay( index, 0, numOfMoves, 0);
    }
    checkIfPuzzleSolved();
}



/*******************************************************************************
* Function name: randomPlay()
*
* Arguments: x - coordinate of the square
*            y - coordinate of the square
*            numOfMoves - number of trees that have to be placed(aka number of times
*                         the function is going to call itself)
*            linhasOuColunas - determines if random play is made on a row or a column
*
* Return: 1 - the puzzle has been solved
*         0 - the puzzle has not been solved
*
* Side-effects: creates a new changes stak, saves changes on it, and uses it everytime
*               it reaches an impossible state for the board to undo all changes up to
*               the moves made before calling the current instance of the function
*
* Description: updates a playable square into a tent, and tries to solve puzzle;
*              if it is impossible, undoes that move and tries to put a tent on
*              the next playable square or until it reaches the edge
*
*******************************************************************************/
int randomPlay(int x, int y, int numOfMoves, int linhasOuColunas) {
    int i = 0, edge = 0, modified = 1, j = 0;
    long unsigned int indexAux = 0, index = 0;
    changeStore *changes = NULL;

    if (linhasOuColunas) {
        edge = puzzleInfo.colunas;
        i = x;
    } else {
        edge = puzzleInfo.linhas;
        i = y;
    }

    for (index = y*(puzzleInfo.colunas) +x; i < edge; ++i) {
        if ((puzzleInfo.tabuleiro)[index] == 'P') {
            (puzzleInfo.tabuleiro)[index] = 'T';
            if (linhasOuColunas) {
                pushChange(&changes, NULL, i, y, 1, 0, 'P', 1);
                removesP(index, i, y, 1, &changes);
                assignsTentToATree(index, i, y, 1, &changes);
            } else {
                pushChange(&changes, NULL, x, i, 1, 0, 'P', 1);
                removesP(index, x, i, 1, &changes);
                assignsTentToATree(index, x, i, 1, &changes);
            }

            if (numOfMoves > 1) {
                if (linhasOuColunas) {
                    if (randomPlay( i, y, numOfMoves-1, linhasOuColunas)) {
                        freeChangeList(&changes);
                        return 1;
                    } else {
                        deleteChanges(&changes, (puzzleInfo.tabuleiro), puzzleInfo.colunas);
                    }
                } else {
                    if(randomPlay( x, i, numOfMoves-1, linhasOuColunas)) {
                        freeChangeList(&changes);
                        return 1;
                    } else {
                        deleteChanges(&changes, (puzzleInfo.tabuleiro), puzzleInfo.colunas);
                    }
                }
            } else {
                modified = 1;
                if (linhasOuColunas) {
                    while (modified) {
                        modified = 0;
                        for (indexAux = y*puzzleInfo.colunas, j = 0; j < puzzleInfo.colunas; ++j, ++indexAux) {
                            if ((puzzleInfo.tabuleiro)[indexAux] == 'V') {
                                (puzzleInfo.tabuleiro)[indexAux] = 'T';
                                assignsTentToATree(indexAux, j, y, 1, &changes);
                                if ((puzzleInfo.tabuleiro)[indexAux] != 'V') {
                                    pushChange(&changes, NULL, j, y, 1, 0, 'V', 1);
                                    modified = 1;
                                }
                            }
                        }
                    }
                } else {
                    while (modified) {
                        modified = 0;
                        for (indexAux = x, j = 0; j < puzzleInfo.linhas; ++j, indexAux += puzzleInfo.colunas) {
                            if ((puzzleInfo.tabuleiro)[indexAux] == 'V') {
                                (puzzleInfo.tabuleiro)[indexAux] = 'T';
                                assignsTentToATree(indexAux, x, j, 1, &changes);
                                if ((puzzleInfo.tabuleiro)[indexAux] != 'V') {
                                    pushChange(&changes, NULL, x, j, 1, 0, 'V', 1);
                                    modified = 1;
                                }
                            }
                        }
                    }
                }
                if (makeSureMoves(&changes)) {
                    deleteChanges(&changes, (puzzleInfo.tabuleiro), puzzleInfo.colunas);
                } else {
                    if(checkIfPuzzleSolved()) {
                        freeChangeList(&changes);
                        return 1;
                    } else {
                        heuristicsForRandomPlay();
                        if(checkIfPuzzleSolved()) {
                            freeChangeList(&changes);
                            return 1;
                        } else {
                            deleteChanges(&changes, (puzzleInfo.tabuleiro), puzzleInfo.colunas);
                        }
                    }
                }
            }

        }
        if (linhasOuColunas) {
            ++index;
        } else {
            index += puzzleInfo.colunas;
        }
    }
    deleteChanges(&changes, (puzzleInfo.tabuleiro), puzzleInfo.colunas);
    return 0;
}



/*******************************************************************************
* Function name: soler()
*
* Arguments: none
*
* Return: none
*
* Side-effects: none
*
* Description: finds one solution to the puzzle or determines if it is a
*              puzzle with impossible inicial constraints
*
*******************************************************************************/
void solver(void) {
    (puzzleInfo.tabuleiro) = getBoardLayout();
    puzzleInfo.linhas = getBoardRows(), puzzleInfo.colunas = getBoardColumns();
    puzzleInfo.treeInfo = NULL, puzzleInfo.treeList = NULL;
    changeStore *changeStorePtr = NULL;

    if(!(puzzleInfo.season = findPossibleLocations())) {
        setBoardAnswer(-1);
        return;
    }
    puzzleInfo.treeInfo = createTreeInfo();
    puzzleInfo.treeList = createTreeList();

    if (getBoardAnswer() != -1) {
        makeSureMoves(&changeStorePtr);

        if(!checkIfPuzzleSolved()) {
            heuristicsForRandomPlay();
        }

        if(!checkIfPuzzleSolved()) {
            setBoardAnswer(-1);
        }
    }

    freeChangeList(&changeStorePtr);

    freeTreeInfo();
}




/*******************************************************************************
* Function name: freeSolver()
*
* Arguments: none
*
* Return: none
*
* Side-effects: none
*
* Description: frees arrays containing information about each row and column
*
*******************************************************************************/
void freeSolver(void) {
    free(row_vector);
    free(column_vector);
}
