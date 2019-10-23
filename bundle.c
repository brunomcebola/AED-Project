#include <stdio.h>
#include <stdlib.h>

void checkNull(void *pointer) {
    if(pointer == NULL) {
        exit(0);
    }
}
