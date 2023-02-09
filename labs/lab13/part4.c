#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
vulnerable_fn(char *argument)
{
  int *p;
  int a;
  char buff[1024];

  strncpy(buff, argument, sizeof(buff) + 8);

  *p = a;
}


int
_main(int argc, char **argv)
{
  if (argc < 2) {
    fprintf(stderr, "ERROR: Required input argument\n");
    exit(EXIT_FAILURE);
  }

  vulnerable_fn(argv[1]);
  exit(0);
}
