#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  int x = 1;
  if (fork() == 0) {
    x = 2;
    printf("x=%d at %p\n", x, (void *)&x);
  } else {
    wait(NULL);
    printf("x=%d at %p\n", x, (void *)&x);
  }
}
