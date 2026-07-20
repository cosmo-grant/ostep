#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int fd = open("./my_file", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
  int rc = fork();
  if (rc == 0) {
    write(fd, "hello\n", 6);
  } else {
    wait(NULL);
    write(fd, "goodbye\n", 8);

    char buf[64];
    int rfd = open("./my_file", O_RDONLY);
    int n = read(rfd, buf, sizeof(buf));
    write(1, buf, n);
  }
}

// fork() shares the open fd and its file offset between parent and child.
// So the child's "hello\n" and the parent's "goodbye\n" don't overwrite;
// they land one after the other. Output:
//   hello
//   goodbye
