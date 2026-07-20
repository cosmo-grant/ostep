#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main (int argc, char *argv[]) {
  int rc = fork();
  if (rc == 0) {
    char *myargs[3];
    myargs[0] = strdup("echo");
    myargs[1] = strdup("this");
    myargs[2] = NULL;
    execvp(myargs[0], myargs);
    printf("here\n");
  }
}
