#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>

#include <pthread.h>

/**
 * The goal of this activity is to order the events in two different threads.
 * Our desired output is:
 * Step 1
 * Step 2
 * Step 3
 *
 * HINT: You might need more than one semaphore.
 */

void *
thread1_fn(void *arg)
{
	/* modify this function as needed */
	printf("Step 1\n");
	printf("Step 3\n");
	return NULL;
}

void *
thread2_fn(void *arg)
{
	/* modify this function as needed */
	printf("Step 2\n");
	return NULL;
}

int
main(int argc, char **argv)
{
	pthread_t threads[2];

	/* create the threads */
	pthread_create(&threads[0], 0, thread1_fn, NULL);
	pthread_create(&threads[1], 0, thread2_fn, NULL);

	/* wait for the threads */
	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);
}
