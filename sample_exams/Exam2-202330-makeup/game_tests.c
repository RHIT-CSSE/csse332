#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

#include "game.h"
#include "prettyprint.h"

void test1(void)
{
  pthread_t threads[NUM_PLAYERS + 1];
  unsigned long i = 0, wtid = 0, ltid = 0;

  // all players arrive first
  for(; i < NUM_PLAYERS; i++) {
    if(i < NUM_PLAYERS / 2)
      pthread_create(&threads[i], 0, home_players, (void*)(wtid++));
    else
      pthread_create(&threads[i], 0, away_players, (void*)(ltid++));
  }

  // let some time pass
  sleep(1);

  // referee now arrives
  pthread_create(&threads[i++], 0, referee, 0);

  // wait for them
  for(i = 0; i < NUM_PLAYERS + 1; i++)
    pthread_join(threads[i], 0);

  printf("\nEverything finished...\n");
}

void test2(void)
{
  pthread_t threads[NUM_PLAYERS + 1];
  unsigned long i = 0, wtid = 0, ltid = 0;

  // referee arrives first
  pthread_create(&threads[i++], 0, referee, 0);

  // let some time pass
  sleep(1);

  // all players arrive after
  for(; i < NUM_PLAYERS + 1; i++) {
    if(i < NUM_PLAYERS / 2 + 1)
      pthread_create(&threads[i], 0, home_players, (void*)(wtid++));
    else
      pthread_create(&threads[i], 0, away_players, (void*)(ltid++));
  }

  // wait for them
  for(i = 0; i < NUM_PLAYERS + 1; i++)
    pthread_join(threads[i], 0);

  printf("\nEverything finished...\n");
}

void test3(void)
{
  pthread_t threads[NUM_PLAYERS + 1];
  unsigned long i = 0, wtid = 0, ltid = 0;

  // some players arrive
  pthread_create(&threads[i++], 0, home_players, (void*)(wtid++));
  pthread_create(&threads[i++], 0, away_players, (void*)(ltid++));
  pthread_create(&threads[i++], 0, home_players, (void*)(wtid++));

  // some time passes
  sleep(1);

  // referee arrives
  pthread_create(&threads[i++], 0, referee, 0);

  // some time passes
  sleep(1);

  // rest of player come in
  pthread_create(&threads[i++], 0, home_players, (void*)(wtid++));
  pthread_create(&threads[i++], 0, away_players, (void*)(ltid++));
  pthread_create(&threads[i++], 0, away_players, (void*)(ltid++));

  // wait for them
  for(i = 0; i < NUM_PLAYERS + 1; i++)
    pthread_join(threads[i], 0);

  printf("\nEverything finished...\n");
}
