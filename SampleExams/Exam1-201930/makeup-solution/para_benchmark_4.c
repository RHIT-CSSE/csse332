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
	pid_t pid1;
	
	printf("testing %s\n", sndrs[j]);
	gettimeofday(&start, NULL);

	pid1 = fork();
	if (pid1 < 0) {
	  printf("fork failed\n");
	  return 1;
	} else if (pid1 == 0) {
	  execl(sndrs[j], sndrs[j], "-1", NULL);

	  printf("exec failed\n");
	  return 1;
	}

	printf("waiting for %s to finish\n", sndrs[j]);
	wait(NULL);

	gettimeofday(&end, NULL);
	dt = end.tv_sec-start.tv_sec+(end.tv_usec-start.tv_usec)/1000000.0;
	printf("Done testing %s with %s, %f sec required\n", sndrs[j],
	       rcvrs[i], dt);
	return 0;
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
