#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <pthread.h>

#include "prettyprint.h"

/*

Imagine a group of friends who meet in a particular place to go
together to lunch.  They don't want to wait too long to eat, so they
might go in seperate groups if some friends are late to arrive.  These
friends also like to argue about which resturant to go to.

When a friend arrives they should begin arguing (even if at this point
they are by themselves).  Arguing takes ARGUE_TIME to complete.  If
ARGUE_TIME elapses and no new friends arrive, the friends reach a
decision about which restaurant to go to, and that group of friend
heads off.  Any friends arriving after the decision has been made must
be part of a subsequent group.

If while the friends are arguing a new friend arrives, that new
friend's preferences means the argument must start from scratch.  The
arguement begins again and ARGUE_TIME must elapse before the friends
can go to lunch (and of course if more friends arrive, we must again
reset).

Example output:

Friend 1 arrived.
Friend 2 arrived.
Friend 4 arrived.
Friend 3 arrived.
Friend 5 arrived.
Arguing done! Group 1 decided with 5 people
Friend 5 leaving in group 1
Friend 1 leaving in group 1
Friend 2 leaving in group 1
Friend 4 leaving in group 1
Friend 3 leaving in group 1
Friend 7 arrived.
Friend 6 arrived.
Arguing done! Group 2 decided with 2 people
Friend 6 leaving in group 2
Friend 7 leaving in group 2
Everyting finished...

 */

#define TOTAL_THREADS 7
#define ARGUE_TIME 1

// state of the world
int group_size = 0;
int group_num = 1;


void *friend(void *arg)
{
  unsigned long tid = (unsigned long)arg;

  pr_red("Friend %ld arrived.\n", tid);

  group_size++;
      
  sleep(ARGUE_TIME);

  // once we get waiting working properly
  // pr_green("Arguing done! Group %ld decided with %ld people\n", my_group, my_size);

  pr_blue("Friend %ld leaving in group %ld\n", tid, group_num);

  
  return 0;

}


int main(int argc, char **argv)
{
  pthread_t threads[TOTAL_THREADS];
  int i;

  for(i = 0; i < TOTAL_THREADS; i++) {
    pthread_create(&threads[i], 0, friend,
                   (void *)(unsigned long)(i + 1));

    if((i + 1) %5 == 0)
      sleep(2);
  }

  for(i = 0; i < TOTAL_THREADS; i++)
    pthread_join(threads[i], 0);

  pr_yellow("Everyting finished...\n");
  exit(0);
}
