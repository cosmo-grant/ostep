#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  int *x = calloc(10, sizeof(int));
  for (int i = 0; i < 10; i++)
    printf("%d ", x[i]);
  printf("\n");
}

/*
 * Always prints ten 0s: calloc zeroes the memory it allocates.
 *
 * malloc allocates without initialising, so reading a fresh malloc block is an
 * uninitialized read. There is deliberately no snippet for that: what it
 * prints depends on the allocator, so it can't be demonstrated reproducibly.
 * Fresh pages from the kernel arrive zero-filled, so a first malloc often
 * looks zeroed by accident, which is exactly what makes the bug dangerous.
 */
