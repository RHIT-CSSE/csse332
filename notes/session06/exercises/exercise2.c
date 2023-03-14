#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// TODO:
//
//  In this exercise, we would like to child to send its process id back to its
//  parent. Yes we can get it from the return value of fork, but we'd like to
//  have a backup way for getting it as well.
//
// HINT 1: You might find the functions sprintf, snprintf, and atoi very
// helpful.
//
// HINT 2: Alternatively, remember that everything is just a bunch of bytes. How
// many bytes is an integer?
//

int main(int argc, char **argv)
{
  pid_t pid;

  pid = fork();
  if(pid < 0) {
    perror("PANIC: fork failed");
    exit(EXIT_FAILURE);
  }

  if(pid == 0) {
    // child

    // leave
    exit(EXIT_SUCCESS);
  } else {
    // parent


    // leave
    exit(EXIT_SUCCESS);
  }
}
