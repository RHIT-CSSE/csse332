#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void
vulnerable_fn(char *argument)
{
  char buff[1024];
  strcpy(buff, argument);
}

unsigned int getranduint(void)
{
  FILE *f;
  unsigned int r;

  f = fopen("/dev/urandom", "rb");
  assert(f);
  fread(&r, sizeof r, 1, f);
  fclose(f);

  return r;
}


int
_main(int argc, char **argv)
{
  unsigned int ssize;

  if (argc != 2) {
    fprintf(stderr, "ERROR: Need an input argument!\n");
    exit(EXIT_FAILURE);
  }

  ssize = getranduint();
  alloca(ssize & 0xFF);

  vulnerable_fn(argv[1]);
  exit(0);
}
