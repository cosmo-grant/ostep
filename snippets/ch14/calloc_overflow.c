#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  size_t n = ((size_t)1 << 62) + 1;

  int *a = malloc(n * sizeof(int));
  int *b = calloc(n, sizeof(int));

  printf("malloc: %s\n", a ? "succeeded" : "failed");
  printf("calloc: %s\n", b ? "succeeded" : "failed");
}

/*
 * Asking for the same absurd number of ints two ways. Assumes 64-bit size_t.
 *
 * n * sizeof(int) is n * 4 = 2^64 + 4, which wraps (defined behaviour for
 * unsigned types) to 4. So malloc quietly hands back a 4-byte block, and
 * writing a[0..n) would run off the end of it.
 *
 * calloc takes the count and the element size as separate arguments, so it can
 * check the product for overflow, and refuses.
 */
