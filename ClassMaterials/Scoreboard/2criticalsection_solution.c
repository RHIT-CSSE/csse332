#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_THREADS 5

/*

  The goal for this activity is to create a critical section that 1 or
  2 threads can enter but not more.  This is easy to accomplish with
  the use of a semaphore initialized to 2, but we want to do it using
  semaphores initialized to no more than 1.

 */


int num_in_cs;
int num_waiting;
sem_t mutex, queue;


void* thread(void *id) {
    sem_wait(&mutex);
    if(num_in_cs <=1) {
        num_in_cs++;
        sem_post(&mutex);
    } else {
        num_waiting++;
        sem_post(&mutex);
        sem_wait(&queue);
    }
    printf("Entering critical section\n");
    sleep(1);
    printf("Leaving critical section\n");
    sem_wait(&mutex);
    if(num_waiting > 0) {
        num_waiting--;
        sem_post(&queue);
    } else {
        num_in_cs--;
    }
    sem_post(&mutex);
}

int main(int argc, char *argv[]) {
    pthread_t threads[MAX_THREADS];
    num_in_cs = 0;
    num_waiting = 0;
    
    sem_init(&mutex, 0, 1);
    sem_init(&queue, 0, 0);
        
    int rc;
    for (int tid = 0; tid < MAX_THREADS; tid++) {
        rc = pthread_create(&threads[tid], NULL, thread, NULL);
        if (rc) {
            printf("ERROR creating thread; error code is %d.\n", rc);
            exit(1);
        }
    }

    for (int tid = 0; tid < MAX_THREADS; tid++) {
        pthread_join(threads[tid], NULL);
    }
    return 0;
}
