#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define NUM_CUSTOMERS     20
#define NUM_WAITING_CHAIRS 4

// This solution has a minor issue that might cause some out of order prints,
// but we will accept those for now. The problem might arise in a weird
// scheduling case when the barber is done cutting a customer's hair and
// directly calls out for the next one. At that point the scheduler decides to
// run the next customer before the one that is actually still in the chair, at
// that point there will be customers in the work chair, the one that is
// supposed to leave momentarily and the new one that is supposed to enter the
// work chair and get a hair cut. This is a minor issue that we will accept, and
// we can easily solve with one more condition variable; we do that in the full
// solution in this directory.

// state variables
volatile int num_customers = 0;   // includes both waiting and getting a haircut
volatile int customer_ready = 0;

// concurrency means
pthread_mutex_t lock          = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t barber_cond    = PTHREAD_COND_INITIALIZER;
pthread_cond_t barber_done    = PTHREAD_COND_INITIALIZER;
pthread_cond_t waiting_chairs = PTHREAD_COND_INITIALIZER;
pthread_cond_t ready          = PTHREAD_COND_INITIALIZER;

void cut_hair(void)
{
  int period = rand() % 3;
  printf("Barber starting to cut customer's hair...\n");
  sleep(period);
  printf("Barber is done cutting the customer's hair...\n");
}

void *barber_fn(void *arg)
{
  printf("The barber opened the barber shop...\n");

  while(1) {
    // check if there's a customer ready from the waiting room.
    pthread_mutex_lock(&lock);
    while(num_customers == 0) {
      pthread_cond_wait(&barber_cond, &lock);
    }

    // grab a customer from the waiting room.
    num_customers--;
    pthread_cond_signal(&waiting_chairs);

    // wait for the customer to get to the work room
    while(!customer_ready)
      pthread_cond_wait(&ready, &lock);
    customer_ready = 0;
    pthread_mutex_unlock(&lock);

    // customer is ready, go in and cut their hair. Note that again here, no
    // need to have any locks since we are not touching any shared variables.
    cut_hair();

    // done cutting the hair, now let the customer leave with their hair cut
    pthread_mutex_lock(&lock);
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

  // waste a signal, but it's not that bad.
  pthread_cond_signal(&barber_cond);

  // this is a somewhat awkward case. So this is the first time we encounter a
  // pthread_cond_wait without an associate condition. And the reasoning if the
  // following: we want our new coming customer to ALWAYS join the queue, even
  // if there is not one already. So the first customer to come in the shop
  // (while the shop is empty), will kind of create a queue and then wait for
  // the barber to call on them. This is a bit annoying but it saves us from
  // hitting the case of starvation.
  pthread_cond_wait(&waiting_chairs, &lock);

  // let the barber know we are ready
  customer_ready = 1;
  pthread_cond_signal(&ready);

  // barber should be awake now and so we go have our fun
  printf("Customer %d is ready for a haircut...\n", tid);

  // this is again a spot where we have to do the weird approach where we have
  // to wait without a condition, if we end up using a state variable here, we
  // risk a race condition on that variable due to the fact that the barber is
  // the one that is going to wake up the customers, and not the last customer
  // themselves.
  printf("Customer %d is waiting for barber to finish haircut...\n", tid);
  pthread_cond_wait(&barber_done, &lock);

  printf("Customer %d done with their haircut...\n", tid);
  printf("Customer %d leaves the shop happy...\n", tid);
  pthread_mutex_unlock(&lock);

  return NULL;
}

int
main(int argc, char **argv)
{
  int i, ids[NUM_CUSTOMERS];
  pthread_t barber;
  pthread_t customers[NUM_CUSTOMERS];
  srand(time(NULL));

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
