
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


//         INSTRUCTION
// Use condition variables to make  A, B, C print out in order.
// HINT: You need more than one condition variables

int a_done = 0;
int b_done = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cA = PTHREAD_COND_INITIALIZER;
pthread_cond_t cB = PTHREAD_COND_INITIALIZER;


void* thread_func_A(void* arg) {
  sleep(3);
  printf("A\n");
  pthread_mutex_lock(&lock);
  a_done = 1;
  pthread_cond_signal(&cA);
  pthread_mutex_unlock(&lock);
  return NULL;
}

void* thread_func_B(void* arg) {
  sleep(2);
  // the lock is held by B
  pthread_mutex_lock(&lock);
  while(!a_done) {
      pthread_cond_wait(&cA, &lock);
  }
  pthread_mutex_unlock(&lock);

  printf("B\n");
  pthread_mutex_lock(&lock);
  b_done = 1;
  pthread_cond_signal(&cB);
  pthread_mutex_unlock(&lock);


  return NULL;
}
void* thread_func_C(void* arg) {
  pthread_mutex_lock(&lock);
  while(!b_done) {
      pthread_cond_wait(&cB, &lock);
  }
  pthread_mutex_unlock(&lock);


    printf("C\n");
  return NULL;
}

int main(int argc, char *argv[]) {                    
  pthread_t tA, tB, tC;
  pthread_create(&tC, NULL, thread_func_C, NULL);
  pthread_create(&tB, NULL, thread_func_B, NULL);
  pthread_create(&tA, NULL, thread_func_A, NULL); 
  // join waits for the threads to finish
  pthread_join(tA, NULL); 
  pthread_join(tB, NULL); 
  pthread_join(tC, NULL); 
  return 0;
}
