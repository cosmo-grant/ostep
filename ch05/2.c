#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main (int argc, char *argv[]) {
  int fd = open("./2.testfile", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
  int rc = fork();
  if (rc < 0) {
    // fork failed
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) {
    // child
    write(fd, "child\n", 6);
  } else {
    // parent
    write(fd, "parent\n", 7);
  }
  return 0;
}


/*
% repeat 5 ./2 && cat 2.testfile
parent
child
parent
child
parent
child
parent
child
parent
child

So parent and child can both write to the file opened in the parent.
That means fork copies the file descriptor table too, pointing at same file.
Just one file, with one offset, so no clobbering.
I'm surprised the output order is consistent.
Claude says that's chance: in general, child could write before parent.
*/
