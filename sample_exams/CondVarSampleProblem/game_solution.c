/* I relunctantly moved to 2 spaces per tab to keep students happy */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>

#define NUM_PLAYERS 16 

enum player_type {
  PLAYER_GOALKEEPER = 0,
  PLAYER_DEFENDER,
  PLAYER_MIDFIELDER,
  PLAYER_ATTACKER,

  /* don't change this */
  PLAYER_LAST_ONE,
};

char *player_names[] = { "goalkeeper", "defender", "midfielder", "attacker", };

/* NOT STUDENT CODE */
pthread_mutex_t lock;
pthread_cond_t referee_cond, game_cond, liverpool, manutd;

int liverpool_ready = 0;
int liverpool_arrvd = 0;
int manutd_ready = 0;
int manutd_arrvd = 0;

int game_not_done = 1;

int liverpool_arrived[PLAYER_LAST_ONE];
int manutd_arrived[PLAYER_LAST_ONE];
/********************/

void*
liverpool_fn(void *arg)
{
  int kind = *(int*)arg;
  const char *type = player_names[kind];

  printf("%s from Liverpool arrived...\n", type);

  pthread_mutex_lock(&lock);
  switch(kind) {
  case PLAYER_GOALKEEPER:
    if(liverpool_arrived[kind] >= 1) {
      pthread_mutex_unlock(&lock);
      return NULL;
    }
    liverpool_arrived[kind]++;
    break;
  case PLAYER_DEFENDER:
  case PLAYER_MIDFIELDER:
    if(liverpool_arrived[kind] >= 4) {
      pthread_mutex_unlock(&lock);
      return NULL;
    }
    liverpool_arrived[kind]++;
    break;
  case PLAYER_ATTACKER:
    if(liverpool_arrived[kind] >= 2) {
      pthread_mutex_unlock(&lock);
      return NULL;
    }
    liverpool_arrived[kind]++;
    break;
  default:
    pthread_mutex_unlock(&lock);
    fprintf(stderr, "WUUT\n");
    exit(EXIT_FAILURE);
  }

  liverpool_arrvd++;
  printf("New liverpool player arrived: %d\n", liverpool_arrvd);
  if(liverpool_arrvd == 11) {
    // wake up all other sleeping players
    liverpool_ready = 1;
    pthread_cond_signal(&referee_cond);
    pthread_cond_broadcast(&liverpool);
  } else {
    while(liverpool_arrvd != 11) {
      // sleep until last player arrives
      pthread_cond_wait(&liverpool, &lock);
    }
  }
  pthread_mutex_unlock(&lock);

  sleep(1);

  pthread_mutex_lock(&lock);
  while(game_not_done)
    pthread_cond_wait(&game_cond, &lock);
  pthread_mutex_unlock(&lock);

  return NULL;
}

void*
manutd_fn(void *arg)
{
  int kind = *(int*)arg;
  const char *type = player_names[kind];

  printf("%s from Man Utd arrived...\n", type);

  pthread_mutex_lock(&lock);
  switch(kind) {
  case PLAYER_GOALKEEPER:
    if(manutd_arrived[kind] >= 1) {
      pthread_mutex_unlock(&lock);
      return NULL;
    }
    manutd_arrived[kind]++;
    break;
  case PLAYER_DEFENDER:
  case PLAYER_MIDFIELDER:
    if(manutd_arrived[kind] >= 4) {
      pthread_mutex_unlock(&lock);
      return NULL;
    }
    manutd_arrived[kind]++;
    break;
  case PLAYER_ATTACKER:
    if(manutd_arrived[kind] >= 2) {
      pthread_mutex_unlock(&lock);
      return NULL;
    }
    manutd_arrived[kind]++;
    break;
  default:
    pthread_mutex_unlock(&lock);
    fprintf(stderr, "WUUT\n");
    exit(EXIT_FAILURE);
  }

  manutd_arrvd++;
  printf("New Man Utd player arrived: %d\n", manutd_arrvd);
  if(manutd_arrvd == 11) {
    // wake up all other sleeping players
    manutd_ready = 1;
    pthread_cond_signal(&referee_cond);
    pthread_cond_broadcast(&manutd);
  } else {
    while(manutd_arrvd != 11) {
      // sleep until last player arrives
      pthread_cond_wait(&manutd, &lock);
    }
  }
  pthread_mutex_unlock(&lock);

  sleep(1);

  pthread_mutex_lock(&lock);
  while(game_not_done)
    pthread_cond_wait(&game_cond, &lock);
  pthread_mutex_unlock(&lock);

  return NULL;
}

void*
referee_fn(void *arg)
{
  printf("Referee arrived\n");

  pthread_mutex_lock(&lock);
  while(!liverpool_ready || !manutd_ready)
  {
    pthread_cond_wait(&referee_cond, &lock);
  }
  pthread_mutex_unlock(&lock);

  // should sleep for 90 minutes for the duration of the game
  sleep(2);

  pthread_mutex_lock(&lock);
  game_not_done = 0;
  pthread_cond_broadcast(&game_cond);
  pthread_mutex_unlock(&lock);
}

int
main(int argc, char **argv)
{
  /* declarations */
  int i;
  int player_kinds[NUM_PLAYERS];

  pthread_t livplayers[NUM_PLAYERS];
  pthread_t manutd_players[NUM_PLAYERS];
  pthread_t referee;

  /* NOT STUDENT CODE */
  pthread_cond_init(&referee_cond, 0);
  pthread_cond_init(&game_cond, 0);
  pthread_cond_init(&liverpool, 0);
  pthread_cond_init(&manutd, 0);
  pthread_mutex_init(&lock, 0);
  /********************/

  /* initialize the kinds */
  for(i = 0; i < NUM_PLAYERS; i++) {
    player_kinds[i] = i % 4;
  }

  pthread_create(&referee, 0, referee_fn, 0);
  for(i = 0; i < NUM_PLAYERS; i++) {
    pthread_create(&livplayers[i], 0, liverpool_fn, &player_kinds[i]);
  }

  for(i = 0; i < NUM_PLAYERS; i++) {
    pthread_create(&manutd_players[i], 0, manutd_fn, &player_kinds[i]);
  }

  /* wait for everything */
  for(i = 0; i < NUM_PLAYERS; ++i) {
    pthread_join(livplayers[i], 0);
    pthread_join(manutd_players[i], 0);
  }
  pthread_join(referee, 0);

  printf("Match Ended. Liverpool won 5 - 1\n");

}