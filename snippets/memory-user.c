#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  int n = atoi(argv[1]) * 1024 * 1024 / sizeof(int);
  int *a = malloc(n * sizeof(int));
  while (1)
    for (int i = 0; i < n; i++)
      a[i] += 1;
}
