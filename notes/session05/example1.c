#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#include "setsighandler.h"

int ouch_count = 0;

// TODO: Add your code here.

int main(int argc, char **argv)
{
  int i;

  for(i = 0; i < 6; i++) {
    printf("Hello from iteration %d\n", i);
    sleep(1);
  }

  printf("Exiting naturally\n");
  exit(0);
}
