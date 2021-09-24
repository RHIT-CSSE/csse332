#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

/**

Given the text file (lotr-part.txt), the program will reverse the file
content character (byte) by character, and then verify if the file is
successfully reversed.

The code below provides you the non-parallel solution. If you compile and
run the code, you will see something like this:

$ ./reverse_verify
Non-parallel method finished in 3686 usecs.
All done

The time cost may vary on your machine.

Your task is to accelerate the program by parallelizing it using pthread.
Note that you also need to print out the time cost for the parallel method
for comparision. The code for calculating run time and printing out is
provided.

STEP 1: Parallelize Reverse

The code for openning and reading the file is provided, you should start with
adding code to the reverse function to support parallel operations. As you can see
in the code of the reverse function, it simply reads the data in file_buff
backward and writes into output_buff.

Your job is to:

1) Add code to the reverse function to make it runnable by pthreads. When run by
pthreads, a pthread will pass a non-NULL pointer as the input argument to this
function. Note that the number of reverse pthread is fixed and pre-defined (as
2).
2) Add code in the main function to set up pthreads (including create and
join). After joining pthreads, add one line of code "verify(NULL)" to verify
the reverse result.
4) Uncomment the code that measures the run time and prints out the time cost. The
time cost should include both reverse (parallelized now) and verify (still
non-parallel).

After this step, the program output looks like this:
$ ./reverse_verify
Non-parallel method finished in 3797 usecs.
    Parallel method finished in 2621 usecs.
All done

You should see the speed of the parallel method is significantly improved
over the non-parallel way with the speedup of 1.44(3797/2621).

STEP 2: Parallelize Verify

In this step, you need to use ADDITIONAL pthreads to parallelize the verify
function.

Similar to the previous step, you need to add code in the verify function as well
as in main. The number of verify pthreads is also pre-defined, which
is equal to the number of reverse pthreads.

In order to maximize the parallelism, you want to start(i.e., call
pthread_create) all pthreads (2 reverse pthreads + 2 verify pthreads)
together.

However, a verify pthread must wait until the part of data it will check
has been reversed. Otherwise, the data is not ready and will result in an error
when verifying. YOU NEED TO USE SEMAPHORES TO SOLVE THIS PROBLEM.

Hint: I used an array of semaphores with size equal to NUM_VERI_THREADS (or
NUM_REV_THREADS).

When calculating time cost, don't forget to comment out the verify(NULL) used
in the previous step. Instead, you want to use pthreads to do verify.

After parallelizing verify and adding semaphores to solve concurrency issues,
the output should be something like this:

$ ./reverse_verify
Non-parallel method finished in 3689 usecs.
    Parallel method finished in 2093 usecs.
All done

As you can see, the run time for the parallel method is further improved with
the speedup equal to 1.76 (3689/2093). The speedup may vary on your computer
but should be better compared to the previous case.

If your semaphore solution is faulty, most likely you will see the error
message: "Reversing is not correct. Quiting program now...."


**/

#define FILE_SIZE 1004422
#define NUM_REV_THREADS 2
#define NUM_VERI_THREADS 2

char* file_buff;
char* output_buff;

void* reverse(void* arg) {
  if (arg == NULL) {
    for (int i = 0; i < FILE_SIZE; i++)
      output_buff[i] = file_buff[FILE_SIZE - i - 1];
    return NULL;
  }
  // Don't modify the code above and add your code below
  //####################################################





  //####################################################
  return NULL;
}

void* verify(void* arg) {
  if (arg == NULL) {
    for (int i = 0; i < FILE_SIZE; i++) {
      if (output_buff[i] != file_buff[FILE_SIZE - 1 - i]) {
        printf("Reversing is not correct. Quiting program now....");
        exit(1);
      }
    }
    return NULL;
  }
  // Don't modify the code above and add your code below
  //####################################################





  //####################################################
  return NULL;
}

int main(int argc, char* argv[]) {
  file_buff = malloc(FILE_SIZE);
  output_buff = malloc(FILE_SIZE);
  FILE* f = fopen("lotr-part.txt", "r");
  if (f == NULL) {
    perror("Failed to open file\n!!");
    return 1;
  }
  int len = fread(file_buff, 1, FILE_SIZE, f);
  if (len != FILE_SIZE) printf("File not read correctly\n!!!");

  struct timeval startt, stopt;
  suseconds_t time_cost;
  gettimeofday(&startt, NULL);
  reverse(NULL);
  verify(NULL);
  gettimeofday(&stopt, NULL);
  time_cost = stopt.tv_usec - startt.tv_usec;
  printf("Non-parallel method finished in %ld usecs.\n", time_cost);
  // reset output buff
  memset(output_buff, 0, FILE_SIZE);

  //############################################################
  // Don't modify the code above and add your code below

  //initialization and setup space



  // gettimeofday(&startt, NULL);
  // Start reversing here
  //####################################################





  //####################################################
  // Verifying ends here
  // gettimeofday(&stopt, NULL);
  // time_cost = stopt.tv_usec - startt.tv_usec;
  // printf("    Parallel method finished in %ld usecs.\n", time_cost);

  printf("All done\n");
  return 0;
}
