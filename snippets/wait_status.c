#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int rc = fork();
  if (rc == 0) {
    printf("here\n");
  } else {
    int status;
    wait(&status);
    printf("%d\n", WEXITSTATUS(status));
  }
}

// wait() also reports how the child ended.
// The child's exit code is retrieved with WEXITSTATUS.
// Output: child exited with 3
