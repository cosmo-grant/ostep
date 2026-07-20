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
    char *myargs[3];
    myargs[0] = strdup("ls");
    myargs[1] = strdup(".");
    myargs[2] = NULL;
    execvp(myargs[0], myargs);
    printf("this won't print\n");
  } else {
    // parent
    printf("in parent\n");
  }
  return 0;
}
