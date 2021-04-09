#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>

#include <pthread.h>

/**
 * The goal of this activity is to simulate pthread_join using a semaphore.
 * You should first create an thread and a semaphore, and then use it to make
 * the master thread wait for the newly created child.
 */

void *
child(void *arg)
{
	printf("Child\n");
	/* add code here to simulate wait */
	return NULL;
}

int
main(int argc, char **argv)
{
	pthread_t thread;

	/* create the thread */
	pthread_create(&thread, 0, child, NULL);

	/* Add code here to simulate wait! */
}
