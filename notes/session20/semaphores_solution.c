#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>

#include <pthread.h>
#include <semaphore.h>

/**
 * In this activity, we will implement our own version of a semaphore using the
 * concurrency constructs that we learned in class.
 *
 * Step 1: Think about what which constructs you need to implement the
 * semaphore.
 *
 * Step 2: Add your constructs to the definition of our class semaphore
 *
 * Step 3: Implement the four functions:
 *	- csse332_sem_init: initialize our class semaphore
 *	- csse332_sem_wait: check if you can acquire the lock, or wait for
 *				posting
 *	- csse332_sem_post: post to the semaphore's value
 *	- csse332_sem_destroy
 *
 */

typedef struct __csse332_sem {
  int value;
  /* ADD YOUR CODE HERE */
  pthread_mutex_t lock;
  pthread_cond_t cond;
} csse332_sem_t;

/*
 * Initialize your semaphore
 *
 * @s: A pointer to the semaphore to initialize
 * @value: The initial value of the semaphore
 */
void
csse332_sem_init(csse332_sem_t *s, int value)
{
  s->value = value;
  /* ADD YOUR CODE HERE */
  pthread_mutex_init(&s->lock, NULL);
  pthread_cond_init(&s->cond, NULL);
}

/*
 * Check if can enter critical section, and wait otherwise.
 */
void
csse332_sem_wait(csse332_sem_t *s)
{
  /* ADD YOUR CODE HERE */
  pthread_mutex_lock(&s->lock);
  s->value--;
  while(s->value < 0)
    pthread_cond_wait(&s->cond, &s->lock);
  pthread_mutex_unlock(&s->lock);
}

/*
 * Post to the semaphore
 */
void
csse332_sem_post(csse332_sem_t *s)
{
  /* ADD YOUR CODE HERE */
  pthread_mutex_lock(&s->lock);
  s->value++;
  pthread_cond_signal(&s->cond);
  pthread_mutex_unlock(&s->lock);
}

/*
 * Destroy and clean up
 */
void
csse332_sem_destroy(csse332_sem_t *s)
{
  pthread_mutex_destroy(&s->lock);
  pthread_cond_destroy(&s->cond);
  s->value = 0;
}

/* declare a global custom semaphore */
static csse332_sem_t s;

void *
child(void *arg)
{
  printf("Child\n");
  sleep(5);
  printf("Child completed\n");
  csse332_sem_post(&s);
  return NULL;

}

int
main(int arc, char **argv)
{
  pthread_t thread;

  /* intialize the sempahore */
  csse332_sem_init(&s, 0);

  /* create the thread */
  pthread_create(&thread, 0, child, NULL);

  /* wait for child to be done */
  printf("Parent waiting for the child\n");
  csse332_sem_wait(&s);
  /* cleanup */
  printf("Child completed, parent can exit!\n");
  csse332_sem_destroy(&s);

  return 0;
}
