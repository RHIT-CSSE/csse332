#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "setsighandler.h"

int main(int argc, char **argv)
{
  printf("Hello from process %u\n", getpid());

  printf("Sending the stop signal to myself\n");
  kill(getpid(), SIGSTOP);

  printf("Recovered from the stop signal, continuing...\n");

  printf("Waiting for you to terminate\n");
  while(1);

  exit(0);
}
