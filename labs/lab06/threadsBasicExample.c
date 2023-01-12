#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_THREADS 5

void *PrintHelloWorld(void *id) {
  long *tidPtr;
  tidPtr = (long *)id;
  printf("Hello World! from thread %ld!\n", *tidPtr);
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  pthread_t threads[MAX_THREADS];
  int rc;
  long tid,  tids[MAX_THREADS];
  for (tid = 0; tid < MAX_THREADS; tid++) {
    tids[tid] = tid;
    printf("About to create thread %ld.\n", tid);
    rc = pthread_create(&threads[tid], NULL, PrintHelloWorld, (void
                                                               *)&tids[tid]);
    if (rc) {
      printf("ERROR creating thread; error code is %d.\n", rc);
      exit(1);
    }
  }

  for (tid = 0; tid < MAX_THREADS; tid++) {
    pthread_join(threads[tid], NULL);
  }
  return 0;
}
