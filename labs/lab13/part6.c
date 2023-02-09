#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
vulnerable_fn(char *argument)
{
  char buff[32];
  strcpy(buff, argument);
}


int
_main(int argc, char **argv)
{
  if (argc != 2) {
    fprintf(stderr, "ERROR: Need an input argument!\n");
    exit(EXIT_FAILURE);
  }

  vulnerable_fn(argv[1]);
  exit(0);
}
