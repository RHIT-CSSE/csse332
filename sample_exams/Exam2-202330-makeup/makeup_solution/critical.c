#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#include "critical.h"
#include "prettyprint.h"

// array of all test cases, first and last element should always be NULL
void (*test_fn[])(void) = { 0, test1, test2, test3, 0, };

// descriptions for each test case, first and last should always be Invalid
// OCD calm down, okay to go over 79 chars here!
const char *descriptions[] = {
  "Invalid",
  "All red threads, should go 2 by 2.",
  "All blue threads, should go 2 by 2.",
  "All threads arrive together, they go 3 by 3 by which ones go in non-determined.",
  "Invalid",
};

// state of the world
int num_threads_in = 0;
int num_red_in = 0;
int num_blue_in = 0;

// condition variables and mutex lock
pthread_mutex_t lock    = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cv       = PTHREAD_COND_INITIALIZER;

void *red(void *arg)
{
  unsigned long tid = (unsigned long)arg;

  pr_red("Red thread %lu arrived\n", tid);
  pthread_mutex_lock(&lock);
  while(num_threads_in >= 3 || num_red_in >= 2)
    pthread_cond_wait(&cv, &lock);
  num_threads_in++;
  num_red_in++;
  pthread_mutex_unlock(&lock);

  pr_red("Red thread %lu entering critical section\n", tid);
  sleep(1);
  pr_red("Red thread %lu leaving critical section\n", tid);

  pthread_mutex_lock(&lock);
  num_threads_in--;
  num_red_in--;
  pthread_cond_broadcast(&cv);
  pthread_mutex_unlock(&lock);

  return 0;
}

void *blue(void *arg)
{
  unsigned long tid = (unsigned long)arg;

  pr_blue("Blue thread %lu arrived\n", tid);
  pthread_mutex_lock(&lock);
  while(num_threads_in >= 3 || num_blue_in >= 2)
    pthread_cond_wait(&cv, &lock);
  num_threads_in++;
  num_blue_in++;
  pthread_mutex_unlock(&lock);

  pr_blue("Blue thread %lu entering critical section\n", tid);
  sleep(1);
  pr_blue("Blue thread %lu leaving critical section\n", tid);

  pthread_mutex_lock(&lock);
  num_threads_in--;
  num_blue_in--;
  pthread_cond_broadcast(&cv);
  pthread_mutex_unlock(&lock);

  return 0;
}

int main(int argc, char **argv)
{
  int test_num = -1;

  if(argc < 2) {
    fprintf(stderr, "Usage: %s <test_num>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  test_num = atoi(argv[1]);
  if(test_num >= ARRAY_SIZE(test_fn, void(*)(void)) || !test_fn[test_num]) {
    fprintf(stderr, "Invalid test number %d, out of range of available tests\n",
            test_num);
    exit(EXIT_FAILURE);
  }

  pr_green("Running test number %d: %s\n\n", test_num, descriptions[test_num]);
  test_fn[test_num]();

  exit(0);
}
