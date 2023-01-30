#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define NUM_EW_CARS 15
#define NUM_WE_CARS 15
#define NUM_AMBULANCE 7
#define TOTAL_THREADS NUM_EW_CARS + NUM_WE_CARS + NUM_AMBULANCE

// car speeds
#define EW_SPEED 3
#define WE_SPEED 2

// state variables
int ewcars_in_tunnel = 0;
int wecars_in_tunnel = 0;
int ambulance_waiting = 0;
int ambulance_in_tunnel = 0;

// concurrency means
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ambulance_wait = PTHREAD_COND_INITIALIZER;
pthread_cond_t ew_wait = PTHREAD_COND_INITIALIZER;
pthread_cond_t ew_wait_for_ambulance = PTHREAD_COND_INITIALIZER;
pthread_cond_t we_wait = PTHREAD_COND_INITIALIZER;
pthread_cond_t we_wait_for_ambulance = PTHREAD_COND_INITIALIZER;

void *ewcar(void *arg)
{
  int tid = *(int*)arg;

  pthread_mutex_lock(&lock);
  while(ambulance_in_tunnel || ambulance_waiting) {
    pthread_cond_wait(&ew_wait_for_ambulance, &lock);
  }

  while(ewcars_in_tunnel == 3) {
    pthread_cond_wait(&ew_wait, &lock);
  }
  ewcars_in_tunnel++;
  pthread_mutex_unlock(&lock);

  printf("Car (%d) entered tunnel in EW direction...\n", tid);
  sleep(EW_SPEED);
  printf("Car (%d) exited tunnel in EW direction...\n", tid);

  pthread_mutex_lock(&lock);
  ewcars_in_tunnel--;
  // now need to check where to signal, depending on whether there's an
  // ambulance waiting or not
  if(ambulance_waiting) {
    pthread_cond_signal(&ambulance_wait);
  } else if(!ambulance_in_tunnel) {
    pthread_cond_signal(&ew_wait);
  }
  pthread_mutex_unlock(&lock);

  return 0;
}

void *wecar(void *arg)
{
  int tid = *(int *)arg;

  pthread_mutex_lock(&lock);
  while(ambulance_in_tunnel || ambulance_waiting) {
    pthread_cond_wait(&we_wait_for_ambulance, &lock);
  }

  while(wecars_in_tunnel == 1) {
    pthread_cond_wait(&we_wait, &lock);
  }
  wecars_in_tunnel++;
  pthread_mutex_unlock(&lock);

  printf("Car (%d) entered tunnel in WE direction...\n", tid);
  sleep(WE_SPEED);
  printf("Car (%d) exited tunnel in WE direction...\n", tid);

  pthread_mutex_lock(&lock);
  wecars_in_tunnel--;
  // now need to check where to signal, depending on whether there's an
  // ambulance waiting or not
  if(ambulance_waiting) {
    pthread_cond_signal(&ambulance_wait);
  } else if(!ambulance_in_tunnel) {
    pthread_cond_signal(&we_wait);
  }
  pthread_mutex_unlock(&lock);

  return 0;
}

void *ambulance(void *arg)
{
  int tid = *(int*)arg;
  int direction = 0;

  // while(!(!ambulance_in_tunnel && (ewcars_in_tunnel < 3 || wecars_in_tunnel < 1)))
  //   pthread_cond_wait(&ambulance_wait, &lock);
  pthread_mutex_lock(&lock);
  ambulance_waiting++;
  while(ewcars_in_tunnel == 3 && wecars_in_tunnel == 1)
    pthread_cond_wait(&ambulance_wait, &lock);
  ambulance_waiting--;
  ambulance_in_tunnel++;

  // here it is a bit tricky, because we have to figure out in which direction
  // did we go into
  if(ewcars_in_tunnel < 3) {
    printf("Ambulance %d entered the tunnel in EW direction\n", tid);
    ewcars_in_tunnel++;
  } else if (wecars_in_tunnel < 1) {
    printf("Ambulance %d entered the tunnel in WE direction\n", tid);
    wecars_in_tunnel++;
    direction = 1;
  }
  pthread_mutex_unlock(&lock);

  sleep(1); // ambulance is the fastest

  // ambulance left
  pthread_mutex_lock(&lock);
  ambulance_in_tunnel--;
  if(!direction) {
    printf("Ambulance %d exited the tunnel in EW direction\n", tid);
    ewcars_in_tunnel--;
  } else {
    wecars_in_tunnel--;
    printf("Ambulance %d exited the tunnel in WE direction\n", tid);
  }

  if(ambulance_waiting) {
    pthread_cond_signal(&ambulance_wait);
  } else {
    // no ambulances waiting, but only last ambulance leaving will signal!
    if(ambulance_in_tunnel==0) {
      // if there was anyone waiting in EITHER direction, then let them in!
      if(ewcars_in_tunnel < 3)
        pthread_cond_signal(&ew_wait);
      if(wecars_in_tunnel == 0)
        pthread_cond_signal(&we_wait);

      // unlock all those waiting for the ambulances
      pthread_cond_broadcast(&ew_wait_for_ambulance);
      pthread_cond_broadcast(&we_wait_for_ambulance);
    }
  }
  pthread_mutex_unlock(&lock);

  return 0;
}


int
main(int argc, char **argv)
{
  pthread_t threads[NUM_EW_CARS + NUM_WE_CARS + NUM_AMBULANCE];
  int tids[NUM_EW_CARS + NUM_WE_CARS + NUM_AMBULANCE];
  int i;
  srand(time(0));

  for(i = 0; i < NUM_EW_CARS; i++) {
    tids[i] = i + 1;
    pthread_create(&threads[i], 0, ewcar, &tids[i]);
  }

  for(i = NUM_EW_CARS; i < NUM_WE_CARS + NUM_EW_CARS; i++) {
    tids[i] = i + 1;
    pthread_create(&threads[i], 0, wecar, &tids[i]);
  }

  for(i = NUM_EW_CARS + NUM_WE_CARS; i < TOTAL_THREADS; i++) {
    tids[i] = i + 1;
    pthread_create(&threads[i], 0, ambulance, &tids[i]);
    // make the ambulances arrive at random times
    sleep(rand() % 10);
  }

  for(i = 0; i < TOTAL_THREADS; i++) {
    pthread_join(threads[i], 0);
  }

  printf("Everyting finished...\n");
  exit(EXIT_SUCCESS);
}
