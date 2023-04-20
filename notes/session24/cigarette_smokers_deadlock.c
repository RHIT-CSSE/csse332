#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MATCH   1 << 0
#define PAPER   1 << 1
#define TOBACCO 1 << 2

// Concurrency means
pthread_mutex_t lock    = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t paper    = PTHREAD_COND_INITIALIZER;
pthread_cond_t match    = PTHREAD_COND_INITIALIZER;
pthread_cond_t tobacco  = PTHREAD_COND_INITIALIZER;
pthread_cond_t smoke    = PTHREAD_COND_INITIALIZER;

// State variables
int done_smoking      = 0;
int paper_on_table    = 0;
int match_on_table    = 0;
int tobacco_on_table  = 0;

// Agent function, you are not allowed to change this function.
void *agent_fn(void *arg)
{
  static const int choices[] = {MATCH|PAPER, MATCH|TOBACCO, PAPER|TOBACCO};
  static const int corresponding_smoker[] = {TOBACCO, PAPER, MATCH};
  int r, c;

  while(1) {
    r = rand() % 3;
    c = choices[r];

    pthread_mutex_lock(&lock);
    done_smoking = 0;
    if(c & MATCH) {
      printf("Agent posted matches to the table...\n");
      match_on_table = 1;
      pthread_cond_broadcast(&match);
    }
    if(c & PAPER) {
      printf("Agent posted paper to the table...\n");
      paper_on_table = 1;
      pthread_cond_broadcast(&paper);
    }
    if(c & TOBACCO) {
      printf("Agent posted tobacco to the table...\n");
      tobacco_on_table = 1;
      pthread_cond_broadcast(&tobacco);
    }

    while(!done_smoking)
      pthread_cond_wait(&smoke, &lock);
    pthread_mutex_unlock(&lock);
  }
}

// This function assumes you have the lock. When it function returns, it will
// return with the lock held as well.
void __smoke_fn(const char *msg)
{
  done_smoking = 0;
  pthread_mutex_unlock(&lock);
  printf("%s is smoking...\n", msg);
  sleep(1);
  printf("%s is done smoking...\n", msg);
  pthread_mutex_lock(&lock);
  done_smoking = 1;
  pthread_cond_signal(&smoke);
}

// Tobacco smoker
void *tobacco_smoker(void *arg)
{
  while(1) {
    pthread_mutex_lock(&lock);
    while(!paper_on_table)
      pthread_cond_wait(&paper, &lock);
    paper_on_table = 0;
    printf("Tobacco smoker has paper...\n");

    while(!match_on_table)
      pthread_cond_wait(&match, &lock);
    match_on_table = 0;
    printf("Tobacco smoker has matches...\n");

    __smoke_fn("Tobacco smoker");
    pthread_mutex_unlock(&lock);
  }
}

// Paper smoker
void *paper_smoker(void *arg)
{
  while(1) {
    pthread_mutex_lock(&lock);
    while(!tobacco_on_table)
      pthread_cond_wait(&tobacco, &lock);
    tobacco_on_table = 0;
    printf("Paper smoker has tobacco...\n");

    while(!match_on_table)
      pthread_cond_wait(&match, &lock);
    match_on_table = 0;
    printf("Paper smoker has matches...\n");

    __smoke_fn("Paper smoker");
    pthread_mutex_unlock(&lock);
  }
}

// Match smoker
void *match_smoker(void *arg)
{
  while(1) {
    pthread_mutex_lock(&lock);
    while(!tobacco_on_table)
      pthread_cond_wait(&tobacco, &lock);
    tobacco_on_table = 0;
    printf("Match smoker has tobacco...\n");

    while(!paper_on_table)
      pthread_cond_wait(&paper, &lock);
    paper_on_table = 0;
    printf("Match smoker has paper...\n");

    __smoke_fn("Match smoker");
    pthread_mutex_unlock(&lock);
  }
}

int
main(int argc, char **argv)
{
  pthread_t agent;
  pthread_t smokers[3];
  int i;

  void *(*smokers_fn[3]) (void*) = {tobacco_smoker, paper_smoker, match_smoker};

  pthread_create(&agent, NULL, agent_fn, NULL);
  for(i = 0; i < 3; i++)
    pthread_create(&smokers[i], NULL, smokers_fn[i], NULL);

  pthread_join(agent, NULL);
  for(i = 0; i < 3; i++)
    pthread_join(smokers[i], NULL);

  printf("Everything finished...\n");

  exit(EXIT_SUCCESS);
}


