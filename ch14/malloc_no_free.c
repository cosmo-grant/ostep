#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int* p = malloc(sizeof(int));
  printf("%p\n", p);
}

// No problems reported by gdb that I can see
// valgrind not happy: definitely lost 4 bytes in 1 blocks
