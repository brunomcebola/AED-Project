#include <stdio.h>
#include <stdlib.h>

void checkNull(void *pointer) {
    if(pointer == NULL) {
        exit(0);
    }
}

int main(int argc, char const *argv[]) {
    FILE *fp = NULL;
    char **board = NULL, aux = '\0';
    int l = 0, c = 0;

    //check if two arguments are passed
    if (argc != 2) {
        exit(0);
    }

    fp = fopen(argv[1], "r");
    checkNull(fp);

    fscanf(fp, "%d %d", &l, &c);
    board = (char **) malloc(l * sizeof(char *));
    checkNull(board);
    for(int i = 0; i < l; i++) {
        board[l] = (char *) malloc(c * sizeof(char));
        checkNull(board[l]);
    }

    while(feof(fp)) {
        fscanf(fp, "%c", &aux);

    }
    fclose(fp);

    return 0;
}
