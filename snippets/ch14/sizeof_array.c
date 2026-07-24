#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  int x[10];
  printf("%lu\n", sizeof(x));
}

/*
 * x is an array of 10 ints, i.e. 40 bytes.
 * In most expressions the name x decays to a pointer, but not in sizeof(x).
 */
