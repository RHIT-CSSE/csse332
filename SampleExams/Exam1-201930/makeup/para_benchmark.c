#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/mman.h>

// static double *glob_var;  // for use on the last two parts

int main(void)
{
  char *rcvrs[] = {"./fast_receiver", "./slow_receiver",
		   "./unreliable_receiver"};
  char *sndrs[] = {"./fast_sender", "./slow_sender", "./unreliable_sender"};
  int nrcvrs = 2;
  int nsndrs = 2;

  for (int i=0; i<nrcvrs; i++) {
    for (int j=0; j<nsndrs; j++) {
      struct timeval start, end;
      double dt;

      gettimeofday(&start, NULL);

      printf("I would like to test %s with %s\n", rcvrs[i], sndrs[j]);
      sleep(1);

      gettimeofday(&end, NULL);
      dt = end.tv_sec-start.tv_sec+(end.tv_usec-start.tv_usec)/1000000.0;
      printf("Done testing %s with %s, %f sec required\n", sndrs[j],
	     rcvrs[i], dt);
    }
  }

  return 0;
}
