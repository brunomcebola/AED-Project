#ifndef solver_included
#define solver_included

typedef struct _PlayableNode PlayableNode;

typedef struct _HeadNode {
    int puzzleTents;
    int tentsNeeded;
    int availablePositions;
    PlayableNode *first;
} HeadNode;

typedef struct _TreeNode {
    int x, y;
    int hasTentAssigned;
    int num_playables;
    struct _TreeNode *next;
} TreeNode;


HeadNode *getSolverVectorRow(void);
HeadNode *getSolverVectorColumn(void);


void setSolverVectors(HeadNode* , HeadNode*);
void solver(void);
void freeSolver(void);

#endif
