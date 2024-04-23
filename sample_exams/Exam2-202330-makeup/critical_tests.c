#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

#include "critical.h"
#include "prettyprint.h"

void test1(void)
{
  // all red threads
  pthread_t threads[NUM_RED_THREADS];
  unsigned long long i = 0;

  for(; i < NUM_RED_THREADS; i++) {
    pthread_create(&threads[i], 0, red, (void*)(i+1));
  }

  for(i = 0; i < NUM_RED_THREADS; i++) {
    pthread_join(threads[i], 0);
  }

  printf("\nEverything finished...\n");
}

void test2(void)
{
  // all blue threads
  pthread_t threads[NUM_BLUE_THREADS];
  unsigned long long i = 0;

  for(; i < NUM_BLUE_THREADS; i++) {
    pthread_create(&threads[i], 0, blue, (void*)(i+1));
  }

  for(i = 0; i < NUM_BLUE_THREADS; i++) {
    pthread_join(threads[i], 0);
  }

  printf("\nEverything finished...\n");
}

void test3(void)
{
  // all arrive together
  const int total_threads = NUM_RED_THREADS + NUM_BLUE_THREADS;
  pthread_t threads[total_threads];
  unsigned long long i = 0;

  for(; i < total_threads; i++) {
    if(i%2 == 0)
      pthread_create(&threads[i], 0, blue, (void*)(i+1));
    else
      pthread_create(&threads[i], 0, red, (void*)(i+1));
  }

  for(i = 0; i < total_threads; i++) {
    pthread_join(threads[i], 0);
  }

  printf("\nEverything finished...\n");
}
