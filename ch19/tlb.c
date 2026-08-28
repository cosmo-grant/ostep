
#include <stdio.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[]) {
  int jump = PAGESIZE / sizeof(int);
  for (i = 0; i < NUMPAGES * jump; i += jump)
    a[i] += 1;
}
