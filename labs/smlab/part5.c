#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
say_hello_using_echo(void)
{
  system("echo hello world");
}

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

  if (argc < 2) {
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

  say_hello_using_echo();
  vulnerable_fn(tmp);
  exit(0);

}
