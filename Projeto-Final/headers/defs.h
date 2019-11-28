#ifndef defs_h
#define defs_h

typedef struct _Playable_Node {
    int x, y;
    int valid;
    struct _Playable_Node *horizontal_next;
    struct _Playable_Node *vertical_next;
    struct _Playable_Node *horizontal_prev;
    struct _Playable_Node *vertical_prev;
} PlayableNode;

typedef struct _TreeNode {
    int x, y;
    int hasTent;
    int num_playables;
    int North, South, East, West;
    struct _TreeNode *next;
} TreeNode;

typedef struct {
    int puzzleTents;
    int tentsNeeded;
    PlayableNode *first;
} HeadNode;


#endif
