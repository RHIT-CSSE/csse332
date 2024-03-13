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
#define NUM_CHILDREN 10

int main(int argc, char **argv) {
  char* msg = argv[1];
  int readpipe[2];
  int writepipe[2];
  pipe(readpipe);
  pipe(writepipe);

  int r = fork();
  if(r == 0){
    char pipe0[20];
    char pipe1[20];
    char pipe2[20];
    char pipe3[20];
    snprintf(pipe0, 20, "%d", writepipe[0]);
    snprintf(pipe1, 20, "%d", writepipe[1]);
    snprintf(pipe2, 20, "%d", readpipe[0]);
    snprintf(pipe3, 20, "%d", readpipe[1]);
    execlp("./buffalopipe.bin", "./buffalopipe.bin", pipe0, pipe1, pipe2, pipe3, NULL);
    perror("buffalosay.bin failed to run");
    exit(1);
  }

  close(writepipe[0]);
  close(readpipe[1]);

  write(writepipe[1], msg, BUFSIZE);

  char writebuffer[BUFSIZE];
  strcpy(writebuffer, "goodbye world");
  write(writepipe[1], writebuffer, BUFSIZE);
  
  close(writepipe[1]);

  char readbuffer[BUFSIZE];
  read(readpipe[0], readbuffer, BUFSIZE);
  int status;
  wait(&status);

  printf("%s", readbuffer);

  printf("Exited with: %u\n", WEXITSTATUS(status));

  exit(0);
}

