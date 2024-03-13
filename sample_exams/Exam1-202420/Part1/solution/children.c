#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>


// Default buffer size for use with snprintf
#define BUFSIZE 128
#define NUM_CHILDREN 10

void part1(void) {
  int i = 0;
  int rc, status;
  char buf[BUFSIZE];
  int child_ids[NUM_CHILDREN];

  int ppid = getpid();
  for(; i< NUM_CHILDREN; i++) {
    rc = fork();
    if(!rc) {
      snprintf(buf, BUFSIZE, "%d", ppid);
      execlp("./pretty.bin", "./pretty.bin",
          buf, "Be brave and never give up!", NULL);
      perror("execlp failed: ");
      exit(111);
    }
    child_ids[i] = rc;
  }

  for(i = 0; i < NUM_CHILDREN; i++) {
    waitpid(child_ids[i], &status, 0);
    if(WIFEXITED(status)) {
      printf("Child %d finished with exit code %d!\n", i, WEXITSTATUS(status));
    } else {
      printf("Child %d crashed!\n", i);
    }
  }
}

void part2(void) {
  int i = 0;
  int rc, status;
  char buf[BUFSIZE];
  int child_ids[NUM_CHILDREN];

  int ppid = getpid();
  for(; i< NUM_CHILDREN; i++) {
    rc = fork();
    if(!rc) {
      if(getpid() % 3 == 0) {
        // bad child, should skip over it.
        printf("Child %d is bad, will not execute.\n", getpid());
        exit(0);
      }
      snprintf(buf, BUFSIZE, "%d", ppid);
      execlp("./pretty.bin", "./pretty.bin",
          buf, "Be brave and never give up!", NULL);
      perror("execlp failed: ");
      exit(111);
    }

    child_ids[i] = rc;
    if(rc % 3 == 0) {
      // bad child, must wait for it.
      //
      // Challenge here is that we must use waitpid, otherwise we might end up
      // waiting for another child that was forked before.
      //
      waitpid(rc, 0, 0);

      // need to decrement i so that we still have a total of NUM_CHILDREN
      // children.
      i--;
    }
  }

  for(i = 0; i < NUM_CHILDREN; i++) {
    waitpid(child_ids[i], &status, 0);
    if(WIFEXITED(status)) {
      printf("Child %d finished with exit code %d!\n", i, WEXITSTATUS(status));
    } else {
      printf("Child %d crashed!\n", i);
    }
  }
}

int main(int argc, char **argv) {
  // Call part1() when testing part1 of this problem.
  // part1();

  // Comment the line above and uncomment part2(); to test part2 of this
  // problem.
  part2();

  printf("Parent %d finished ....\n", getpid());
  exit(0);
}

