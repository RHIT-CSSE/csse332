#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>

#define NUM_LOOPS 4

/*

   In this problem we imagine we have 4 files named: a b c d.  We also
   have 4 threads ab bc cd da.  Thread ab requires files a and b,
   thread bc requires files b and c, etc.  For each thread, the process
   calculates for some time, then requires both files at once to write
   to for output.  This process of calculating and writing occurs
   NUM_LOOPS times and calculation time is random.  We want to maximise
   paralellism in the system (e.g. not require that the various threads
   do their writes in a particular order, or require that only one
   thread write at one time).

   The given code uses mutex locks to protect files a b c d.  However,
   it also has a concurrency bug.

   PART 1 (10 points): In a comment, explain what the exisiting
   concurrency bug is in a detailed way (i.e. detailed enough that I
   could add sleeps & etc to reproduce it, though you do not have to
   actually add those sleeps).

   PART 2 (10 points) propose and write the code for a solution that
   meets ALL the above requirements and does not have the concurrency
   bug.  You MUST (in comment) explain why your solution does not have
   the bug for full credit.

*/

pthread_mutex_t a, b, c, d;

void* ab(void *ignore) {
  for(int i = 0; i < NUM_LOOPS; i++) {
    // calculate
    usleep(10000 + (random() % 10000));
    // now write
    pthread_mutex_lock(&a);
    pthread_mutex_lock(&b);

    printf("ab starting writing to files a & b\n");
    sleep(1);
    printf("ab finished writing to files a & b\n");

    pthread_mutex_unlock(&a);
    pthread_mutex_unlock(&b);
  }
  return NULL;
}

void* bc(void *ignore) {
  for(int i = 0; i < NUM_LOOPS; i++) {
    // calculate
    usleep(10000 + (random() % 10000));
    // now write
    pthread_mutex_lock(&b);
    pthread_mutex_lock(&c);

    printf("bc starting writing to files b & c\n");
    sleep(1);
    printf("bc finished writing to files b & c\n");

    pthread_mutex_unlock(&b);
    pthread_mutex_unlock(&c);
  }
  return NULL;
}

void* cd(void *ignore) {
  for(int i = 0; i < NUM_LOOPS; i++) {
    // calculate
    usleep(10000 + (random() % 10000));
    // now write
    pthread_mutex_lock(&c);
    pthread_mutex_lock(&d);

    printf("cd starting writing to files c & d\n");
    sleep(1);
    printf("cd finished writing to files c & d\n");

    pthread_mutex_unlock(&c);
    pthread_mutex_unlock(&d);
  }
  return NULL;
}

void* da(void *ignore) {
  for(int i = 0; i < NUM_LOOPS; i++) {
    // calculate
    usleep(10000 + (random() % 10000));
    // now write
    pthread_mutex_lock(&d);
    pthread_mutex_lock(&a);

    printf("da starting writing to files d & a\n");
    sleep(1);
    printf("da finished writing to files d & a\n");

    pthread_mutex_unlock(&d);
    pthread_mutex_unlock(&a);
  }
  return NULL;
}



int main(int argc, char **argv) {

  pthread_t threads[4];
  pthread_mutex_init(&a, 0);
  pthread_mutex_init(&b, 0);
  pthread_mutex_init(&c, 0);
  pthread_mutex_init(&d, 0);

  pthread_create(&threads[0], NULL, ab, NULL);
  pthread_create(&threads[1], NULL, bc, NULL);
  pthread_create(&threads[2], NULL, cd, NULL);
  pthread_create(&threads[3], NULL, da, NULL);

  for (int i=0; i<4; i++) {
    pthread_join(threads[i], NULL);
  }

  printf("All done\n");
  return 0;
}
