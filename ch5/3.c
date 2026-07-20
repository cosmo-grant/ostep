#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main (int argc, char *argv[]) {
  int fd = open("./3.testfile", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
  int rc = fork();
  if (rc < 0) {
    // fork failed
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) {
    // child
    write(fd, "hello\n", 6);
  } else {
    // parent
    // wait(NULL);  // waits for state change in child
    sleep(1);  // another way to make child run first
    write(fd, "goodbye\n", 8);
  }
  return 0;
}
