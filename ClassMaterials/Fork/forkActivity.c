#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*
  Write a process that forks 10 child processes, then waits for them all
  to complete, prints a message with their exit code, then exits.

  The child processes should print a message with an ID, sleep for 1 second, then exit with that ID code.

  If you do it correctly the output should look like this:

  I'm child 0
  I'm child 1
  I'm child 2
  I'm child 3
  I'm child 4
  I'm child 5
  I'm child 6
  I'm child 8
  I'm child 9
  I'm child 7 // ~ one second delay
  One child returns with exit code 0
  One child returns with exit code 1
  One child returns with exit code 2
  One child returns with exit code 3
  One child returns with exit code 4
  One child returns with exit code 6
  One child returns with exit code 5
  One child returns with exit code 8
  One child returns with exit code 9
  One child returns with exit code 7
  exiting

  ** The order of print out could change slightly every time.
  
 */

int main() {
  // look at forkExample for help!
  int i = 0;
  for(i = 0;i < 10; i++){
          int fresult = fork();
          if(fresult == 0){
                  printf("I'm child %d\n",i);
                  sleep(1);
                  exit(i);
          }
  }
  int status;
  for(i = 0;i < 10; i++){
          wait(&status);
          printf("One child returns with exit code %d\n",WEXITSTATUS(status));
  }
  printf("exiting\n");

  return 0;
}
