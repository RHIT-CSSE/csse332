/* I relunctantly moved to 2 spaces per tab to keep students happy */

/**
 * In this problem, we are implementing a system that starts and ends a soccer 
 * game between Liverpool and Manchester United. 
 * There are four types of players: goalkeepers, defenders, midfielders, and 
 * attackers, and we have one referee. 
 * Assume that both teams adopt a similar strategy and decide to have the
 * following approach:
 * - 1 goalkeeper
 * - 4 defenders
 * - 4 midfielders
 * - 2 attackers.
 * 
 * 16 players (4 of each kind) from each team arrive, and we only need to let 
 * a portion of them on the field, according to the above strategy. 
 * 
 * If a player arrives and there are already enough players of the same kind,
 * that player goes back home (i.e., does a `return NULL;`). For example, if 4
 * defenders from Liverpool have arrived, the 5th defender to arrive will simply
 * go home and return.
 * 
 * The referee will wait until BOTH teams have fully arrived, i.e., 11 players
 * from Liverpool and 11 players from Man Utd. Once both teams have arrived, the
 * referee will blow the whistle and start the game. The referee will sleep
 * for 2 seconds to represent the game. All players (there's 22 of them now)
 * should wait until the final whistle to end the game. In other words, after
 * the starting whistle, the players will wait for the referee's final whistle
 * signaling that they can go home. 
 * 
 * Implement the semantics of this game using condition variables and mutex
 * locks.
 */

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

void*
liverpool_fn(void *arg)
{
  int kind = *(int*)arg;
  const char *type = player_names[kind];

  printf("%s from Liverpool arrived...\n", type);

  /* play the game */
  sleep(1);

  return NULL;
}

void*
manutd_fn(void *arg)
{
  int kind = *(int*)arg;
  const char *type = player_names[kind];

  printf("%s from Man Utd arrived...\n", type);

  /* play the game */
  sleep(1);

  return NULL;
}

void*
referee_fn(void *arg)
{
  printf("Referee arrived\n");

  // should sleep for 90 minutes for the duration of the game
  sleep(2);

  return NULL;
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

  return 0;
}