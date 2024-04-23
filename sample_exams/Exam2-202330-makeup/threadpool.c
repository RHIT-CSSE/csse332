#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "prettyprint.h"

#define NUM_THREADS 10

const char *stack = "abcdefghijklmnopqrstuvwxyz123456789";
int stackptr = 0;

char pop()
{
  if(stackptr >= strlen(stack)) {
    fprintf(stderr,
            "Buffer overflow when reading the stack. Fix your code!\n");
    return 0;
  }

  return stack[stackptr++];
}

int is_empty()
{
  return (stackptr >= strlen(stack));
}

// TODO: Add any extra functions you need here

int main(int argc, char **argv)
{
  /* TODO: YOUR IMPLEMENTATION GOES HERE */

  /* DO NOT REMOVE THIS LINE */
  printf("****** Everything finished ******\n");
}
