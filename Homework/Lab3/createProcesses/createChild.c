/* createChild.c 

   This program creates a child process using the fork() system call. 

   In theory, every time the program is executed, the behavior of the
   program could be different.  The execution of the two processes
   could be interleaved, and in general it is not possible to predict
   how.  However, in practice, one of the processes will always run to
   completion before the other one is allocated to the processor
   because both processes run very quickly.

   modified: Delvin Defoe, March 20, 2008
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
  pid_t fork_pid, my_pid;
  int x;

  fork_pid = fork();   /* Create a child process */
  if (fork_pid < 0) {
    fprintf(stderr, "Fork failed\n");
    exit(2);
  }

  my_pid = getpid();

  if (fork_pid == 0) { /* If the current process is the child process.*/
    printf("I am the child process.  fork_pid = %d, my_pid = %d\n",
           fork_pid, my_pid);
  } else { /* If the current   process is the parent process. */
    printf("I am the parent process.  fork_pid = %d, my_pid = %d\n",
           fork_pid, my_pid);
  }

  /* Adding some random delay to make the process run a little longer. */
  srand(my_pid);
  for (x=0; x < 10000; x++) {
    if (rand()%2 == 0) {
      sleep(0.01);
    }
  }

  printf("Now exiting -- pid %d over and out.\n", my_pid);

  return 0;
}
