#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define NUM_READERS 20
#define NUM_WRITERS 3

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  turn = PTHREAD_COND_INITIALIZER;
volatile int data = 0;
unsigned int running_readers = 0;
unsigned int running_writers = 0;

void *reader_fn(void *arg)
{
  int f = *(int*)arg;

  while(1) {
    pthread_mutex_lock(&lock);
    while(running_writers > 0)
      pthread_cond_wait(&turn, &lock);
    running_readers++;
    pthread_mutex_unlock(&lock);

    // reading critical section starts
    printf("[Reader %d]: Reading data is %d\n", f, data);
    sleep(1); // simulate actual work being done!
    // reading critical section ends

    pthread_mutex_lock(&lock);
    running_readers--;
    if(running_readers == 0)
      pthread_cond_signal(&turn);
    pthread_mutex_unlock(&lock);
  }
}

void *writer_fn(void *arg)
{
  int f = *(int*)arg;

  while(1) {
    pthread_mutex_lock(&lock);
    while(running_readers > 0 || running_writers > 0)
      pthread_cond_wait(&turn, &lock);
    running_writers++;
    pthread_mutex_unlock(&lock);

    // writing critical section starts
    printf("[Writer %d]: Starting to write ...\n", f);
    data++;
    printf("[Writer %d]: Finished writing (%d) ...\n", f, data);
    // writing critical section ends

    pthread_mutex_lock(&lock);
    running_writers--;
    pthread_cond_broadcast(&turn);
    pthread_mutex_unlock(&lock);

    // go away and do other stuff
    sleep(rand()%5);
  }
}

int
main(int argc, char **argv)
{
  pthread_t rtid[NUM_READERS], wtid[NUM_WRITERS];
  int i, rnums[NUM_READERS], wnums[NUM_WRITERS];
  srand(time(NULL));

  /* create the readers */
  for(i = 0; i < NUM_READERS; i++) {
    rnums[i] = i;
    pthread_create(&rtid[i], NULL, reader_fn, (void*)&rnums[i]);
  }

  /* create the writers */
  for(i = 0; i < NUM_WRITERS; i++) {
    wnums[i] = i;
    pthread_create(&wtid[i], NULL, writer_fn, (void*)&wnums[i]);
  }

  /* wait for the any one of them since they all run infinite loops */
  pthread_join(rtid[0], NULL);

  exit(EXIT_SUCCESS);
}

