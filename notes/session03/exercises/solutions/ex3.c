#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

void child(void)
{
  int *p = 0;
  int coin = rand() % 10;

  sleep(2);
  printf("Child %d: Rolled a weird dice and outcome is %d\n", getpid(), coin);
  if(coin < 3) {
    // passed the exam
    exit(0);
  } else if (coin < 7) {
    // failed the exam
    exit(99);
  } else {
    // get hit by a bus?
    *p = 3;
  }
}

void parent(int rc)
{
  // TODO:
  // Add code here for the parent to check on the status of its child and
  // print a message that indicates what happened to the child when it or if it
  // exited.

  int status = -1;

  wait(&status);
  if(!WIFEXITED(status)) {
    printf("Parent %d: My child %d has exited with error\n", getpid(), rc);
  } else if(!WEXITSTATUS(status)) {
    printf("Parent %d: My child %d has passed the exam\n", getpid(), rc);
  } else if(WEXITSTATUS(status) == 99) {
    printf("Parent %d: My child %d has failed the exam\n", getpid(), rc);
  } else {
    printf("Parent %d: huh?\n", getpid());
  }
}

int
main(int argc, char **argv)
{
  int rc;

  srand(time(NULL));
  rc = fork();
  if(rc < 0) {
    perror("Could not fork correctly.");
    exit(EXIT_FAILURE);
  }

  if(rc == 0) child();
  else parent(rc);

  exit(EXIT_SUCCESS);
}

