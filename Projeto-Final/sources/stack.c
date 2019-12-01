#include <stdlib.h>
#include "../headers/solver.h"
#include "../headers/stack.h"


/*
changeNodeSelector:
    1 - PlayableNode
    2 - TreeNode
    3 - HeadNode

valueID:
    PlayableNode:
        1 - valid
        2 - isTent
    TreeNode:
        1 - hasTent
        2 - num_playables
    HeadNode:
        1 - tentsNeeded
        2 - availablePositions

*/

void pushChange(changeStore *changeStorePtr, void *ptr, int x, int y, int changeNodeSelector, int previousValue, int valueID) {
    changeStore *new = (changeStore *) malloc(sizeof(changeStore));
    new -> prevChange = changeStorePtr;
    new -> changeNodeSelector = changeNodeSelector;
    switch(changeNodeSelector) {
        case 1:
            new -> changedNode.coord.x = x;
            new -> changedNode.coord.y = y;
            break;
        case 2:
            new -> changedNode.T = (TreeNode*) ptr;
            break;
        case 3:
            new -> changedNode.H = (HeadNode*) ptr;
            break;
    }
    new -> previousValue = previousValue;
    new -> valueID = valueID;
    changeStorePtr = new;
}

void deleteChanges(changeStore *changeStorePtr, char *tabuleiro, int colunas) {
    TreeNode *treeAux = NULL;
    HeadNode *headAux = NULL;

    while(changeStorePtr != NULL) {
        changeStore *current = changeStorePtr;
        changeStorePtr = changeStorePtr -> prevChange;
        switch (current -> changeNodeSelector) {
            case 1:
                switch (current -> valueID) {
                    case 1:
                        tabuleiro[(current->changedNode.coord.y*colunas) +current->changedNode.coord.x]  = current -> previousChar;
                        break;
                }
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
}
