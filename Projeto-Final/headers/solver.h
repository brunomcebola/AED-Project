#ifndef solver_included
#define solver_included

typedef struct _PlayableNode PlayableNode;

typedef struct _HeadNode {
    int puzzleTents;
    int tentsNeeded;
    PlayableNode *first;
} HeadNode;

HeadNode *getSolverVectorRow(void);
HeadNode *getSolverVectorColumn(void);

void setSolverVectors(HeadNode* , HeadNode*);

void solver(void);

void freeSolver(void);

#endif
