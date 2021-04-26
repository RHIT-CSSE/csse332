/* Copyright 2021 Rose-Hulman */
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

/**

There is a special intersection that people can cross. There are two types of
people will cross this intersection: the leaders and followers. The rules of
crossing the intersection are listed as follows:

1. Followers can not cross alone. When followers arrive, they need to wait in a
queue until any leader arrives and lead them to cross the intersection together.

2. Only one leader can cross at a time. Other leaders arrive later have to
wait until the current leader finishes crossing.

3. When a leader is ready to cross, it will lead all followers that are
currently waiting in the queue to cross the intersection together. Any
followers arrive after the moment the current leader starts to cross need to
wait for next leader.

HINT:

* You need a semaphore as a queue to hold the waiting followers.
* Two or three semaphores are enough for this question (I used two).
Don't overthink.
* It involes sem_post in a loop.
* An effective/efficient implementation will even prevent followers going to
that dedicated waitting queue while a leader is crossing. But your solution may vary.

The sample output looks like this:

```
follower 0 is waiting.
follower 1 is waiting.
leader 0 is waiting.
        ---->leader 0 is crossing.
leader 1 is waiting.
                follower 0 is crossing.
                follower 0 is finished.
                follower 1 is crossing.
                follower 1 is finished.
        <----leader 0 is finished.
        ---->leader 1 is crossing.
        <----leader 1 is finished.
follower 2 is waiting.
leader 2 is waiting.
        ---->leader 2 is crossing.
follower 3 is waiting.
                follower 2 is crossing.
                follower 2 is finished.
leader 3 is waiting.
        <----leader 2 is finished.
        ---->leader 3 is crossing.
                follower 3 is crossing.
                follower 3 is finished.
        <----leader 3 is finished.
All done
```
**/

#define NUM_LEADER 4
#define NUM_FOLLOWER 4

void* leader(void* arg) {
  int* id = arg;
  printf("leader %d is waiting.\n", *id);

  // Critical Section
  printf("\t---->leader %d is crossing.\n", *id);
  sleep(1);
  printf("\t<----leader %d is finished.\n", *id);
}

void* follower(void* arg) {
  int* id = arg;
  printf("follower %d is waiting.\n", *id);

  // Critical Section
  printf("\t\tfollower %d is crossing.\n", *id);
  printf("\t\tfollower %d is finished.\n", *id);

}

int main(int argc, char** argv) {
  pthread_t lthreads[NUM_LEADER];
  int lthread_ids[NUM_LEADER];
  pthread_t fthreads[NUM_FOLLOWER];
  int fthread_ids[NUM_FOLLOWER];
  

  //initialize your semaphores here

  int i, j;
  for (i = 0; i < NUM_FOLLOWER / 2; i++) {
    fthread_ids[i] = i;
    pthread_create(&fthreads[i], NULL, (void* (*)(void*))follower,
                   &fthread_ids[i]);
  }
  sleep(1);
  for (j = 0; j < NUM_LEADER / 2; j++) {
    lthread_ids[j] = j;
    pthread_create(&lthreads[j], NULL, (void* (*)(void*))leader,
                   &lthread_ids[j]);
  }
  sleep(3);
  for (; i < NUM_FOLLOWER; i++, j++) {
    fthread_ids[i] = i;
    pthread_create(&fthreads[i], NULL, (void* (*)(void*))follower,
                   &fthread_ids[i]);
    lthread_ids[j] = j;
    pthread_create(&lthreads[j], NULL, (void* (*)(void*))leader,
                   &lthread_ids[j]);
  }

  for (int i = 0; i < NUM_FOLLOWER; i++) {
    pthread_join(fthreads[i], NULL);
  }
  for (int i = 0; i < NUM_LEADER; i++) {
    pthread_join(lthreads[i], NULL);
  }

  printf("All done\n");
  return 0;
}
