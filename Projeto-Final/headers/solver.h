#ifndef solver_included
#define solver_included

typedef struct _Playable_Node PlayableNode;

typedef struct _HeadNode{
    int puzzleTents;
    int tentsNeeded;
    PlayableNode *first;
} HeadNode;

void setSolverVectors(HeadNode* , HeadNode*);

#endif
