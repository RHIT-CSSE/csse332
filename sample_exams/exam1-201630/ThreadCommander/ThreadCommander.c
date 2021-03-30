/* Copyright 2016 Rose-Hulman Institute of Technology

 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

int main(void) {
  char command[103];
  
  printf("Enter 'c <THREAD_NAME>' to create a new thread\n");
  printf("Enter 'k' to stop most recently started thread\n");

  while(1) {
    //enter Control-D on the command line to exit
    if(fgets(command,103,stdin) == NULL) {
      printf("Exiting normally.\n");
      exit(0);
    }
    if(command[0] == 'c') {
      printf("Create %s",command+2);
    }
  }

  return 0;
}

