#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_CUSTOMERS     20
#define NUM_WAITING_CHAIRS 4

// state variables
volatile int num_customers = 0;   // includes both waiting and getting a haircut
volatile int customer_ready = 0;
volatile int barber_ready = 0;
volatile int haircut_done = 1;

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
    // check if there's a customer ready from the waiting room.
    pthread_mutex_lock(&lock);
    while(!customer_ready) {
      pthread_cond_wait(&barber_cond, &lock);
    }
    pthread_mutex_unlock(&lock);

    // customer is ready, go in and cut their hair. Note that again here, no
    // need to have any locks since we are not touching any shared variables.
    cut_hair();

    // done cutting the hair, now let the customer leave with their hair cut
    pthread_mutex_lock(&lock);
    haircut_done = 1;
    customer_ready = 0;
    pthread_cond_signal(&barber_done);
    pthread_mutex_unlock(&lock);
  }
}

void *customer_fn(void *arg)
{
  int tid = *(int*)arg;

  printf("Customer %d arrived at the shop...\n", tid);

  // first we have to directly go into the waiting room and grab a turn even if
  // there is no one else out there.
  pthread_mutex_lock(&lock);
  if(num_customers == NUM_WAITING_CHAIRS + 1) {
    printf("Customer %d leaving the shop pissed...\n", tid);
    pthread_mutex_unlock(&lock);
    return 0;
  }

  // there are spots in the waiting room, grab a chair and tell the barber
  // about it.
  num_customers++;
  while(customer_ready) {
    printf("Customer %d entering the waiting room...\n", tid);
    pthread_cond_wait(&waiting_chairs, &lock);
    printf("Customer %d exited the waiting room...\n", tid);
  }

  // exit here when num_free_chairs is NUM_WAITING_CHAIRS - 1, or when we are
  // the first customer in.
  customer_ready = 1;
  haircut_done = 0;
  pthread_cond_signal(&barber_cond);

  // barber should be awake now and so we go have our fun
  printf("Customer %d is ready for a haircut...\n", tid);
  while(!haircut_done)
    pthread_cond_wait(&barber_done, &lock);
  printf("Customer %d done with their haircut...\n", tid);

  // now we are done with out haircut, still holding the lock, and must let
  // waiting customer know.
  num_customers--;
  pthread_cond_signal(&waiting_chairs);
  pthread_mutex_unlock(&lock);

  printf("Customer %d leaves the shop happy...\n", tid);

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
