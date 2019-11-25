#include <stdio.h>
#include <stdlib.h>

#include "../headers/bundle.h"

void checkNull(void *pointer) {
    if(pointer == NULL) {
        exit(0);
    }
}
