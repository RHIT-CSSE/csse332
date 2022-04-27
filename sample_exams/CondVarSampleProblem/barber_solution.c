#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_CUSTOMERS 10
#define MAX_CHAIRS 4

int barber_done = 0;
int num_waiting = 0;
int main_chair_is_occupied = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t customer_waiting = PTHREAD_COND_INITIALIZER;
pthread_cond_t barber_waiting = PTHREAD_COND_INITIALIZER;
pthread_cond_t main_chair = PTHREAD_COND_INITIALIZER;


void*
barber_fn(void *arg)
{
	printf("The barber is now in the shop...\n");
	while(1) {
		pthread_mutex_lock(&lock);
		while(main_chair_is_occupied == 0) {
			pthread_cond_wait(&barber_waiting, &lock);
		}
		pthread_mutex_unlock(&lock);

		// start cutting hair
		printf("Barber cutting customer hair\n");
		sleep(1);
		pthread_mutex_lock(&lock);
		main_chair_is_occupied = 0;
    barber_done = 1;
		pthread_cond_signal(&main_chair);
		pthread_mutex_unlock(&lock);
	}
}

void*
customer_fn(void *arg)
{
  int added = 0;
  int num = *(int *)arg;
	printf("New customer %d arrived...\n", num);
  pthread_mutex_lock(&lock);
  if(num_waiting >= MAX_CHAIRS) {
    pthread_mutex_unlock(&lock);
    printf("Customer %d leaves since shop is full!\n", num);
    return NULL;
  }

  while(main_chair_is_occupied || num_waiting > 0) {
    // this statement should only execute once!
    if(!added) {
      printf("customer %d is in the waiting chair...\n", num);
      num_waiting++;
      added = 1;
    }
    pthread_cond_wait(&customer_waiting, &lock);
    printf("customer %d awake and is checking...(%d)\n",
           num, main_chair_is_occupied);
    if(main_chair_is_occupied == 0)
      break;
  }
	printf("customer %d ready to sit in the barber chair\n", num);
  main_chair_is_occupied = 1;
  if(added)
    num_waiting--;
  pthread_mutex_unlock(&lock);

  // tell the barber that I am ready to go in
  pthread_cond_signal(&barber_waiting);

  pthread_mutex_lock(&lock);
  while(!barber_done)
    pthread_cond_wait(&main_chair, &lock);
  barber_done = 0;
  pthread_mutex_unlock(&lock);

  printf("customer %d done with haircut (%d)\n",
         num, main_chair_is_occupied);
  pthread_cond_signal(&customer_waiting);
	return NULL;
}

int
main(int argc, char **argv)
{
	int i;
	pthread_t barber;
	pthread_t customers[NUM_CUSTOMERS];
  int ids[NUM_CUSTOMERS];


	pthread_create(&barber, 0, barber_fn, 0);
	for(i = 0; i < NUM_CUSTOMERS; ++i) {
    ids[i] = i+1;
		pthread_create(&customers[i], 0, customer_fn, &ids[i]);

		if(i%3 == 0) sleep(1);
	}

	for(i = 0; i < NUM_CUSTOMERS; ++i) {
		pthread_join(customers[i], 0);
	}

	printf("All customers finished, barber shop closed...\n");
}
