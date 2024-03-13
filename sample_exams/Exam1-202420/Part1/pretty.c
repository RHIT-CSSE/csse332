#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv) {
  int ppid;
  const char *msg;
  int ec = -1;

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
    // grab a int between 0 and 2.
    ec = getpid() % 3;
  }

  // print message from parent.
  printf("%s\t%d\tMy parent (%d) told me: %s\n", argv[0], getpid(), ppid, msg);
  fflush(0);

  // sleep for a bit.
  sleep(ec + 1);

  // check how to exit.
  if(!ec) *(char*)0 = 1;
  exit(ec);
}

