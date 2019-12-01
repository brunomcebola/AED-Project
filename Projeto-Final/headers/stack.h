struct _coord {
    int x;
    int y;
};


typedef union {
    struct _coord coord;
    TreeNode *T;
    HeadNode *H;
} changedNode;

typedef struct _changeStore {
    struct _changeStore *prevChange;
    changedNode changedNode;
    int changeNodeSelector;
    int previousValue;
    char previousChar;
    int valueID;
} changeStore;

/* TODO: include struct prototypes so stack.c can edit them */

void pushChange(changeStore **, void *, int, int, int , int , int );
void deleteChanges(changeStore *, char *, int);
