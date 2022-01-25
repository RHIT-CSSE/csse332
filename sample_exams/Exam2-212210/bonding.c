#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_OXYGEN_THREADS 3
#define NUM_HYDROGEN_THREADS 6





void*
oxy_thread(void *arg)
{
  printf("Oxygen thread arrived\n");
  
  

  printf("Oxygen bonding...\n");
  sleep(3);

  return NULL;
}

void*
hydro_thread(void *arg)
{
  printf("Hydrogen thread arrived\n");

  printf("Hydrogen bonding...\n");
  sleep(3);

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

  printf("Everything finished ... \n");

  return 0;
}
