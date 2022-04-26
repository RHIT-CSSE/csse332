#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_PRIORITIES 5
#define NUM_THREADS 6

/* Test functions */
void test_wait_signal(void);
void test_wait_broadcast(void); void test_wait_signal_pr(void);

typedef struct pr_cond_ {
  // TODO: Add your code here.
} pr_cond_t;

/**
 * pr_cond_init -- Initialize a priority condition variable
 * 
 * @condvar The priority condition variable to initialize.
 */
void
pr_cond_init(pr_cond_t *condvar)
{

}

/**
 * pr_cond_wait -- Wait on a given condition variable with a given priority
 * 
 * @condvar The priority condition variable to wait on
 * @priority The priority of the calling thread
 * @mutex The mutex that must be LOCKED by the thread
 */
void
pr_cond_wait(pr_cond_t *condvar, unsigned priority, pthread_mutex_t *mutex)
{

}

/**
 * pr_cond_signal -- Signal on a the condition variable with the highest
 * priority.
 * 
 * @condvar The priority condition variable to signal on
 */
void
pr_cond_signal(pr_cond_t *condvar)
{

}
 
/**
 * pr_cond_broadcast -- Broadcast to all priorities
 * 
 * @condvar The priority condition variable to broadcast to.
 */
void
pr_cond_broadcast(pr_cond_t *condvar)
{

}

/**
 * pr_cond_signal_pr -- Signal to a specified priority
 * 
 * @condvar The priority condition variable to signal on.
 * @priority The prioirity to signal on.
 * 
 * @return -1 on failure, 0 on success.
 */
int
pr_cond_signal_pr(pr_cond_t *condvar, unsigned priority)
{

}

/**
 * pr_cond_destroy -- Destroy a priority condition variable
 * 
 * @condvar The priority condition variable to destroy.
 */
void
pr_cond_destroy(pr_cond_t *condvar)
{

}

pr_cond_t pr_cond;
pthread_mutex_t lock;

int
main(int argc, char **argv)
{
  /* Comment/Uncomment lines of code to test individual components */
  puts("###################################################################");
  // test_wait_signal();
  puts("###################################################################");

  puts("###################################################################");
  // test_wait_broadcast();
  puts("###################################################################");

  puts("###################################################################");
  // test_wait_signal_pr();
  puts("###################################################################");
}

void *test1_thread_fn(void *arg)
{
  unsigned my_pr = *(unsigned*)arg;

  pthread_mutex_lock(&lock);
  printf("Thread with pr=%d waiting\n", my_pr);
  pr_cond_wait(&pr_cond, my_pr, &lock);
  pthread_mutex_unlock(&lock);
  printf("Thread with pr=%d awake\n", my_pr);

  return NULL;
}

void
test_wait_signal(void)
{
  int i;
  pthread_t threads[NUM_THREADS];
  unsigned priorities[NUM_THREADS];

  pthread_mutex_init(&lock, 0);
  pr_cond_init(&pr_cond);

  for(i = 0; i < NUM_THREADS; i++) {
    priorities[i] = i % NUM_PRIORITIES;
    pthread_create(&threads[i], 0, test1_thread_fn, &priorities[i]);
  }

  for(i = 0; i < NUM_THREADS; i++) {
    sleep(1);
    pr_cond_signal(&pr_cond);
  }

  for(i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], 0);
  }

  pr_cond_destroy(&pr_cond);
  pthread_mutex_destroy(&lock);

  printf("Everything done ...\n");
}

void
test_wait_broadcast(void)
{
  int i;
  pthread_t threads[NUM_THREADS];
  unsigned priorities[NUM_THREADS];


  pthread_mutex_init(&lock, 0);
  pr_cond_init(&pr_cond);

  for(i = 0; i < NUM_THREADS; i++) {
    priorities[i] = i % NUM_PRIORITIES;
    pthread_create(&threads[i], 0, test1_thread_fn, &priorities[i]);
  }

  // wait until everyone sleeps so that we dont miss any signals
  sleep(1);

  pr_cond_broadcast(&pr_cond);

  for(i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], 0);
  }

  pr_cond_destroy(&pr_cond);
  pthread_mutex_destroy(&lock);
  printf("Everything done ...\n");
}

void
test_wait_signal_pr(void)
{
  int i;
  pthread_t threads[NUM_THREADS];
  unsigned priorities[NUM_THREADS];


  pthread_mutex_init(&lock, 0);
  pr_cond_init(&pr_cond);

  for(i = 0; i < NUM_THREADS; i++) {
    priorities[i] = i % NUM_PRIORITIES;
    pthread_create(&threads[i], 0, test1_thread_fn, &priorities[i]);
  }

  sleep(1);
  pr_cond_signal_pr(&pr_cond, 4);
  sleep(1);
  pr_cond_signal_pr(&pr_cond, 0);
  sleep(1);
  pr_cond_signal_pr(&pr_cond, 1);
  sleep(1);
  pr_cond_signal_pr(&pr_cond, 3);
  sleep(1);
  pr_cond_signal_pr(&pr_cond, 2);
  sleep(1);
  pr_cond_signal_pr(&pr_cond, 0);


  for(i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], 0);
  }

  pr_cond_destroy(&pr_cond);
  pthread_mutex_destroy(&lock);
  printf("Everything done ...\n");
}