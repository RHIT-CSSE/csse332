/* Copyright 2021 Rose-Hulman */
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/**
 
There are three types of carts (A, B, and C) that want to go through a
tunnel. When passing through the tunnel, the carts must follow certain
policies as listed below:

1) When there is no cart in the tunnel, whoever arrives first can enter
(i.e., FCFS). 

2) If there is a cart currently passing the tunnel, the
late-arrived carts with the same type as the current passing cart(s) can
enter the tunel without waiting. While the carts with different type have to
wait. For example, if there is a B cart currently in the tunnel, then any B
cart arrives before the current B cart finishes passing can directly go into
the tunnel. However, A carts and C carts will wait until all the B carts
finish. 

3) With the two policies above, it could incur starvation. For
example, if there are B carts in the tunnel, then as long as there are
continous B carts arrving before the last B cart in the tunnel leaves, the
carts with other types (A and C) will never enter. Therefore, we need to
enable priority among the carts to prevent starvation of certain types of
carts. The priority order we want is A>B>C. And the way priority works is
that, when a cart with a higher priority arrives and waits for entering the
tunnel, it will prevent later-arrived low priority cars from entering the
tunnel. For example, assume there are B carts in the tunnel, if there is a
cart A arrives, then any B carts arrive after that cart A can't go into the
tunnel even they have the same type as the carts currently in the tunnel.
Similarly, A carts will prevent later-arrived C carts, and B carts will also
prevent later-arrived C carts.

Hint: My minimalistic solution uses in total three semaphores as I only use
one semaphore to protect all global variables.

If your implementation is correct, try to run program using the command (it
will take a few seconds to finish)

$ ./priority_tunnel|grep tunnel

You should see something like this:

Competing tunnel between B and C...
                ---> a <C> cart goes into the tunnel.
                ---> a <B> cart goes into the tunnel.
                ---> a <B> cart goes into the tunnel.
                ---> a <B> cart goes into the tunnel.
                ---> a <B> cart goes into the tunnel.
                ---> a <B> cart goes into the tunnel.
                ---> a <C> cart goes into the tunnel.
                ---> a <C> cart goes into the tunnel.
                ---> a <C> cart goes into the tunnel.
                ---> a <C> cart goes into the tunnel.
Competing tunnel between A and B...
                ---> a <B> cart goes into the tunnel.
                ---> an <A> cart goes into the tunnel.
                ---> an <A> cart goes into the tunnel.
                ---> an <A> cart goes into the tunnel.
                ---> an <A> cart goes into the tunnel.
                ---> an <A> cart goes into the tunnel.
                ---> a <B> cart goes into the tunnel.
                ---> a <B> cart goes into the tunnel.
                ---> a <B> cart goes into the tunnel.
                ---> a <B> cart goes into the tunnel.
Competing tunnel between A and C...
                ---> a <C> cart goes into the tunnel.
                ---> a <C> cart goes into the tunnel.
                ---> an <A> cart goes into the tunnel.
                ---> an <A> cart goes into the tunnel.
                ---> an <A> cart goes into the tunnel.
                ---> an <A> cart goes into the tunnel.
                ---> an <A> cart goes into the tunnel.
                ---> a <C> cart goes into the tunnel.
                ---> a <C> cart goes into the tunnel.
                ---> a <C> cart goes into the tunnel.


It is okay to have a few low-priority carts go before high-priorty carts. It is
because they arrived slightly earlier. But the overall printout order should follow
the priority order.
 **/


sem_t lock;
sem_t B, C;
int nA, nB, nC;

void* A_cart(void* arg) {
  printf("an <A> cart waiting...\t");
  sem_wait(&lock);
  nA++;
  if(nA == 1){
    sem_post(&lock);
    sem_wait(&B);
    sem_wait(&C);
  }else
    sem_post(&lock);

  printf("\n\t\t---> an <A> cart goes into the tunnel.\n");
  sleep(2);
  sem_wait(&lock);
  nA--;
  if (nA == 0) {
    sem_post(&C);
    sem_post(&B);
  }
  sem_post(&lock);
//   printf("\t\t\tan <A> cart LEFT the tunnel. --->\n");
}

void* B_cart(void* arg) {
  printf("a <B> cart waiting...\t");
  
  sem_wait(&B);
  sem_post(&B);
  sem_wait(&lock);
  nB++;
  if(nB == 1){
    sem_post(&lock);
    sem_wait(&C);
  }else
    sem_post(&lock);
  printf("\n\t\t---> a <B> cart goes into the tunnel.\n");
  sleep(1);
  sem_wait(&lock);
  nB--;
  if (nB == 0) {
    sem_post(&C);
  }
  sem_post(&lock);
//   printf("\t\t\ta <B> cart LEFT the tunnel. --->\n");
}

void* C_cart(void* arg) {
  printf("a <C> cart waiting...\t");
  sem_wait(&B);
  sem_post(&B);
  sem_wait(&C);
  sem_post(&C);
  printf("\n\t\t---> a <C> cart goes into the tunnel.\n");
  sleep(0.5);
//   printf("\t\t\ta <C> cart LEFT the tunnel. --->\n");
}

int main(int argc, char** argv) {
  sem_init(&lock, 0, 1);
  sem_init(&B, 0, 1);
  sem_init(&C, 0, 1);
  nA = 0, nB = 0, nC = 0;

  pthread_t threads[10];
  int i = 0;
  printf("\nCompeting tunnel between B and C...\n\n");
  while(i < 10){
    pthread_create(&threads[i++], NULL, C_cart, NULL);
    pthread_create(&threads[i++], NULL, B_cart, NULL);
    // pthread_create(&threads[i++], NULL, A_cart, NULL);
  }
  for (int j = 0; j < 10; j++) pthread_join(threads[j], NULL);
  sleep(2);
  i = 0;
  printf("\nCompeting tunnel between A and B...\n\n");
  while(i < 10){
    // pthread_create(&threads[i++], NULL, C_cart, NULL);
    pthread_create(&threads[i++], NULL, B_cart, NULL);
    pthread_create(&threads[i++], NULL, A_cart, NULL);
  }
  for (int j = 0; j < 10; j++) pthread_join(threads[j], NULL);
  sleep(2);
  i = 0;
  printf("\nCompeting tunnel between A and C...\n\n");
  while(i < 10){
    pthread_create(&threads[i++], NULL, C_cart, NULL);
    // pthread_create(&threads[i++], NULL, B_cart, NULL);
    pthread_create(&threads[i++], NULL, A_cart, NULL);
  }
  for (int j = 0; j < 10; j++) pthread_join(threads[j], NULL);

  printf("All done\n");
  return 0;
}
