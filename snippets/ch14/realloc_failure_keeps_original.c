#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  int *x = malloc(4 * sizeof(int));
  x[0] = 123;

  int *saved = x;
  x = realloc(x, SIZE_MAX);

  printf("realloc: %s\n", x ? "succeeded" : "failed");
  printf("original still holds %d\n", saved[0]);
}

/*
 * Always prints "failed" then 123 (cf. malloc_null.c for the failing size).
 *
 * On failure realloc returns NULL and leaves the original block untouched,
 * which is why saved is still valid to read here. It would be a dangling
 * pointer had the call succeeded.
 *
 * So "x = realloc(x, n)" leaks whenever it fails: NULL overwrites the only
 * pointer to a block that is still allocated. The safe idiom keeps the old
 * pointer until the new one is known good:
 *
 *   int *tmp = realloc(x, n);
 *   if (tmp) x = tmp;
 */
