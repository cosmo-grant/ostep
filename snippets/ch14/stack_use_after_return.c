#include <stdio.h>
#include <stdlib.h>

int *foo() {
  int x = 123;
  printf("%d\n", x);
  return &x;
}

int main(int argc, char *argv[]) {
  int *p = foo();
  printf("%d\n", *p);
}

/*
 * Undefined behaviour: stack-use-after-return.
 * When func returns, its stack memory is marked as available for re-use, not
 * wiped. So the print in main MAY (but doesn't have to) print 1. Results may
 * vary by platform, compiler, options, ...
 */
