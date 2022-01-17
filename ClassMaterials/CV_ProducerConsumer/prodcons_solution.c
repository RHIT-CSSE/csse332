#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * The goal of this activity is to implement the correct concurrency management
 * for the producer/consumer problem using condition variables.
 */

const int loops = 100;
#define MAX 10
#define NUM_PRODUCERS 5
#define NUM_CONSUMERS 5

int buffer[MAX];
int fill = 0;
int use = 0;
int count = 0;

pthread_mutex_t lock;
pthread_cond_t empty;
pthread_cond_t full;

/**
 * Put a value into the buffer and increment the fill index.
 */
void put(int value) {
    buffer[fill] = value;
    fill = (fill + 1) % MAX;
    count++;
}

/**
 * Get a value from the buffer and increment the use index.
 */
int get() {
    int value = buffer[use];
    use = (use + 1) % MAX;
    count--;
    return value;
}

/**
 * The producer can fill item in the buffer when empty
 */
void *
producer(void *arg) {
    int i;
    for (i = 0; i < loops; i++) {
        pthread_mutex_lock(&lock);
        while (count == MAX)
            pthread_cond_wait(&empty, &lock);
        put(i);
        pthread_cond_signal(&full);
        pthread_mutex_unlock(&lock);
        printf("Producer produced %d\n", i);
    }
}

/**
 * The consumer can consume items from the buffer when full
 */
void *
consumer(void *arg) {
    int value = 0, i = 0;
    for (; i < loops; i++) {
        pthread_mutex_lock(&lock);
        while (count == 0)
            pthread_cond_wait(&full, &lock);
        value = get();
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&lock);
        printf("Consumer consumed %d\n", value);
    }
}

int main(int argc, char **argv) {
    /* the threads */
    pthread_t producer_threads[NUM_PRODUCERS];
    pthread_t consumer_threads[NUM_CONSUMERS];

    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&empty, NULL);
    pthread_cond_init(&full, NULL);

    /* create the producer threads */
    for (int i = 0; i < NUM_PRODUCERS; ++i)
        pthread_create(&producer_threads[i], 0, producer, NULL);

    /* create the consumer threads */
    for (int i = 0; i < NUM_CONSUMERS; ++i)
        pthread_create(&consumer_threads[i], 0, consumer, NULL);

    /* wait for producers */
    for (int i = 0; i < NUM_PRODUCERS; ++i)
        pthread_join(producer_threads[i], NULL);

    /* wait for consumers */
    for (int i = 0; i < NUM_CONSUMERS; ++i)
        pthread_join(consumer_threads[i], NULL);
}
