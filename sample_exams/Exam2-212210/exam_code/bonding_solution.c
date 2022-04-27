#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_OXYGEN_THREADS 3
#define NUM_HYDROGEN_THREADS 6


pthread_mutex_t lock;
pthread_cond_t oxygen;
pthread_cond_t hydrogen;
pthread_cond_t barrier;

int num_hydrogen = 0;
int num_oxygen = 0;
int bonding = 0;
int num_hydrogen_needed = 2;
int num_oxygen_needed = 1;
int done_bonding = 0;

void*
oxy_thread(void *arg)
{
  printf("Oxygen thread arrived\n");
  int waited = 0;

  pthread_mutex_lock(&lock);
  num_oxygen++;
  while((bonding && num_oxygen_needed == 0) ||
         (!bonding && num_hydrogen < 2)) {
    //printf("Oxygen waiting for hydrogen...\n");
    pthread_cond_wait(&oxygen, &lock);
  }
  bonding = 1;
  num_oxygen_needed--;
  num_oxygen--;

  if(num_hydrogen_needed > 0) {
    pthread_cond_broadcast(&hydrogen);
  }
  pthread_mutex_unlock(&lock);

  printf("Oxygen bonding...\n");
  sleep(3);

  pthread_mutex_lock(&lock);
  done_bonding++;
  if(done_bonding == 3) {
    printf("H2O generated...\n");
    num_hydrogen_needed = 2;
    num_oxygen_needed = 1;
    bonding = 0;
    done_bonding = 0;

    // check if we need to wake anyone up
    if(num_oxygen >= 1 && num_hydrogen >= 2)
      pthread_cond_signal(&oxygen);
  }
  pthread_mutex_unlock(&lock);
  return NULL;
}

void*
hydro_thread(void *arg)
{
  printf("Hydrogen thread arrived\n");

  pthread_mutex_lock(&lock);
  num_hydrogen++;
  while((!bonding && num_oxygen < 1 && num_hydrogen < 2) ||
         (bonding && num_hydrogen_needed == 0)) {
    //printf("Hydrogen waiting for oxygen...\n");
    pthread_cond_wait(&hydrogen, &lock);
  }

  bonding = 1;
  num_hydrogen_needed--;
  num_hydrogen--;

  if(num_oxygen_needed > 0) {
    pthread_cond_signal(&oxygen);
  }

  pthread_mutex_unlock(&lock);

  printf("Hydrogen bonding...\n");
  sleep(3);

  pthread_mutex_lock(&lock);
  done_bonding++;
  if(done_bonding == 3) {
    printf("H2O generated...\n");
    num_hydrogen_needed = 2;
    num_oxygen_needed = 1;
    bonding = 0;
    done_bonding = 0;

    // check if we need to wake anyone up
    if(num_oxygen >= 1 && num_hydrogen >= 2)
      pthread_cond_signal(&oxygen);
  }
  pthread_mutex_unlock(&lock);
  return NULL;
}

int
main(int argc, char **argv)
{
  int i;
  pthread_t othreads[NUM_OXYGEN_THREADS];
  pthread_t hthreads[NUM_HYDROGEN_THREADS];

  /* DO NOT REMOVE THIS LINE */
	setbuf(stdout, NULL);
  srand(time(NULL));

  pthread_cond_init(&oxygen, 0);
  pthread_cond_init(&hydrogen, 0);
  pthread_cond_init(&barrier, 0);
  pthread_mutex_init(&lock, 0);

  for(i = 0; i < NUM_OXYGEN_THREADS; i++) {
    pthread_create(&othreads[i], 0, oxy_thread, 0);
  }

  for(i = 0; i < NUM_HYDROGEN_THREADS; i++) {
    pthread_create(&hthreads[i], 0, hydro_thread, 0);
  }


  for(i = 0; i < NUM_OXYGEN_THREADS; i++) {
    pthread_join(othreads[i], 0);
  }
  for(i = 0; i < NUM_HYDROGEN_THREADS; i++) {
    pthread_join(hthreads[i], 0);
  }

  pthread_cond_destroy(&oxygen);
  pthread_cond_destroy(&hydrogen);
  pthread_cond_destroy(&barrier);
  pthread_mutex_destroy(&lock);

  printf("Everything finished ... \n");

  return 0;
}
