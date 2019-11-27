#include <stdlib.h>
#include "../headers/solver.h"

struct _PlayableNode {
    int x, y;
    int valid;
    struct PlayableNode *horizontal_next;
    struct PlayableNode *vertical_next;
};

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

HeadNode *getSolverVectorRow(void) {
    return row_vector;
}

HeadNode *getSolverVectorColumn(void) {
    return column_vector;
}

void setSolverVectors(HeadNode* row, HeadNode* column) {
    row_vector = row;
    column_vector = column;
}

void freeSolver(void) {
    free(row_vector);
    free(column_vector);
}
