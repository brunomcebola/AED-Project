#include <stdio.h>
#include <stdlib.h>

#include "bundle.h"

void checkNull(void *pointer) {
    if(pointer == NULL) {
        exit(0);
    }
}
