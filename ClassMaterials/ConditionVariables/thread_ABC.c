
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


//         INSTRUCTION
// Use condition variables to make  A, B, C print out in order.
// HINT: You need more than one condition variables

void* thread_func_A(void* arg) {
    sleep(3);
    printf("A\n");
    return NULL;
}
void* thread_func_B(void* arg) {
    sleep(2);
    printf("B\n");
    return NULL;
}
void* thread_func_C(void* arg) {
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
