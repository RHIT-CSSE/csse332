#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdbool.h>

/*
   Several workers need to use two similar tools, i.e., toolA and toolB.  In
   total, there are 2 toolA and 3 toolB that can be used. Here are some
   policies they have to follow when using the tools:
   1) Each time, a worker will use one tool.  
   2) The workers have their preference: they all love toolA over toolB.
   Therefore, when choosing a tool to use, a worker always uses toolA if any
   toolA is available. 
   3) If there is no toolA available, the work will use one of the toolB.  
   4) If all the tools are running out, they will wait for other workers
   finishing using the tools. To make the case easy, we will let them always
   wait on toolBs. Namely, if a worker finds no tools available when he/she
   tries to grab a tool, then he/she will only use toolB whenever a toolB is
   available again.
 
Hint: I use 3 semaphores and one counter to solve the problem.

The desired output should look something like this:

Test case 1: All 7 worker arrive at the same time.
worker2 wants to use tool A.
work2 is using tool A :)
worker0 wants to use tool A.
work0 is using tool A :)
worker1 wants to use tool A.
work1 is using tool B reluctantly :(
worker3 wants to use tool A.
work3 is using tool B reluctantly :(
worker4 wants to use tool A.
worker5 wants to use tool A.
work4 is using tool B reluctantly :(
worker6 wants to use tool A.
work0 is done!
work2 is done!
work1 is done!
work5 is using tool B reluctantly :(
work3 is done!
work4 is done!
work6 is using tool B reluctantly :(
work5 is done!
work6 is done!


Test case 2: 5 worker arrive first, then 2 workers arrive later.
worker0 wants to use tool A.
work0 is using tool A :)
worker1 wants to use tool A.
work1 is using tool A :)
worker2 wants to use tool A.
work2 is using tool B reluctantly :(
worker3 wants to use tool A.
work3 is using tool B reluctantly :(
worker4 wants to use tool A.
work4 is using tool B reluctantly :(
work0 is done!
work1 is done!
work2 is done!
work3 is done!
work4 is done!
worker5 wants to use tool A.
work5 is using tool A :)
worker6 wants to use tool A.
work6 is using tool A :)
work5 is done!
work6 is done!

*/
sem_t toolA;
sem_t toolB;
sem_t lock;

int waiting = 0;

void *worker(void *arg)
{
  int workerID = *(int*) arg;
  bool use_toolB = false;
  printf("worker%d wants to use tool A.\n", workerID);
  sem_wait(&lock);
  waiting++;
  if(waiting > 2){
    sem_post(&lock);
    use_toolB = true;
    sem_wait(&toolB);
  }else{
    sem_post(&lock);
    sem_wait(&toolA);
  }

  if(use_toolB)
    printf("work%d is using tool B reluctantly :(\n", workerID);
  else
    printf("work%d is using tool A :)\n", workerID);

  sleep(1);


  sem_wait(&lock);
  waiting--;
  if(use_toolB)
    sem_post(&toolB);
  else
    sem_post(&toolA);
  sem_post(&lock);

  printf("work%d is done!\n", workerID);

}

int main(int argc, char **argv)
{
  setbuf(stdout, NULL);
  sem_init(&toolB, 0, 3);
  sem_init(&toolA, 0, 2);
  sem_init(&lock, 0, 1);

  pthread_t workers[7];
  int tid[7];


  printf("Test case 1: All 7 worker arrive at the same time.\n");
  for(int i = 0; i < 7; i++){
    tid[i] = i;
    pthread_create(&workers[i], 0, worker,tid + i );
  }

  for(int i = 0; i < 7; i++)
    pthread_join(workers[i], NULL);

  printf("\n\nTest case 2: 5 worker arrive first, then 2 workers arrive later.\n");
  int i = 0;
  for(i = 0; i < 5; i++){
    tid[i] = i;
    pthread_create(&workers[i], 0, worker,tid + i );
  }
  sleep(2);
  pthread_create(&workers[i], 0, worker,tid + i );
  i++;
  pthread_create(&workers[i], 0, worker,tid + i );


  for(i = 0; i < 7; i++)
    pthread_join(workers[i], NULL);
}
