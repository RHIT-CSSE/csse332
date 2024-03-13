#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

// Problem 2
//
// Name: 


// Default buffer size for use with snprintf
#define BUFSIZE 128
#define NUM_CHILDREN 10

int main(int argc, char **argv) {

  printf("Parent %d finished ....\n", getpid());
  exit(0);
}

