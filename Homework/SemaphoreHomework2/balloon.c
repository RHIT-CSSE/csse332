/* Copyright 2016 Rose-Hulman */
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_CHILDREN 15

/**
   Bryan the balloon artist has been hired for a kid's birthday party. 
   All 15 kids at the party want a balloon animal, but Bryan is inexperienced
   and can only make one balloon animal at a time. Each kid has to wait for
   their turn. When it is their turn, each kid tells Bryan what animal they want
   and waits for him to make it before running away gleefully. 
   Bryan is impatient, so kids are not allowed to change their mind about what 
   animal they want while he is making it. 

   This system has two types of threads: the kid thread and the Bryan thread. 
   The Bryan thread waits for a kid thread to come to him and tell him what 
   they want. Then, he makes their balloon animal and gives it to them. 

  Use semaphores to enforce these constraints.
**/

char **animalName;

void *bryanThread(void *arg) {
  int kidsLeft = NUM_CHILDREN;

  while (kidsLeft > 0) {
    printf("I am making a balloon %s now.\n", *animalName);
    kidsLeft--;
  }

  printf("I'm all done making balloons. Bye, kids!\n");
  return NULL;
}

void *kidThread(void *arg) {
  char **name = (char**)arg;
  if (random() > random()) {  // Some kids are slow to get in line
    sleep(1);
  }

  printf("Please make a balloon %s.\n", *name);
  animalName = name;


  printf("Thanks for my balloon %s, Bryan!\n", *name);

  return NULL;
}

int main(int argc, char **argv) {
  pthread_t tid[NUM_CHILDREN];
  pthread_t bryan;
  char *name = "flamingo";
  animalName = &name;

  const char *animalNames[NUM_CHILDREN];
  animalNames[0] = "giraffe";
  animalNames[1] = "alligator";
  animalNames[2] = "monkey";
  animalNames[3] = "starfish";
  animalNames[4] = "cat";
  animalNames[5] = "pterodactyl";
  animalNames[6] = "rhino";
  animalNames[7] = "praying mantis";
  animalNames[8] = "goose";
  animalNames[9] = "worm";
  animalNames[10] = "dog";
  animalNames[11] = "snail";
  animalNames[12] = "lion";
  animalNames[13] = "catfish";
  animalNames[14] = "lemur";

  pthread_create(&bryan, NULL, bryanThread, NULL);

  int i;
  for (i = 0; i < NUM_CHILDREN; i++) {
    pthread_create(&tid[i], NULL, kidThread, animalNames+i);
  }

  for (i = 0; i < NUM_CHILDREN; i++) {
    pthread_join(tid[i], NULL);
  }

  pthread_join(bryan, NULL);

  printf("Everything finished.\n");
}
