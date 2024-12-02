#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "prettyprint.h"

#define NUM_THREADS 10

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

const char *stack = "abcdefghijklmnopqrstuvwxyz123456789";
int stackptr = 0;

char pop()
{
  if(stackptr >= strlen(stack)) {
    fprintf(stderr,
            "Buffer overflow when reading the stack. Fix your code!\n");
    return 0;
  }

  return stack[stackptr++];
}

int is_empty()
{
  return (stackptr >= strlen(stack));
}

void *worker_thread(void *arg)
{
  /* TODO: YOUR IMPLEMENTATION GOES HERE */
  int tid = *(int*)arg;
  pr_green("Worker thread %d started.\n", tid);

  pthread_mutex_lock(&lock);
  while(!is_empty()) {
    char c = pop();
    pthread_mutex_unlock(&lock);

    printf("Thread %d processing request %c\n", tid, c);
    sleep(1);

    pthread_mutex_lock(&lock);
  }
  pthread_mutex_unlock(&lock);

  return NULL;
}

int main(int argc, char **argv)
{
  /* TODO: YOUR IMPLEMENTATION GOES HERE */
  int i;
  pthread_t threads[NUM_THREADS];
  int tid[NUM_THREADS];

  for(i = 0; i < NUM_THREADS; i++) {
    tid[i] = i;
    pthread_create(&threads[i], 0, worker_thread, &tid[i]);
  }

  for(i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], 0);
  }

  /* DO NOT REMOVE THIS LINE */
  printf("****** Everything finished ******\n");
}
