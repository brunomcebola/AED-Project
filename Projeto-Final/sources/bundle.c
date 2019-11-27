#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "../headers/bundle.h"

void checkNull(int num, ...) {
    va_list valist;
    int i;
    va_start(valist, num);

    for (i = 0; i < num; i++) {
      if(va_arg(valist, void*) == NULL){
          exit(0);
      }
   }

   va_end(valist);
}
