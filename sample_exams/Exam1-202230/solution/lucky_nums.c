#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>

#define MAX_CHAIN 5

int fork7() {
  int pid;

  do {
    wait(NULL);
    pid = fork();
    if(pid == 0) {
      int myresult = getpid();

      if(myresult % 10 != 7) {
        //printf("unlucky child pid %d exiting\n", myresult);
        exit(0);

      }

      printf("lucky child pid %d created\n", myresult);

      return 0;
    }

  } while (pid % 10 != 7);

  return pid;
}


int main(int argc, char** argv) {
  int pvals[2];
  pipe(pvals);

  int pid = fork7();
  if(pid == 0) {

    close(pvals[0]);

    for(int i = 2; i < MAX_CHAIN; i++) {
      if(fork7() == 0) continue;

      close(pvals[1]);
      printf("child running forever\n");
      while(1);
    }

    int mypid = getpid();
    write(pvals[1], &mypid, sizeof(mypid));

    close(pvals[1]);
    printf("child running forever\n");
    while(1);


  }

  close(pvals[1]);
  read(pvals[0], &pid, sizeof(pid));
  close(pvals[0]);

  int parent_pid = getpid();
  printf("parent %d thinks it found a good process %d \n", parent_pid, pid);

  pid = fork();
  if(pid == 0) {
    char num[100];
    snprintf(num, 100, "%d", parent_pid);
    execlp("pstree", "pstree", "-p", "-c", num, NULL);
  }

  wait(NULL);
  printf("parent running forever\n");
  while(1);
}
