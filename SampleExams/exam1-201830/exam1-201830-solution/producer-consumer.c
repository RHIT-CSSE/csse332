/* Copyright 2016 Rose-Hulman */
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

/*

So in this problem we are processing data "chunks".  The process takes
2 steps, and step 2 cannot begin before step 1 completes.  In the
given code, we are processing the chunks one at a time.

BUILDING THIS CODE

Compile like this:

gcc -pthread producer-consumer.c -o prodcon

STEP 1 (15 Points):

We would like to take advange of the fact that we can work on a
different chunk's step 1 at the same time we are processing step 2.
But we don't want to start a large number of threads.  Instead, we
want a system that has two threads:

THREAD1 "Producer".  This thread goes through the list of chunks,
processing each chunk's Step 1.  After finishing each chunk, it will
update a global state indicating that it is safe to proceed with Step
2 for that chunk.

It always processes the chunks as fast as possible, regardless of whether
the consumer is "behind".

THREAD2 "Consumer".  This thread waits until a new chunk is available,
then runs that chunk's Step 2.  If there are many chunks ready
(because the producer is faster) the consumer will still only process
one at a time.  After finishing a chunk, this thread will immediately
process the next chunk if the producer has marked it as ready.  If the
producer has not marked the next chunk as ready, the thread will wait
(perhaps in a loop?) till a chunk becomes available.

Both threads exit once all chunks have been processed.

Here's some sample output with 3 chunks.  In this case we've set it up
so that the producer produces faster than the consumer can consume:

START STEP1 chunk 0
END   STEP1 chunk 0
START STEP1 chunk 1
START STEP2 chunk 0
END   STEP1 chunk 1
START STEP1 chunk 2
END   STEP1 chunk 2
producer done
END   STEP2 chunk 0
START STEP2 chunk 1
END   STEP2 chunk 1
START STEP2 chunk 2
END   STEP2 chunk 2
consumer done
Everything finished

You can decide what you want the global state that the producer and
the consumer use to communicate is...but because the producer always
processes the chunks in order it doesn't need to be complex.

You can use semaphores if you wish here (so long as your threads show
the desired behavior) but semaphores are not necessary to solve this
subproblem.

Be sure to vary the step1 time and step2 time in your testing to
ensure your code works in all cases.

STEP 2:

Now we would like to improve the system by having 2 consumer threads
(but still one producer).  Assuming there are two chunks available,
both consumers should be able to run in parallel, but they should never
work on the same chunk.

You'll have to use semaphores in this subproblem to prevent both consumers 
from taking the same chunk if they both finish at the same time.  To do 
this, you'll need to add some more data to the global state of the system.
Be sure that semaphores protect not only writes to this global data,
but reads as well - otherwise concurrency problems have a tendecy to
creep in.

Here's some sample output (again with the producer producing very
fast):

START STEP1 chunk 0
END   STEP1 chunk 0
START STEP1 chunk 1
START STEP2 chunk 0
END   STEP1 chunk 1
START STEP1 chunk 2
START STEP2 chunk 1
END   STEP1 chunk 2
producer done
END   STEP2 chunk 0
START STEP2 chunk 2
END   STEP2 chunk 1
consumer done
END   STEP2 chunk 2
consumer done
Everything finished

 */
    
#define NUM_CHUNKS 3
#define STEP1_TIME 5
#define STEP2_TIME 100

int producer_position;
int consumer_position;
sem_t semaphore;


//no need to edit this function
void do_step1(int chunk_num) {
    printf("START STEP1 chunk %d\n", chunk_num);
    usleep(STEP1_TIME);
    printf("END   STEP1 chunk %d\n", chunk_num);
}

//no need to edit this function
void do_step2(int chunk_num) {
    printf("START STEP2 chunk %d\n", chunk_num);
    usleep(STEP2_TIME);
    printf("END   STEP2 chunk %d\n", chunk_num);
}


void* producer(void* p) {

    for(producer_position = 0; producer_position < NUM_CHUNKS; producer_position++) {
        do_step1(producer_position);
    }
    
    printf("producer done\n");
    pthread_exit(NULL);
}

void* consumer(void* p) {

    int my_position;
    while(1) {
        sem_wait(&semaphore);
        if(consumer_position < producer_position) {
            my_position = consumer_position;
            consumer_position++;
            sem_post(&semaphore);
            do_step2(my_position);               
        } else {
            if(consumer_position == NUM_CHUNKS) {
                printf("consumer done\n");
                sem_post(&semaphore);
                pthread_exit(NULL);
                return NULL;
            }
            sem_post(&semaphore);
        }
        
    }
}


int main(int argc, char **argv) {
    pthread_t threads[3];

    sem_init(&semaphore, 0, 1);
    producer_position = 0;
    consumer_position = 0;
    // to fully test your code, feel free to try other configurations
    // (e.g. a couple thread1s running at the same time, etc.)
    pthread_create(&threads[0], NULL, producer, NULL);
    pthread_create(&threads[1], NULL, consumer, NULL);
    pthread_create(&threads[2], NULL, consumer, NULL);

    int i;
    for (i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Everything finished\n");

    return 0;
}
