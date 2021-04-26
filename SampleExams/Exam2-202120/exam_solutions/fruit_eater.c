/* Copyright 2021 Rose-Hulman */
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/**
 
 There are 8 bananas and 8 apples in the kitchen. Three kinds of eaters will
 go to the kitchen to consume the fruits. A banana eater will eat a banana
 every time go to the kitchen. An apple eater will eat an apple every time go
 to the kitchen. An BOTH eater will eat both an apple and a banana every time
 go to the kitchen.

 The kitchen can only allow TWO eaters to go in at the same time. The same
 type of fruit can not be eaten by multiple eaters at the same time, i.e.,
 only one thread can touch the global counters at a time.

 HINT:
 * I used 3 semaphores to solve this problem.

The sample output is like:
>>>>> Banana Eater is in the kitchen.
                Banana Eater just ate one of the 5 bananas.
>>>>> Both Eater is in the kitchen.
                Both Eater just ate one of the 5 apples.
                Both Eater just ate one of the 4 bananas.
<<<<< Both Eater LEFT in the kitchen.
>>>>> Both Eater is in the kitchen.
                Both Eater just ate one of the 4 apples.
                Both Eater just ate one of the 3 bananas.
<<<<< Banana Eater LEFT in the kitchen.
>>>>> Banana Eater is in the kitchen.
                Banana Eater just ate one of the 2 bananas.
<<<<< Banana Eater LEFT in the kitchen.
>>>>> Banana Eater is in the kitchen.
                Banana Eater just ate one of the 1 bananas.
<<<<< Banana Eater LEFT in the kitchen.
<<<<< Both Eater LEFT in the kitchen.
>>>>> Apple Eater is in the kitchen.
                Apple Eater just ate one of the 3 apples.
<<<<< Apple Eater LEFT the kitchen.
>>>>> Apple Eater is in the kitchen.
>>>>> Apple Eater is in the kitchen.
                Apple Eater just ate one of the 2 apples.
<<<<< Apple Eater LEFT the kitchen.
                Apple Eater just ate one of the 1 apples.
<<<<< Apple Eater LEFT the kitchen.
All done


 **/

int num_banana;
int num_apple;

sem_t kitchen, banana, apple;

void* banana_eater(void* arg) {
  sem_wait(&kitchen);
  printf(">>>>> Banana Eater is in the kitchen.\n");
  sem_wait(&banana);
  if (num_banana > 0) {
    printf("\t\tBanana Eater just ate one of the %d bananas.\n", num_banana);
    num_banana--;
  }
  sem_post(&banana);
  if (random() > random()) {  // Adds variety to output
    sleep(1);
  }
  sem_post(&kitchen);
  printf("<<<<< Banana Eater LEFT in the kitchen.\n");
}

void* apple_eater(void* arg) {
  sem_wait(&kitchen);
  printf(">>>>> Apple Eater is in the kitchen.\n");
  sem_wait(&apple);
  if (num_apple > 0) {
    printf("\t\tApple Eater just ate one of the %d apples.\n", num_apple);
    num_apple--;
  }
  sem_post(&apple);
  printf("<<<<< Apple Eater LEFT the kitchen.\n");
  sem_post(&kitchen);
}

void* both_eater(void* arg) {
  sem_wait(&kitchen);
  printf(">>>>> Both Eater is in the kitchen.\n");
  sem_wait(&apple);
  sem_wait(&banana);
  if (num_apple > 0 && num_banana > 0) {
    printf("\t\tBoth Eater just ate one of the %d apples.\n", num_apple);
    printf("\t\tBoth Eater just ate one of the %d bananas.\n", num_banana);
    num_apple--;
    num_banana--;
  }
  sem_post(&banana);
  sem_post(&apple);
  if (random() > random()) {  // Adds variety to output
    sleep(1);
  }
  sem_post(&kitchen);
  printf("<<<<< Both Eater LEFT in the kitchen.\n");
}

int main(int argc, char** argv) {
  num_apple = 5;
  num_banana = 5;
  sem_init(&kitchen, 0, 2);
  sem_init(&banana, 0, 1);
  sem_init(&apple, 0, 1);

  pthread_t threads[8];
  int i = 0;
  pthread_create(&threads[i++], NULL, banana_eater, NULL);
  pthread_create(&threads[i++], NULL, both_eater, NULL);
  pthread_create(&threads[i++], NULL, both_eater, NULL);
  pthread_create(&threads[i++], NULL, banana_eater, NULL);
  pthread_create(&threads[i++], NULL, banana_eater, NULL);
  pthread_create(&threads[i++], NULL, apple_eater, NULL);
  pthread_create(&threads[i++], NULL, apple_eater, NULL);
  pthread_create(&threads[i++], NULL, apple_eater, NULL);
  for (int j = 0; j < 8; j++) pthread_join(threads[j], NULL);

  printf("All done\n");
  return 0;
}
