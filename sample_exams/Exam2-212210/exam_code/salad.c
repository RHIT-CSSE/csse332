#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>



int num_cucumbers = 0;
int num_tomatoes = 0;

void *cook_fn(void *arg)
{
  int salad_orders = 10;
  int i;

  for(i = 0; i < salad_orders; ++i) {


    // chop and toss
    printf("Cook chopping and tossing...\n");
    num_cucumbers-=2;
    num_tomatoes-=3;

  }
  printf("Cook is done...\n");

  return NULL;
}

void *farmer_fn(void *arg)
{
  int i;
  int tot_cucumbers = 0, tot_tomatoes = 0;
  
  while(tot_cucumbers <= 20 && tot_tomatoes <= 30) {


    // produce the cucumbers and tomatoes
    printf("Farmer producing produce...\n");
    num_cucumbers += 1;
    tot_cucumbers += 1;
    num_tomatoes += 1;
    tot_tomatoes += 1;
    
    printf("Total cucumber produced: %d\n", tot_cucumbers);
    printf("Total tomatoes produced: %d\n", tot_tomatoes);
  }

  return NULL;
}

int
main(int argc, char **argv)
{
  pthread_t cook_th, farmer_th;
  pthread_create(&cook_th, 0, cook_fn, 0);
  pthread_create(&farmer_th, 0, farmer_fn, 0);

  pthread_join(cook_th, 0);
  pthread_join(farmer_th, 0);

  printf("Everything finished....\n");
}