#include <stdio.h>
#include <unistd.h>

int x = 111;

int main(int argc, char *argv[]) {
  if (argc == 1) {
    x = 222;
    execl(argv[0], argv[0], "again", (char *)NULL);
  }
  printf("%d\n", x);
}

// exec() throws away the current address space and loads a fresh program image.
// Same program, same global, same initializer: the only thing between setting
// shared = 999 and reading it back is the exec. So 42 proves the address space
// was re-initialized; the 999 lived only in the old, now-discarded image.
// (Relies on argv[0] being a usable path, i.e. run it as ./exec_reinit.)
