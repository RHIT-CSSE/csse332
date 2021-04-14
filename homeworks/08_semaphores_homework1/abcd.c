/* Copyright 2016 Rose-Hulman */
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

/**
   In this system, there are 2 threads.  Thread one prints out A and
   C.  Thread two prints out B and D.  Use semaphores to ensure they
   always print A B C D, in that order.

   Use semaphores to enforce this constraint.
**/

void *threadOne(void *arg) {
  sleep(1);  /* just makes it obvious that it won't work without
                semaphores */
  printf("A\n");
  printf("C\n");
  return NULL;
}

void *threadTwo(void *arg) {
  printf("B\n");
  printf("D\n");
  return NULL;
}

int main(int argc, char **argv) {
  pthread_t one, two;

  pthread_create(&one, NULL, threadOne, NULL);
  pthread_create(&two, NULL, threadTwo, NULL);
  pthread_join(one, NULL);
  pthread_join(two, NULL);

  printf("Everything finished.\n");
}
