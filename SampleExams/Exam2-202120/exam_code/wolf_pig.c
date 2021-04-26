/* Copyright 2021 Rose-Hulman */
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 There are three types of animals (i.e., wolf, small pig, big pig) trying to
 take a boad and cross a river. The boat can take two animals each time. And
 more strictly, the boat won't go until two seats are occupied. Namely,
 animals must cross river as pairs. However, certain animals can not on the
 same boat at the same time. That is WOLF and Small Pig. When an animal
 arrives and waits for taking the boat, they have their preference/priority
 to pick a "boat-mate".

 As a big pig, the priority for the boat-mate (from the highest to the
 lowest) is 1) another big pig, 2) small pig, and 3) wolf.

 As a small pig, the priority for the boat-mate (from the highest to the
 lowest) is 1) another small pig, 2) big pig. It never takes the boat with a
wolf.

 As a wolf, the priority for the boat-mate (from the highest to the
 lowest) is 1) another wolf, 2) big pig. It never takes the boat with a small
pig.

There may have several way to solve this problem, but here is what I recommend.
* Use three queues for three types of animals.
* When an animal arrives and waits for taking boat, it checks on the status for
different queues and acts according to its priority.

A huge hint!!!!!!!!!!!!!
This is similar to the "band" problem but slightly more difficult.

The sample out is like:
Wolf arrives
Big Pig arrives
        Big Pig is on the boat to cross the river
Small Pig arrives
Wolf arrives
Wolf arrives
        Wolf is on the boad to cross the river
Small Pig arrives
Small Pig arrives
Big Pig arrives
        Big Pig FINISHED
        Wolf FINISHED
        Wolf is on the boad to cross the river
        Wolf is on the boad to cross the river
        Wolf FINISHED
        Wolf FINISHED
        Small Pig is on the boad to cross the river
        Small Pig is on the boad to cross the river
        Small Pig FINISHED
        Small Pig FINISHED
        Big Pig is on the boat to cross the river
        Small Pig is on the boad to cross the river
        Small Pig FINISHED
        Big Pig FINISHED
All done

Note that, occasionally, even a correct program can freeze because the last
two animals are a wolf and a small pig. But it seldom occurs.
 **/


void* small_pig(void* arg) {
  printf("Small Pig arrives\n");
  // Critical Section
  printf("\tSmall Pig is on the boad to cross the river\n");
  sleep(1);
  printf("\tSmall Pig FINISHED\n");

}

void* wolf(void* arg) {
  printf("Wolf arrives\n");
  // Critical Section

  printf("\tWolf is on the boad to cross the river\n");
  sleep(1);
  printf("\tWolf FINISHED\n");

}

void* big_pig(void* arg) {
  printf("Big Pig arrives\n");
  // Critical Section

  printf("\tBig Pig is on the boat to cross the river\n");
  sleep(1);
  printf("\tBig Pig FINISHED\n");

}

int main(int argc, char** argv) {
  //Initialize your semaphores here

  pthread_t threads[8];
  int i = 0;
  pthread_create(&threads[i++], NULL, wolf, NULL);
  sleep(1);
  pthread_create(&threads[i++], NULL, big_pig, NULL);
  pthread_create(&threads[i++], NULL, small_pig, NULL);
  pthread_create(&threads[i++], NULL, wolf, NULL);
  pthread_create(&threads[i++], NULL, wolf, NULL);
  pthread_create(&threads[i++], NULL, small_pig, NULL);
  pthread_create(&threads[i++], NULL, small_pig, NULL);
  pthread_create(&threads[i++], NULL, big_pig, NULL);
  for (int j = 0; j < 8; j++) pthread_join(threads[j], NULL);

  printf("All done\n");
  return 0;
}
