#include <stdio.h>
#include <unistd.h>
#include "hybrid_threads.h"

void fun1() {
  for(int i = 0; i < 40; i++) {
    printf("ONE\n");
    usleep(20);

  }
  printf("fun1 done\n");
}
void fun2() {
  for(int i = 0; i < 40; i++) {
    printf("TWO\n");
    usleep(20);
  }
  printf("fun2 done\n");
}

int main(int argc, char** argv) {
  printf("starting\n");
  initialize_basic_threads();
  create_new_thread(fun1);
  create_new_thread(fun2);
  schedule_hybrid_threads(2);
  printf("done\n");
}
