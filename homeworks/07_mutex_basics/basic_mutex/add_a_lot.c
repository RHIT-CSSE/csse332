/* Copyright 2016 Rose-Hulman */
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_THREADS 10
/*
 * You can use the provided makefile to compile your code.
 */
int total;

pthread_mutex_t lock;

void *add10000(void *arg) {
  int local_sum = 0;
  for (int i = 0; i < 10000; i++) {
    // this area: critical section
    // want this to run in mutual exclusion mode
    /* total++; */
    local_sum++;
  }

  pthread_mutex_lock(&lock); // try to lock the mutex
  total += local_sum;
  pthread_mutex_unlock(&lock);
  return NULL;
}

int main(int argc, char **argv) {
  total = 0;
  pthread_t threads[NUM_THREADS];
  pthread_mutex_init(&lock, 0);

  int i;
  for (i = 0; i < NUM_THREADS; i++) {
    pthread_create(&threads[i], NULL, add10000, NULL);
  }

  for (i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  printf("Everything finished.  Final total %d\n", total);
  pthread_mutex_destroy(&lock);


  return 0;
}
