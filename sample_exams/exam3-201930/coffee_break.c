#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

/*

  In this problem there are 2 workers who work for a while and then
  take a coffe break together.  When they are both finished with their
  coffee, they resume working.  This cycle repeats.

  Neither worker should actually take a coffee break until both are ready.
  Neither worker should resume work until both are finished with their coffee.

  Use semaphores to ensure these constraints are met.

  Example output:

Worker A is working
Worker B is working
Worker A is ready for a coffee break
Worker B is ready for a coffee break
Worker B is taking a coffee break
Worker A is taking a coffee break
Worker A is ready to go back to work
Worker B is ready to go back to work
Worker B is working
Worker A is working
Worker A is ready for a coffee break
Worker B is ready for a coffee break
Worker B is taking a coffee break
Worker A is taking a coffee break
Worker A is ready to go back to work
Worker B is ready to go back to work
Worker B is working
Worker A is working
Worker A is ready for a coffee break
Worker B is ready for a coffee break
Worker B is taking a coffee break
Worker A is taking a coffee break
Worker A is ready to go back to work
Worker B is ready to go back to work
Worker B is working
Worker A is working
Worker A is ready for a coffee break
Worker B is ready for a coffee break
Worker B is taking a coffee break
Worker A is taking a coffee break
Worker A is ready to go back to work
Worker B is ready to go back to work
Worker B is working
Worker A is working
Worker A is ready for a coffee break
Worker B is ready for a coffee break
Worker B is taking a coffee break
Worker A is taking a coffee break
Worker A is ready to go back to work
Worker B is ready to go back to work
Worker B is working
Worker A is working
Worker A is ready for a coffee break
Worker B is ready for a coffee break
Worker B is taking a coffee break
Worker A is taking a coffee break
Worker A is ready to go back to work
Worker B is ready to go back to work
Worker B is working
Worker A is working
Worker A is ready for a coffee break
Worker B is ready for a coffee break
Worker B is taking a coffee break
Worker A is taking a coffee break
Worker A is ready to go back to work
Worker B is ready to go back to work
Worker B is working
Worker A is working
Worker A is ready for a coffee break
Worker B is ready for a coffee break
Worker B is taking a coffee break
Worker A is taking a coffee break
Worker A is ready to go back to work
Worker B is ready to go back to work
Worker B is working
Worker A is working
Worker A is ready for a coffee break
Worker B is ready for a coffee break
Worker B is taking a coffee break
Worker A is taking a coffee break
Worker A is ready to go back to work
Worker B is ready to go back to work
Worker B is working
Worker A is working
Worker A is ready for a coffee break
Worker B is ready for a coffee break
Worker B is taking a coffee break
Worker A is taking a coffee break
Worker A is ready to go back to work
Worker B is ready to go back to work
All done

 */

#define NUM_BREAKS 10

void *workerA(void* arg) {
  
  for (int i=0; i<NUM_BREAKS; i++) {
    
    // imagine this is doing work
    printf("Worker A is working\n");
    sleep(1);

    printf("Worker A is ready for a coffee break\n");

    printf("Worker A is taking a coffee break\n");
    sleep(1);

    printf("Worker A is ready to go back to work\n");
  }

  return NULL;
}

void *workerB(void* arg) {
  
  for (int i=0; i<NUM_BREAKS; i++) {
    
    // imagine this is doing work
    printf("Worker B is working\n");
    sleep(2);

    printf("Worker B is ready for a coffee break\n");

    printf("Worker B is taking a coffee break\n");
    sleep(2);

    printf("Worker B is ready to go back to work\n");
  }

  return NULL;
}

int main(int argc, char **argv) {

  pthread_t workerA_pid;
  pthread_t workerB_pid;
  
  pthread_create(&workerA_pid, NULL, workerA, NULL);
  pthread_create(&workerB_pid, NULL, workerB, NULL);
  
  pthread_join(workerA_pid, NULL);
  pthread_join(workerB_pid, NULL);
  
  printf("All done\n");
  return 0;
}
