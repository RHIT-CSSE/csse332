#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>

/**
    When threads are queued up on a semaphore, their order they are
    reawakened with posts is usually something similar to arrival
    order.  In this case though, what we'd like is a semaphore-ish
    thing that awakens threads based on priority.  So if multiple
    threads are waiting and there is a post on the semaphore, the
    thread with highest priority is the one reawakened.

    We'll limit the number of threads that can be waiting on a
    priority semaphore to MAX_PRI_SEM_SIZE.

    The way you will implement this is a structure pri_sem_t that
    contains an array of MAX_PRI_SEM_SIZE semaphores and a
    corresponding array of priority values.  When a user waits on the
    semaphore an its value is 0, you'll go through the list to find a
    unused slot, record the priority and waits on the particular slots
    semaphore.

    When a post occurs, you'll loop through the array to find the
    currently waiting slot with max priority & post on that slot.

    That's the basics.  What makes this interesting is:

    a.  We'll have to duplicate the behavior of semaphores with
    regards to storing a value, initializing, etc. only in this
    priority case.

    b.  We need this to be robust with concurrent accesses.  So if
    multiple threads post or wait on the priority semaphore we must
    keep the data consistent and also prevent deadlocks.  To do this,
    each pri_sem will also need a singular mutex that is locked before
    a thread modifies the pri_sem_t state.  This will pose some
    interesting conundrums.
    
    See comments on the methods below for specific details of the
    methods you must implement.


    Example output:

    thread with pri 1 waiting
    thread with pri 1 starting
    thread with pri 2 waiting
    thread with pri 3 waiting
    thread with pri 4 waiting
    thread with pri 5 waiting
    thread with pri 1 finished
    joining pri 1
    thread with pri 5 starting
    thread with pri 5 finished
    thread with pri 4 starting
    thread with pri 4 finished
    thread with pri 3 starting
    thread with pri 3 finished
    thread with pri 2 starting
    thread with pri 2 finished
    joining pri 2
    joining pri 3
    joining pri 4
    joining pri 5
    Everything finished.

**/
// max number of threads in the priority semaphore
#define MAX_PRI_SEM_SIZE 10

struct pri_sem_t {
    sem_t sems[MAX_PRI_SEM_SIZE];
    int priority[MAX_PRI_SEM_SIZE];
    sem_t mutex;
    // these are things I mentioned above, but you will also need more
    // values in the struct.  Put in what you need.

    // it should be possible to create multiple pri_sem_ts at once -
    // don't rely on global variables!
};

// initializes the semaphore with a starting value (positive or
// zero).  Should act like sem_init.
void pri_sem_init(struct pri_sem_t* pri_sem, int value) {
    // Implement me
    // don't forget to init your semaphores
}

// waits on the semaphore.  Includes a priority, 
void pri_sem_wait(struct pri_sem_t* pri_sem, int priority) {
    // Implement me
}

// posts on the semaphore, perhaps reawakening a waiting thread.
// Should act like sem_post, except if multiple threads are waiting
// its the highest prioirty semaphore that will be selected
//
// this method has some interesting subtlety that I think warrents a
// few hints.  But I don't think you can understand it easily till you
// attempt to implement it.  So go ahead and try and implement it, and
// then look at the bottom of the file for some discussion.
void pri_sem_post(struct pri_sem_t* pri_sem) {
    // Implement me
}

struct pri_sem_t my_pri_sem;

void* example_thread(void* priority) {
    // for ease of implementation, the void pointer is actually an int
    // this is technically unsafe (could be different sizes) but
    // it'll be good enough here
    int pri = (int) priority;
    printf("thread with pri %d waiting\n", pri);
    pri_sem_wait(&my_pri_sem, pri);
    printf("thread with pri %d starting\n", pri);
    sleep(1);
    printf("thread with pri %d finished\n", pri);
    pri_sem_post(&my_pri_sem);
    return NULL;
}

int main(int argc, char **argv) {
    pthread_t threads[100];
    int cur_thread = 0;

    pri_sem_init(&my_pri_sem, 1);
   
    pthread_create(&threads[cur_thread], NULL, example_thread, (void*) 1);
    cur_thread++;
    pthread_create(&threads[cur_thread], NULL, example_thread, (void*) 2);
    cur_thread++;
    pthread_create(&threads[cur_thread], NULL, example_thread, (void*) 3);
    cur_thread++;
    pthread_create(&threads[cur_thread], NULL, example_thread, (void*) 4);
    cur_thread++;
    pthread_create(&threads[cur_thread], NULL, example_thread, (void*) 5);
    cur_thread++;

   
    //sleep(1);
    //pri_sem_post(&my_pri_sem);
   
    for(int i = 0; i < cur_thread; i++) {

        pthread_join(threads[i], NULL);
        printf("joining pri %d\n", i + 1);
    }
    printf("Everything finished.\n");
   
}


/*************** MORE HINTS


Ok, so hopefully at this point you've realized that you cannot have
pri_sem_wait wait on the sems semaphore while holding the mutex or you
deadlock.  That is, you must have something like this in wait:

     sem_post(&pri_sem->mutex);
     sem_wait(&pri_sem->sems[target]);

This poses some challenges though.

The key issue is that when a post occurs, although you can do the post
on the sems semaphore you cannot ensure that the waiting thread has
actually started wait.  It could have switched out before the second
line above occured.

If you implement post straightforwardly, it seems likely that you want
to update the metadata indicating the slot in the array is free within
the post logic.  This leads to an obsure concurrency bug:

- Step 1 THREAD A: About to wait, but not yet waiting on sems[target].  Has
freed mutex.

- Step 2 THREAD B: Posts on pri_semaphore, aquires the mutex, decides to post
  on Thread As slot, posts, then frees the mutex

- Step 3 THREAD C: waits on pri_semaphore, aquires the mutex, decides to use
  As slot, waits, but is immediately unlocked because the semaphore's
  value is 1

- Step 4 THREAD A: finally waits on the sems[target] but ends up in
  waiting state because THREAD C has already stolen its post

However, if THREAD B doesn't update the metadata and posts on
sems[target] then releases the mutex, the same semaphore could
actually get freed twice.

Carefully consider if you can solve this problem.  I have my solution below.












My idea was to "pass" the mutex from THREAD B to THREAD A.  That is,
Thread B locks the mutex but does not unlock before returning.
Meanwhile, THREAD B knows it must have "aquired" the mutex when it
wakes up, so it posts to it after updating the metadata.  Because the
mutex is locked between these two events, no waits or posts can occur
in the meantime. */












