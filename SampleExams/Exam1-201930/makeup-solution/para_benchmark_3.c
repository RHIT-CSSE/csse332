#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/mman.h>

// static double *glob_var;  // for use on the last two parts

int main(void)
{
  int fd[2];
  pid_t childpid;

  char *rcvrs[] = {"./fast_receiver", "./slow_receiver",
		   "./unreliable_receiver"};
  char *sndrs[] = {"./fast_sender", "./slow_sender", "./unreliable_sender"};
  int nrcvrs = 2;
  int nsndrs = 2;

  for (int i=0; i<nrcvrs; i++) {
    for (int j=0; j<nsndrs; j++) {
      childpid = fork();
      if (childpid < 0) {
	printf("fork failed\n");
	return 1;
      } else if (childpid == 0) {
	struct timeval start, end;
	double dt;
	
	gettimeofday(&start, NULL);

	printf("testing %s\n", sndrs[j]);
	execl(sndrs[j], sndrs[j], "-1", NULL);

	printf("exec failed\n");
	return 1;
      }
    }
  }
  
  printf("waiting for all tests to complete\n");
  for (int i=0; i<nrcvrs*nsndrs; i++) {
    wait(NULL);
  }

  printf("Testing complete\n");

  return 0;
}
