#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// comment this out if you want a real challenge!
static const char *cheater[] = {"/bin/sh", NULL};

void
vulnerable_fn(char *argument)
{
  char buff[32];
  strcpy(buff, argument);
}


int
_main(int argc, char **argv)
{
  char *tmp;
  int len;

  if (argc < 2 || argc > 3) {
    fprintf(stderr, "ERROR: Need an input argument!\n");
    exit(EXIT_FAILURE);
  }

  tmp = argv[1];
  if(argc == 3) {
    len = strlen(argv[1]) + strlen(argv[2]) + 2;
    tmp = malloc(len);
    memcpy(tmp, argv[1], strlen(argv[1]));
    tmp[strlen(argv[1])] = 0x20;
    memcpy(tmp + strlen(argv[1]) + 1, argv[2], strlen(argv[2]));
    tmp[len] = 0;
  }

  vulnerable_fn(tmp);
  exit(0);
}
