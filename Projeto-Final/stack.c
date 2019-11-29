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

typedef union {
    PlayableNode *P;
    TreeNode *T;
    HeadNode *H;
} changedNode;

typedef struct _changeStore {
    struct _changeStore *prevChange;
    changedNode changedNode;
    int changeNodeSelector;
    int previousValue;
    int valueID;
} changeStore;

void pushChange(changeStore *changeStorePtr, void *ptr, int changeNodeSelector, int previousValue, int valueID) {
    changeStore *new = (changeStore *) malloc(sizeof(changeStore));
    new -> prevChange = changeStorePtr;
    new -> changeNodeSelector = changeNodeSelector;
    switch(changeNodeSelector) {
        case 1:
            new -> changedNode.P = (PlayableNode*) ptr;
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

void deleteChanges(changeStore *changeStorePtr) {
    PlayableNode *playAux = NULL;
    TreeNode *treeAux = NULL;
    HeadNode *headAux = NULL;

    while(changeStorePtr != NULL) {
        changeStore *current = changeStorePtr;
        changeStorePtr = changeStorePtr -> prevChange;
        switch (current -> changeNodeSelector) {
            case 1:
                playAux = current -> changedNode.P;
                switch (current -> valueID) {
                    case 1:
                        playAux -> valid = current -> previousValue;
                        break;
                    case 2:
                        playAux -> isTent = current -> previousValue;
                        break;
                }
                break;
            case 2:
                treeAux = current -> changedNode.T;
                switch (current -> valueID) {
                    case 1:
                        treeAux -> hasTent = current -> previousValue;
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
