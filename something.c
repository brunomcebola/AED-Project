#include "stdio.h"
#include "stdlib.h"

int main(int argc, char const *argv[]) {
  int i=5;
  do {
    if (i > 0) {
      printf("%d\n", i);
      continue;
    }
  } while (--i);
}
