#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_CUSTOMERS 10
#define MAX_CHAIRS 4

pthread_cond_t barber_cond;
pthread_cond_t chairs_cond;
pthread_cond_t barber_done;
pthread_mutex_t lock;

int customer_in_chair = 0;
int num_chairs_occupied = 0;

void*
barber_fn(void *arg)
{
	printf("The barber is now in the stop...\n");

	while(1) {
		pthread_mutex_lock(&lock);
		while(customer_in_chair == 0) {
			pthread_cond_wait(&barber_cond, &lock);
		}
		pthread_mutex_unlock(&lock);

		// start cutting hair
		printf("Barber cutting customer hair\n");
		sleep(1);
		pthread_mutex_lock(&lock);
		pthread_cond_signal(&barber_done);
		customer_in_chair = 0;
		pthread_mutex_unlock(&lock);
	}
}

void*
customer_fn(void *arg)
{
	printf("New customer arrived...\n");
	int added = 0;

	pthread_mutex_lock(&lock);
	// if no room, just leave
	if(num_chairs_occupied >= MAX_CHAIRS) {
		pthread_mutex_unlock(&lock);
		printf("Customer leaving for chairs are fully occupied...\n");
		return NULL;
	}

	// check if there's customer on the barber chair
	while(customer_in_chair) {
		if(!added) {
			printf("Customer waiting in the chair...\n");
			num_chairs_occupied++;
			added=1;
		}
		pthread_cond_wait(&chairs_cond, &lock);
	}
	pthread_mutex_unlock(&lock);

	printf("customer ready to sit in the barber chair\n");
	pthread_mutex_lock(&lock);
	// remove one from the chairs if added
	if(added)
		num_chairs_occupied--;
	customer_in_chair++;

	// signal the barber
	pthread_cond_signal(&barber_cond);

	// wait for barber to finish cutting hair
	pthread_cond_wait(&barber_done, &lock);

	// done with the haircut
	printf("customer done with the haircut\n");
	pthread_cond_signal(&chairs_cond);
	pthread_mutex_unlock(&lock);

	return NULL;
}

int
main(int argc, char **argv)
{
	int i;
	pthread_t barber;
	pthread_t customers[NUM_CUSTOMERS];

	pthread_cond_init(&barber_cond, 0);
	pthread_cond_init(&chairs_cond, 0);
	pthread_cond_init(&barber_done, 0);
	pthread_mutex_init(&lock, 0);

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
