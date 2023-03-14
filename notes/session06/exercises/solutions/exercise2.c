#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
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
  int fd[2];
  pid_t pid;
  int child_pid = -1, nbytes = 0;

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

    // write the 4 bytes of the process id as a 4 bytes character array
    child_pid = getpid();
    printf("Child: Sending my pid %d to my parent\n", child_pid);
    write(fd[1], (char*)&child_pid, 4);

    // don't forget to close the pipe end
    close(fd[1]);

    // leave
    exit(EXIT_SUCCESS);
  } else {
    // parent
    close(fd[1]);

    // read 4 bytes from the child
    if((nbytes = read(fd[0], (char*)&child_pid, 4)) != 4) {
      perror("PANIC: Could not read 4 bytes from the pipe");
      close(fd[0]);
      exit(EXIT_FAILURE);
    }

    assert(child_pid == pid);
    printf("[Parent %d]: The child's pid is %d\n", getpid(), child_pid);

    // don't forget to close the reading end
    close(fd[0]);

    // leave
    exit(EXIT_SUCCESS);
  }
}
