#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  size_t n = (size_t)atoi(argv[1]) * 1024 * 1024 / sizeof(int);
  int *a = malloc(n * sizeof(int));
  while (1)
    for (size_t i = 0; i < n; i++)
      a[i] += 1;
}

/*
* free shows that the memory isn't used immediately.
* It takes a second or two, presumably because only becomes used when touched.
* It becomes available immediately on killing the program though.
*
* Allocating 1000 Mb reduced MemFree by 1009, not 1000.
* The rest of the program takes up some space.
*
* The numbers reported by free vary a bit (a few mb) even when doing nothing.
*/ 
