#include <stdio.h>
#include <stdlib.h>

#include "a.h"

extern int another_useless(char);

int
main()
{
  printf("Calling useless():\n");
  useless(3);
  printf("Calling another useless(): %d\n", another_useless('c'));

  exit(0);
}
