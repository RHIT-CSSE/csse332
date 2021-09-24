/* Copyright 2021 Rose-Hulman */
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/**

A factory uses two kinds of machines, stick producer and bundler, to make
chopsticks. Two stick producers (i.e., two pthreads) will produce sticks and
feed them into a bundler (another pthread).

For every two sticks produced by the stick producers, the bundler will pack
them into a pair of chopsticks. Note that the two sticks can come from either
of one the two stick producers or from both of them. In other words, it is not
necessary that each stick producer will contribute exactly one stick to the
bundler at a time.

The stick producers won't produce more sticks until the two newly-produced
sticks are bundled by the bundler. In other words, for every two sticks the
stick producers produce, they will stop and wait for the bundler to finish
bundling these two sticks and then resume producing next the two sticks.

The program will stop after it finishes processing 8 sticks in total, i.e.,
the stick producers will stop when the TOTAL (produced by both) count of sticks
reaches 8.


Hint: I used in total three semaphores to solve this problem.

If your implementation is correct, try to run program using the commands
below (it will take a few seconds to finish):

$ make chopsticks
$ ./chopsticks

You should see something like this:

One stick produced from #1 machine...
One stick produced from #2 machine...
Two sticks were bundled as a pair of chopsticks
One stick produced from #1 machine...
One stick produced from #2 machine...
Two sticks were bundled as a pair of chopsticks
One stick produced from #1 machine...
One stick produced from #2 machine...
Two sticks were bundled as a pair of chopsticks
One stick produced from #1 machine...
One stick produced from #2 machine...
Two sticks were bundled as a pair of chopsticks
bundler done
producer 1 done
producer 2 done
All done

#1 and #2 producers may not alternate perfectly, which is okay. Technically,
the #1 and #2 here can be totally random.

Note that you want to see all three the machines done at the end. If not,
your program may be blocked by an unwanted sem_wait. You need to fix it.

 **/

#define NUM_STICK 8

int count;

void* stick_producer(void* arg) {
  while (1) {
    count++;
    if (count > NUM_STICK) {
      break;
    }
    printf("One stick produced from #%d machine...\n", *(int*)arg);


    usleep(1000); //keep this line at the bottom of the while loop
  }
  printf("producer %d done\n", *(int*) arg);
}
void* bundler(void* arg) {
  int b_counter = 0;
  while (b_counter < NUM_STICK) {

    printf("Two sticks were bundled as a pair of chopsticks\n");
    sleep(1);
    b_counter += 2;

  }
  printf("bundler done\n");
}

int main(int argc, char** argv) {
  //###############################
  //Set up semaphores




  //###############################
  count = 0;

  pthread_t threads[3];
  int i = 0;
  int tid[2] = {1, 2};

  pthread_create(&threads[i++], NULL, stick_producer, tid);
  pthread_create(&threads[i++], NULL, stick_producer, tid + 1);
  sleep(1);
  pthread_create(&threads[i++], NULL, bundler, NULL);
  for (int j = 0; j < 3; j++) pthread_join(threads[j], NULL);
  printf("All done\n");
  return 0;
}
