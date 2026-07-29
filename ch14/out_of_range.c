#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  int *data = malloc(100 * sizeof(int));
  data[100] = 0;
  printf("%d\n", data[100]);
  free(data);
}

// gdb says exited normally and shows output 0
// valgrind says 2 errors in 2 contexts
