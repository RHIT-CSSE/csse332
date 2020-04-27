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

    This is more complex that it seems for three reasons:

    1. the two semaphores can be unlocked at almost the same time, so
    we must ensure that only 1 semaphore can be considered first.  The
    sem_wait_or function also returns a pointer to the semaphore that
    finished first - we must ensure that the second semaphore thread
    does not dispose of the relevant resources before that value is
    safely returned.

    2. Because we want to be able to call sem_wait_or as much as we
    wish, all the data and semaphores associated must be allocated on
    the heap (e.g. with malloc).  

    3. It is not safe to free the heap data when the first semaphore
    returns.  There is a second thread, waiting on the 2nd semaphore
    that requires resources to run.  For that reason, the sem_wait_or
    process is not complete until both of the two branches of the or
    have recieved a post.  This is a natural consequence of the fact
    that there is no safe way to remove a thread from a semaphore
    queue, once it is waiting.

    In the second branch, the resources of the sem_wait_or must be
    fully cleaned up.

    This includes (NOT necessarily in order):

        a. ensuring that the first thread has returned the pointer to
        the first semaphore to the main thread

        b. doing a sem_destroy on all semaphores created in heap
        memory at the beginning of the process

        c. ensuring that a thread_join has been done on the first
        thread (either here or elsewhere) to free its resources

        d. ensuring that a pthread_detach has been done on the 2nd
        thread (check out the main page, but this in essence frees the
        resources of a thread that will not be pthread_joined).

        e. freeing all resources malloced in the sem_wait_or process

        f. doing a post on the 2nd semaphore - this last one is
        debatable but the basic idea is that if we doing something
        like this:

        sem_wait_or(&sema, &semb)
        sem_wait_or(&sema, &semb)
        
        two posts to either semaphore OR a combination should unlock
        the process.  Put another way, the sem_or only needs to be
        unlocked once - once it has been, the other thread "passes"
        any post it gets back into the semaphore as if it was not
        really in the queue.

    We will use valgrind to ensure your system does not leak
    resources.

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
    // store whatever you need to get this working
    // just be sure you clean up
};


    
    
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
    sem_t *result = sem_wait_or(&x, &y);
    printf("or wait got %s (%p)\n", result == &x ? "x" : "y", result);
    if(result != &x && result != &y) {
        printf("ERROR or returned a bogus result\n");
        return NULL;
    }
    if(result == &x) sem_post(&y);
    if(result == &y) sem_post(&x);
    return NULL;

}

int main(int argc, char **argv) {
    pthread_t threads[100];
    int cur_thread = 0;

    // to see the or behavior, only leave one of these unlocked
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

    // ensure all of the threads of one type have finished
    sleep(1);
    
    // now the my_or will have to use one of these semaphores but not
    // the other.  However, having used one, it unlocks the other so
    // the other side can finish its threads too.
    pthread_create(&threads[cur_thread], NULL, my_or, NULL);
    cur_thread++;

    sleep(5);
    for(int i = 0; i < cur_thread; i++) {

        pthread_join(threads[i], NULL);
        printf("joining %d\n", i);
    }
    printf("Everything finished.\n");
    
}
