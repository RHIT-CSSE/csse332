/*

Here is some code that uses fork to speed up factoring.  Yes, I agree
it factors in a super dumb way.

First: Understand the code

Second:

Modify this code so that rather than have the 2 child processes
individually print out the factors discovered, instead the child
processes IPC communicate back to the parent process which prints them
out. (In a more realistic example, the parent would use the results in
its own calculations)

Its fine if doing this changes the ordering of the solutions printed
out.

You can either use shared memory or pipes to accomplish this.  If you
finish one solution, try the other.

Take a look at shared_simple and pipes_simple

 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
int main(void)
{

  long long int *global_var = mmap(NULL, sizeof(long long int)*2000, PROT_READ | PROT_WRITE, 
		  MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    
  unsigned long long int target, i, start = 0;
  printf("Give a number to factor.\n");
  // 18446744073709551615 is max, I think
  // I think 666666662 gives good results
  scanf("%llu",&target);

  int pid = fork();
  int outputPoint = 0;
  if(pid == 0) {
    start = 2;
  } else {
    pid = fork();
    if(pid == 0) {
      start = 3;
      outputPoint = 1000;
    } else {
      // I'm the parent
      wait(NULL);
      wait(NULL);
      for(int i = 0; i < 1000; i++) {
          if(global_var[i] == 0) break;
          printf("%llu is a factor\n", global_var[i]);
      }
      for(int i = 1000; i < 2000; i++) {
          if(global_var[i] == 0) break;
          printf("%llu is a factor\n", global_var[i]);
      }
      printf("Finished.\n");
      return 0;
    }
  }

  for(i = start; i <= target/2; i = i + 2) {
    if(target % i == 0) {
        //printf("%llu is a factor\n", i);
        global_var[outputPoint] = i;
        outputPoint++;
    }
  }
  global_var[outputPoint] = 0; //marks the end
  return 0;
}
