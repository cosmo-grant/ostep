#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int* x = NULL;
  printf("%d\n", *x);
}

/*
 * run: segmentation fault 
 * run with gdb:
 *
 *
 * Program received signal SIGSEGV, Segmentation fault.
 * 0x0000555555555168 in main (argc=1, argv=0x7fffffffd488)
 * at ch14/null.c:7
 * 7	 printf("%d\n", *x);
*/
