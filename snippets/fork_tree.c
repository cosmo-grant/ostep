#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  fork();
  fork();
  printf("hello\n");
}

// Each fork() doubles the number of processes: n forks give 2^n.
// After two forks there are 4 processes, so "hello" prints 4 times.
// Order is irrelevant here because all four lines are identical.
