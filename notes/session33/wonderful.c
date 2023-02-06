#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void _main(void)
{
  char message[20];
  char name[10];

  strcpy(message, "a wonderful person!");

  gets(name);

  printf("Hi %s you are %s.\n", name, message);

  exit(0);
}

int
main(int argc, char **argv)
{
  _main();
}
