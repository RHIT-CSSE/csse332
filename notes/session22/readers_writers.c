#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define NUM_READERS 20
#define NUM_WRITERS 3

volatile int data = 0;

void *reader_fn(void *arg)
{
  int f = *(int*)arg;

  while(1) {

    // reading critical section starts
    printf("[Reader %d]: Reading data is %d\n", f, data);
    sleep(1); // simulate actual work being done!
    // reading critical section ends

  }
}

void *writer_fn(void *arg)
{
  int f = *(int*)arg;

  while(1) {

    // writing critical section starts
    printf("[Writer %d]: Starting to write ...\n", f);
    data++;
    printf("[Writer %d]: Finished writing (%d) ...\n", f, data);
    // writing critical section ends

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

