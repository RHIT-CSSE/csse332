/* Copyright 2021 Rose-Hulman */
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/**
In a water park, people take canoe for a short tour for fun. Here are some
constraints/regulations we have to work with when coordinating the canoe tour.

1) There is only one canoe in this park, i.e., one tour at a time. 
2) The tourists include both adult and kid (they are different pthreads in
our program here). An adult can ride the canoe alone. While a kid can not. A
kid must take the tour with an adult. The capacity of the canoe is exactly an
adult and a kid.
3) Assume all the adults are friendly that always voluntarily take a kid with
him/her if there are kids waiting. But if there is no kid waiting when an adult
starts the tour, the adult can go alone without delay and wait.
4) In the Kid and Adult function, both will print out messages before and after
finishing a tour. Particularly, for the Adult function, it will print out
different messages (see the commented code) after finishing canoeing to
identify whether or not this adult took the tour with a kid.

Hint: I used in total three semaphores to solve this problem. 

If your implementation is correct, try to run program using the command (it
will take a few seconds to finish)

$ make canoe
$ ./canoe|grep canoe

You should see something like this:

                ---> an Adult is canoeing.
                an Adult finished canoeing.--->
                ---> an Adult is canoeing.
                ---> a Kid is canoeing.
                an Adult finished canoeing with a Kid.--->
                ---> an Adult is canoeing.
                ---> a Kid is canoeing.
                an Adult finished canoeing with a Kid.--->
                ---> an Adult is canoeing.
                ---> a Kid is canoeing.
                an Adult finished canoeing with a Kid.--->
                ---> an Adult is canoeing.
                ---> a Kid is canoeing.
                an Adult finished canoeing with a Kid.--->
                ---> an Adult is canoeing.
                an Adult finished canoeing.--->

Given the pthreads setup, the program should always finish without
halting/blocking. If your program is halted, it indicates possible currentcy
issues. You need to fix them.

 **/


sem_t lock, canoe, kid;
int nC;

void* Adult(void* arg) {
  printf("an Adult waiting...\t");
  int takeKid = 0;
  sem_wait(&lock);
  if (nC > 0) {
    nC--;
    takeKid = 1;
  }
  sem_post(&lock);
  sem_wait(&canoe);
  if (takeKid) sem_post(&kid);
  printf("\n\t\t---> an Adult is canoeing.\n");
  sleep(0.5);
  sem_post(&canoe);
  if (takeKid)
    printf("\n\t\tan Adult finished canoeing with a Kid.---> \n");
  else
    printf("\n\t\tan Adult finished canoeing.---> \n");
}

void* Kid(void* arg) {
  printf("a Kid waiting...\t");
  sem_wait(&lock);
  nC++;
  sem_post(&lock);
  sem_wait(&kid);
  printf("\n\t\t---> a Kid is canoeing.\n");
}

int main(int argc, char** argv) {
  sem_init(&lock, 0, 1);
  sem_init(&canoe, 0, 1);
  sem_init(&kid, 0, 0);
  nC = 0;

  pthread_t threads[10];
  int i = 0;

  pthread_create(&threads[i++], NULL, Adult, NULL);
  pthread_create(&threads[i++], NULL, Kid, NULL);
  sleep(1);
  pthread_create(&threads[i++], NULL, Kid, NULL);
  pthread_create(&threads[i++], NULL, Kid, NULL);
  pthread_create(&threads[i++], NULL, Kid, NULL);
  sleep(1);
  pthread_create(&threads[i++], NULL, Adult, NULL);
  pthread_create(&threads[i++], NULL, Adult, NULL);
  pthread_create(&threads[i++], NULL, Adult, NULL);
  pthread_create(&threads[i++], NULL, Adult, NULL);
  pthread_create(&threads[i++], NULL, Adult, NULL);
  for (int j = 0; j < 10; j++) pthread_join(threads[j], NULL);
  printf("All done\n");
  return 0;
}
