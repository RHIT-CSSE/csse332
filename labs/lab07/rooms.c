#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

// Constants in the code
#define FIRST_ROOM_CAPACITY 5
#define FIRST_ROOM_SERVICE_RATE 1

#define SECOND_ROOM_CAPACITY 2
#define SECOND_ROOM_SERVICE_RATE 2
#define WAITING_ROOM_CAPACITY 2

#define NUM_PEOPLE_ARRIVING 10

// capture the of customers that leave early
int num_left_customers = 0; 
// TODO: Define other state of the world variables here

// TODO: Define your mutex locks and condition variables:

void *customer(void *arg)
{
  long int tid = (long int)arg;

  printf("[Customer %ld] Just arrived at first room...\n", tid);

  // Enter the first room.
  printf("[Customer %ld] Entered first room...\n", tid);
  sleep(FIRST_ROOM_SERVICE_RATE);
  printf("[Customer %ld] Left first room...\n", tid);

  // You might want to check if you need to enter the waiting room here or leave
  // here...
  // if(some condition) {
  //   printf("[Customer %ld] Left unhapy because waiting room is full...\n", tid);
  //   return 0;
  // }

  // In case the customer wants to enter the waiting room, you might want to
  // print the following:
  // printf("[Customer %ld] Joined the waiting room for second room...\n", tid);

  // Enter the second room
  printf("[Customer %ld] Entered second room...\n", tid);
  sleep(SECOND_ROOM_SERVICE_RATE);
  printf("[Customer %ld] Left second room...\n", tid);

  // Done, time to leave...
  return 0;
}

int
main(int argc, char **argv)
{
  int i;
  long int tids[NUM_PEOPLE_ARRIVING];
  pthread_t threads[NUM_PEOPLE_ARRIVING];
  srand(time(0));

  for(i = 0; i < NUM_PEOPLE_ARRIVING; i++) {
    tids[i] = i + 1;
    pthread_create(&threads[i], 0, customer, (void*)tids[i]);
    if(!(i % 2))
      sleep(rand() % 2);
  }

  for(i = 0; i < NUM_PEOPLE_ARRIVING; i++) {
    pthread_join(threads[i], 0);
  }

  printf("Everything finished: %d customers left unhappy...\n",
         num_left_customers);
  exit(EXIT_SUCCESS);
}
