#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int rc = fork();
  if (rc == 0) {
    printf("foo");
  } else {
    wait(NULL);
    printf("parent: fork returned child's pid (> 0)\n");
  }
}

// fork() returns TWICE: 0 in the child, the child's pid in the parent.
// This is how the two processes tell themselves apart.
// (wait() is only here to keep the two lines in a fixed order.)
