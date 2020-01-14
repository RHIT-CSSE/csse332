#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/mman.h>

static double *glob_var;

int main(void)
{
  int fd[2];
  pid_t childpid;

  char *rcvrs[] = {"./fast_receiver", "./slow_receiver",
		   "./unreliable_receiver"};
  char *sndrs[] = {"./fast_sender", "./slow_sender", "./unreliable_sender"};
  int nrcvrs = 3;
  int nsndrs = 3;

  glob_var = mmap(NULL, nrcvrs*nsndrs*sizeof(double), PROT_READ | PROT_WRITE, 
		  MAP_SHARED | MAP_ANONYMOUS, -1, 0);

  for (int i=0; i<nrcvrs; i++) {
    for (int j=0; j<nsndrs; j++) {
      childpid = fork();
      if (childpid < 0) {
	printf("fork failed\n");
	return 1;
      } else if (childpid == 0) {
	struct timeval start, end;
	double dt;
	pid_t pid1, pid2;
	int status1, status2;
	int fd[2];
	char buffer[20];
	
	printf("testing %s with %s\n", sndrs[j], rcvrs[i]);

	gettimeofday(&start, NULL);
	pipe(fd);

	pid1 = fork();
	if (pid1 < 0) {
	  printf("fork failed\n");
	  return 1;
	} else if (pid1 == 0) {
	  close(fd[0]);

	  snprintf(buffer, sizeof(buffer), "%d", fd[1]);
	  execl(sndrs[j], sndrs[j], buffer, NULL);

	  printf("exec failed\n");
	  return 1;
	}

	pid2 = fork();
	if (pid2 < 0) {
	  printf("fork failed\n");
	  return 1;
	} else if (pid2 == 0) {
	  close(fd[1]);

	  snprintf(buffer, sizeof(buffer), "%d", fd[0]);
	  execl(rcvrs[i], rcvrs[i], buffer, NULL);

	  printf("exec failed\n");
	  return 1;
	}

	close(fd[0]);
	close(fd[1]);

	printf("waiting for %s and %s to finish\n", sndrs[j], rcvrs[i]);
	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);

	gettimeofday(&end, NULL);
	dt = end.tv_sec-start.tv_sec+(end.tv_usec-start.tv_usec)/1000000.0;
	
	if (WEXITSTATUS(status1) || WEXITSTATUS(status2)) {
	  glob_var[i*nsndrs+j] = -1;
	} else {
	  glob_var[i*nsndrs+j] = dt;
	}

	return 0;
      }
    }
  }
  
  printf("waiting for all tests to complete\n");
  for (int i=0; i<nrcvrs*nsndrs; i++) {
    wait(NULL);
  }

  for (int i=0; i<nrcvrs; i++) {
    for (int j=0; j<nsndrs; j++) {
      if (glob_var[i*nsndrs+j] < 0) {
	printf("Unsuccessful testing %s with %s\n", sndrs[j], rcvrs[i]);
      } else {
	  printf("Done testing %s with %s, %f sec required\n", sndrs[j], 
		 rcvrs[i], glob_var[i*nsndrs+j]);
      }
    }
  }
  printf("Testing complete\n");

  return 0;
}
