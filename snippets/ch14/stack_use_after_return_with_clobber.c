#include <stdio.h>
#include <stdlib.h>

int *foo() {
  int x = 123;
  return &x;
}

void bar() {
  int y = 456;
}

int main(int argc, char *argv[]) {
  int *p = foo();
  bar();
  printf("%d\n", *p);
}
