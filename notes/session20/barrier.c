#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include <pthread.h>

/**
 * In this exercise, we would like to implement a barrier to make all of our
 * threads wait at a certain location in the code before they can move formward.
 * No thread can get passed the barrier until all threads reach that barrier,
 * then they can move out together in any order.
 */

#define NUM_THREADS 5


void barrier_wait(void)
{
  // TODO: Add your code here.
}

void *thread_fn(void *arg)
{
  int tid = *(int*)arg;

  printf("Thread %d started ...\n", tid);

  sleep(rand()%10);

  printf("Thread %d waiting at barrier ...\n", tid);
  barrier_wait();
  printf("Thread %d left the barrier ...\n", tid);

  return NULL;
}

int main()
{
  pthread_t threads[NUM_THREADS];
  int tids[NUM_THREADS];
  int i;

  srand(time(NULL));

  for(i = 0; i < NUM_THREADS; i++) {
    tids[i] = i;
    pthread_create(&threads[i], NULL, thread_fn, &tids[i]);

    if(i == NUM_THREADS/2)
      sleep(2);
  }

  for(i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  printf("All done, goodbye...\n");
  exit(0);
}

