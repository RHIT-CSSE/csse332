/* Copyright 2016 Rose-Hulman */
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

/**
   In this system, there is only 1 kind of process.  This process has
   a critical section.  However, the critical section must be done in
   pairs.  That is, if there is only 1 process that process must wait.
   If there are two, they enter the critical section together.  If
   there are more than two, two enter the critical section and the
   others wait.

   If you want to keep track of things like "how many threads are
   waiting on this semaphore" feel free to add other vairables and
   code.

   You also may wish to add sleeps to the pthread creates below to
   make sure things are working as your intend.
**/

void *process(void *arg) {
  printf("Starting critical section (hopefully with pair) \n");
  sleep(1);
  printf("Finishing critical section \n");
  return NULL;
}

int main(int argc, char **argv) {
  pthread_t p[6];
  int i;

  pthread_create(&p[0], NULL, process, NULL);
  pthread_create(&p[1], NULL, process, NULL);
  pthread_create(&p[2], NULL, process, NULL);
  pthread_create(&p[3], NULL, process, NULL);
  pthread_create(&p[4], NULL, process, NULL);
  pthread_create(&p[5], NULL, process, NULL);

  for(i = 0; i < 6; i++) {
    pthread_join(p[i], NULL);
  }

  printf("Everything finished.\n");
}
