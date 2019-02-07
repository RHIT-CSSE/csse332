/* Copyright 2016 Rose-Hulman */
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

/**
   In this system, there is one kind of thread.  Three copies of this
   thread should run at the same time.

   The system has 3 stages, A B C.  

   Compile this code like this:

      gcc -pthread para_seri_para.c -o para_seri_para
   
   STEP 1 (6 points): 

   Make the system run completely in parallel using threads.

   Sample output:

      Starting A
      Starting A
      Starting A
      Finishing A 
      Starting B
      Finishing A 
      Finishing A 
      Starting B
      Starting B
      Finishing B 
      Starting C
      Finishing B 
      Starting C
      Finishing B 
      Starting C
      Finishing C 
      Finishing C 
      Finishing C 
      Everything finished.
      

   STEP 2 (7 points)

   Make stage B serial.  A thread should attempt to go to B immediately
   after it finishes A, but only one thread should be allowed to do
   step B at a time.  For this part, it doesn't matter what you do
   with part C (it can be either serial or parallel, though it's
   parallel in my example output).

   Use semaphores to accomplish this.

   Sample output:

      Starting A
      Starting A
      Starting A
      Finishing A 
      Starting B
      Finishing A 
      Finishing A 
      Finishing B 
      Starting C
      Starting B
      Finishing C 
      Finishing B 
      Starting C
      Starting B
      Finishing C 
      Finishing B 
      Starting C
      Finishing C 
      Everything finished.

   STEP 3 (7 points)

   Stage C is parallel.  HOWEVER no thread should start Stage C until
   all threads have finished stage B.

   There are several approaches that can solve this step - all are
   allowed as long as they function as specified and avoid deadlocks
   and busywaiting.

   Sample output:
   
      Starting A
      Starting A
      Starting A
      Finishing A 
      Finishing A 
      Starting B
      Finishing A 
      Finishing B 
      Starting B
      Finishing B 
      Starting B
      Finishing B 
      Starting C
      Starting C
      Starting C
      Finishing C 
      Finishing C 
      Finishing C 
      Everything finished.

**/

sem_t semaphore;
sem_t semaphore2;
int passedB = 0;;

void stageA() {
    // you should not need to modify this function
    printf("Starting A\n");
    sleep(1);
    printf("Finishing A \n");
}

void stageB() {
    // you should not need to modify this function
    printf("Starting B\n");
    sleep(1);
    printf("Finishing B \n");
}

void stageC() {
    // you should not need to modify this function
    printf("Starting C\n");
    sleep(1);
    printf("Finishing C \n");
}


void* runWholeProcess(void* arg) {
    // you should feel free to modify this function
    // including changing it's parameter list return type
    // breaking it into several functions etc.
    stageA();
    sem_wait(&semaphore);
    stageB();
    passedB++;
    if(passedB == 3) sem_post(&semaphore2);
    sem_post(&semaphore);

    sem_wait(&semaphore2);
    sem_post(&semaphore2);
    
    stageC();
}


int main(int argc, char **argv) {
    pthread_t x, y, z;

    sem_init(&semaphore, 0, 1);
    sem_init(&semaphore2, 0, 0);
    
    pthread_create(&x, NULL, runWholeProcess, NULL);
    pthread_create(&y, NULL, runWholeProcess, NULL);
    pthread_create(&z, NULL, runWholeProcess, NULL);

    pthread_join(x, NULL);
    pthread_join(y, NULL);
    pthread_join(z, NULL);

    sem_destroy(&semaphore);
    sem_destroy(&semaphore2);
    
    printf("Everything finished.\n");
}
