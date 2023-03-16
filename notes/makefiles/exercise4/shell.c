#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
  if(fork() == 0) {
    execlp("./donothing.bin", "./donothing.bin", NULL);
    perror("exec failed");
    exit(1);
  }

  wait(0);
  printf("Goodbye world!\n");
  exit(0);
}
