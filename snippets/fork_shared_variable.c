#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int x = 1;
  int rc = fork();
  if (rc == 0) {
    printf("%d\n", x);
    x = 2;
    printf("%d\n", x);
  } else {
    wait(NULL);
    printf("%d\n", x);
    x = 3;
    printf("%d\n", x);
  }
}

// % ./1                                                          +
// parent: x is 100
// parent: x is 300
// child: x is 100
// child: x is 200
//
// This makes sense.
// The forked process gets its own address space, initially a copy of the
// parent's. The address space includes the stack, where x lives (in main's
// stack frame).
