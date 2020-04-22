/* Copyright 2020 Rose-Hulman */
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
southbound cars can enter but no northbound cards.

If there is a northbound car on the one-lane road, any number of
northbound cars can enter but no southbound cards.

Note that this system has the possibility of starvation - that is, if
southbound cards keep arriving nothbound may never get a chance.
That's OK.

**/


void* southbound(void * ignored) {


    printf("starting southbound on the one-lane road\n");
    sleep(1);
    printf("finishing southbound on the one-lane road\n");
    
    return NULL;
}

void* northbound(void * ignored) {

    printf("starting northbound on the one-lane road \n");
    sleep(1);
    printf("finishing northbound on the one-lane road\n");    
    return NULL;
}




#define NUM_SOUTH 3
int main(int argc, char **argv) {

    int num_threads = 0;
    pthread_t threads[100];

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
