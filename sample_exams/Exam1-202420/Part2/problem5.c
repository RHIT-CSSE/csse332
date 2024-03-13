#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

// Problem 5
//
// Name: 

// IMPORTANT: buffer size for use with ALL reads and writes
#define BUFSIZE 1000

int main(int argc, char **argv) {

  printf("Parent %d finished ....\n", getpid());
  exit(0);
}

