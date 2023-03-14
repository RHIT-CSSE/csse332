#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

//
// TODO:
//  This code contains a small bug. What we want to happen is for the child to
//  send some characters to the parent, and then after that, we want both parent
//  and child to enter an infinite loop.
//
//  STEP 1: Run this and examine the bug.
//
//  STEP 2: Fix the problem.
//


int main(int argc, char **argv)
{
  int fd[2];
  pid_t pid;
  int nbytes;
  char readbuff[128];
  const char *msg = "My name is Aloy";
  int i;

  if(pipe(fd) < 0) {
    perror("PANIC: pipe failed");
    exit(EXIT_FAILURE);
  }

  pid = fork();
  if(pid < 0) {
    perror("PANIC: fork failed");
    exit(EXIT_FAILURE);
  }

  if(pid == 0) {
    // child
    close(fd[0]);

    // write 5 characters
    for(i = 0; i < 5; i++) {
      write(fd[1], "a", 1);
    }

    close(fd[1]);
    printf("[Child %d] Done writing the 5 characters\n", getpid());
    while(1);

    // leave
    exit(EXIT_SUCCESS);
  } else {
    // parent
    close(fd[1]);

    // read as many characters as the child can send
    while((nbytes = read(fd[0], readbuff, 1)) > 0);

    close(fd[0]);
    printf("[Parent %d] Done reading and now doing other stuff\n", getpid());
    while(1);

    // leave
    exit(EXIT_SUCCESS);
  }
}
