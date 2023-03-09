#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  pid_t mypid = getpid();
  int i;

  printf("My process id is: %d\n", mypid);
  printf("You are running the program: %s\n", argv[0]);

  if(argc > 1) {
    printf("You have passed %d arguments\n", argc-1);
    for(i = 1; i < argc; i++) {
      printf("\t Argument %d is %s\n", i, argv[i]);
    }
  }

  exit(EXIT_SUCCESS);
}
