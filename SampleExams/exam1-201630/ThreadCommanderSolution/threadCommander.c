/* Copyright 2016 Rose-Hulman Institute of Technology


Compile me like this:
gcc threadCommander.c -o threadCommander -pthread

 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

struct thread_data {
  int shouldRun;
  char name[101];
  pthread_t tid;
  struct thread_data *previousThread;
};

void *runner(void* ptr) {
  struct thread_data *data = (struct thread_data *) ptr;
  while(data->shouldRun) {
    printf("Hello from thread %s!\n", data->name);
    sleep(1);
  }
}

int main(void) {
  struct thread_data * latestThread, * newThread;

  char command[103];
  pthread_t current_tid;

  latestThread = NULL;
  
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
      command[strlen(command) - 1] = '\0';
      newThread = (struct thread_data*) malloc(sizeof(struct thread_data));
      newThread->previousThread = latestThread;
      latestThread = newThread;
      
      strcpy(latestThread->name, command+2);
      latestThread->shouldRun = 1;
      pthread_create(&current_tid, NULL, runner, latestThread);
      latestThread->tid = current_tid;
    }
    if(command[0] == 'k') {
      if(latestThread == NULL) {
        printf("No threads running!\n");
        continue;
      }
      latestThread->shouldRun = 0;
      pthread_join(latestThread->tid, NULL);
      newThread = latestThread->previousThread;
      printf("Thread %s stopped.\n", latestThread->name);
      free(latestThread);
      latestThread = newThread;
    }

  }

  return 0;
}

