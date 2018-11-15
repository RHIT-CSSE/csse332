#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

/*
  Write a process that forks 10 subprocesses, then waits for them all
  to complete, prints a message, then exits.

  The subprocesses should print a message, sleep for 1 second, then exit.

  If you do it correctly the output should look like this:

I am the child process
I am the child process
I am the child process
I am the child process
I am the child process
I am the child process
I am the child process
I am the child process
I am the child process
I am the child process <- ~1 second elapses after this
Child process exiting
Child process exiting
Child process exiting
Child process exiting
Child process exiting
Child process exiting
Child process exiting
Child process exiting
Child process exiting
Child process exiting
All children complete
exiting.
  
 */

int main() {
  // look at forkExample for help!
  return 0;
}
