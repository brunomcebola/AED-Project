#include <stdlib.h>
#include <stdio.h>

typedef struct _Playable_Node {
  int x, y;
  int valid;
  struct _Node *horizontal;
  struct _Node *vertical;
}Playable_Node;


typedef struct _TreeNode {
  int x, y;
  int hasTent;
  int num_playables;
  struct _Node *North;
  struct _Node *South;
  struct _Node *West;
  struct _Node *East;
}TreeNode;
