#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (fork() == 0) {
    printf("foo\n");
  } else {
    wait(NULL);
    printf("bar\n");
  }
}

// wait() makes the parent block until the child exits.
// Output is always "child" then "parent", regardless of scheduling.
