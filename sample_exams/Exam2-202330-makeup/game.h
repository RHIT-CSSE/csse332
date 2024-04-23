#ifndef _GAME_H
#define _GAME_H

// constants
#define   NUM_PLAYERS      6   // number of players on BOTH teams
#define   GAME_TIME        2   // game time in seconds
#define   INTERVIEW_TIME   1   // interview time in seconds

// thread functions
void *home_players(void*);
void *away_players(void*);
void *referee(void*);

// test function signature go here
void test1(void);
void test2(void);
void test3(void);

#endif /* game.h */
