#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#include "setsighandler.h"

int ouch_count = 0;

// TODO: Add your code here.
void ouch(int sig)
{
  ouch_count++;

  switch(ouch_count) {
  case 1:
    printf("Ouch! - I got signal %d. That hurt.\n", sig);
    break;
  case 2:
    printf("Outch! - I got signal %d. I'm nearly dead.\n", sig);
    break;
  case 3:
  default:
    printf("Goodbye cruel world\n");
    exit(0);
  }
}

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
