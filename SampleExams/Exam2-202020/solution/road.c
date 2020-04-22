/* Copyright 2019 Rose-Hulman */
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>

/**

So in this system, we imagine a road that narrows down to one lane at
a particular point.  There are two types of threads in the system,
southbound cars and northbound cars.

The rules are:

If there are no cars on the one-lane road, any car can enter.

If there is a southbound car on the one-lane road, any number of
southbound cards can enter but no northbound cards.

If there is a northbound car on the one-lane road, any number of
northbound cards can enter but no southbound cards.

Note that this system has the possibility of starvation - that is, if
southbound cards keep arriving nothbound may never get a chance.
That's OK.

**/


sem_t north, south, road;
int north_count = 0;
int south_count = 0;

void* southbound(void * ignored) {
    sem_wait(&south);
    south_count++;
    if(south_count == 1) {
        sem_wait(&road);
    }
    sem_post(&south);
    
    printf("starting southbound on the one-lane road\n");
    sleep(1);
    printf("finishing southbound on the one-lane road\n");

    sem_wait(&south);
    south_count--;
    if(south_count == 0) {
        sem_post(&road);
    }
    sem_post(&south);
    return NULL;
}

void* northbound(void * ignored) {

    sem_wait(&north);
    north_count++;
    if(north_count == 1) {
        sem_wait(&road);
    }
    sem_post(&north);

    printf("starting northbound on the one-lane road \n");
    sleep(1);
    printf("finishing northbound on the one-lane road\n");    

    sem_wait(&north);
    north_count--;
    if(north_count == 0) {
        sem_post(&road);
    }
    sem_post(&north);
    return NULL;
}




#define NUM_SOUTH 3
int main(int argc, char **argv) {

    int num_threads = 0;
    pthread_t threads[100];

    sem_init(&north, 0, 1);
    sem_init(&south, 0, 1);
    sem_init(&road, 0, 1);
    
    pthread_create(&threads[num_threads], NULL, northbound, NULL);
    num_threads++;
    pthread_create(&threads[num_threads], NULL, northbound, NULL);
    num_threads++;
    pthread_create(&threads[num_threads], NULL, northbound, NULL);
    num_threads++;
    pthread_create(&threads[num_threads], NULL, southbound, NULL);
    num_threads++;
    pthread_create(&threads[num_threads], NULL, southbound, NULL);
    num_threads++;
    pthread_create(&threads[num_threads], NULL, southbound, NULL);
    num_threads++;
    sleep(1);
    pthread_create(&threads[num_threads], NULL, northbound, NULL);
    num_threads++;

    
    for(int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Everything finished.\n");

}
