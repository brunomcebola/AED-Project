#include "stdio.h"
#include "stdlib.h"

int main(int argc, char const *argv[]) {
  int **matriz = NULL;
  char c;
  long long int tendas = 0;
  matriz = (int **) calloc(5000, sizeof(int *));
  FILE *fptr1 = NULL, *fptr2 = NULL;
  fptr1 = fopen("abc.txt", "w");
  for (size_t i = 0; i < 5000; i++) {
    matriz[i] = (int *) calloc(5000, sizeof(int));
  }
  for (size_t i = 0; i < 5000; i++) {
    for (size_t j = 0; j < 5000; j++) {
      matriz[j][i] ='T';
    }
  }
  for (size_t i = 0; i < 5000; i++) {
    for (size_t j = 0; j < 5000; j++) {
      if (matriz[i][j] == 'T') tendas++;
      fputc(matriz[i][j], fptr1);
    }
  }
  for (size_t i = 0; i < 5000; i++) {
    for (size_t j = 0; j < 5000; j++) {
      if (matriz[j][i] == 'T') tendas++;
    }
  }
  fclose(fptr1);
  fptr1 = fopen("abc.txt", "r");
  fptr2 = fopen("aaa.txt", "w");
  while ((c=fgetc(fptr1)) != EOF) {
    fputc(c, fptr2);
  }

  printf("%lld\n", tendas);
  fclose(fptr1);
  fclose(fptr2);
  for (size_t i = 0; i < 5000; i++) {
    free(matriz[i]);
  }
  free(matriz);
  remove("abc.txt");
  remove("aaa.txt");
  return 0;
}
