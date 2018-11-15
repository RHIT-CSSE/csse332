#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
  int pid = fork();

  if (pid < 0) {
    printf("Fork failed.\n");
    return 1;  // nonzero means a failure result in unix
  } else if (pid == 0) {
    printf("I am the child process\n");
    sleep(1);  // wait 1 second
  } else {
    printf("I am the parent process\n");
    wait(NULL);
    printf("Child complete\n");
  }
  printf("exiting.\n");
  return 0;
}
