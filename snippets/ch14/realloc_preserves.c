#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  int *x = malloc(4 * sizeof(int));
  for (int i = 0; i < 4; i++)
    x[i] = 100 + i;

  int *y = realloc(x, 1000 * sizeof(int));
  for (int i = 0; i < 4; i++)
    printf("%d ", y[i]);
  printf("\n");
}

/*
 * Always prints 100 101 102 103: realloc preserves contents up to the smaller
 * of the old and new sizes.
 *
 * y[4..1000) is uninitialised, not zeroed, so realloc is not calloc.
 *
 * realloc may satisfy the request in place or move the block, so y may or may
 * not equal x. Which one happens is allocator-dependent, hence not printed
 * here. The rule that follows is unconditional: always use the returned
 * pointer, and treat x as dangling once realloc succeeds.
 *
 * realloc(NULL, n) is defined to be exactly malloc(n).
 */
