#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  int *x = malloc(SIZE_MAX);
  printf("%p\n", (void *)x);
}

/*
 * Prints 0x0 / null, because malloc was unsuccessful (requested size too
 * large.)
 */
