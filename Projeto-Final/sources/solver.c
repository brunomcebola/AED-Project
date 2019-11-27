#include <stdlib.h>
#include "../headers/solver.h"

typedef struct _Playable_Node {
    int x, y;
    int valid;
    struct _Playable_Node *horizontal_next;
    struct _Playable_Node *vertical_next;
} PlayableNode;

typedef struct _TreeNode {
    int x, y;
    int hasTent;
    int num_playables;
    struct _Node *North;
    struct _Node *South;
    struct _Node *West;
    struct _Node *East;
} TreeNode;


HeadNode *row_vector = NULL, *column_vector = NULL;

void setSolverVectors(HeadNode* row, HeadNode* column) {
    row_vector = row;
    column_vector = column;
}
