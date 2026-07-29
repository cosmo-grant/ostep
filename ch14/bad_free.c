#include <stdlib.h>

int main(int argc, char *argv[]) {
  int *data = malloc(100 * sizeof(int));
  free(data + 100);
}

// compiles with warning; fails at runtime
// free(): invalid pointer
// zsh: IOT instruction (core dumped)  ./a.out
