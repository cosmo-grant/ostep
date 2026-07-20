#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  printf("hi\n");
  fork();
  printf("bye\n");
}

// fork() duplicates the calling process,
// and returns the child's pid in the parent and 0 in the child.
