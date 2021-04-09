#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>

#include <pthread.h>
#include <semaphore.h>

/**
 * The goal of this activity is to simulate pthread_join using a semaphore.
 * You should first create an thread and a semaphore, and then use it to make
 * the master thread wait for the newly created child.
 */

static sem_t s;

void *
child(void *arg)
{
	printf("Child\n");
	sleep(5);
	printf("Child completed\n");
	/* add code here to simulate wait */
	sem_post(&s);
	return NULL;
}

int
main(int argc, char **argv)
{
	pthread_t thread;

	/* intialize the sempahore */
	sem_init(&s, 0, 0);

	/* create the thread */
	pthread_create(&thread, 0, child, NULL);

	printf("Parent waiting for the child\n");
	/* Add code here to simulate wait! */
	sem_wait(&s);
	printf("Child completed, parent can exit!\n");

	return 0;
}
