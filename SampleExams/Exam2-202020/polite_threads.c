/* Copyright 2019 Rose-Hulman */
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>

/**

So in this system, we have have two kinds of threads politeA and
politeB.  There is a critical section that only one thread (of any
type) can be in at once.  

Polite threads are very concerned about being fair.  So if there are
more than one kind of thread waiting to enter the critical section,
polite threads always allow the type with more waiting to go first.
For example if there are 4 A threads and 2 B threads - it will look
like this:

B thread leaving critical section
4A 2B releasing an A
A thread entering cirtical section
A thread leaving critical section
3A 2B  releasing an A
A thread entering cirtical section
A thread leaving critical section
2A 2B  releasing an A <------ note in this case either could be released
A thread entering cirtical section
A thread leaving critical section
1A 2B releasing a B
etc.

Note you will have to add the print of "XA YB releasing a Z" yourself.

Example output:

A thread entering critical section
A thread leaving critical section
0A 1B releasing an B
B thread entering critical section
B thread leaving critical section
3A 1B releasing an A
A thread entering critical section
A thread leaving critical section
2A 1B releasing an A
A thread entering critical section
A thread leaving critical section
1A 1B releasing an A
A thread entering critical section
A thread leaving critical section
0A 1B releasing an B
B thread entering critical section
B thread leaving critical section
0A 0B no threads waiting
Everything finished.



**/


void* politeA(void * ignored) {

    printf("A thread entering critical section\n");
    sleep(1);
    printf("A thread leaving critical section\n");
    return NULL;
}

void* politeB(void * ignored) {

    printf("B thread entering critical section\n");
    sleep(1);
    printf("B thread leaving critical section\n");
    return NULL;
}

int main(int argc, char **argv) {

    int num_threads = 0;
    pthread_t threads[100];

    pthread_create(&threads[num_threads], NULL, politeA, NULL);
    num_threads++;
    pthread_create(&threads[num_threads], NULL, politeB, NULL);
    num_threads++;
    sleep(1);
    pthread_create(&threads[num_threads], NULL, politeA, NULL);
    num_threads++;
    pthread_create(&threads[num_threads], NULL, politeB, NULL);
    num_threads++;
    pthread_create(&threads[num_threads], NULL, politeA, NULL);
    num_threads++;
    pthread_create(&threads[num_threads], NULL, politeA, NULL);
    num_threads++;

    for(int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Everything finished.\n");

}
