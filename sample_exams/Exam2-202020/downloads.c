/* Copyright 2019 Rose-Hulman */
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>

/**

This system downloads data into the slots of an array called
data_array (well, actually data array just contains short strings but
we'll imagine it contains big structs filled with data).  Once the
data is downloaded in can be read.

There are two types of threads in this system downloaders and readers.

Downloaders download data, and it necessary for the system that
downloads proceed in paralell (i.e. one download should not block
others).  When a downloader downloads, it should pick the first slot
available (e.g. first download should be slot 0, 2nd download 1, etc.)
but two downloaders should never download to the same slot.  Use
semaphores to enforce this constraint.

Readers read a particular slot of downloaded data.  The read itself is
fast, but the reader should wait till the download is complete before
it reads.  Because reads are fast, it isn't important if they are in
paralell.  Use semaphores (maybe an array of semaphores?) to enforce
this constraint.

Example:

started downloading data into slot 1
started downloading data into slot 0
started downloading data into slot 2
finished downloading data into slot 1
finished downloading data into slot 2
I read COMPLETE DATA from slot 1
finished downloading data into slot 0
I read COMPLETE DATA from slot 1
I read COMPLETE DATA from slot 0
I read COMPLETE DATA from slot 2
I read COMPLETE DATA from slot 0
I read COMPLETE DATA from slot 2
Everything finished.

Note that even if we assign the slots in order to the downloaders,
they won't necessarily print in order as you can see in the example
above.

**/

#define MAX_DOWNLOADS 100

char* data_array[MAX_DOWNLOADS];
int cur_slot = 0; 

void* download(void * ignored) {
    int myslot = cur_slot;
    cur_slot++;

    printf("started downloading data into slot %d\n", myslot);
    data_array[myslot] = "PARTIAL GARBAGE";
    sleep(1);
    data_array[myslot] = "COMPLETE DATA";
    printf("finished downloading data into slot %d\n", myslot);
    
    return NULL;
}

void* reader(int * slot) {

    char* data = data_array[*slot];
    printf("I read %s from slot %d\n",data, *slot); 
    return NULL;
}


#define NUM_DOWNLOADERS 3

int main(int argc, char **argv) {

    pthread_t downloaders[NUM_DOWNLOADERS];
    pthread_t readers[NUM_DOWNLOADERS*2];
    
    for(int i = 0; i < NUM_DOWNLOADERS; i++) {
        pthread_create(&downloaders[i], NULL, download, NULL);
    }

    //we'll have one two readers for every downloader
    int params[NUM_DOWNLOADERS];
    for(int i = 0; i < NUM_DOWNLOADERS*2; i++) {
        params[i/2] = i/2;
        pthread_create(&readers[i], NULL, (void * (*)(void *)) reader, &params[i/2]);
    }
    
    for(int i = 0; i < NUM_DOWNLOADERS; i++) {
        pthread_join(downloaders[i], NULL);
    }

    for(int i = 0; i < NUM_DOWNLOADERS*2; i++) {
        pthread_join(readers[i], NULL);
    }

    printf("Everything finished.\n");

}
