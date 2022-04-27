#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

#include "say_hello.h"


#define MAX_HEATING_DEMONS 2
#define MAX_REPAIR_DEMONS 4

int
main(int argc, char **argv)
{
	/* Let beezy say hello to you, he's a polite devil! */
	/* say_hello(); */

	/* you code goes here.... */
  for(int i = 0; i < MAX_REPAIR_DEMONS; i++) {
    int pid = fork();

    if(pid == 0) {
      // MANAGER
      pid = fork();
      if(pid == 0) {
        // GRANDCHILD: REPAIR DEMON
        // spwan the repair demon
        execlp("./repair_demon", "./repair_demon", NULL);
        perror("PANIC:");
        exit(EXIT_FAILURE);
      } else {
        // MANAGER
        int status;
        wait(&status);

        if(WEXITSTATUS(status) == 0) {
          printf("SUCCESS\n");
        } else {
          printf("FAILURE\n");
        }

        exit(EXIT_SUCCESS);
      }
    } else {
      // PARENT: BEEZY
      // do nothing and continue with the loop
    }
  }
}
