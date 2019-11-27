#ifndef defs_h
#define defs_h

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

typedef struct {
    int puzzleTents;
    int tentsNeeded;
    PlayableNode *first;
} HeadNode;


typedef struct {
    int n_rows, n_columns;
    int *n_el_row, *n_el_column;
    char *buffer;
    char *layout;
    HeadNode *horizontals;
    HeadNode *verticals;
    int answer;
    int sum;
    int season;
} board;

#endif
