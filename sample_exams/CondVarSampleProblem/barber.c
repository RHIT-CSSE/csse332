#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_CUSTOMERS 10
#define MAX_CHAIRS 4


void*
barber_fn(void *arg)
{
	printf("The barber is now in the stop...\n");

	while(1) {
		// start cutting hair
		printf("Barber cutting customer hair\n");
		sleep(1);
	}
}

void*
customer_fn(void *arg)
{
	printf("New customer arrived...\n");

	printf("customer ready to sit in the barber chair\n");

	return NULL;
}

int
main(int argc, char **argv)
{
	int i;
	pthread_t barber;
	pthread_t customers[NUM_CUSTOMERS];


	pthread_create(&barber, 0, barber_fn, 0);
	for(i = 0; i < NUM_CUSTOMERS; ++i) {
		pthread_create(&customers[i], 0, customer_fn, 0);

		if(i==6) sleep(1);
	}

	for(i = 0; i < NUM_CUSTOMERS; ++i) {
		pthread_join(customers[i], 0);
	}

	printf("All customers finished, barber shop closed...\n");
}
