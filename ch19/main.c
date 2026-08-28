#include <stdio.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[]) {
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  printf("%ld, %ld\n", ts.tv_sec, ts.tv_nsec);
}
