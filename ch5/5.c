#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main (int argc, char *argv[]) {
  int rc = fork();
  if (rc < 0) {
    // fork failed
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) {
    // child
    pid_t y = wait(NULL);
    printf("%d\n", y);  // -1
    printf("in child\n");
  } else {
    // parent
    pid_t x = wait(NULL); // so child runs first
    printf("%d\n", x);
    printf("in parent\n");
  }
  return 0;
}

/*
wait(): on success, returns the process ID of the terminated child; on failure, -1 is returned.
Calling wait() in child returns -1.
So I guess calling wait() in a process with no children is treated as a failure.
*/


