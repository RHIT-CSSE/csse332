#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

// Problem 3
//
// Name: 


// Default buffer size for use with snprintf
#define BUFSIZE 128
#define NUM_CHILDREN 10

// setsighandler()
void setsighandler(int signum, void (*handler)(int))
{
  struct sigaction act;

  act.sa_handler = handler;
  sigemptyset(&act.sa_mask);
  act.sa_flags = SA_RESTART;
  sigaction(signum, &act, NULL);
}

int main(int argc, char **argv) {

  printf("Parent %d finished ....\n", getpid());
  exit(0);
}

