#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

/*
 * In this problem, there is a sushi bar with 5 open seats. If a customer
 * arrives, then two conditions might happen:
 * 
 * 1. If there is an empty seat, then the customer walks in and take a seat.
 * 
 * 2. If all 5 seats are occupied, then the customer must wait for ALL five
 * customers to leave before they can enter. 
 * 
 */

#define NUM_CUSTOMERS 23


void* customer_fn(void *arg)
{
  // TODO: Add code here!
  printf("Customer waiting...\n");

  // TODO: Add code here!

  printf("Customer eating...\n");
  sleep(3);

  // TODO: Add code here!

  printf("Customer finished eating...\n");

  return NULL;
}

int
main(int argc, char **argv)
{
  int i;
  pthread_t customers[NUM_CUSTOMERS];

  srand(time(NULL));

  for(i = 0; i < NUM_CUSTOMERS; i++) {
    pthread_create(&customers[i], 0, customer_fn, 0);

    sleep(rand() % 2);
  }
  
  for(i = 0; i < NUM_CUSTOMERS; i++) {
    pthread_join(customers[i], 0);
  }
}