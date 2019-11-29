typedef struct _changeStore changeStore;

/* TODO: include struct prototypes so stack.c can edit them */

void pushChange(changeStore *, void *, int , int , int );
void deleteChanges(changeStore *);
