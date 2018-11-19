/* createChildAndWait.c 

   This program creates a child process using the fork() system call. 

   In theory, every time the program is executed, the behavior of the
   program could be different.  The execution of the two processes
   could be interleaved, and in general it is not possible to predict
   how.  However, in practice, one of the processes will always run to
   completion before the other one is allocated to the processor
   because both processes run very quickly.

   It is possible to make the parent wait for the child process using
   the "wait" or "waitpid" system call.  The child process gets a copy 
   of the parent's image. It is possible to overlay the image of the child
   process with another image using the "exec" family of system calls.  
   A process can use the "getpid" function to get its process id.  

   modified: Delvin Defoe, March 20, 2008
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
  pid_t fork_pid, my_pid;
  int x;
  int stat_loc;

  if (argc != 3) {
    printf("./createAndWait <source-file> <destination-file>\n");
    exit(-1);
  }

  fork_pid = fork();   /* Create a child process */
  if (fork_pid < 0) {
    fprintf(stderr, "Fork failed\n");
    exit(-1);
  }

  my_pid = getpid();

  if (fork_pid == 0) { /* If the current process is the child process.*/
    printf("I am the child process.  fork_pid = %d, my_pid = %d\n",
           fork_pid, my_pid);

    execlp("./myCopy", "myCopy", argv[1], argv[2], NULL);
  } else { /* If the current process is the parent process */
    printf("I am the parent process.  fork_pid = %d, my_pid = %d\n",
           fork_pid, my_pid);

    /* wait(NULL); - Use NULL if you do not want to examine the
       value returned by the child process. 
       
       wait() is a blocking call.  You can use waitpid() to exercise
       more options and wait on specific pids.
    */    
    wait(&stat_loc);

    if (WEXITSTATUS(stat_loc)) {  /* "man wait" for more info */
      printf(" The value returned by the child process is %d\n",
             WEXITSTATUS(stat_loc));
      printf("Program returned with an   error ....\n");
    }
  }

  srand(my_pid);
  for (x=0; x < 10000; x++) {
    if (rand()%2 == 0) {
      sleep(0.01);
    }
  }

  printf("Now exiting -- pid %d over and out.\n", my_pid);

  return 0;
}
