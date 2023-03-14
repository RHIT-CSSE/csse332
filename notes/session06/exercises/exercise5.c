#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
  int fd[2];
  pid_t pid;
  int wstatus;
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
    printf("[Child %d] Done writing the 5 characters\n", getpid());

    // leave
    exit(100);
  } else {
    // parent
    close(fd[1]);

    // BUG: changed my mind and don't want to read anymore
    printf("[Parent %d] Changed my mind and won't do any reading\n", getpid());
    close(fd[0]);

    // wait for my child
    wait(&wstatus);
    if(!WIFEXITED(wstatus)) {
      printf("[Parent %d] Something bad happened to my child\n", getpid());
      exit(EXIT_FAILURE);
    } else if (WEXITSTATUS(wstatus) == 100) {
      printf("[Parent %d] My child exited happily\n", getpid());
    }

    // leave
    exit(EXIT_SUCCESS);
  }
}
