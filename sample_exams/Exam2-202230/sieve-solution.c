
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_NUM 50
#define NUM_THREADS 3

char nums[MAX_NUM];

void* sieve_for(void* param) {

    long num = *((long*) param);
    printf("starting sieve for %ld\n", num);
    long current = num + num;
    while(current < MAX_NUM) {
        usleep(100);
        if(nums[current] != 'C') printf("sieve %ld found composite %ld\n", num, current);
        nums[current] = 'C'; // C for composite
        current = current + num;
    }
    printf("sieve for %ld finished\n", num);
    return NULL;
}

int main(int argc, char **argv)
{
  /* TODO: YOUR IMPLEMENTATION GOES HERE */
  int i;
  pthread_t threads[NUM_THREADS];
  long params[] = {2, 3, 5};

  for(int i = 2; i < MAX_NUM; i++) {
      nums[i] = 'P'; //mark all numbers as potentially prime
  }
  
  pthread_create(&threads[0], 0, sieve_for, &params[0]);
  pthread_create(&threads[1], 0, sieve_for, &params[1]);
  pthread_create(&threads[2], 0, sieve_for, &params[2]);

  int num_thread = 0;
  int current_num = 6;
  while(1) {
      int new_thread = num_thread % 3;
      printf("joinig %d\n", num_thread % 3);
      pthread_join(threads[new_thread], NULL);

      while(current_num < MAX_NUM && nums[current_num] == 'C') {
          current_num++;
      }
      if(current_num == MAX_NUM) break;
      params[new_thread] = current_num;
      pthread_create(&threads[new_thread], 0, sieve_for, &params[new_thread]);
      current_num++;
      num_thread++;
  }
  
  pthread_join(threads[(num_thread + 1) % 3], NULL);
  pthread_join(threads[(num_thread + 2) % 3], NULL);


  printf("primes are ");
  for(long i = 2; i < MAX_NUM; i++) {
      if(nums[i] == 'P') printf("%ld ", i);
  }
  printf("\n");
}
