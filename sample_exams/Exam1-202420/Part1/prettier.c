#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

// setsighandler()
void setsighandler(int signum, void (*handler)(int))
{
  struct sigaction act;

  act.sa_handler = handler;
  sigemptyset(&act.sa_mask);
  act.sa_flags = SA_RESTART;
  sigaction(signum, &act, NULL);
}

void handle_timeout(int signum) {
  exit(99);
}

int main(int argc, char **argv) {
  int ppid;
  const char *msg;
  int ec = -1;

  setsighandler(SIGALRM, handle_timeout);

  if(argc < 3) {
    printf("[%s : %d] Incorrect number of arguments provided!\n",
        argv[0], getpid());
    exit(99);
  }

  // grab arguments.
  ppid = atoi(argv[1]);
  msg = argv[2];
  if(argc == 4) {
    ec = atoi(argv[3]);
  } else {
    // grab a int between 0 and 3.
    ec = getpid() % 4;
  }

  // print message from parent.
  printf("%s\t%d\tMy parent (%d) told me: %s\n", argv[0], getpid(), ppid, msg);
  fflush(0);

  if(ec == 3) while(1);

  // sleep for a bit.
  sleep(ec + 1);

  // check how to exit.
  if(!ec) *(char*)0 = 1;
  exit(ec);
}

