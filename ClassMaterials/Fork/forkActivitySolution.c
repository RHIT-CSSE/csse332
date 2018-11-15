#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
  int i;
  for (i = 0; i < 10; i++) {
    int pid = fork();

    if (pid < 0) {
      printf("Fork failed.\n");
      return 1;  // nonzero means a failure result in unix
    } else if (pid == 0) {
      printf("I am the child process\n");
      sleep(1);
      printf("Child process exiting\n");
      return 0;
    }
  }
  for (i = 0; i < 10; i++) {
    wait(NULL);
  }
  printf("All children complete\n");
  printf("exiting.\n");
  return 0;
}
