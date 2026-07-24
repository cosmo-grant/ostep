#include <stdio.h>
#include <stdlib.h>

int *foo() {
  static int x = 123;
  printf("%d\n", x);
  return &x;
}

void bar() {
  int y = 456;
  printf("%d\n", y);
}

int main(int argc, char *argv[]) {
  int *p = foo();
  bar();
  printf("%d\n", *p);
}

/*
 * x is a static local: static storage duration, so it outlives foo returning
 * (unlike an automatic local, cf. stack_use_after_return.c).
 * *p reliably prints 123.
 *
 * But static buys only lifetime: x is a single shared instance, fixed size,
 * living for the whole program. For dynamic quantity/size, a lifetime you
 * control, or a fresh instance per call, use the heap (cf.
 * heap_use_after_return.c).
 */
