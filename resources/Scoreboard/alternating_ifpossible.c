#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>

/* 
   In this system there are two kinds of threads - red threads and
   black threads.  All of the threads need to enter a critical
   section.  Only one thread may be in the critical section at once
   but there is an additional rule - the two types of threads should
   alternate if possible.  That is if red finishes and there a black
   waiting, then a black should go (and vice versa).  However, if no
   black is available red may invoke another red.

   HINT: you'll want a variable to track if something is in the
   critical section and a count of reds and blacks waiting.

   HINT 2: this one has some edge cases!  Be careful!

   Example output:

red thread 0 entering critical section
red starts waiting
red starts waiting
red starts waiting
red starts waiting
red thread 0 leaving critical section
red thread 1 entering critical section
black starts waiting
black starts waiting
black starts waiting
black starts waiting
black starts waiting
red thread 1 leaving critical section
black thread 0 entering critical section
black thread 0 leaving critical section
red thread 2 entering critical section
red thread 2 leaving critical section
black thread 1 entering critical section
black thread 1 leaving critical section
red thread 3 entering critical section
red thread 3 leaving critical section
black thread 2 entering critical section
black thread 2 leaving critical section
red thread 4 entering critical section
red thread 4 leaving critical section
black thread 3 entering critical section
black thread 3 leaving critical section
black thread 4 entering critical section
black thread 4 leaving critical section

*/

sem_t mutex, red_queue, black_queue;
bool in_critical_section = false;
int red_queue_waiting = 0;
int black_queue_waiting = 0;

#define NUM_THREADS 5

void* red_thread(void *num_p) {
    int num = *((int*) num_p);

    printf("red thread %d entering critical section\n", num);
    sleep(1);
    printf("red thread %d leaving critical section\n", num);
    return NULL;
}

void* black_thread(void *num_p) {
    int num = *((int*) num_p);

    printf("black thread %d entering critical section\n", num);
    sleep(1);
    printf("black thread %d leaving critical section\n", num);
    return NULL;
} 

int main(int argc, char **argv) {

    int nums[NUM_THREADS];
    pthread_t reds[NUM_THREADS];
    pthread_t blacks[NUM_THREADS];

    sem_init(&mutex, 0, 1);
    sem_init(&red_queue, 0, 0);
    sem_init(&black_queue, 0, 0);
    
    for(int i = 0; i < NUM_THREADS; i++) {
        nums[i] = i;
        pthread_create(&reds[i], NULL, red_thread, &nums[i]);
    }

    sleep(2); //wait a while before we start the blacks
    for(int i = 0; i < NUM_THREADS; i++) {
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
