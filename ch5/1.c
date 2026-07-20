#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char *argv[]) {
  int x = 100;
  int rc = fork();
  if (rc < 0) {
    // fork failed
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) {
    // child
    printf("child: x is %d\n", x);
    x = 200;
    printf("child: x is %d\n", x);
  } else {
    // parent
    printf("parent: x is %d\n", x);
    x = 300;
    printf("parent: x is %d\n", x);
  }
  return 0;
}

// % ./1                                                          +
// parent: x is 100
// parent: x is 300
// child: x is 100
// child: x is 200
//
// This makes sense.
// The forked process gets its own address space, initially a copy of the parent's.
// The address space includes the stack, where x lives (in main's stack frame).
