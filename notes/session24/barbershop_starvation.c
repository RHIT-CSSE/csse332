#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_CUSTOMERS     20
#define NUM_WAITING_CHAIRS 4

// state variables
volatile int customer_in_work_chair = 0;
volatile int customers_waiting      = 0;

// concurrency means
pthread_mutex_t lock          = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t barber_cond    = PTHREAD_COND_INITIALIZER;
pthread_cond_t barber_done    = PTHREAD_COND_INITIALIZER;
pthread_cond_t waiting_chairs = PTHREAD_COND_INITIALIZER;

void cut_hair(void)
{
  printf("Barber starting to cut customer's hair...\n");
  sleep(2);
  printf("Barber is done cutting the customer's hair...\n");
}

void *barber_fn(void *arg)
{
  printf("The barber opened the barber shop...\n");

  while(1) {
    // check if there's anyone there, if not, then just sleep
    pthread_mutex_lock(&lock);
    while(!customer_in_work_chair)
      pthread_cond_wait(&barber_cond, &lock);
    pthread_mutex_unlock(&lock);

    // customer is ready, go in and cut their hair. Note that again here, no
    // need to have any locks since we are not touching any shared variables.
    cut_hair();

    // done cutting the hair, now let the customer leave with their hair cut
    pthread_mutex_lock(&lock);
    customer_in_work_chair = 0;
    pthread_cond_signal(&barber_done);
    pthread_mutex_unlock(&lock);
  }
}

void *customer_fn(void *arg)
{
  int tid = *(int*)arg;
  int added = 0;

  printf("Customer %d arrived at the shop...\n", tid);

  // check if should go in or not
  pthread_mutex_lock(&lock);
  if(customers_waiting >= NUM_WAITING_CHAIRS) {
    // no more waiting room, then just leave
    printf("Customer %d is pissed and now leaving...\n", tid);
    pthread_mutex_unlock(&lock);
    return NULL;
  }

  // still holding the lock, go to waiting room if needed
  customers_waiting++;
  while(customer_in_work_chair) {
    pthread_cond_wait(&waiting_chairs, &lock);
  }

  // The customer is ready for a haircut!
  // Still holding the lock here so good to go.
  customers_waiting--;
  customer_in_work_chair = 1;
  pthread_cond_signal(&barber_cond);

  // barber should be awake now and so we go have our fun
  printf("Customer %d is ready for a haircut...\n", tid);
  while(customer_in_work_chair)
    pthread_cond_wait(&barber_done, &lock);
  printf("Customer %d done with their haircut...\n", tid);

  // now we are done with out haircut, still holding the lock, and must let
  // waiting customer know.
  pthread_cond_signal(&waiting_chairs);
  pthread_mutex_unlock(&lock);

  printf("Customer %d leaves the shop...\n", tid);

  return NULL;
}

int
main(int argc, char **argv)
{
  int i, ids[NUM_CUSTOMERS];
  pthread_t barber;
  pthread_t customers[NUM_CUSTOMERS];

  pthread_create(&barber, NULL, barber_fn, NULL);
  for(i = 0; i < NUM_CUSTOMERS; i++) {
    ids[i] = i;
    pthread_create(&customers[i], NULL, customer_fn, &ids[i]);

    if(i%3 == 0) sleep(1);
  }

  for(i = 0; i < NUM_CUSTOMERS; i++)
    pthread_join(customers[i], 0);

  printf("All customers finished, barber is closing the shop...\n");

  exit(EXIT_SUCCESS);
}
