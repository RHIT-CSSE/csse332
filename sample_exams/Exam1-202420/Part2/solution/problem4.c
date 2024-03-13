#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

// Problem 4
//
// Name: 


// IMPORTANT: buffer size for use with ALL reads and writes
#define BUFSIZE 1000

int main(int argc, char **argv) {
  char* msg = argv[1];
  int writepipe[2];
  pipe(writepipe);

  int r = fork();
  if(r == 0){
    char pipe0[20];
    char pipe1[20];
    snprintf(pipe0, 20, "%d", writepipe[0]);
    snprintf(pipe1, 20, "%d", writepipe[1]);
    execlp("./buffalosay.bin", "./buffalosay.bin", pipe0, pipe1, NULL);
    perror("buffalosay.bin failed to run");
    exit(1);
  }

  close(writepipe[0]);

  char writebuffer[BUFSIZE];

  strcpy(writebuffer, "secret handshake");
  write(writepipe[1], writebuffer, BUFSIZE);
  write(writepipe[1], msg, BUFSIZE);
  close(writepipe[1]);

  int status;
  wait(&status);

  printf("Exited with: %u\n", WEXITSTATUS(status));

  exit(0);
}

