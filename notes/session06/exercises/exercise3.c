#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

//
// TODO:
//  In this exercise, we would like the child to send a random number of
//  characters to its parent. Sounds simple enought.
//  The trick is that we don't know beforehand how many characters will the
//  child send to its parent, so we need a way for the parent to keep reading
//  until the child is done sending stuff.
//
//  HINT: The return value from read will prove to be helpful.
//

char get_rand_char(void)
{
  return (rand() % 35) + 65;
}

int main(int argc, char **argv)
{
  pid_t pid;
  int nums = 0;
  int fd[2];
  char c;
  int i;

  srand(time(0));

  if(pipe(fd) < 0) {
    perror("PANIC: pipe failed");
    exit(EXIT_FAILURE);
  }

  pid = fork();
  if(pid < 0) {
    perror("PANIC: fork failed");
    exit(EXIT_FAILURE);
  }

  if(pid == 0) {
    // child
    close(fd[0]);

    // generate random integer between 0 and 10
    nums = rand() % 10;

    for(i = 0; i < nums; i++) {
      c = get_rand_char();
      printf("[Child %d]: Sending the %c character to parent\n", getpid(), c);

      // TODO: Add code to send the char to the parent
    }

    // TODO: Add more code here

    // leave
    exit(EXIT_SUCCESS);
  } else {
    // parent
    close(fd[1]);

    // TODO: Add code here to read ALL nums characters from the child and print
    // them.
    printf("[Parent %d]: Received the %c character from the child\n",
           getpid(), c);

    // leave
    exit(EXIT_SUCCESS);
  }
}
