#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "setsighandler.h"

/*
   Here is a sample program representing a long running process that
   might need to be aborted.

   STEP 1

   Right now ^C (Ctrl-C) kills the process but it does not run the cleanup()
   function.  Register a signal handler so that the cleanup function is
   called when you use ^C to end the process.

   STEP 2

   Use the signal mask to disable ^C while Part 2.  Don't just install a
   new empty handler - use sigprocmask.  You should reenable ^C after
   Part 2 is done.

   STEP 3

   We'd like to make part 3 abort (and cleanup) automatically after 4
   seconds.  Use the alarm function and an appropriate signal handler to
   do that (note that although the man page warns you, in my testing
   sigalrm and sleep seem to work OK together).


*/

void part1()
{
  printf("Starting slow process Part 1 of 3.  It is currently safe to abort this process with ^C.\n");
  sleep(4);
}

void part2()
{
  printf("Starting slow process Part 2 of 3.  It is NOT safe to abort/interrupt this process with ^C.\n");
  sleep(4);
}

void part3()
{
  printf("Starting slow process Part 3 of 3.  If this part takes more than 4 seconds you should abort it.\n");
  sleep(30);
}

void cleanup() {
  printf("Abort detected!  Cleaning up and exiting...");
  exit(0);
}

void alarm_handler(int alarm) {
  cleanup();
}

int main(int argc, char **argv)
{
  setsighandler(SIGINT, alarm_handler);
  setsighandler(SIGALRM, alarm_handler);
  part1();
  mask_signal(SIGINT);
  part2();
  unmask_signal(SIGINT);
  alarm(4);
  part3();

  exit(0);
}
