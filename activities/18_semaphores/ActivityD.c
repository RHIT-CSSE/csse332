#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>

#include <pthread.h>
#include <semaphore.h>

/**
 * The goal of this activity is to correct the code below to avoid deadlocks.
 *
 * Step 1: Read the code below and figure out why there is a deadlock happening!
 * A deadlock is when both threads (produce and consumer) are waiting for each
 * other and neither of them can move forward.
 *
 * Step 2: Modify the code in a way that avoid the deadlocks above.
 */

const int loops = 100;
#define MAX 10
#define NUM_PRODUCERS 1
#define NUM_CONSUMERS 1

int buffer[MAX];
int fill = 0;
int use = 0;

sem_t empty;
sem_t full;
sem_t mutex;

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
		sem_wait(&mutex);
		sem_wait(&empty);
		put(i);
		sem_post(&full);
		sem_post(&mutex);
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
		sem_wait(&mutex);
		sem_wait(&full);
		value = get();
		sem_post(&empty);
		sem_post(&mutex);
		printf("Consumer consumed %d\n", value);
	}
}

int
main(int argc, char **argv)
{
	/* the threads */
	pthread_t producer_threads[NUM_PRODUCERS];
	pthread_t consumer_threads[NUM_CONSUMERS];

	/* initialize semaphores */
	sem_init(&empty, 0, MAX);
	sem_init(&full, 0, 0);
	sem_init(&mutex, 0, 1);

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
