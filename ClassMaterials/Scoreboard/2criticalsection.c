#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MAX_THREADS 5

/*

  The goal for this activity is to create a critical section that 1 or
  2 threads can enter but not more.  This is easy to accomplish with
  the use of a semaphore initialized to 2, but we want to do it using
  semaphores initialized to no more than 1.

 */

void* thread(void *id) {
    printf("Entering critical section\n");
    sleep(1);
    printf("Leaving critical section\n");
}

int main(int argc, char *argv[]) {
    pthread_t threads[MAX_THREADS];
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
