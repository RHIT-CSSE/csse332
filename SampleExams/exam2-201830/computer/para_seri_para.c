/* Copyright 2018 Rose-Hulman */
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


void runWholeProcess() {
    // you should feel free to modify this function
    // including changing its parameter list, return type,
    // breaking it into several functions, etc.
    stageA();
    stageB();
    stageC();
}


int main(int argc, char **argv) {

    // you'll convert these into threads in step 1
    runWholeProcess();
    runWholeProcess();
    runWholeProcess();
        
    printf("Everything finished.\n");
}
