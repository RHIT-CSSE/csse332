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

  The given code uses semaphores to protect files a b c d.  However,
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

/*
 This system can deadlock.  Every process requests a different
 semaphore first so if they all get their starting semaphore every
 process will have one and need one.

 Solution: reverse the order that da requests it's semaphores.  Now
 the system always requests is files in a canoncial order
 (alphabetical) so deadlocks cannot occur.
 */

sem_t a, b, c, d;

void* ab(void *ignore) {
    for(int i = 0; i < NUM_LOOPS; i++) {
        // calculate
        usleep(10000 + (random() % 10000));
        // now write
        sem_wait(&a);
        sem_wait(&b);

        printf("ab starting writing to files a & b\n");
        sleep(1);
        printf("ab finished writing to files a & b\n");
        
        sem_post(&a);
        sem_post(&b);
    }
    return NULL;
}

void* bc(void *ignore) {
    for(int i = 0; i < NUM_LOOPS; i++) {
        // calculate
        usleep(10000 + (random() % 10000));
        // now write
        sem_wait(&b);
        sem_wait(&c);

        printf("bc starting writing to files b & c\n");
        sleep(1);
        printf("bc finished writing to files b & c\n");
        
        sem_post(&b);
        sem_post(&c);
    }
    return NULL;
}

void* cd(void *ignore) {
    for(int i = 0; i < NUM_LOOPS; i++) {
        // calculate
        usleep(10000 + (random() % 10000));
        // now write
        sem_wait(&c);
        sem_wait(&d);

        printf("cd starting writing to files c & d\n");
        sleep(1);
        printf("cd finished writing to files c & d\n");
        
        sem_post(&c);
        sem_post(&d);
    }
    return NULL;
}

void* da(void *ignore) {
    for(int i = 0; i < NUM_LOOPS; i++) {
        // calculate
        usleep(10000 + (random() % 10000));
        // now write
        sem_wait(&a);
        sem_wait(&d);

        printf("da starting writing to files d & a\n");
        sleep(1);
        printf("da finished writing to files d & a\n");
        
        sem_post(&d);
        sem_post(&a);
    }
    return NULL;
}



int main(int argc, char **argv) {

    pthread_t threads[4];
    sem_init(&a, 0, 1);
    sem_init(&b, 0, 1);
    sem_init(&c, 0, 1);
    sem_init(&d, 0, 1);
    
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
