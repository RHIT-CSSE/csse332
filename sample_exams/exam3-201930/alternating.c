#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

/* 
   In this system there are two kinds of threads - red threads and
   black threads.  All of the threads need to enter a critical
   section.  Only one thread may be in the critical section at once
   but there is an additional rule - the two types of threads must
   alternate - red black red black etc.  If a red thread finishes it
   does not matter which of the waiting black threads are selected to
   go next (and the same is true for reds after a black finish).

   Red should be the first color to enter the critical section.

   Example output:

red thread 0 entering critical section
red thread 0 leaving critical section
black thread 0 entering critical section
black thread 0 leaving critical section
red thread 1 entering critical section
red thread 1 leaving critical section
black thread 1 entering critical section
black thread 1 leaving critical section
red thread 2 entering critical section
red thread 2 leaving critical section
black thread 2 entering critical section
black thread 2 leaving critical section
red thread 3 entering critical section
red thread 3 leaving critical section
black thread 3 entering critical section
black thread 3 leaving critical section
done


Note: my example output happens to be in numerical order but that is
not required, only the alternating is requried.


*/

#define NUM_THREADS 10

void* red_thread(void *num_p) {
    int num = *((int*) num_p);
    sleep(1);

    printf("red thread %d entering critical section\n", num);
    sleep(1);
    printf("red thread %d leaving critical section\n", num);
    return NULL;
}

void* black_thread(void *num_p) {
    int num = *((int*) num_p);
    // I didn't put a sleep in the black threads so it's obvious they
    // don't alternate by default

    printf("black thread %d entering critical section\n", num);
    sleep(1);
    printf("black thread %d leaving critical section\n", num);
    return NULL;
}

int main(int argc, char **argv) {

    int nums[NUM_THREADS];
    pthread_t reds[NUM_THREADS];
    pthread_t blacks[NUM_THREADS];
    
    for(int i = 0; i < NUM_THREADS; i++) {
        nums[i] = i;
        pthread_create(&reds[i], NULL, red_thread, &nums[i]);
        pthread_create(&blacks[i], NULL, black_thread, &nums[i]);
    }

    for(int i = 0; i < NUM_THREADS; i++) {
        nums[i] = i;
        pthread_join(reds[i], NULL);
        pthread_join(blacks[i], NULL);
    }

    printf("done");
    return 0;
}
