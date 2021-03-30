#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>

/* 
   In this system we have a variable number of threads, controlled by
   NUM_THREADS define.  Each thread is numbered 0 through NUM_THREADS
   - 1.  When the threads are started we want them to run in parallel
   (and they can print starting messages in any order).  However, we
   want the threads to finish in order (e.g. threads 0 should be first
   to finish, then thread 1, etc.).  If a thread finishes out of order
   it should wait till all previous numbered threads finish and then
   finish itself.

   Example output:

thread 7 starting
thread 1 starting
thread 9 starting
thread 2 starting
thread 6 starting
thread 8 starting
thread 3 starting
thread 4 starting
thread 5 starting
thread 0 starting
thread 0 finishing
thread 1 finishing
thread 2 finishing
thread 3 finishing
thread 4 finishing
thread 5 finishing
thread 6 finishing
thread 7 finishing
thread 8 finishing
thread 9 finishing
done

*/

#define NUM_THREADS 10

void* thread(void *num_p) {
    int num = *((int*) num_p);

    //sleep for a variable amount of time so the threads don't start in order
    // (which is something we don't want to change)
    usleep(random() % 10000);
    
    printf("thread %d starting\n", num);

    //sleep for a variable amount of time so the threads don't finish
    //in order naturally
    usleep(10000 + (random() % 10000) );

    // this is where a thread should wait if the lower numbered
    // threads haven't finished yet
    printf("thread %d finishing\n", num);
    return NULL;
}

int main(int argc, char **argv) {

    int nums[NUM_THREADS];
    pthread_t threads[NUM_THREADS];

    
    for(int i = 0; i < NUM_THREADS; i++) {
        nums[i] = i;
        pthread_create(&threads[i], NULL, thread, &nums[i]);
    }

    for(int i = 0; i < NUM_THREADS; i++) {
        nums[i] = i;
        pthread_join(threads[i], NULL);
    }

    printf("done\n");
    return 0;
}
