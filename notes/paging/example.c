#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define NB 5

int main(int argc, char **argv)
{
  char *p = 0;
  int content = 3;

  sleep(1);
  if(argc > 1)
    content = atoi(argv[1]);

  if(fork() == 0) {
    p = malloc(NB);
    memset(p, content, NB);
    printf("(Process %d): p contains %p and points to %x\n", getpid(), p,
           *(int*)p);

    free(p);
    exit(0);
  }

  p = malloc(NB);
  memset(p, content+1, NB);
  printf("(Process %d): p contains %p and points to %x\n", getpid(), p,
         *(int*)p);

  free(p);
  exit(0);
}
