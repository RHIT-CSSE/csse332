#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#include "game.h"
#include "prettyprint.h"

// array of all test cases, first and last element should always be NULL
void (*test_fn[])(void) = { 0, test1, test2, test3, 0, };

// descriptions for each test case, first and last should always be Invalid
// OCD calm down, okay to go over 79 chars here!
const char *descriptions[] = {
  "Invalid",
  "Players arrive first, referee is the last one to make it through.",
  "Referee arrives first, players arrive after that.",
  "Some players arrive before the referee, some players arrive after.",
  "Invalid",
};

// state of the world

// condition variables and mutex locks

void *home_players(void *arg)
{
  unsigned long tid = (unsigned long)arg;

  pr_yellow("Player %lu from home team arrived\n", tid);

  pr_green("Player %lu from home team starts playing\n", tid);

  // play the game
  sleep(GAME_TIME);

  // conduct the interview if needed
  if(tid == 0) {
    pr_magenta("Star player of home team conducting interview\n");

    sleep(INTERVIEW_TIME);

    pr_magenta("Interview is over\n");
  }

  pr_yellow("Player %lu from home team exiting the field\n", tid);

  return 0;
}

void *away_players(void *arg)
{
  unsigned long tid = (unsigned long)arg;

  pr_blue("Player %lu from away team arrived\n", tid);

  pr_green("Player %lu from away team starts playing\n", tid);

  // play the game
  sleep(GAME_TIME);

  pr_blue("Player %lu from away team exiting the field\n", tid);

  return 0;
}

void *referee(void *arg)
{
  pr_cyan("Referee has arrived on the field\n");

  // blow the whistle
  pr_cyan("\n🏀🏀 REFEREE BLOWING THE WHISTLE, GAME CAN START 🏀🏀\n\n");

  // join the game
  sleep(GAME_TIME);

  // nothing left to do, just leave.
  pr_cyan("Referee exiting the field\n");

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
