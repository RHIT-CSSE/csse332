#include <stdio.h>
#include <stdlib.h>

void b_func();
void c_func();

void a_func() {
    printf("a");
}

void main() {
    a_func();
    b_func();
    c_func();
    printf("\nprogram done\n");
    exit(0);
}
