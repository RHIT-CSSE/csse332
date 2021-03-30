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

   Be sure that the second pair doesn't enter the critical section
   until both members of the previous pair are done.

   Use semaphores to enforce this constraint.
   
**/

sem_t bigSemaphore, mutex, firstWaiting;
int isFirst;

void *process(void *arg) {

  int amFirst;
  
  sem_wait(&bigSemaphore);
  sem_wait(&mutex);
  if(isFirst) {
    amFirst = 1;
    isFirst = 0;
  } else {
    amFirst = 0;
  }
  sem_post(&mutex);
  if(amFirst) {
    sem_wait(&firstWaiting);
  } else {
    sem_post(&firstWaiting);
  }
  printf("Starting critical section (hopefully with pair) \n");
  sleep(1);
  printf("Finishing critical section \n");

  if(amFirst) {
    sem_wait(&firstWaiting);
    isFirst = 1;
    sem_post(&bigSemaphore);
    sem_post(&bigSemaphore);    
  } else {
    sem_post(&firstWaiting);
  }

  return NULL;
}

int main(int argc, char **argv) {
  pthread_t p[6];
  sem_init(&bigSemaphore, 0, 2);
  sem_init(&firstWaiting, 0, 0);
  sem_init(&mutex, 0, 1);
  isFirst = 1;

  int i;

  pthread_create(&p[0], NULL, process, NULL);
  sleep(1);
  pthread_create(&p[1], NULL, process, NULL);
  pthread_create(&p[2], NULL, process, NULL);
  pthread_create(&p[3], NULL, process, NULL);
  pthread_create(&p[4], NULL, process, NULL);
  pthread_create(&p[5], NULL, process, NULL);

  for(int i = 0; i < 6; i++) {
    pthread_join(p[i], NULL);
  }

  printf("Everything finished.\n");
}
