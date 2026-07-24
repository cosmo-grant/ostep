#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  int *x = malloc(10 * sizeof(int));
  printf("%lu\n", sizeof(x));
}

/*
 * We allocated space for an array of 10 ints, i.e. 40 bytes.
 * But sizeof(x) is the size of the returned pointer, not the allocated space.
 */
