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

int waiting_big_pig, waiting_small_pig, waiting_wolf, crossing;
sem_t big_pig_q, small_pig_q, wolf_q, mutex, boat;

void* small_pig(void* arg) {
  printf("Small Pig arrives\n");
  sem_wait(&mutex);
  waiting_small_pig++;
  if (waiting_small_pig > 1) {
    waiting_small_pig--;
    waiting_small_pig--;
    sem_post(&mutex);
    sem_wait(&boat);
    crossing = 2;
    sem_post(&small_pig_q);
  } else if (waiting_big_pig > 0) {
    waiting_small_pig--;
    waiting_big_pig--;
    sem_post(&mutex);
    sem_wait(&boat);
    crossing = 2;
    sem_post(&big_pig_q);
  } else {
    sem_post(&mutex);
    sem_wait(&small_pig_q);
  }

  printf("\tSmall Pig is on the boad to cross the river\n");
  sleep(1);
  printf("\tSmall Pig FINISHED\n");

  sem_wait(&mutex);
  crossing--;
  if (crossing == 0) sem_post(&boat);
  sem_post(&mutex);
}

void* wolf(void* arg) {
  printf("Wolf arrives\n");
  sem_wait(&mutex);
  waiting_wolf++;
  if (waiting_wolf > 1) {
    waiting_wolf--;
    waiting_wolf--;
    sem_post(&mutex);
    sem_wait(&boat);
    crossing = 2;
    sem_post(&wolf_q);
  } else if (waiting_big_pig > 0) {
    waiting_wolf--;
    waiting_big_pig--;
    sem_post(&mutex);
    sem_wait(&boat);
    crossing = 2;
    sem_post(&big_pig_q);
  } else {
    sem_post(&mutex);
    sem_wait(&wolf_q);
  }

  printf("\tWolf is on the boad to cross the river\n");
  sleep(1);
  printf("\tWolf FINISHED\n");

  sem_wait(&mutex);
  crossing--;
  if (crossing == 0) sem_post(&boat);
  sem_post(&mutex);
}

void* big_pig(void* arg) {
  printf("Big Pig arrives\n");
  sem_wait(&mutex);
  waiting_big_pig++;
  if (waiting_big_pig > 1) {
    waiting_big_pig--;
    waiting_big_pig--;
    sem_post(&mutex);
    sem_wait(&boat);
    crossing = 2;
    sem_post(&big_pig_q);
  } else if (waiting_small_pig > 0) {
    waiting_big_pig--;
    waiting_small_pig--;
    sem_post(&mutex);
    sem_wait(&boat);
    crossing = 2;
    sem_post(&small_pig_q);
  } else if (waiting_wolf > 0) {
    waiting_big_pig--;
    waiting_wolf--;
    sem_post(&mutex);
    sem_wait(&boat);
    crossing = 2;
    sem_post(&wolf_q);
  } else {
    sem_post(&mutex);
    sem_wait(&big_pig_q);
  }

  printf("\tBig Pig is on the boat to cross the river\n");
  sleep(1);
  printf("\tBig Pig FINISHED\n");

  sem_wait(&mutex);
  crossing--;
  if (crossing == 0) sem_post(&boat);
  sem_post(&mutex);
}

int main(int argc, char** argv) {
  waiting_big_pig = 0;
  waiting_small_pig = 0;
  waiting_wolf = 0;
  crossing = 0;
  sem_init(&mutex, 0, 1);
  sem_init(&boat, 0, 1);
  sem_init(&big_pig_q, 0, 0);
  sem_init(&small_pig_q, 0, 0);
  sem_init(&wolf_q, 0, 0);

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
