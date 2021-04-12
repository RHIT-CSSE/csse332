#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>

#include <pthread.h>
#include <semaphore.h>

/**
 * The goal of this activity is to implement the correct concurrency management
 * for the producer/consumer problem using condition variables.
 */

const int loops = 100;
#define MAX 10
#define NUM_PRODUCERS 1
#define NUM_CONSUMERS 1

int buffer[MAX];
int fill = 0;
int use = 0;

/**
 * Put a value into the buffer and increment the fill index.
 */
void
put(int value)
{
	buffer[fill] = value;
	fill = (fill + 1) % MAX;
}

/**
 * Get a value from the buffer and increment the use index.
 */
int
get()
{
	int value = buffer[use];
	use = (use + 1) % MAX;
	return value;
}

/**
 * The producer can fill item in the buffer when empty
 */
void *
producer(void *arg)
{
	int i;
	for (i = 0; i < loops; i++) {
		put(i);
		printf("Producer produced %d\n", i);
	}
}

/**
 * The consumer can consume items from the buffer when full
 */
void *
consumer(void *arg)
{
	int value = 0, i = 0;
	for(; i < loops; i++) {
		value = get();
		printf("Consumer consumed %d\n", value);
	}
}

int
main(int argc, char **argv)
{
	/* the threads */
	pthread_t producer_threads[NUM_PRODUCERS];
	pthread_t consumer_threads[NUM_CONSUMERS];

	/* create the producer threads */
	for (int i = 0; i < NUM_PRODUCERS; ++i)
		pthread_create(&producer_threads[i], 0, producer, NULL);

	/* create the consumer threads */
	for (int i = 0; i < NUM_CONSUMERS; ++i)
		pthread_create(&consumer_threads[i], 0, consumer, NULL);

	/* wait for producers */
	for (int i = 0; i < NUM_PRODUCERS; ++i)
		pthread_join(producer_threads[i], NULL);

	/* wait for consumers */
	for (int i = 0; i < NUM_CONSUMERS; ++i)
		pthread_join(consumer_threads[i], NULL);

}
