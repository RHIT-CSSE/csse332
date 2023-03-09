#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

// STEP 1:
// Look at the below piece of code and try to answer the question in the printf
// on the line right before the exit call, namely, who will print the line?

// STEP 2:
// Adjust the below code so that the printf is only printed by the parent, but
// only AFTER the child has done executing.

static int global_var = 10;

int
main(int argc, char **argv)
{
  int rc;

  rc = fork();
  if(rc < 0) {
    perror("Failed to fork a process");
    exit(EXIT_FAILURE);
  }

  if(rc == 0) {
    /* child process */
    global_var = 100;
    printf("%d: My value of the global variable is %d\n", getpid(), global_var);
  } else {
    /* parent process */
    global_var = 200;
    printf("%d: My value of the global variable is %d\n", getpid(), global_var);
  }

  printf("Who will print this line? %d.\n", getpid());
  exit(EXIT_SUCCESS);
}

