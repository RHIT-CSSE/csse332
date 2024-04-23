#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#include "lunch.h"
#include "prettyprint.h"

// array of all test cases, first and last element should always be NULL
void (*test_fn[])(void) = { 0, test1, test2, test3, test4, 0, };

// descriptions for each test case, first and last should always be Invalid
// OCD calm down, okay to go over 79 chars here!
const char *descriptions[] = {
  "Invalid",
  "Threads keep coming and they must wait for all of them, even if two seconds or more have passed.",
  "Threads arrive in two groups and should leave in those two groups.",
  "Threads arrive with shorty durations in between, they should be leaving in groups that are less than 2 seconds apart.",
  "Similar to test 3, but now threads in first group cut it close and break up, last group definitely breaks up.",
  "Invalid",
};


// state of the world

// condition variables and locks

void *friend(void *arg)
{
  unsigned long tid = (unsigned long)arg;

  pr_red("Friend %ld arrived.\n", tid);

  sleep(ARGUE_TIME);

  // pr_green("Arguing done! Group %ld decided with %ld people\n", my_group, my_size);

  // pr_blue("Friend %ld leaving in group %ld\n", tid, my_group);

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

