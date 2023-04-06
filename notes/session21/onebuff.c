#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <time.h>

#include <pthread.h>

/**
 * One buffer entry version of the producer consumer problem.
 */

#define LOOPS 10
// change these to make the problem more interesting
#define NUM_PRODUCERS 1
#define NUM_CONSUMERS 1

volatile int buff;
volatile int count = 0;

void
put(int v)
{
  assert(count == 0);
  count = 1;
  buff = v;
}

int
get(void)
{
  assert(count == 1);
  count = 0;
  return buff;
}

void *producer(void *arg)
{
  int num = *(int*)arg;
  int i, v;

  printf("Producer %d started...\n", num);

  for(i = 0; i < LOOPS; i++) {
    v = rand();
    printf("\tProducer %d produced value %d!\n", num, v);
    put(v);
  }

  printf("Producer %d finished...\n", num);
}

void *consumer(void *arg)
{
  int num = *(int*)arg;
  int i, v;

  printf("Consumer %d started...\n", num);

  for(i = 0; i < LOOPS; i++) {
    v = get();
    printf("\tConsumer %d consumed value %d!\n", num, v);
  }

  printf("Consumer %d finished...\n", num);
}

int main(int argc, char **argv)
{
  int i;
  int producer_ids[NUM_PRODUCERS];
	pthread_t producer_threads[NUM_PRODUCERS];
  int consumer_ids[NUM_PRODUCERS];
	pthread_t consumer_threads[NUM_CONSUMERS];

  // do not change this line
  setvbuf(stdout, NULL, _IONBF, 0);
  srand(time(NULL));

  // create the threads
  for(i = 0; i < NUM_PRODUCERS; i++) {
    producer_ids[i] = i;
    pthread_create(&producer_threads[i], 0, producer, &producer_ids[i]);
  }

  for(i = 0; i < NUM_CONSUMERS; i++) {
    consumer_ids[i] = i;
    pthread_create(&consumer_threads[i], 0, consumer, &consumer_ids[i]);
  }

  // wait for them
  for(i = 0; i < NUM_PRODUCERS; i++) {
    pthread_join(producer_threads[i], 0);
  }

  for(i = 0; i < NUM_PRODUCERS; i++) {
    pthread_join(consumer_threads[i], 0);
  }

  printf("Everything is done...\n");
  exit(0);
}

