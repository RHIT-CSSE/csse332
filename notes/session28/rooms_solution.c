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

// state of the world
int num_first_room = 0;
int num_waiting_room = 0;
int num_second_room = 0;
int num_left_customers = 0;

// condition variables
pthread_cond_t first_room = PTHREAD_COND_INITIALIZER;
pthread_cond_t waiting_room = PTHREAD_COND_INITIALIZER;

// the mutex lock
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *customer(void *arg)
{
  long int tid = (long int)arg;

  printf("[Customer %ld] Just arrived at first room...\n", tid);

  // Enter the first room.
  pthread_mutex_lock(&lock);
  while(num_first_room >= FIRST_ROOM_CAPACITY) {
    pthread_cond_wait(&first_room, &lock);
  }
  num_first_room++;
  pthread_mutex_unlock(&lock);

  printf("[Customer %ld] Entered first room...\n", tid);
  sleep(FIRST_ROOM_SERVICE_RATE);
  printf("[Customer %ld] Left first room...\n", tid);

  pthread_mutex_lock(&lock);
  num_first_room--;
  pthread_cond_signal(&first_room);

  // You might want to check if you need to enter the waiting room here or leave
  // here...
  if(num_waiting_room >=  WAITING_ROOM_CAPACITY) {
    // waiting room is full, will leave
    num_left_customers++;
    printf("[Customer %ld] Left unahppy because waiting room is full...\n",
           tid);
    pthread_mutex_unlock(&lock);
    return 0;
  }

  // at this point, there is room in the waiting room, this means two cases:
  // the second room might be empty, then no need to wait
  // the second room is full but there's room in the waiting room, then join the
  // queue and update the number
  while(num_second_room >= SECOND_ROOM_CAPACITY) {
    num_waiting_room++;
    printf("[Customer %ld] Joined the waiting room for second room...\n", tid);
    pthread_cond_wait(&waiting_room, &lock);
    num_waiting_room--;
  }
  num_second_room++;
  pthread_mutex_unlock(&lock);

  // Enter the second room
  printf("[Customer %ld] Entered second room...\n", tid);
  sleep(SECOND_ROOM_SERVICE_RATE);
  printf("[Customer %ld] Left second room...\n", tid);

  pthread_mutex_lock(&lock);
  num_second_room--;
  pthread_cond_signal(&waiting_room);
  pthread_mutex_unlock(&lock);

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
