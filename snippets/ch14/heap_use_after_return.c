#include <stdio.h>
#include <stdlib.h>

int *foo() {
  int *x = (int *)malloc(sizeof(int));
  *x = 123;
  printf("%d\n", *x);
  return x;
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
 * x lives on the heap, so outlives foo returning.
 * *x still points to the right value.
 */
