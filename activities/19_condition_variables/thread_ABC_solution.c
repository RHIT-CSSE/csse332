
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


//         INSTRUCTION
// Use condition variables to make  A, B, C print out in order.
// HINT: You need more than one condition variables

int A_done, B_done;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cA = PTHREAD_COND_INITIALIZER;
pthread_cond_t cB = PTHREAD_COND_INITIALIZER;

void* thread_func_A(void* arg) {
    sleep(3);
    pthread_mutex_lock(&lock);
    printf("A\n");
    A_done = 1;
    pthread_cond_signal(&cA);
    pthread_mutex_unlock(&lock);
    return NULL;
}
void* thread_func_B(void* arg) {
    sleep(2);
    pthread_mutex_lock(&lock);
    while (A_done == 0)
        pthread_cond_wait(&cA, &lock);
    printf("B\n");
    B_done = 1;
    pthread_cond_signal(&cB);
    pthread_mutex_unlock(&lock);
    return NULL;
}
void* thread_func_C(void* arg) {
    pthread_mutex_lock(&lock);
    while (B_done == 0)
        pthread_cond_wait(&cB, &lock);
    printf("C\n");
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main(int argc, char *argv[]) {                    
    A_done = 0;
    B_done = 0;
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
