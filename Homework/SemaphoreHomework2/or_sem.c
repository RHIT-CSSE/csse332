#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>

/**
    The goal of sem_or is to create a new type of waiting that will
    wait on two semaphores at once, and then return when either
    semaphore is unlocked.

    The basic implementation of the idea is to create two new threads,
    each of which will wait on one of the two semaphores.  Then the
    original thread waits on a newly created semaphore.  When one of
    the newly created thread wakes up, it sees if it is the first one
    to awaken, and if so it posts on the newly created semaphore
    allowing the original thread to continue.

    This is more complex that it seems for two reasons:

    1. the two semaphores can be unlocked at almost the same time, so
    we must ensure that only 1 semaphore can be considered first.  We
    also must allow the main thread to determine which semaphore
    finished first before things are cleaned up.

    2. Because we want to be able to call sem_wait_or as much as we
    wish, all the data and semaphores associated must be allocated on
    the heap.  All that data must be freed when the second semaphore
    is posted.  This includes using sem_destory in the semaphores as
    well.  We'll use valgrind to check this is correct.




sempahore that will run two threads
    and return the thead result that finishes first. The first thread finished does not 
    need to do much additional work; however, the second thread will have to deal with any
    freeing of memory or destruction of semaphore components. Hint: Look into pthread_detach
    for some clues on removing an active thread.

    Example output:
    got y
    or called
    or called
    freeing y
    got y
    freeing y
    or wait returned
    or wait got y (0x5582829a8060)
    got x
    freeing x
    got x
    joining 0
    freeing x
    or wait returned
    joining 1
    joining 2
    joining 3
    joining 4
    Everything finished.

**/
struct all_data {
    pthread_t a_thread, b_thread;
    sem_t* first_finished;
    sem_t true_sem, mutex, wait_done;
};

struct param_t {
    sem_t* for_waiting;
    struct all_data* data;
};

    
void* or_func(struct param_t* params) {
    // Implement me

    return NULL;
}
    
sem_t* sem_wait_or(sem_t* a, sem_t* b) {
    // Implement me
}

sem_t x, y;

void* xfunc(void* ignored) {
    sem_wait(&x);
    printf("got x\n");
    sleep(1);
    printf("freeing x\n");
    sem_post(&x);
    return NULL;
}

void* yfunc(void* ignored) {
    sem_wait(&y);
    printf("got y\n");
    sleep(1);
    printf("freeing y\n");
    sem_post(&y);
    return NULL;
}


void* my_or(void* ignored) {
    printf("or wait got %s (%p)\n", result == &x ? "x" : "y", result);
    
    return NULL;
}

int main(int argc, char **argv) {
    pthread_t threads[100];
    int cur_thread = 0;
    
    sem_init(&x, 0, 0);
    sem_init(&y, 0, 1);

    pthread_create(&threads[cur_thread], NULL, xfunc, NULL);
    cur_thread++;
    pthread_create(&threads[cur_thread], NULL, xfunc, NULL);
    cur_thread++;
    pthread_create(&threads[cur_thread], NULL, yfunc, NULL);
    cur_thread++;
    pthread_create(&threads[cur_thread], NULL, yfunc, NULL);
    cur_thread++;

    // now the or
    pthread_create(&threads[cur_thread], NULL, my_or, NULL);
    cur_thread++;

    //sleep(4);
    for(int i = 0; i < cur_thread; i++) {

        pthread_join(threads[i], NULL);
        printf("joining %d\n", i);
    }
    printf("Everything finished.\n");
    
}
