#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_CUSTOMERS 20
#define NUM_WAITING_CHAIRS 4

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
    // TODO: Add your code here...
  }
}

void *customer_fn(void *arg)
{
  int tid = *(int*)arg;

  printf("Customer %d arrived at the shop...\n", tid);
  // TODO: Add your code here...
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
