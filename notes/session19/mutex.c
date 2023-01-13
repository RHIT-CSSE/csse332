#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>

#include <pthread.h>

/**
 * This is the same exercise as the one we did in session 18. Instead this time,
 * we would like to use a single lock (a mutex) to solve this problem. Your
 * solution must be optimal, i.e., for large arrays, it should perform FASTER
 * than the sequential attempt.
 */

int *array;
int array_size;
int share;
volatile unsigned long long sum;

// statically initialized mutex, you don't to use pthread_mutex_init anymore
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

static void output_time_difference(char* name, struct timeval* start,
                                   struct timeval* end)
{
  long secs_used =
    (end->tv_sec - start->tv_sec);  // avoid overflow by subtracting first
  long usecs_used = (end->tv_usec - start->tv_usec);
  double secs = secs_used + (double)usecs_used / 1000000;
  printf("%s took %f seconds\n", name, secs);
}

void *run_fn(void *arg)
{
  int start_index = *((int *)arg);
  for (int i = start_index; i < start_index + share; ++i) {
    sum += array[i];
  }

  return NULL;
}

int main(int argc, char **argv)
{
  pthread_t threads[2];
  int start_index[2];
  struct timeval start, end;
  if (argc > 1)
    array_size = atoi(argv[1]);
  else
    array_size = 1024;

  if (array_size % 2) {
    fprintf(stderr, "Please enter a multiple of two for the size\n");
    exit(EXIT_FAILURE);
  }

  /* initialize the array */
  array = malloc(array_size * sizeof(int));
  for (int i = 0; i < array_size; ++i) {
    array[i] = 1;
  }

  /* first do it sequentially */
  unsigned long long seq_sum = 0;
  gettimeofday(&start, NULL);
  for (int i = 0; i < array_size; i++) {
    seq_sum += array[i];
  }
  gettimeofday(&end, NULL);
  printf("== \tThe value of the sum under sequential execution is %llu ==\n",
         seq_sum);
  output_time_difference("== \tSequential sum", &start, &end);

  /* now do it using two threads */
  share = array_size / 2;
  start_index[0] = 0;
  start_index[1] = share;
  gettimeofday(&start, NULL);
  pthread_create(&threads[0], NULL, run_fn, &start_index[0]);
  pthread_create(&threads[1], NULL, run_fn, &start_index[1]);

  pthread_join(threads[0], NULL);
  pthread_join(threads[1], NULL);
  gettimeofday(&end, NULL);

  printf("== \tThe value of the sum under threads is %llu ==\n", sum);
  output_time_difference("== \tThreaded sum", &start, &end);

}
