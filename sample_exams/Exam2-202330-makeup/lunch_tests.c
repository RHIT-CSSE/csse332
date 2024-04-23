#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

#include "lunch.h"
#include "prettyprint.h"

void test1(void)
{
  pthread_t threads[TOTAL_THREADS];
  unsigned long i;

  for(i = 0; i < TOTAL_THREADS; i++) {
    pthread_create(&threads[i], 0, friend, (void*)(i + 1));
  }

  for(i = 0; i < TOTAL_THREADS; i++)
    pthread_join(threads[i], 0);

  pr_yellow("\nEverything finished...\n");
}

void test2(void)
{
  pthread_t threads[TOTAL_THREADS];
  unsigned long i;

  for(i = 0; i < TOTAL_THREADS; i++) {
    pthread_create(&threads[i], 0, friend, (void*)(i + 1));

    if((i + 1) %5 == 0)
      sleep(2);
  }

  for(i = 0; i < TOTAL_THREADS; i++)
    pthread_join(threads[i], 0);

  pr_yellow("\nEverything finished...\n");
}

void test3(void)
{
  pthread_t threads[TOTAL_THREADS];
  unsigned long i;

  for(i = 0; i < TOTAL_THREADS / 3; i++) {
    pthread_create(&threads[i], 0, friend, (void*)(i+1));

    usleep(500);
  }

  sleep(ARGUE_TIME);

  for(i = TOTAL_THREADS / 3; i < 2 * TOTAL_THREADS / 3; i++) {
    pthread_create(&threads[i], 0, friend, (void*)(i+1));

    usleep(500);
  }

  sleep(ARGUE_TIME);

  for(i = 2 * TOTAL_THREADS / 3; i < TOTAL_THREADS; i++) {
    pthread_create(&threads[i], 0, friend, (void*)(i+1));
  }

  for(i = 0; i < TOTAL_THREADS; i++) {
    pthread_join(threads[i], 0);
  }

  pr_yellow("\nEverything finished...\n");
}

void test4(void)
{
  pthread_t threads[TOTAL_THREADS];
  unsigned long i;

  for(i = 0; i < TOTAL_THREADS / 3; i++) {
    pthread_create(&threads[i], 0, friend, (void*)(i+1));

    usleep(1000000); // cutting it close, so should break up
  }

  sleep(ARGUE_TIME);

  for(i = TOTAL_THREADS / 3; i < 2 * TOTAL_THREADS / 3; i++) {
    pthread_create(&threads[i], 0, friend, (void*)(i+1));

    usleep(500);
  }

  sleep(ARGUE_TIME);

  for(i = 2 * TOTAL_THREADS / 3; i < TOTAL_THREADS; i++) {
    pthread_create(&threads[i], 0, friend, (void*)(i+1));

    sleep(1);
  }

  for(i = 0; i < TOTAL_THREADS; i++) {
    pthread_join(threads[i], 0);
  }

  pr_yellow("\nEverything finished...\n");
}

