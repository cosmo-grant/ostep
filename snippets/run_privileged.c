#include <stdio.h>

int main(void) {
    printf("before\n");
    fflush(stdout);
    asm volatile("mrs x0, sctlr_el1");  // privileged instruction
    printf("after\n");
}
