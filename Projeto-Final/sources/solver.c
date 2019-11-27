#include <stdlib.h>
#include "../headers/solver.h"

struct _PlayableNode {
    int x, y;
    int valid;
    PlayableNode *horizontal_next;
    PlayableNode *vertical_next;
};

typedef struct _TreeNode {
    int x, y;
    int hasTent;
    int num_playables;
    PlayableNode *North;
    PlayableNode *South;
    PlayableNode *West;
    PlayableNode *East;
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
